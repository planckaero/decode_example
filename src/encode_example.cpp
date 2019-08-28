#include <iostream>
#include "mavlink.h"
#include "udp.h"

//Main
int main(int argc, char *argv[]) {

  //Handle first argument as a port.  No error checking done.
  int sendport = 14560;
  if(argc >=2 )
    sendport = atoi(argv[1]);

  //Open the UDP socket
  UDP *udp;
  try {
    udp = new UDP(12345,100);
  } catch(...) {
    std::cerr << "Unable to open UDP port" << std::endl;
    return -1;
  }

  //Create a STATEINFO message
  mavlink_message_t msg;

  mavlink_msg_planck_stateinfo_pack(
    1,                                      //"From" system ID    (1 is AP)
    1,                                      //"From" component ID (1 is AP)
    &msg,                                   //Message to write to
    PLANCK_SYS_ID,                          //Target system ID
    PLANCK_CTRL_COMP_ID,                    //Target component ID
    9876554321,                             //time_usec
    98765543212,                            //time_unix_usec
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
  int sentlen = udp->Write((const char*)buf, (int)len, "127.0.0.1", sendport);

  std::cout << "Sent a mavlink message of " << sentlen << " bytes" << std::endl;

  //Shutdown
  delete udp;
}
