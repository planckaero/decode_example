#include <iostream>
#include "mavlink.h"
#include "udp.h"

//To nicely shutdown
bool stop = false;
void sigIntHandler(int signal) { stop = true; }

//Takes a byte buffer and creates a mavlink message, if possible
bool to_mavlink_message(const uint8_t *buf,
  const int len,
  mavlink_message_t *msg)
{
    int channel = 0;
    mavlink_status_t status;

    for(int i=0; i<len; ++i) {
      uint8_t ret = mavlink_parse_char(
        channel,
        buf[i],
        msg,
        &status);

      if(ret == MAVLINK_FRAMING_OK)
        return true;
    }
    return false;
}

//Main loop
int main(int argc, char *argv[]) {

  //Handle first argument as a port.  No error checking done.
  int recvport = 14560;
  if(argc >=2 )
    recvport = atoi(argv[1]);

  //Open the UDP socket
  UDP *udp;
  try {
    udp = new UDP(14560,100);
  } catch(...) {
    std::cerr << "Unable to open UDP port" << std::endl;
    return -1;
  }

  std::cout << "Running, waiting for data on port " << recvport << std::endl;

  while(!stop) {

    //Read a datagram
    uint8_t buf[MAVLINK_MAX_PAYLOAD_LEN];
    int readlen = udp->Read((char*)buf,MAVLINK_MAX_PAYLOAD_LEN);

    if(readlen <= 0)
      continue;


    //Attempt to convert it into a mavlink message
    mavlink_message_t msg;
    if(!to_mavlink_message(buf,readlen,&msg))
      continue;

    //Determine the message type
    if(msg.msgid == MAVLINK_MSG_ID_PLANCK_LANDING_TAG_STATE_BODY) {

      //Decode the message
      mavlink_planck_landing_tag_state_body_t b_msg;
      mavlink_msg_planck_landing_tag_state_body_decode(&msg, &b_msg);

      //Access specific members of this message type
      std::cout << "BODY: " << b_msg.age
        << " [" << b_msg.x << "," << b_msg.y << "," << b_msg.z
        << "] [" <<  b_msg.roll << "," << b_msg.pitch << "," << b_msg.yaw
        << "]" << std::endl;
    }

    //Different message
    if(msg.msgid == MAVLINK_MSG_ID_PLANCK_LANDING_TAG_STATE_NED) {

      mavlink_planck_landing_tag_state_ned_t n_msg;
      mavlink_msg_planck_landing_tag_state_ned_decode(&msg, &n_msg);

      std::cout << "NED: " <<  n_msg.age
        << " [" << n_msg.x << "," << n_msg.y << "," << n_msg.z
        << "] [" << n_msg.vx << "," << n_msg.vy << "," << n_msg.vz
        << "] [" <<  n_msg.roll << "," << n_msg.pitch << "," << n_msg.yaw
        << "]" << std::endl;
    }
  }

  //Shutdown
  delete udp;
}
