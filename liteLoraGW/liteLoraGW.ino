#include <SPI.h>
#include <LoRa.h>
#include <Console.h>
#include <time.h>
#include <Process.h>
#include <lmic.h> //時間の取得に利用
#include <FileIO.h>

#define PROTOCOL_VERSION 1
#define PKT_PUSH_DATA 0
#define PKT_PUSH_ACK  1
#define PKT_PULL_DATA 2
#define PKT_PULL_RESP 3
#define PKT_PULL_ACK  4

#define TX_BUFF_SIZE  2048
#define STATUS_SIZE  1024

#define LORAMAC_PHY_MAXPAYLOAD 255
#define UPLINK 0
#define DOWNLINK 1
static uint16_t datasize = 10;

// MACアドレス等は、python側で管理

// define servers
// TODO: use host names and dns
char SERVER1[] = "router.au.thethings.network"; // The Things Network
unsigned int ttnPort = 1700;//ttn
// Set center frequency
uint32_t freq = 923200000;

// 位置情報
float lat=139.6315;
float lon=35.479;
int alt=0; //高さ

void sendudp(char *rssi, char *packetSize, char *freq) {
  Process p;
  String strRssi = rssi;
  String strPacketSize = packetSize;
  String strFreq = freq;

  delay(3000);
  p.begin("python");
  p.addParameter("/mnt/mtdblock3/gwstat.py");
  p.addParameter(String(SERVER1));
  p.addParameter(String(ttnPort));
  p.addParameter(strRssi);
  p.addParameter(strPacketSize);
  p.addParameter(strFreq);
  p.run();
  while(p.running());
  while (p.available()) {
    char c = p.read();
    Console.print(c);
  }
  Console.flush();
}


void sendstat() {
  // ゲートウェイステータスの送信はpythonソース側で行う
  sendudp("stat","",String((double)freq/1000000).c_str());
}


void receivepacket() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    // received a packet
    Console.print("packetSize : ");
    Console.println(packetSize);
    int rssicorr = 157;
    
    // read packet
    int i = 0;
    char message[256];
    
    while (LoRa.available() && i < 256) {
      message[i]=LoRa.read();
      i++;
    }
    FileSystem.begin();
    File dataFile = FileSystem.open("/mnt/mtdblock3/data/bin", FILE_WRITE);
    for(int j=0;j<i;j++)
        dataFile.print(message[j]);
    dataFile.close();
    delay(1000);
    //send the messages
    sendudp(String(LoRa.packetRssi()).c_str(), String(packetSize).c_str(),String((double)freq/1000000).c_str());
  }
}

void setup() {
  Bridge.begin(115200);
  Console.begin();
  while(!Console);
  // put your setup code here, to run once:
  Console.println("LoRa Receiver");

  if (!LoRa.begin(freq)) {
    Console.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSyncWord(0x34); //同期するための値
  LoRa.receive(0);        //受信状態にする
}

long lasttime;
void loop() {
  receivepacket();
  //60秒インターバル
  long nowseconds = os_getTime();

  if(nowseconds-lasttime > 6000000){
      Console.println("send stat\n");
      lasttime = nowseconds;
      sendstat();
  }
}
