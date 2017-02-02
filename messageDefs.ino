#define SEND_BUFFER_SIZE 255

#define STATUS_MESSAGE 's'
#define PING_MESSAGE 'p'
#define CMD_MESSAGE 'c'

#define STATUS_SIZE 5
#define PING_SIZE 1
#define CMD_SIZE 2

union Messages{
  char data[SEND_BUFFER_SIZE];
  struct {
    char type;
    union{
      struct{
        byte valveAngle;
        byte valveTarget;
        byte bateryPercent;
        byte state;
      }stat;
      struct{
        byte valveTarget;
      }cmd;
    }payload;
  }message;
};
