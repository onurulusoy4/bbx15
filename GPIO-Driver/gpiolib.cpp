#include "gpiolib.h"

GPIO_Device::GPIO_Device(const char *dev_name) {
    this->dev_name = dev_name;
}

void GPIO_Device::device_open(command request, GPIO_Device* gpioDevHandler) {


    switch (request) {

        case READONLY:
            cout << "function 'GPIO_Device::device_open' worked as READONLY" << endl;


            fd.open(dev_name, ios::in);
            //cout << fd.is_open() << endl;

            if (!fd.is_open()){
                GPIO_Device::device_open(WRITEONLY, gpioDevHandler);

                devContent.fill(DEFAULT, gpioDevHandler);

                GPIO_Device::device_close();
                GPIO_Device::device_open(READONLY, gpioDevHandler);
                break;

            }

            if (!fd.is_open())
            {
                printf("Unabled to open %s: %s\n", dev_name, strerror(errno));
                break;
            }

            else
                cout << dev_name << " is opened successfully as READONLY" <<endl;

            cout << endl;
            break;

        case WRITEONLY:
            cout << "function 'GPIO_Device::device_open' worked as WRITEONLY" << endl;


            fd.open(dev_name, ios::out | ios::trunc);

            if (!fd.is_open())
            {
                printf("Unabled to open %s: %s\n", dev_name, strerror(errno));
                break;
            }

            else
                cout << dev_name << " is opened successfully as WRITEONLY" << endl;

            cout << endl;
            break;

        default:
            break;

    }

}

void GPIO_Device::device_close() {

    cout << "function 'GPIO_Device::device_close' worked" << endl;

    if (fd.is_open()){
        cout << dev_name << " is closed successfully" << endl;
        fd.close();
    }

    else{
        //fd = 0;
        cout << dev_name << " is not open" << endl;
    }

    cout << endl;
}

void GPIO_Device::DeviceContent::fill(command request, GPIO_Device* gpioDevHandler) {

    cout << "function 'GPIO_Device::DeviceContent::fill' worked" << endl;

    if (request == DEFAULT){

        std::ifstream jsonFile("dev/default_chipInfo.json");
        nlohmann::json commands;
        jsonFile >> commands;

        nlohmann::json data = commands["gpioDevices"];
        unsigned long dataSize = data.size();

        cout << "Number of items in gpioDevices: " << data.size() << endl << endl;

        for (int i = 0; i<dataSize; i++) {


            gpioDevHandler->fd << data.at(i).value("offset", 0) << "  ";
            gpioDevHandler->fd << data.at(i).value("name", "-") << "  ";
            gpioDevHandler->fd << data.at(i).value("consumer", "-") << "  ";
            gpioDevHandler->fd << data.at(i).value("FLAG_IS_OUT", "-") << "  ";
            gpioDevHandler->fd << data.at(i).value("FLAG_ACTIVE_LOW", "-") << "  ";
            gpioDevHandler->fd << data.at(i).value("FLAG_OPEN_DRAIN", "-") << "  ";
            gpioDevHandler->fd << data.at(i).value("FLAG_OPEN_SOURCE", "-") << "  ";
            gpioDevHandler->fd << data.at(i).value("FLAG_KERNEL", "-");

            gpioDevHandler->fd << endl;

             /*
            chipxInfo[i] = {
                    .offset = data.at(i).value("offset", 0),
                    .name = data.at(i).value("name", "-"),
                    .consumer = data.at(i).value("consumer", "-"),
                    .FLAG_IS_OUT = data.at(i).value("FLAG_IS_OUT", "-"),
                    .FLAG_ACTIVE_LOW = data.at(i).value("FLAG_ACTIVE_LOW", "-"),
                    .FLAG_OPEN_DRAIN = data.at(i).value("FLAG_OPEN_DRAIN", "-"),
                    .FLAG_OPEN_SOURCE = data.at(i).value("FLAG_OPEN_SOURCE", "-"),
                    .FLAG_KERNEL = data.at(i).value("FLAG_KERNEL", "-")
            };
            */


        }

        cout << "Default chip info is written to the file successfully" << endl;

    }
}

void GPIO_Device::DeviceContent::show(GPIO_Device* gpioDevHandler){

    cout << "function 'GPIO_Device::DeviceContent::show' worked" << endl << endl;
    gpioDevHandler->fdi.open(gpioDevHandler->dev_name, ios::in);
    while (true) {
        string line, word;
        for (int i=0; i<6;i++){
            gpioDevHandler->fdi >> word;
            line += word + " ";
        }

        if( gpioDevHandler->fdi.eof() ) break;
        cout << line << endl;
    }

    cout << endl;
    cout << "Default chip info is shown successfully" << endl;
}
/*
int GPIO_Device::device_write(int offset, uint8_t value)
{
    printf("Write value %d to GPIO at offset %d (OUTPUT mode) on chip %s\n", value, offset, dev_name);

    if (fd <= 0)
        device_open();

    rq.lineoffsets[0] = offset;

    rq.flags = GPIOHANDLE_REQUEST_OUTPUT;

    rq.lines = 1;

    ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &(rq));

    device_close();

    if (ret == -1)
    {
        printf("Unable to line handle from ioctl : %s\n", strerror(errno));
        return 0;
    }

    data.values[0] = value;

    ret = ioctl(rq.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);

    if (ret == -1)
    {
        printf("Unable to set line value using ioctl : %s\n", strerror(errno));
    }

    else

    {

        usleep(2000000);

    }

    close(rq.fd);
    return 0;
}

/*
int GPIO_Device::gpio_list()
{
    if (fd <= 0)
        device_open();

    ret = ioctl(fd, GPIO_GET_CHIPINFO_IOCTL, &info);

    if (ret == -1)
    {

        printf("Unable to get chip info from ioctl: %s\n", strerror(errno));

        device_close();

        return 0;
    }

    printf("Chip name: %s\n", info.name);

    printf("Chip label: %s\n", info.label);

    printf("Number of lines: %d\n", info.lines);


    for (int i = 0; i < info.lines; i++)
    {

        line_info.line_offset = i;

        ret = ioctl(fd, GPIO_GET_LINEINFO_IOCTL, &line_info);

        if (ret == -1)
        {
            printf("Unable to get line info from offset %d: %s\n", i, strerror(errno));
        }

        else
        {

            printf("offset: %d, name: %s, consumer: %s. Flags:\t[%s]\t[%s]\t[%s]\t[%s]\t[%s]\n",

                   i,

                   line_info.name,

                   line_info.consumer,

                   (line_info.flags & GPIOLINE_FLAG_IS_OUT) ? "OUTPUT" : "INPUT",

                   (line_info.flags & GPIOLINE_FLAG_ACTIVE_LOW) ? "ACTIVE_LOW" : "ACTIVE_HIGHT",

                   (line_info.flags & GPIOLINE_FLAG_OPEN_DRAIN) ? "OPEN_DRAIN" : "...",

                   (line_info.flags & GPIOLINE_FLAG_OPEN_SOURCE) ? "OPENSOURCE" : "...",

                   (line_info.flags & GPIOLINE_FLAG_KERNEL) ? "KERNEL" : "");

        }

    }

    device_close();

    return 0;
}





int GPIO_Device::gpio_read(int offset) {

    if (fd <= 0)
        device_open();

    rq.lineoffsets[0] = offset;

    rq.flags = GPIOHANDLE_REQUEST_INPUT;

    rq.lines = 1;

    ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &rq);

    device_close();

    if (ret == -1) {
        printf("Unable to get line handle from ioctl : %s", strerror(errno));

        return 0;
    }

    ret = ioctl(rq.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);

    if (ret == -1) {
        printf("Unable to get line value using ioctl : %s", strerror(errno));
    } else {
        printf("Value of GPIO at offset %d (INPUT mode) on chip %s: %d\n", offset, dev_name, data.values[0]);
    }

    close(rq.fd);
    return 0;
}
 */