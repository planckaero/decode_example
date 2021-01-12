#include <iostream>
#include "mavlink.h"
#include "udp.h"
#include <signal.h>
#include <chrono>
#include <thread>

//Fail gracefully
bool stop = false;
void sigIntHandler(int signal) { stop = true; }

//Main
int main(int argc, char *argv[]) {

  signal(SIGINT, sigIntHandler);  //Handle ctrl+c

  //Handle first argument as an IP, 2nd argument as a port.  No error checking done.
  std::string send_ip("192.168.1.110");
  int sendport = 14559;
  if(argc >=2 )
    send_ip = std::string(argv[1]);
  if(argc >=3 )
    sendport = atoi(argv[2]);

  //Open the UDP socket
  UDP *udp;
  try {
    udp = new UDP(12345,100);
  } catch(...) {
    std::cerr << "Unable to open UDP port" << std::endl;
    return -1;
  }

  while(!stop) {

    //Create a STATEINFO message
    mavlink_message_t msg;

    uint64_t boot_us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    uint64_t unix_us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    mavlink_msg_planck_stateinfo_pack(
      1,                                      //"From" system ID    (1 is AP)
      1,                                      //"From" component ID (1 is AP)
      &msg,                                   //Message to write to
      PLANCK_SYS_ID,                          //Target system ID
      PLANCK_CTRL_COMP_ID,                    //Target component ID
      boot_us,                                //time_usec
      unix_us,                                //time_unix_usec
      3,                                      //mode
      0x03,                                   //status
      0.2,                                    //roll
      -0.1,                                   //pitch
      0.01,                                   //yaw
      0.2,                                    //gyro_x
      -0.1,                                   //gyro_y
      0.01,                                   //gyro_z
      1.01,                                   //xacc (North acceleration)
      2.01,                                   //yacc (East acceleration)
      3.01,                                   //zacc (Down acceleartion)
      321345678,                              //latitude (deg * 1e7)
      -1171345678,                            //longitude (deg * 1e7)
      3000,                                   //altitude (mm MSL)
      0,                                      //altitude above launch (mm)
      0,                                      //altitude above terrain (mm)
      0.1,                                    //Velocity north
      0.2,                                    //Velocity east
      0.3                                     //Velocity down
    );

    //Pack this data into a byte buffer
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    uint16_t len = mavlink_msg_to_send_buffer(buf,&msg);

    //Send it
    udp->Write((const char*)buf, (int)len, send_ip, sendport);
    std::cout << "Sent: " << boot_us << "/" << unix_us << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }

  //Shutdown
  delete udp;
}
