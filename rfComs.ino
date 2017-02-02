#define NETWORKID 100 //same on all boards
#define NODEID 2 //feather's address
#define MASTERID 1 //other address

//n.american radio law allows us to use 915MHz for whatever
#define FREQUENCY RF69_915MHZ //legal in canada
#define IS_RFM69HCW true //rf chip model number (look on the board)

//internaly connected pins to rf chip
#define RFM69_CS   8
#define RFM69_IRQ  7
#define RFM69_IRQN 4
#define RFM69_RST  4

Messages sendBuffer;

RFM69 radio = RFM69(RFM69_CS, RFM69_IRQ, IS_RFM69HCW, RFM69_IRQN);

void ini_rf(){
  
  //hard reset the chip
  pinMode(RFM69_RST,OUTPUT);
  digitalWrite(RFM69_RST,HIGH);
  delay(100);
  digitalWrite(RFM69_RST,LOW);
  delay(100);

   //initialize radio
  radio.initialize(FREQUENCY,NODEID,NETWORKID);
  if(IS_RFM69HCW)
    radio.setHighPower();
  radio.setPowerLevel(31); //highest she goes
  radio.encrypt(ENCRYPTKEY);

  #ifdef DEBUG_VALVE
  Serial.print("\nListening at ");
  Serial.print(radio.getFrequency());
  Serial.println(" MHz");

  Serial.print("Radio send state: ");
  Serial.println(radio.canSend());
  #endif

}
bool radioPoleUpdate(){
    if(radio.receiveDone()){
      if(radio.ACKRequested()){
        sendBuffer.message.type = STATUS_MESSAGE;
        sendBuffer.message.payload.stat.valveAngle = angle;
        sendBuffer.message.payload.stat.valveTarget = target;
        sendBuffer.message.payload.stat.bateryPercent = powerlv;
        sendBuffer.message.payload.stat.state = rfTimeout < RF_TIMEOUT;

        radio.sendACK(&(sendBuffer.data), STATUS_SIZE);
      }

      Messages* reply = (Messages*)(radio.DATA);

      switch(reply->message.type){
        case 'c':
        target = reply->message.payload.cmd.valveTarget;
        break;
      }

      return true;
      
    }
    return false;
}

