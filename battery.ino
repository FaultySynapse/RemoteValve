
float const VOLTAGE_CURVE[] = {4.2,3.9,3.825,3.75,3.7,3.65,3.6,3.575,3.5,3.4,3};
#define VCURVE_SIZE 11

byte getBatteryCharge(){
  byte def = (VCURVE_SIZE - 2)/2;
  return getBatteryCharge(def);
}

byte getBatteryChargeWD(byte &def){
  def = (VCURVE_SIZE - 2)/2;
  return getBatteryCharge(def);
}

byte getBatteryCharge(byte &i)
{
  float voltage = analogRead(A9) * 0.0064453125;
  byte iLow = 0,iHigh = VCURVE_SIZE - 2;

do{
  if(voltage < VOLTAGE_CURVE[i + 1]){
    iLow = i + 1;
  }else if(voltage > VOLTAGE_CURVE[i]){
    iHigh = i;
  }else{
    break;
  }
  i = (iLow+iHigh)/2;
}while(iLow < iHigh);

if(iLow > iHigh)return 255;

float part = (voltage - VOLTAGE_CURVE[i+1])/(VOLTAGE_CURVE[i] - VOLTAGE_CURVE[i+1]);

return 100 - 10 * i - 10.0f * part;

  
}
