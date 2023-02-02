#include "DriverLibrary.h"

using namespace std;

fstream receiver;
string _command;
ofstream _com;

int receive_command(enum command_request req, string devType);
void throw_command();

void slave_writing(ofstream& com, string message);

int main() {

    float sleep_time = 0.2;

    cout << "driver simulator started working."<< endl;

    receiver.open("command", ios::in);
    string device;

    receiver >> device;

    cout << device << " device is being simulated." << endl;

    receiver.close();

    receiver.open("command", ios::out);
    receiver << "start";

    receiver.close();

    ofstream logFile;
    logFile.open("log");
    logFile.close();

    usleep(1000000 * sleep_time);

    while (receive_command(ONESHOT, device) != -1)
        usleep(1000000 * sleep_time);

    cout << "Program is terminating .." << endl;
    return 0;
}

int receive_command(enum command_request req, string devType) {
    string returnVal = "false";

    if (req == ONESHOT){
        receiver.open("command");
        receiver >> _command;
        receiver.close();
    }

    else if (req == RECURSIVE)
        ;

    if (_command == "-1"){
        slave_writing(_com, "disconnecting");
        return -1;
    }

    else if (_command == "&"){
        return 0;
    }

    else {
        string delimiter = "-";
        vector<string> substrings;
        stringstream ss(_command);
        string item;

        while (getline(ss, item, delimiter[0])) {
            substrings.push_back(item);
        }

        if (substrings[0] == "show") {
            string path = "dev/" + devType + "/" + substrings[1];
            char* dev_name = new char[path.length() + 1];
            strcpy(dev_name, path.c_str());

            if (devType == "gpio"){
                GPIO_Device* gpioDevHandler = new GPIO_Device(dev_name);
                returnVal = gpioDevHandler->devContent.show(gpioDevHandler);
            }
            else if (devType == "spi"){
                SPI_Device* spiDevHandler = new SPI_Device(dev_name);
                returnVal = spiDevHandler->devContent.show(spiDevHandler);
            }
            else if (devType == "i2c"){
                I2C_Device* i2cDevHandler = new I2C_Device(dev_name);
                returnVal = i2cDevHandler->devContent.show(i2cDevHandler);
            }
            else if (devType == "ethernet"){
                ETHERNET_Device* ethernetDevHandler = new ETHERNET_Device(dev_name);
                returnVal = ethernetDevHandler->devContent.show(ethernetDevHandler);
            }
            else if (devType == "usart"){
                USART_Device* usartDevHandler = new USART_Device(dev_name);
                returnVal = usartDevHandler->devContent.show(usartDevHandler);
            }
            else if (devType == "uart"){
                UART_Device* uartDevHandler = new UART_Device(dev_name);
                returnVal = uartDevHandler->devContent.show(uartDevHandler);
            }
            else if (devType == "can"){
                CAN_Device* canDevHandler = new CAN_Device(dev_name);
                returnVal = canDevHandler->devContent.show(canDevHandler);
            }

            slave_writing(_com, returnVal);

        }

        else if (substrings[0] == "read") {
            string path = "dev/" + devType + "/" + substrings[1];
            char* dev_name = new char[path.length() + 1];
            strcpy(dev_name, path.c_str());

            int offset = stoi(substrings[2]);
            string data;
            if (devType == "gpio"){
                GPIO_Device* gpioDevHandler = new GPIO_Device(dev_name);
                data = gpioDevHandler->devContent.read(offset, substrings[3], gpioDevHandler);
            }

            else if (devType == "spi"){
                SPI_Device* spiDevHandler = new SPI_Device(dev_name);
                data = spiDevHandler->devContent.read(offset, substrings[3], spiDevHandler);
            }

            else if (devType == "i2c"){
                I2C_Device* i2cDevHandler = new I2C_Device(dev_name);
                data = i2cDevHandler->devContent.read(offset, substrings[3], i2cDevHandler);
            }

            else if (devType == "ethernet"){
                ETHERNET_Device* ethDevHandler = new ETHERNET_Device(dev_name);
                data = ethDevHandler->devContent.read(offset, substrings[3], ethDevHandler);
            }

            else if (devType == "usart"){
                USART_Device* usartDevHandler = new USART_Device(dev_name);
                data = usartDevHandler->devContent.read(offset, substrings[3], usartDevHandler);
            }

            else if (devType == "uart"){
                UART_Device* uartDevHandler = new UART_Device(dev_name);
                data = uartDevHandler->devContent.read(offset, substrings[3], uartDevHandler);
            }

            else if (devType == "can"){
                CAN_Device* canDevHandler = new CAN_Device(dev_name);
                data = canDevHandler->devContent.read(offset, substrings[3], canDevHandler);
            }

            slave_writing(_com, data);

        }

        else if (substrings[0] == "write") {
            string path = "dev/" + devType + "/" + substrings[1];
            char* dev_name = new char[path.length() + 1];
            strcpy(dev_name, path.c_str());

            int offset = stoi(substrings[2]);

            if (devType == "gpio"){
                GPIO_Device* gpioDevHandler = new GPIO_Device(dev_name);
                returnVal = gpioDevHandler->devContent.write(offset, substrings[3], substrings[4], gpioDevHandler);
            }

            else if (devType == "spi"){
                SPI_Device* spiDevHandler = new SPI_Device(dev_name);
                returnVal = spiDevHandler->devContent.write(offset, substrings[3], substrings[4], spiDevHandler);            }

            else if (devType == "i2c"){
                I2C_Device* i2cDevHandler = new I2C_Device(dev_name);
                returnVal = i2cDevHandler->devContent.write(offset, substrings[3], substrings[4], i2cDevHandler);            }

            else if (devType == "ethernet"){
                ETHERNET_Device* ethDevHandler = new ETHERNET_Device(dev_name);
                returnVal = ethDevHandler->devContent.write(offset, substrings[3], substrings[4], ethDevHandler);            }

            else if (devType == "usart"){
                USART_Device* usartDevHandler = new USART_Device(dev_name);
                returnVal = usartDevHandler->devContent.write(offset, substrings[3], substrings[4], usartDevHandler);            }

            else if (devType == "uart"){
                UART_Device* uartDevHandler = new UART_Device(dev_name);
                returnVal = uartDevHandler->devContent.write(offset, substrings[3], substrings[4], uartDevHandler);            }

            else if (devType == "can"){
                CAN_Device* canDevHandler = new CAN_Device(dev_name);
                returnVal = canDevHandler->devContent.write(offset, substrings[3], substrings[4], canDevHandler);            }

            slave_writing(_com, returnVal);
        }

        else if (substrings[0] == "config") {
            string path = "dev/" + devType + "/" + substrings[1];
            char* dev_name = new char[path.length() + 1];
            strcpy(dev_name, path.c_str());

            if (devType == "gpio"){
                GPIO_Device* gpioDevHandler = new GPIO_Device(dev_name);
                returnVal = gpioDevHandler->devContent.config(DEFAULT, gpioDevHandler);
            }
            else if (devType == "spi"){
                SPI_Device* spiDevHandler = new SPI_Device(dev_name);
                returnVal = spiDevHandler->devContent.config(DEFAULT, spiDevHandler);
            }
            else if (devType == "i2c"){
                I2C_Device* i2cDevHandler = new I2C_Device(dev_name);
                returnVal = i2cDevHandler->devContent.config(DEFAULT, i2cDevHandler);
            }
            else if (devType == "ethernet"){
                ETHERNET_Device* ethernetDevHandler = new ETHERNET_Device(dev_name);
                returnVal = ethernetDevHandler->devContent.config(DEFAULT, ethernetDevHandler);
            }
            else if (devType == "usart"){
                USART_Device* usartDevHandler = new USART_Device(dev_name);
                returnVal = usartDevHandler->devContent.config(DEFAULT, usartDevHandler);
            }
            else if (devType == "uart"){
                UART_Device* uartDevHandler = new UART_Device(dev_name);
                returnVal = uartDevHandler->devContent.config(DEFAULT, uartDevHandler);
            }
            else if (devType == "can"){
                CAN_Device* canDevHandler = new CAN_Device(dev_name);
                returnVal = canDevHandler->devContent.config(DEFAULT, canDevHandler);
            }

            slave_writing(_com, returnVal);
        }

        else if (substrings[0] == ".commandSet") {

            fstream f;
            f.open(substrings[1]);

            while (true) {
                if(f.eof()) break;
                _command.clear();
                f >> _command;
                if (_command.length()  == 0)
                    break;
                receive_command(RECURSIVE, devType);
            }

        }

        else {
            slave_writing(_com, "Invalid command");
        }

    }

    throw_command();
    return 0;
}

void throw_command(){
    ofstream outfile ("command");
    outfile << "&";
    outfile.close();
}

void slave_writing(ofstream& com, string message){
    com.open("communication-register", ios::app);
    com << now() << "\t      slave writing: " << message << endl;
    com.close();
}


