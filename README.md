# Washed-Pwd-Manager
## Disclaimer: This project was mostly made for fun and as so, the encryption algorithm in this program is very basic, if you're looking for a more secure password manager I recommend using [pass](https://www.passwordstore.org/)
### Description
Washed-Pwd-Manager is an easy to use, completely offline and open source password manager. It's very small and only uses basic C++ header files for ery little dependencies. Currently it is completely console based but I might make a GUI version in the future

### How to use:
Using the program is very simple just make sure there's a folder called "Saved" in the same file directory as the program and run it, on startup the program will check if files it saves (specifically "Verify.pass" exists) and if it doesn't it will enter into the setup phase where it asks you for a master password before using the program, enter a secure master password between 1 and 32 characters and confirm it to complete setup and enter the main program. On subsequent launches of the program the program will prompt you for your master password before it allows you to enter the program. Once you are inside the program there are 6 pretty self explanatory options, here's what each of the following options does:

**1: Add New Service With Existing Password To File** - prompts the user for a service name and password to add to "Encrypted.pass"

**2: Add New Service With Random Password** - prompts the user for a service name and length to randomly generate and store to "Encrypted.pass"

**3: Decrypt Password File In Console** - decrypts stored passwords and displays them within the program console

**4: Decrypt Password File To Txt** - decrypts stored passwords and outputs them to a new file "Decrypted.txt" in program directory

**5: Generate Random Password** - generates a standalone random password that no operation is done on


### How do I backup saved passwords:
Copy the files in the "Saved" folder (there should be two files one called "Verify.pass" and one called "Encrypted.pass") and store them where you'd like personally I'd recommend either on a drive disconnected from your computer or a cloud storage like Google Drive (important note: both of the files in the "Saved" folder are required for the program to read your saved passwords).

### How do I reset master password/saved passwords:
Delete both "Verify.pass" and "Encrypted.pass" from the "Saved" folder and this will trigger the program to reenter setup mode where you can input a new master password and new passwords to encrypt with it

### How does the encryption work:
The encryption used is essentially xorstr which is where the master password you entered is xor'd by the service and password you enter which makes the stored strings unreadable unless you know the master password, the xorstr implentation is very basic so the encryption isn't top level but the encryption is more than enough to make the file contents unreadable and unaccessible to people that do not know the master password you used.
