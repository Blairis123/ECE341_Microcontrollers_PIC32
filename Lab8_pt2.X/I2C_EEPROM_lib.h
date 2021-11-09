#define Fsck 400000
#define BRG_VAL ((FPB/2/Fsck)-2)

// Function prototypes
void init_EEPROM();
int readEEPROM(int, int, char *, int);
int writeEEPROM(int, int, char *, int);
void wait_I2C_Xfer(int);