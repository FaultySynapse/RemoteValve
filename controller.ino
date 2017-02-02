//usb
//#define VT_OFFSET -0.1133f
//#define VT_SLOPE 533.6638f

//bat
#define VT_OFFSET -0.1368f
#define VT_SLOPE 435.9560f

#define N_HIT_STABLE 10
#define ACT_ERROR 5

void updateValve()
{
  int error = abs(angle - target);

  if(error > ACT_ERROR){
    if(onTargetHits > N_HIT_STABLE){
      servo.attach(PWM_PIN);
    }
    onTargetHits = 0;
    servo.write(target);
  }else if(onTargetHits < N_HIT_STABLE){
    ++onTargetHits;
  }else if(onTargetHits == N_HIT_STABLE){
    ++onTargetHits;
    servo.detach();
  }
}

int getCurrentAngle()
{
  float temp = 0;
  for(int i = 0;i<10;++i){
    temp += ((float)analogRead(VP_APIN))/((float)analogRead(VS_APIN));
    delay(1);
  }
  return (temp / 10.0f + VT_OFFSET) * VT_SLOPE;
}

