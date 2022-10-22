/*
	PasswordMan By Enlisted, This Is An Easy To Use Library Which Will Encrypt/Decrypt Strings And Files
	For You With An Encryption Key You Can Set. Note This Encryption Is Not The Most Secure Since This
	Project Was Only Made To Test A Concept And Simplicity Was Prioritized.
*/

#pragma once

#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <ctime>

class PasswordMan
{
public:
	//Constructors
	PasswordMan();

	/*
		[Description]
		Constructor With Both Encryption Strings:

		[Arg] "std::string eStr" - String To Encrypt
		[Arg] "std::string pStr" - Encryption Key
		[Arg] "LPCSTR fStr" - FileName To Write/Read Encrypted Strings
	*/
	PasswordMan(std::string eStr, std::string pStr, LPCSTR fStr);

	//Setter Methods
	void SetStringToEnc(std::string eStr);
	void SetEncryptionKey(std::string pStr);
	void SetFileName(LPCSTR fStr);

	//Accessor Methods
	std::string GetStringToEnc();
	std::string GetEncryptionKey();
	std::string GetDecryptBuffer();
	LPCSTR GetFileName();

	// [Misc Methods]
	/*
		[Description]
		Function To Randomly Generate An Alphanumeric Password Randomly And Return It

		[Arg] "int pLen" - Length Of Password To Generate
	*/
	std::string GenerateRandomPass(int pLen); 

	// [Encryption Methods]

	/*
		[Description]
		Function To Encrypt An Already Existing File With Class Data Provided,
		After Being Ran Creates A New File With The Title Of "fileName" Which Contains
		Encrypted Data

		[Arg] "std::string preFile" - Name Of File To Encrypt
	*/
	BOOL EncryptPreExistingFile(std::string preFile);

	/*
		[Description]
		Function To Create An Entirely New File Or Overwrite The Data Of An Already Existing
		File With New Encrypted Data, This Function Will Not Append To An Already Encrypted
		File, Use "AppendToEncryptedFile" If You'd Like To Append To An Already Existing File
	*/
	BOOL EncryptStringToFile();

	/*
		[Description]
		Function That Will Append "encStr" Var To An Already Encrypted File, An Encrypted
		File Should Be Created Before Using This Function Via "EncryptPreExistingFile" Method
		Or "EncryptStringToFile" Method
	*/
	BOOL AppendToEncryptedFile();

	/*
		[Description]
		Function That Takes In A String And Will Xor It With Encryption Key And Return Encrypted
		String

		[Arg] "std::string newEnc" - String To Encrypt
	*/
	std::string EncryptString(std::string newEnc);

	/*
		[Description]
		Function That Will Encrypt Decryption Key To A File With A Static Master Pass Built Into
		This Function, Made To Make Decryption Process Easier By Being Able To Store Key To File
		And Load it
	*/
	BOOL SecureDecryptionKeyToFile();

	/*
		[Description]
		Function That Will Load Decryption Key Previously Saved To A File From The "SecureDecryptionKeyToFile"
		Method
	*/
	BOOL LoadDecryptionKeyFromFile();

	//Decryption Methods

	/*
		[Description]
		Function That Will Decrypt A File Already Encrypted By This Library, The Encrypted Data
		Can Be Printed To Console And/Or Outputted To A New File Called "Decrypt.txt"

		[Arg] "BOOL printFile" - Print To Console Or Not
		[Arg] "BOOL outputFile" - Output Decrypted File Or Not
		[Arg] "BOOL saveDecryptToBuffer" - Output Decrypted File Or Not
	*/
	BOOL DecryptEntireFile(BOOL printFile, BOOL outputFile, BOOL saveDecryptToBuffer);

	//[DEPRECIATED, USE NOT RECOMMENDED!] Returns Encrypted String
	std::string EncryptString();

private:
	std::string encStr;
	std::string pass;
	std::string staticMasterKey;
	std::string decryptBuffer;
	LPCSTR fileName;
	std::ofstream passFileWrite;
	std::ifstream passFileRead;
	int curIndex;
};