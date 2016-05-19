#include <SoftwareSerial.h>
#define bufferSize 100
class esp{
  Stream* serialPort;
  public:
  String response;
//  bool debug = true;
  esp(){}
  
  esp(Stream &port){
    serialPort = &port;
    response.reserve(bufferSize);
  }

  bool joinAP(String ssid, String password){
    serialPort->println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");
    return readResponse(0, 10, "OK");
  }

  bool TCPConnect(String ip, int port){
    serialPort->println("AT+CIPSTART=\"TCP\",\""+ip+"\","+String(port));
    return readResponse(2, 5, "CONNECT");
  }
  
  bool sendData(String data){
    int len = data.length();
    serialPort->println("AT+CIPSEND="+String(len));
    delay(50);
    serialPort->print(data);
    return readResponse(13, 5, "##");
  }
  
 private: 
  bool readResponse(int LinesToSkip,int Timeout, String KeyWord){
  Timeout *= 1000;
  char inChar = 0;
  response = "";
  int counter = 0;
  int LinesRead = 0;
  bool saveText;
  if(LinesToSkip == 0)
   saveText = true;
  else
    saveText = false;
  while(counter < Timeout){
    if(serialPort->available()){
      inChar = (char)serialPort->read();
      if(inChar == '\n')
        LinesRead++;
      if(saveText)
        response +=inChar;
      if(response.indexOf(KeyWord) != -1)
        return true;
      if(LinesRead >= LinesToSkip)
        saveText = true;
    }
    delay(1);
    counter++;
  }
  return false;
}
};
