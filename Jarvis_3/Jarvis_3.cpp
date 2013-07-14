//Jarvis_3

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include "dirent.h"
#include <algorithm>
#include <ctime>
#include <sapi.h>
#include <vector>
#include <Windows.h>
#include <io.h>
#include <sstream>

#define SIZEOF(OBJ) sizeof(OBJ)/sizeof(OBJ[0])

using namespace std;

class Jarvis_3
{
public:
	Jarvis_3()
		: top_dir("C:\\"), request(""), fname(""), ret_str("Your request for name or directory could not be found."), request_mount(""), compsize("mount"), line(""), tempname("")
	{
	}
	
	~Jarvis_3()
	{
	}

	/*void newlogin()
	{
		system("CLS");
		string pass;
		cout << "New Password: ";
		cin >> pass;
		system("CLS");
		ofstream password;
		password.open("password.txt", ios::out);
		password << "setupinitializer";
		password.close();
		login();
	}

	void login()
	{
		string pass;
		string comppass;
		cout << "Password: ";
		getline(cin, pass);
		if (pass == "new")
		{
			newlogin();
		}
		else
		{
			ifstream password;
			password.open("password.txt", ios::in);
			getline(password, comppass);
			password.close();
			if (comppass == "nopassword") //file is empty (implies new user)
			{
				cout << "Password undefined" << endl;
				Respond(L"Password undefined");
				login();
			}
			else if (comppass == "setupinitializer")
			{
				ofstream password;
				password.open("password.txt", ios::out);
				password << pass;
				password.close();
				beginning(); //for now
			}
			else if (comppass == pass) //passwords do match
			{
				system("CLS");
				Questions_Help();
				beginning();
			}
			else //passwords just don't match
			{
				cout << "Authorization unsuccesful" << endl;
				Respond(L"Authorization unsuccesful");
				login();
			}
		}
	}

	void setup()
	{
		string dirdir;
		ofstream fweather;
		ofstream ftime;
		ofstream fdate;
		cout << "Initializing setup...";
		Respond(L"Initializing setup");

		fweather.open("weather.txt", ios::out);
		find(top_dir, "weather.exe", dirdir);
		fweather << dirdir;
		fweather.close();

		ftime.open("telltime.txt", ios::out);
		find(top_dir, "telltime.exe", dirdir);
		ftime << dirdir;
		ftime.close();

		fdate.open("telldate.txt", ios::out);
		find(top_dir, "telldate.exe", dirdir);
		fdate << dirdir;
		fdate.close();
		
		cout << " Done" << endl;
		Respond(L"Done");
		Sleep(1000);
		system("CLS");
		pythonmanager();
	}
	void waiter()
	{
		ifstream fff;
		for(;;)
		{
			fff.open("text_0.txt", ios::in);
			getline(fff, request);
			fff.close();
			if (request != ""){
				beginning();
				break;
			}
			else
				continue;
		}
	}

	void waiter_end()
	{
		ofstream fff;
		fff.open("text_0.txt", ios::trunc);
		fff.close();
		waiter();
	}*/

	string& flagcheck(string& line)
	{
		if (string::npos != line.find("SMS--*--$"))
		{
			cin.ignore(256, '$');
			Respond(L"SMS");
		}
		else if (string::npos != line.find("CONSOLE--*--$"))
			cin.ignore(256, '$');
		else if (string::npos != line.find("VOICE--*--$"))
			cin.ignore(256, '$');

	}

	void beginning()
	{
		getline(cin, request);
		request = lower_name(request);
		if (request == "remember name")
		{
			pResp();
			in_file_name();
			beginning();
			
		}
		else if (request == "Jarvis" || request == "yo Jarvis" ||
				 request == "Yo Jarvis" || request == "Jarvis you there")
		{
			Respond(L"Yes sir?");
			beginning();
			
		}
		else if (request == "remember mount name" || request == "remember mounting name" || request == "remember mount")
		{
			pResp();
			in_file_mount();
			beginning();
			
		}
		
		else if (request == "delete in filename" || request == "delete filename" || request == "filename delete"
				 || request == "delete name" || request == "delete names")
		{
			pResp();
			respondance = L"What would you like to delete sir?";
			Respond(respondance);
			getline(cin, request);
			pResp();
			if (request == "all")
			{
				ofstream filename;
				filename.open("name.txt", ios::trunc);
				filename.close();
				respondance = L"Succesfully truncated names.";
				Respond(respondance);
			}
			else
				delete_line(encrypt(request));
			beginning();
			
		}

		else if (request == "delete in mountname" || request == "delete mountname" || request == "mountname delete" || request == "delete mount")
		{
			pResp();
			respondance = L"What would you like to delete sir?";
			Respond(respondance);
			getline(cin, request);
			pResp();
			if (request == "all")
			{
				ofstream mountname;
				mountname.open("mountname.txt", ios::trunc);
				mountname.close();
				respondance = L"Succesfully truncated mount names.";
				Respond(respondance);
			}
			else
				delete_line_mount(encrypt(request));
			beginning();
			
		}

		else if (request == "delete directory" || request == "delete directories")
		{
			pResp();
			respondance = L"What would you like to delete sir?";
			Respond(respondance);
			getline(cin, request);
			pResp();
			if (request == "all")
			{
				ofstream filename;
				filename.open("name.txt", ios::trunc);
				filename.close();
				respondance = L"Succesfully truncated directories.";
				Respond(respondance);
			}
			else
				delete_line_dir(encrypt(request));
			beginning();
			
		}
		/*else if (string::npos != request.find("define") && request.length() > 6)
		{
			unsigned pos = request.find("define");
			Define(request, pos);
			beginning();
		}*/
		else if (string::npos != request.find("execute") || string::npos != request.find("run"))
		{
			pResp();
			Execprg(request);
			beginning();
			
		}
		else if (string::npos != request.find("mount") && request.size() > compsize.size())
		{
			pResp();
			string c_string = mountfile(request);
			if (c_string.length() > 1)
			{
				c_string = encrypt(c_string);
				delete_line_mount(c_string);
			}
			else
			{
				beginning();
			}
			beginning();
			
		}
		else if (string::npos != request.find("mount") && request.size() == compsize.size())
		{
			pResp();
			ambiguousmountfile();
			beginning();
			
		}
		else if (string::npos != request.find("list directory") || string::npos != request.find("list directories"))
		{
			pResp();
			ifstream directory;
			directory.open("directory.txt", ios::in);
			while(directory)
			{
				getline(directory, line);
				line = decrypt(line);
				cout << line << endl;
			}
			directory.close();
			beginning();
			
		}
		else if (string::npos != request.find("list name") || string::npos != request.find("list names"))
		{
			pResp();
			ifstream filename;
			filename.open("name.txt", ios::in);
			while(filename)
			{
				getline	(filename, line);
				line = decrypt(line);
				cout << line << endl;
			}
			filename.close();
			beginning();
			
		}
		else if (string::npos != request.find("list mount") || string::npos != request.find("list mountnames") ||
				 string::npos != request.find("list mount names"))
		{
			pResp();
			ifstream mountname;
			mountname.open("mountname.txt", ios::in);
			while(mountname)
			{
				getline	(mountname, line);
				line = decrypt(line);
				cout << line << endl;
			}
			mountname.close();
			beginning();
			
		}
		else if (request == "list commands" || request == "help")
		{
			pResp();
			ifstream commands;
			commands.open("commands.txt", ios::in);
			while(commands)
			{
				getline (commands, line);
				line = decrypt(line);
				cout << line << endl;
			}
			commands.close();
			beginning();
			
		}
		else if (string::npos != request.find("what time") || (string::npos != request.find("what") && string::npos != request.find("time")))
		{
			string dirdir;
			ifstream ftime;
			ftime.open("telltime.txt", ios::in);
			getline(ftime, dirdir);
			ftime.close();
			if (dirdir.length() > 1)
			{
				system(("\"" + dirdir + "\"").c_str());
				ifstream ttime;
				ttime.open("time.txt", ios::in);
				getline(ttime, line);
				ttime.close();
				respondance = L"It is " + respondance.assign(line.begin(), line.end());
				Respond(respondance);
			}
			else
			{
				cout << "telltime.exe does not exist under " << top_dir << endl;
				Respond(L"Could not find the directory");
			}
			beginning();
			
		}
		else if (string::npos != request.find("what date") || (string::npos != request.find("what") && string::npos != request.find("date")))
		{
			string dirdir;
			ifstream fdate;
			fdate.open("telldate.txt", ios::in);
			getline(fdate, dirdir);
			fdate.close();
			if (dirdir.length() > 1)
			{
				system(("\"" + dirdir + "\"").c_str());
				ifstream tdate;
				tdate.open("date.txt", ios::in);
				getline(tdate, line);
				tdate.close();
				respondance.assign(line.begin(), line.end());
				Respond(respondance);
			}
			else
			{
				cout << "telldate.exe does not exist under " << top_dir << endl;
				Respond(L"Could not find the directory");
			}
			beginning();
			
		}
		else if (request == "quit" || request == "power off" ||
				 request == "shut down" || request == "leave")
		{
			cout << "Terminating..." << endl;
			respondance = L"Shutting down power";
			Respond(respondance);
			exit(0);
			
		}
		else if (string::npos != request.find("weather"))
		{
			string dirdir;
			ifstream fweather;
			fweather.open("weather.txt", ios::in);
			getline(fweather, dirdir);
			fweather.close();
			if (dirdir.length() > 1)
			{
				system(("\"" + dirdir + "\"").c_str());
				ifstream weather;
				weather.open("datafile.txt", ios::in);
				getline(weather, line);
				weather.close();
				respondance.assign(line.begin(), line.end());
				Respond(respondance);
			}
			else
			{
				Respond(L"Could not find the directory for weather.exe");
			}
			beginning();
			
		}
		/*else if (request == "setup")
		{
			pResp();
			setup();
		}*/
		/*else if (string::npos != request.find("change pass"))
		{
			pResp();
			string pass;
			cout << "What would you like your password to be changed into?" << endl;
			Respond(L"What shall I change it to?");
			getline(cin, pass);
			cout << "Changing... ";
			ofstream password;
			password.open("password.txt", ios::out);
			password << pass;
			password.close();
			cout << "Done" << endl;
			Respond(L"Successfully changed password");
			beginning();
		}*/
		/*else if (request == "logout" || request == "log out")
		{
			pResp();
			system("CLS");
			login();
		}*/
		/*else if (string::npos != request.find("search") || string::npos != request.find("look up") &&
				 request.length() != 5)
		{
			string line;
			ifstream directory;
			directory.open("directory.txt", ios::in);
			while(directory.good())
			{
				getline(directory, line);
				if (string::npos != line.find("chrome"))
				{
					system(("\"" + line + "\"" + " " + "hello" + " " + "hello").c_str());
					
				}
				if (directory.eof() && string::npos == line.find("chrome"))
					Respond(L"You do not have google chrome installed on your computer sir");
			}
			directory.close();
			beginning();
		}*/
		else
		{
			/*respondance = L"Invalid request sir";
			cout << "Invalid request sir" << endl;
			Respond(respondance);*/
			uResp();
			beginning();
		}
		
	}

	//speech variation (positive resp, negative resp, resp action)

	void pResp()
	{
		wstring c_words[] = {L"Yes sir!", L"Will do sir!", L"Certainly!", L"I will get on it sir!", L"As you wish sir",
							 };
		unsigned const nWords = SIZEOF(c_words); 
		pResponse.assign(c_words, c_words+nWords);
		srand(time(0));
		random_shuffle(pResponse.begin(), pResponse.end());
		Respond(pResponse[0]);
	}

	void nResp()
	{
		wstring c_words[] = {L"Sir, I'm afraid I cannot proceed!", L"Sir, your request cannot be fulfilled!", 
							 L"I'm sorry sir!; There's nothing I can do.", L"Sir, I am not able to satisfy your request!"};
		unsigned const nWords = SIZEOF(c_words); 
		pResponse.assign(c_words, c_words+nWords);
		srand(time(0));
		random_shuffle(pResponse.begin(), pResponse.end());
		Respond(pResponse[0]);
	}

	void Questions_Help()
	{
		wstring c_words[] = {L"How may I help you sir?", L"What would you like me to do sir?",
							 L"What is your request sir?", L"How may I be at your service?"};
		unsigned const nWords = SIZEOF(c_words); 
		pResponse.assign(c_words, c_words+nWords);
		srand(time(0));
		random_shuffle(pResponse.begin(), pResponse.end());
		Respond(pResponse[0]);
	}

	void uResp()
	{
		wstring c_words[] = {L"Invalid request sir.", L"Sir, I didn't quite catch that.",
							 L"Sir I don't quite follow.", L"Sir, could you repeat that?"};
		unsigned const nWords = SIZEOF(c_words); 
		pResponse.assign(c_words, c_words+nWords);
		srand(time(0));
		random_shuffle(pResponse.begin(), pResponse.end());
		Respond(pResponse[0]);
	}

	int Respond(wstring const & response) //added "&"
	{
		if (FAILED(::CoInitialize(NULL)))
			return EXIT_FAILURE;

		HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);    
		if(SUCCEEDED(hr))
		{							
			hr = pVoice->Speak((LPCWSTR)response.c_str(), NULL, NULL);        
	
			pVoice->Release();
			pVoice = NULL;
		}
		::CoUninitialize();

		return EXIT_SUCCESS;
	}

	/*operation (find dir for file name, eat_ws - just in case, remember name (ex. starcraft),
				take out name, check if directory is valid, remember directory,
				execute prgm, remember mount directory (user input),
				mount file, ambiguous request for mount to specific file,
				change string to all lowercase) */

	int find(string const & dir_name, string const & entry_name, string & full_path)
	{
	 DIR * dir;
	 struct dirent *ent;
	 int res=-1;
	 dir = opendir(dir_name.c_str());
	 if(dir==NULL)
	  return -1;
	 while(ent=readdir(dir)) {
	  string nn(ent->d_name);
	  if(nn == "." || nn == "..")
	   continue;
	  if(stricmp(nn.c_str(), entry_name.c_str())==0) {
	   // found the entry
	   full_path = dir_name + "\\" + entry_name;
	   res = 0;
	  }
	  else {
	   string sub_dir = dir_name + "\\" + nn;
	   res = find(sub_dir, entry_name, full_path);
	  }
	  if(res==0) return 0;
	 }
	 return res;
	}

	istream & eat_ws(istream & istr)
	{
		char c;
		while( isspace(c = istr.peek()) )
			istr >> noskipws >> c;
		return istr;
	}

	void in_file_name()
	{
		ofstream filename;
		respondance = L"What name would you like me to remember?";
		Respond(respondance);
		filename.open("name.txt",  ios::app | ios::out);
		getline(cin, remember_name);
		remember_name = encrypt(remember_name);
		filename << remember_name << endl;
		filename.close();
		respondance = L"Task completed";
		Respond(respondance);
		int cvalue = in_file_name_leaving(remember_name);
		if (cvalue == 0)
		{
			beginning();
		}
		else
		{
			delete_line(remember_name);
			beginning();
		}
	}

	int in_file_name_leaving(string & name_just_typed)
	{
		ifstream filename;
		filename.open("name.txt", ios::in);
		while(filename)
		{
			getline(filename, out_name);
			if (string::npos != out_name.find(name_just_typed))
			{
				int ccvalue = out_file_name(name_just_typed);
				return ccvalue;
			}
		}
		filename.close();
	}

	int out_file_name(string & request_name)
	{
		ifstream filename;
		filename.open("name.txt", ios::in);
		while(filename)
		{
			getline(filename, fname);
			if (string::npos != fname.find(request_name)) //if my request for a name is found in a line of filename
			{
				request_name = decrypt(request_name);
				tempname = request_name + ".exe";
				res = find(top_dir, tempname, full_path);
				
				if(res==0){
				in_file_fname(full_path, tempname);
				filename.close();
				return 0;
				break;
				}
				else{
				filename.close();
				respondance = L"Perhaps adding your subdirectory to my memory?";
				Respond(respondance);
				return 1;
				break;
				}	
			}
		}
	}

	void in_file_fname(string & path, string & fname_fname)
	{
		respondance = L"Appending";
		Respond(respondance);
		ofstream directory;
		directory.open("directory.txt", ios::out | ios::app);
		directory << path << endl;
		directory.close();
		respondance = L"Successfully completed sir.";
		Respond(respondance);
		beginning();
	}

	void Execprg(string & user_request)
	{
		//npos request in 2nd file full of directories
		ifstream directory;
		ifstream filename;
		directory.open("directory.txt", ios::in);
		filename.open("name.txt", ios::in);
		while(getline(filename, remember_name))
		{
			remember_name = decrypt(remember_name);
			if (string::npos != user_request.find(remember_name))
			{
				while(directory)
				{
					getline(directory, remember_directory);
					remember_directory = decrypt(remember_directory);
					if (string::npos != remember_directory.find(remember_name))
					{
						system(("\"" + remember_directory + "\"").c_str());
						//cout << "Launch was successful" << endl;
						respondance = L"Launch was successful; .";
						Respond(respondance);
						beginning();
						break;
					}
					else
					{
						continue;
					}
				}
				break;
			}
			else
				continue;
		}
		filename.close();
		directory.close();
		//cout << "Could not complete task" << endl;
		nResp();
		respondance = L"Launching was not successful";
		Respond(respondance);
		beginning();
	}

	void in_file_mount()
	{
		ofstream mountname;
		respondance = L"What mounting directory should I remember? Please type sir.";
		Respond(respondance);
		getline(cin, remember_mountname);
		remember_mountname = encrypt(remember_mountname);
		mountname.open("mountname.txt", ios::app | ios::out);
		mountname << remember_mountname << endl;
		mountname.close();
		respondance = L"Task completed";
		Respond(respondance);
	}

	string mountfile(string const & rm)
	{
		ifstream filename;
		ifstream mountname;
		filename.open("name.txt", ios::in);
		mountname.open("mountname.txt", ios::in);
		while(filename)
		{
			getline(filename, out_name_beginning);
			out_name_beginning = decrypt(out_name_beginning);
			if (out_name_beginning == "")
			{
				continue;
			}

			else if (string::npos != rm.find(out_name_beginning))
			{
				respondance = L"Checking for valid mount dir";
				Respond(respondance);
				while(mountname)
				{
					getline (mountname, out_mountname);
					out_mountname = decrypt(out_mountname);
					new_out_mountname = out_mountname;
					new_out_name_beginning = out_name_beginning;
					out_mountname = lower_name(out_mountname);
					out_name_beginning = lower_name(out_name_beginning);
					if (string::npos != out_mountname.find(out_name_beginning))
					{
						run_mount_system = "powershell -command mount-diskimage -imagepath \"" + new_out_mountname + "\"";
						system(("\"" + run_mount_system + "\"").c_str());
						Respond(L"Conclusion sir... (Delete or no?). Please type sir.");
						getline(cin, answer);
						if (string::npos != answer.find("delete") || string::npos != answer.find("erase"))
						{
							respondance = L"Mounting was unsuccessful";
							Respond(respondance);
							return new_out_mountname;
						}
						else
							return "";
						break;
					}
				}
				break;
			}
		}
		nResp();
		respondance = L"I could not mount the drive.";
		Respond(respondance);
		mountname.close();
		filename.close();
	}

	void ambiguousmountfile()
	{
		respondance = L"What would you like to mount sir? Please type sir.";
		Respond(respondance);
		getline (cin, request_mount);

		if (string::npos != request_mount.find("nothing") || string::npos != request_mount.find("exit") ||
			string::npos != request_mount.find("quit") || string::npos != request_mount.find("back to main"))
		{
			pResp();
			beginning();
		}

		else
		{
			pResp();
			mountfile(request_mount);
		}
	}

	string & lower_name(string & needname)
	{
		for (size_t i=0; i<needname.length(); ++i)
		{
			needname[i] = tolower(needname[i]);
		}
		return needname;
	}
	
	void Define(string & req, unsigned & pos)
	{
		ifstream fileInput;
		string line;
		string word;
		string compt;
		string wordt;
		fileInput.open("dictionary.txt", ios::in);

		compt = req;
		wordt = req.substr(pos, req.find(' '));
		while(fileInput)
		{
			string comp;
			
			getline(fileInput, line);
			comp = line;
			word = line.substr(0, line.find(' '));
			if (wordt == word)
			{
			/*if (comp.find(word) != string::npos)
			{

				for (int t = 0;  (unsigned) t < word.length(); t++)
				{
					comp += line[t];
				}
				if(comp == search)
				{*/
					respondance.assign(comp.begin(), comp.end());
					Respond(respondance);
					beginning();
					break;
				//}
				
			}
			else
			{
				continue;
			}
		}
		fileInput.close();
		nResp();
		Respond(L"That word isn't in my database");
		beginning();
	}

	//delete certain line in name.txt
	void delete_line(string & delete_input)
	{
		string line = "";
		ifstream filename;
		ofstream tempname;
		filename.open("name.txt", ios::in);
		tempname.open("tempname.txt", ios::out);
		while(getline(filename, line))
		{	
			if (line != delete_input)
			{
				tempname << line << endl;
			}
		}
		tempname.close();
		filename.close();

		remove("name.txt");
		rename("tempname.txt", "name.txt");
	}

	//delete certain line in mountname.txt
	void delete_line_mount(string & delete_input)
	{
		string line = "";
		ifstream mountname;
		ofstream tempname;
		mountname.open("mountname.txt", ios::in);
		tempname.open("tempname.txt", ios::out);
		while(getline(mountname, line))
		{	
			if (line == delete_input || string::npos != lower_name(line).find(delete_input))
			{
				continue;
			}
			else
			{
				tempname << line << endl;
			}
		}
		tempname.close();
		mountname.close();

		remove("mountname.txt");
		rename("tempname.txt", "mountname.txt");
	}

	//delete certain line in directory.txt
	void delete_line_dir(string & delete_input)
	{
		string line = "";
		ifstream directory;
		ofstream tempname;
		directory.open("directory.txt", ios::in);
		tempname.open("tempname.txt", ios::out);
		while(getline(directory, line))
		{	
			if (line == delete_input || string::npos != lower_name(line).find(delete_input))
			{
				continue;
			}
			else
			{
				tempname << line << endl;
			}
		}
		tempname.close();
		directory.close();

		remove("directory.txt");
		rename("tempname.txt", "directory.txt");
	}

	string encrypt(string & line)
	{
		string encrypted = "";
		char key = 'x';

		for (int temp = 0; temp < line.size(); temp++){
		 encrypted += line[temp] ^ (int(key) + temp) % 255;
		}
		return encrypted;
	}

	string decrypt(string & line)
	{
		string decrypted = "";
		char key = 'x';
		for (int temp = 0; temp < line.size(); temp++){
		 decrypted += line[temp] ^ (int(key) + temp) % 255;
		}
		return decrypted;
	}
private:
	int res;
	int pos_del, pos_real, pos_str3, pos_str6;
	string answer;
	string delete_lline;
	string delete_lline_2;
	string delete_lline_3;
	string top_dir;
	string request;
	string fname;
	string full_path;
	string line;
	string remember_name;
	string remember_directory;
	string remember_mountname;
	string request_name;
	string request_mount;
	string temp1, temp2, temp3, temp4;
	string out_name;
	string out_name_beginning;
	string new_out_name_beginning;
	string out_mountname;
	string new_out_mountname;
	string ret_str;
	string run_mount_system;
	string tempname;
	string compsize;
	wstring respondance;
	vector<wstring> pResponse;
	fstream filename;
	fstream directory;
	fstream mountname;
	fstream commands;
	ISpVoice * pVoice;
};

int main()
{
	Jarvis_3 oJarvis_3;

	oJarvis_3.beginning();

	/*	res = find(top_dir, fname, full_path);
		if(res==0){
		cout << fname << " has the full path: " << full_path << endl;
		system(("\"" + full_path + "\"").c_str());
		}
		else{
		cout << fname << " does not exist under " << top_dir << endl;
		cout << "Perhaps adding your subdirectory to memory?" << endl;
		}
	*/

	return 0;
}
