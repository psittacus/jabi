#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;


bool file_exists(char *filename) {
  ifstream ifile(filename);
  return (bool)ifile;
}


/*
TODO: 
--> Better interface: jabi branch //prints out the branches in the destination folder
--> Improve information in logfile f.e. errors (popen)
*/

//the copying and backuping process

bool backup(string full_path, string destination_path) {
  string order = "cp -R " + full_path + " " + destination_path;
  cout << "copying from [" << full_path << "] to [" << destination_path << "]"<< endl;  
  system(order.c_str()); //the actual cp command
  string logfilepath = destination_path + "logfile"; 
  ofstream of(logfilepath, ios::app);  //writing into logfile
  of << "copyied: [" << full_path << "] to [" << destination_path << "] \n";
  of.close();
  return true;
}

//the adding method - adds a string to the config file
bool addSthToConfig(string path) {
  cout << "adding to config file: " + path;
  ofstream of("config", ios::app);
  of << path;
  of.close();
  cout << "successfully added!" << endl;
  return true;
}

int main(int argc, char** argv) {

 string destination_path;

 cout << "Just A Backup Interface" << endl;
 cout << "-+-+-+-+-+-+-+-+-+-+-+-" << endl;

if(argv[1] == NULL) {
  cout << "Usage: jabi [configfile]\nor:\njabi add [path/this] for adding a path to the configfile\nThe config file contains the paths to the directories or files which should be saved\nIn the first line stands the destination directory" << endl;

} else {
  string arg = argv[1];

  if(arg == "add" && argv[2] != NULL) {                //NEWCOMMAND: add
    arg = argv[2];
    if(arg == "this") {                                //NEWSUBCOMMAND: (add) this
      FILE *p = popen("pwd", "r");                     //FILE because popen returns it
      char tmppath[256];
      fgets(tmppath,256, p);                           //get a string out of that

      string path = tmppath;                           //save it in a string
      addSthToConfig(path);                            //call the method
    } else {
      addSthToConfig(argv[2]);
    }
  }
  else if (file_exists(argv[1])) {
    ifstream ifs(argv[1]);

    if (ifs.is_open()) {
      string temp_path;
      int line_counter = 0;				//this is relevant for the first line (destination directory)
      cout << "moving bits and bytes now..." << endl;
      while(getline(ifs, temp_path)) {
        if(line_counter == 0) {
	  destination_path = temp_path;
	  line_counter++;
        } else {      	
        backup(temp_path, destination_path);
        }
      }
    }
  } else {
    cout << "file does not exist\n";
  }
}


return 0;
} 
