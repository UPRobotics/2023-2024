//#ifndef NUEVO_PROTOCOLO__MYSERIAL_HPP_
//#define NUEVO_PROTOCOLO__MYSERIAL_HPP_


extern "C" {
#include <asm/termbits.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
}




#include <string>
#include<iostream>


#ifndef SERIAL
#define SERIAL
//


class  mySerial
{

public:

  int handle;
  std::string  deviceName;
  int baud;

  mySerial(std::string deviceName, int baud)
  {
    handle=-1;
    Open(deviceName,baud);
  }
  ~mySerial()
  {
    if(handle >=0)
        Close();
  }

  bool Send( unsigned char  * data,int len)
  {
    if(!IsOpen()) return false;
    int rlen= write(handle,data,len); 
    return(rlen == len);
  }

  bool Send(unsigned char value)
  {
    if(!IsOpen()) return false;
    int rlen= write(handle,&value,1);
    return(rlen == 1);
  }
  bool Send( std::string value)
  {
    if(!IsOpen()) return false;
    int rlen= write(handle,value.c_str(),value.size()); 
    return(rlen == value.size());
  }

  int Receive( unsigned char  * data, int len)
  {
    if(!IsOpen()){ std::cout<<"error"; return -1;  }

    // this is a blocking receives
    int lenRCV=0;
    while(lenRCV < len)
      {
        int rlen = read(handle,&data[lenRCV],len - lenRCV);
        lenRCV+=rlen;
      }
    return  lenRCV;
  }
  bool IsOpen(void)
  {
    return( handle >=0);
  }
  void Close(void)
  {
    if(handle >=0)
        close(handle);
    handle = -1;
  }
  bool Open(std::string deviceName, int baud)
  {
      struct termios tio;
      struct termios2 tio2;
      this->deviceName=deviceName;
      this->baud=baud;
      handle  = open(this->deviceName.c_str(),O_RDWR | O_NOCTTY /* | O_NONBLOCK */);

      if(handle <0)
        return false;
      tio.c_cflag =  CS8 | CLOCAL | CREAD;
      tio.c_oflag = 0;
      tio.c_lflag = 0;       //ICANON;
      tio.c_cc[VMIN]=0;
      tio.c_cc[VTIME]=1;     // time out every .1 sec
      ioctl(handle,TCSETS,&tio);

      ioctl(handle,TCGETS2,&tio2);
      tio2.c_cflag &= ~CBAUD;
      tio2.c_cflag |= BOTHER;
      tio2.c_ispeed = baud;
      tio2.c_ospeed = baud;
      ioctl(handle,TCSETS2,&tio2);

  //   flush buffer
      ioctl(handle,TCFLSH,TCIOFLUSH);

      return true;
  }

  bool NumberByteRcv(int &bytelen)
  {
    if(!IsOpen()) return false;
    ioctl(handle, FIONREAD, &bytelen);
    return true;
  }

};

#endif
