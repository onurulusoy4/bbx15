#include <iostream>
#include <fstream>
#include <unistd.h>
#include <chrono>
#include <time.h>
#include <iomanip>
#include <sys/stat.h>


using namespace std;
string now();
void master_writing(ofstream& com, string _command);
string get_last_word(const string& file_name);
void evaluate_slave(string _command, string word);
void create_directories(string directories[], int size);

fstream _log;

int main() {
    string directories[] = {"dev/gpio", "dev/spi", "dev/i2c", "dev/ethernet", "dev/usart", "dev/uart", "dev/can"};
    create_directories(directories, 7);

    ofstream com ("communication-register");
    com.close();

    cout << "tester started working" << endl;
    string _command;
    string emptyString = "---";
    float delay = .4;

    cout << endl;

    cout << "Commands: " << endl;
    cout << "show-gpiochipx: shows gpiox set" << endl;
    cout << "read-gpiochipx-offset-property: reads gpiox spesific preperty from offset" << endl;
    cout << "write-gpiochipx-offset-property-newValue: writes new value to gpiox spesific property from offset" << endl;
    cout << "config-gpiochipx: configs gpiox set from default" << endl;
    cout << ".commandSet-textfile: Executes commands from file" << endl;
    cout << endl;

    cout << "Enter command (-1 to terminate): " << endl;
    while (_command != "-1"){
        cin >> _command;

        ofstream outfile ("command");
        outfile.close();

        fstream file;
        file.open("command");

        file << _command;

        outfile.close();

        master_writing(com, _command);

        com.close();

        if (_command == "-1") break;

        usleep(1000000 * delay);
        emptyString.clear();

        while (emptyString != "&"){
            usleep(1000000 * delay);
            file.open("command");

            file >> emptyString;

            file.close();
        }

        string last_word = get_last_word("communication-register");
        evaluate_slave(_command, last_word);

        usleep(1000000 * delay);

        cout << "Enter command (-1 to terminate): " << endl;
    }

    return 0;
}

string now(){
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm *tm = std::localtime(&now_c);

    // Create a stringstream object
    std::stringstream ss;

    // Use the stringstream's operator << to format the time
    ss << std::put_time(tm, "%c");

    // Get the string from the stringstream
    return ss.str();
}

void master_writing(ofstream& com, string _command){
    com.open("communication-register", ios::app);
    com << now() << "\tMASTER LINE COMMAND: master write" << endl;
    com << now() << "\t     master writing: " << _command << endl;
    com << now() << "\tMASTER LINE COMMAND: slave write" << endl;
    com.close();
}

string get_last_word(const string& file_name) {
    std::ifstream file(file_name);
    std::string buffer;

    // Check if the file is open
    if (!file.is_open()) {
        return buffer;
    }

    file.seekg(0, std::ios::end);
    auto size = file.tellg();
    if (size == 0) {
        file.close();
        return buffer;
    }

    // Move the position to the end of the file
    file.seekg(-1, std::ios_base::end);

    // Find the beginning of the last word
    while (file.peek() != ' ' && file.tellg() > 0) {
        file.seekg(-1, std::ios_base::cur);
    }
    if (file.tellg() == 0) {
        std::getline(file, buffer);
    } else {
        file.seekg(1, std::ios_base::cur);
        std::string line;
        std::getline(file, line);
        buffer = line.substr(line.find_last_of(" ") + 1);
    }

    // Close the file
    file.close();

    return buffer;
}

void evaluate_slave(string _command, string word){
    /*if (_command == -1){
        mas
    }*/
    ifstream temp;
    if (_command.find("read-") != -1){
        _log.open("log", ios::app);

        _log << "Date: " << now() << endl << "Command: " << _command << endl;
        _log << "Output: " << endl;
        cout << "Chip info has read successfully" << endl << endl;
        cout << "Data has been stored: " << word << endl;
        _log << "Data has been stored: " << word << endl;
        _log << endl << endl;
        _log.close();
    }

    else if (_command.find("config-") != -1){
        if (word == "true"){
            _log.open("log", ios::app);

            _log << "Date: " << now() << endl << "Command: " << _command << endl;
            _log << "Output: " << endl;

            cout << "Default chip info is written to the file successfully" << endl;
            _log << "Default chip info is written to the file successfully" << endl;
            _log << endl << endl;

            _log.close();
        }
    }

    else if (_command.find("show-") != -1){
        _log.open("log", ios::app);

        _log << "Date: " << now() << endl << "Command: " << _command << endl;
        _log << "Output: " << endl;
        cout << "Data is shown:" << endl;

        temp.open("temp");

        string line;
        while (getline(temp, line)) {
            cout << line << endl;
            _log << line << endl;
        }

        cout << endl;
        cout << "Chip info is shown successfully" << endl;

        _log << endl << endl;
        cout << endl << endl;

        temp.close();
        _log.close();
        remove( "temp" );
    }

    else if (_command.find("write-") != -1){
        _log.open("log", ios::app);

        _log << "Date: " << now() << endl << "Command: " << _command << endl;
        _log << "Output: " << endl;
        cout << endl;
        cout << "Chip info is changed successfully" << endl;
        _log << "Chip info is changed successfully" << endl;

        _log << endl << endl;
        _log.close();
    }

    else
        cout << "Command is not valid." << endl;


}

void create_directories(string directories[], int size) {
    for (int i = 0; i < size; i++) {
        mkdir(directories[i].c_str(), 0777);
    }
}
