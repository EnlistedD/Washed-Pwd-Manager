#include "PasswordMan.h"

//Constructors
PasswordMan::PasswordMan()
{
	encStr = "This is a password!!!";
	pass = "5%$-*9KRB@3";
	fileName = "Default.pass";
	curIndex = 0;
	staticMasterKey = "2P6Z%*iCUyVYis3I7E#c@3^7Sn23Z*&17mw&lPkmUbhP$w1ndh";
	decryptBuffer = "";
}

PasswordMan::PasswordMan(std::string eStr, std::string pStr, LPCSTR fStr)
{
	encStr = eStr;
	pass = pStr;
	fileName = fStr;
	curIndex = 0;
	staticMasterKey = "2P6Z%*iCUyVYis3I7E#c@3^7Sn23Z*&17mw&lPkmUbhP$w1ndh";
	decryptBuffer = "";
}

//Methods
void PasswordMan::SetStringToEnc(std::string eStr)
{
	encStr = eStr;
}
void PasswordMan::SetEncryptionKey(std::string pStr)
{
	pass = pStr;
}
void PasswordMan::SetFileName(LPCSTR fStr)
{
	fileName = fStr;
}

std::string PasswordMan::GetStringToEnc()
{
	return encStr;
}
std::string PasswordMan::GetEncryptionKey()
{
	return pass;
}
std::string PasswordMan::GetDecryptBuffer()
{
	//Copy To Temp Var & Clear Buffer
	std::string tempStr = decryptBuffer;
	decryptBuffer = "";

	return tempStr;
}
LPCSTR PasswordMan::GetFileName()
{
	return fileName;
}

// [Misc Methods]
std::string PasswordMan::GenerateRandomPass(int pLen)
{
	//Vars
	const char alphanum[] = "0123456789!@#$%^&*abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int len = sizeof(alphanum) - 1;
	std::string retStr = "";

	//Set Random Seed To Current Time
	srand(time(NULL));

	//Generate Pass
	for (int i = 0; i < pLen; i++)
		retStr += alphanum[rand() % len];

	return retStr;
}

// [Encryption Methods]
BOOL PasswordMan::EncryptPreExistingFile(std::string preFile)
{
	//Open File To Encrypt
	passFileRead.open(preFile, std::ios::in | std::ios::binary);

	if (passFileRead.is_open())
	{
		//Open File For Writing
		passFileWrite.open(fileName, std::ios::out | std::ios::binary);

		if (passFileWrite.is_open())
		{
			//Write All Old Data Back To File
			std::string curLine;

			while (std::getline(passFileRead, curLine))
			{
				//Add Back New Line Char
				curLine += "\n";

				passFileWrite << EncryptString(curLine);
			}

			passFileWrite.close();
		}
		else
		{
			printf_s("[ERROR \"EncryptPreExistingFile\"] Can't Open File For Writing!\n");
			return FALSE;
		}

		//Close File
		passFileRead.close();

		//Reset Index
		curIndex = 0;
	}
	else
	{
		printf_s("[ERROR \"EncryptPreExistingFile\"] Couldn't open file for Reading!\n");
		return FALSE;
	}

	return TRUE;
}

BOOL PasswordMan::AppendToEncryptedFile()
{
	//Decrypt File Contents And Output Temp
	if (!DecryptEntireFile(FALSE, TRUE, FALSE))
	{
		printf_s("[ERROR \"AppendToEncryptedFile\"] Decrypt File Returned False!\n");
		return FALSE;
	}

	//Open Decrypted File
	std::ifstream decFile;

	decFile.open("Decrypt.txt", std::ios::in | std::ios::binary);

	if (decFile.is_open())
	{
		//Open File For Writing
		passFileWrite.open(fileName, std::ios::out | std::ios::binary);

		if (passFileWrite.is_open())
		{
			//Write All Old Data Back To File
			std::string curLine;

			while (std::getline(decFile, curLine))
			{
				//Reinsert New Line Char
				curLine += "\n";

				passFileWrite << EncryptString(curLine);
			}

			//Write New String
			passFileWrite << EncryptString(encStr);

			passFileWrite.close();
		}
		else
		{
			printf_s("[ERROR \"AppendToEncryptedFile\"] Can't open file for Writing!\n");
			return FALSE;
		}

		//Close File
		decFile.close();

		//Delete Decrypted File
		DeleteFileA("Decrypt.txt");

		//Reset Index
		curIndex = 0;
	}
	else
	{
		printf_s("[ERROR \"AppendToEncryptedFile\"] Couldn't open Decrypt.txt!\n");
		return FALSE;
	}

	return TRUE;
}

//Returns Encrypted String
std::string PasswordMan::EncryptString()
{
	//Temp Strs To Hold Data
	LPCSTR encStrPtr = encStr.c_str();
	LPCSTR decStrPtr = pass.c_str();
	std::string retStr = "";

	//Loop Encryption
	for (int i = 0; i < encStr.length(); i++)
	{
		//Prevent OOB Read
		int j = i % pass.length();

		//Encrypt
		char curEncChar = *(char*)(encStrPtr + i);
		char curKeyChar = *(char*)(decStrPtr + j);

		retStr += (curEncChar ^ curKeyChar);
	}

	//Return String With Encrypted Contents
	return retStr;
}

//Overload
std::string PasswordMan::EncryptString(std::string newEnc)
{
	//Temp Strs To Hold Data
	LPCSTR encStrPtr = newEnc.c_str();
	LPCSTR decStrPtr = pass.c_str();
	std::string retStr = "";

	//Loop Encryption
	for (int i = 0; i < newEnc.length(); i++)
	{
		//Encrypt
		char curEncChar = *(char*)(encStrPtr + i);
		char curKeyChar = *(char*)(decStrPtr + curIndex);

		//Prevent OOB Read
		if ((curIndex + 1) >= pass.length())
			curIndex = 0;
		else
			curIndex++;

		retStr += (curEncChar ^ curKeyChar);
	}

	//Return String With Encrypted Contents
	return retStr;
}

//Outputs Encrypted String Into A File And Overwrites Data
BOOL PasswordMan::EncryptStringToFile()
{
	//Temp Strs To Hold Data
	LPCSTR encStrPtr = encStr.c_str();
	LPCSTR decStrPtr = pass.c_str();
	char tempChar;

	//Encryption (To File)
	passFileWrite.open(fileName, std::ios::out | std::ios::binary);

	if (passFileWrite.is_open())
	{
		for (int i = 0; i < encStr.length(); i++)
		{
			//Prevent OOB Read
			int j = i % pass.length();

			//Encrypt
			char curEncChar = *(char*)(encStrPtr + i);
			char curKeyChar = *(char*)(decStrPtr + j);

			//tempBufferStr[i] = (curEncChar ^ curKeyChar);
			tempChar = (curEncChar ^ curKeyChar);

			passFileWrite << tempChar;
		}

		//Write Contents
		//passFileWrite.write((char*)&tempBufferStr[0], 32);

		//Close
		passFileWrite.close();
	}
	else
	{
		printf_s("[ERROR \"EncryptStringToFile\"] Couldn't open file for writing!\n");
		return FALSE;
	}

	return TRUE;
}

//Encrypt Decryption Key With Master Pass And Store To File
BOOL PasswordMan::SecureDecryptionKeyToFile()
{
	//Store Old Class Data
	std::string oldEncStr = GetStringToEnc();
	std::string oldDecKey = GetEncryptionKey();

	//Set Encrypt String To Decryption Key
	SetStringToEnc(pass);

	//Set Encryption Key To Master Pass
	SetEncryptionKey(staticMasterKey);

	//Encrypt To File
	if (!EncryptStringToFile())
		return FALSE;

	//Reset Class Data
	SetStringToEnc(oldEncStr);
	SetEncryptionKey(oldDecKey);

	return TRUE;
}

//Load Decryption Key From A File
BOOL PasswordMan::LoadDecryptionKeyFromFile()
{
	//Decrypt File
	if (!DecryptEntireFile(FALSE, FALSE, TRUE))
		return FALSE;

	//Set Key
	SetEncryptionKey(decryptBuffer);

	//Clear Buffer
	decryptBuffer = "";

	return TRUE;
}

//Decryption Methods
BOOL PasswordMan::DecryptEntireFile(BOOL printFile, BOOL outputFile, BOOL saveDecryptToBuffer)
{
	//Temp Strs To Hold Data
	LPCSTR decStrPtr = pass.c_str();

	passFileRead.open(fileName, std::ios::in | std::ios::binary);

	if (passFileRead.is_open())
	{
		//Check IF Outputting
		if (outputFile)
		{
			//Create File With Decryption
			passFileWrite.open("Decrypt.txt", std::ios::out | std::ios::binary);

			//Error Check
			if (!passFileWrite.is_open())
			{
				printf_s("[ERROR \"DecryptEntireFile\"] Couldn't create file for writing!\n");
				return FALSE;
			}
		}

		//Temp Vars
		char tempChar;
		int i = 0, j = 0;

		//Loop Decryption
		while (passFileRead.get(tempChar))
		{
			//Prevent OOB
			j = i % pass.length();

			//Decrypt
			char curKeyChar = *(char*)(decStrPtr + j);
			tempChar = tempChar ^ curKeyChar;

			//Check If We're Printing
			if (printFile)
			{
				//Bot Fix :)
				if (tempChar != 10)
					printf_s("%c", tempChar);
				else
					printf_s("\n", tempChar);
			}

			//Check If We're Outputting Decrypted File
			if (outputFile)
			{
				//Bot Fix :)
				if (tempChar != 10)
					passFileWrite << tempChar;
				else
					passFileWrite << "\n";
			}

			//Check If We're Saving Data To Buffer
			if (saveDecryptToBuffer)
			{
				decryptBuffer += tempChar;
			}

			//Inc
			i++;
		}

		/*
		while (std::getline(passFileRead, curLine))
		{
			printf_s("\nOrg String: %s\nNew String: ", curLine.c_str());

			LPCSTR curLinePtr = curLine.c_str();

			for (int i = 0; i < curLine.length(); i++)
			{
				//Prevent OOB Read
				int j = i % pass.length();

				//Decrypt
				char curKeyChar = *(char*)(decStrPtr + j);
				tempBufferStr[i] = *(char*)(curLinePtr + i);

				tempBufferStr[i] = (tempBufferStr[i] ^ curKeyChar);

				printf_s("%c", (char)tempBufferStr[i]);
			}
		}
		*/

		//Close
		passFileRead.close();

		if (outputFile)
			passFileWrite.close();
	}
	else
	{
		printf_s("[ERROR \"DecryptEntireFile\"] Couldn't open file for reading!\n");
		return FALSE;
	}

	return TRUE;
}