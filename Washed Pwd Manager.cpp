#include "PasswordMan.h"

void validateInput();

int main()
{
	//Vars
	PasswordMan * masterPass = new PasswordMan();
	bool programActive = true;
	int choice;
	int passLen;
	std::string service;
	std::string password;
	std::string ranPass;
	LPCSTR passFile = "Saved\\Encrypted.pass";

	//Verify Vars
	PasswordMan * verify;
	std::string masterStr = "";
	std::string verifyStr = "This Is A Noob Decrypted String";
	std::string input;
	std::string input2;
	char ynInput;

	//Main Loop
	while (programActive)
	{
		/**		[SET-UP]	**/
		//Check First Time Use ('Verify.pass' doesn't exist yet)
		if (GetFileAttributes(L"Saved\\Verify.pass") == INVALID_FILE_ATTRIBUTES) //&& GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			//Show First Time Init
			printf_s("Welcome to washed password manager, no saved passwords detected please continue setup\n");
			system("pause");
			system("cls");

			//Prep Verify Pass
			verify = new PasswordMan(verifyStr, "temp", "Saved\\Verify.pass");

			//Master Loop For Errors :)
			while (true)
			{
				//Get Pass & Check Size
				printf_s("Enter a master password (between 1 and 32 characters):\n");
				std::cin >> input;

				if (input.length() == 0 || input.length() > 32)
				{
					//Loop until valid pass
					do
					{
						printf_s("Error! Invalid chararcter length (%d is not a valid length)\n", input.length());
						printf_s("Enter a master password (between 1 and 32 characters):\n");
						std::cin >> input;
					} while (input.length() == 0 || input.length() > 32);
				}

				system("cls");

				//Verify Password
				printf_s("Confirm master password:\n");

				std::cin >> input2;

				system("cls");

				//Check
				if (input.compare(input2) == 0)
				{
					//Save Verify
					verify->SetEncryptionKey(input);
					
					if (!verify->EncryptStringToFile())
					{
						printf_s("Error occured trying to save master pass, exiting...\n");
						system("pause");
						delete verify;
						return -1;
					}

					//Create Encrypted.pass
					CreateFileA(passFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

					masterStr = input;

					//Print Msg
					printf_s("Master password saved to ~\\Saved\\Verify.pass\n");
					system("pause");
					system("cls");

					//Return To Normal Program
					break;
				}
				else
				{
					printf_s("Error confirming master password! Please reenter credentials:\n");
				}
			}

			delete verify;
		}

		/**		[Main-Program]		**/
		if (masterStr.length() == 0)
		{
			//Get Master Password
			while (true)
			{
				printf_s("Welcome! Please enter master password for current encrypted files:\n");
				std::cin >> input;
				system("cls");

				//Check If This Is Correct Password
				verify = new PasswordMan("temp", input, "Saved\\Verify.pass");

				//Decrypt To Buffer
				verify->DecryptEntireFile(FALSE, FALSE, TRUE);

				//Check Decrypt Is Correct
				if (verifyStr.compare(verify->GetDecryptBuffer()) == 0)
				{
					masterStr = input;
					delete verify;
					break;
				}

				printf_s("Entered password is not correct!\n");
			}
		}

		//Initiate Base PasswordMan
		masterPass = new PasswordMan("temp", masterStr, passFile);

		//Print And Get Input
		printf_s("Welcome to the program, please enter what you'd like to do:\n"
			"1: Add New Service With Existing Password To File\n"
			"2: Add New Service With Random Password\n"
			"3: Decrypt Password File In Console\n"
			"4: Decrypt Password File To Txt\n"
			"5: Generate Random Password\n"
			"6: Exit Program\n");

		std::cin >> choice;
		validateInput();
		system("cls");

		//Process Input
		switch (choice)
		{
		case 1:	//New Service With Existing Pass
			do
			{
				//Get Service Name
				printf_s("Enter name of the service:\n");
				std::cin >> service;
				system("cls");

				//Get Password For Service
				while (true)
				{
					//Get Pass
					printf_s("Enter password for service:\n");
					std::cin >> input;
					system("cls");

					//Verify Pass
					printf_s("Confirm password for service:\n");
					std::cin >> input2;
					system("cls");

					if (input.compare(input2) == 0)
						break;

					printf_s("Error, passwords do not match!\n");
				}

				password = input;

				//Confirm User Entered Information Correct
				printf_s("Service Name: %s\n"
					"Password: %s\n"
					"Is This Information Correct? (Y/N):\n", service.c_str(), password.c_str());
				std::cin >> ynInput;

			} while (ynInput != 'y' && ynInput != 'Y');

			//Make Neat For File
			service = "Service Name: " + service;
			password = "Password: " + password;

			//Initiate Masterpass And Save Data
			masterPass->SetStringToEnc(service);

			if (!masterPass->AppendToEncryptedFile())
				printf_s("Error saving service to file!\n");

			masterPass->SetStringToEnc(password);

			if (!masterPass->AppendToEncryptedFile())
				printf_s("Error saving password to file!\n");
			else
				printf_s("Information saved to ~\\Saved\\Encrypted.pass\n");

			system("pause");
			break;
		case 2:	//New Service With Random Pass
			do
			{
				//Get Service Name
				printf_s("Enter name of the service:\n");
				std::cin >> service;
				system("cls");

				//Get Password For Service
				do
				{
					printf_s("Enter random password length (recommended: 12, max: 32):");
					std::cin >> passLen;
					validateInput();

					printf_s("Error! %d is not a valid length!\n", passLen);
				} while (passLen == 0 || passLen > 32);

				system("cls");

				password = masterPass->GenerateRandomPass(passLen);

				//Confirm User Entered Information Correct
				printf_s("Service Name: %s\n"
					"Password: %s\n"
					"Is This Information Correct? (Y/N):\n", service.c_str(), password.c_str());
				std::cin >> ynInput;

			} while (ynInput != 'y' && ynInput != 'Y');
			
			//Make Neat For File
			service = "Service Name: " + service;
			password = "Password: " + password;

			//Initiate Masterpass And Save Data
			masterPass->SetStringToEnc(service);

			if (!masterPass->AppendToEncryptedFile())
				printf_s("Error saving service to file!\n");

			masterPass->SetStringToEnc(password);

			if (!masterPass->AppendToEncryptedFile())
				printf_s("Error saving password to file!\n");
			else
				printf_s("Information saved to ~\\Saved\\Encrypted.pass\n");

			system("pause");
			break;
		case 3:	//Decrypt File To Console
			masterPass->DecryptEntireFile(TRUE, FALSE, FALSE);
			printf_s("\n");
			system("pause");
			break;
		case 4:	//Decrypt File To Txt
			if (masterPass->DecryptEntireFile(FALSE, TRUE, FALSE))
				printf_s("File outputted to folder with program as Decrypt.txt\n");
			else
				printf_s("Error occured decrypting file!\n");
			system("pause");
			break;
		case 5: //Generate Random Password
			do
			{
				printf_s("Enter random password length (recommended: 12, max: 32):");
				std::cin >> passLen;
				validateInput();

				printf_s("Error! %d is not a valid length!\n", passLen);
			} while (passLen == 0 || passLen > 32);

			system("cls");

			//Generate Password & Output
			ranPass = masterPass->GenerateRandomPass(passLen);

			printf_s("Generated password: %s\n", ranPass.c_str());
			system("pause");
			break;
		case 6: //Exit Program
			programActive = false;
			break;
		default:
			printf_s("Error occurred! Please reenter input!");
			system("pause");
		}

		//Clear Screen
		system("cls");
	}

	//Clean up
	delete masterPass;

	return 0;
}

void validateInput()
{
	if (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
	}
}