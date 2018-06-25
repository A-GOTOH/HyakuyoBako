struct Data {
  time_t epoch;
  bool crc;
  float temp;
  float humid;
  unsigned short lum;
};
struct Hyakuyo{
  unsigned long int hash;
  unsigned short cnt;
  struct Data data[25];
};
struct Hyakuyo hyakuyo;

// AM2321 Product Manualより

unsigned short crc16(unsigned char *ptr, unsigned char len)
{
  unsigned short crc = 0xFFFF;
  unsigned char i;
  while (len--)
  {

    
    crc ^= *ptr++;
    for (i = 0; i < 8; i++)
    {


      
      if (crc & 0x01)
      {
        crc >>= 1;
        crc ^= 0xA001;
      } else
      {
        crc >>= 1;
      }

    }
  }

  return crc;
}
void readAM2321(byte *rdptr, byte length ) {
  int i;
  byte  deviceaddress = 0x5C;

  //step1
  Wire.beginTransmission(deviceaddress);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(1);
  //step2
  Wire.beginTransmission(deviceaddress);
  Wire.write(0x03);
  Wire.write(0x00);
  Wire.write(0x04);
  Wire.endTransmission();
  delay(2);
  //step3
  Wire.requestFrom(deviceaddress, length);
  delayMicroseconds(60);
  if (length <= Wire.available())
  {
    for (i = 0; i < length; i++)
    {
      rdptr[i] = Wire.read();
    }
  }
}

String URLEncode(const char* msg) {
  const char *hex = "0123456789abcdef";
  String encodedMsg = "";

  while (*msg != '\0') {
    if ( ('a' <= *msg && *msg <= 'z')
         || ('A' <= *msg && *msg <= 'Z')
         || ('0' <= *msg && *msg <= '9') ) {
      encodedMsg += *msg;
    } else {
      encodedMsg += '%';
      encodedMsg += hex[*msg >> 4];
      encodedMsg += hex[*msg & 15];
    }
    msg++;
  }
  return encodedMsg;
}

String hyakuyoJSON(struct Hyakuyo hyakuyo) {
    String str = "";
    char buf[100];
    for (int i = 0; i <= hyakuyo.cnt; i++) {
      
      sprintf(buf, "\t【%d回目】UNIXタイム:%10d, CRC:%s, 気温:%5.1f, 湿度:%4.1f, 明るさ:%4d\n",
          i+1,
          hyakuyo.data[i].epoch,
          (hyakuyo.data[i].crc ? "true" : "false"),
          hyakuyo.data[i].temp,
          hyakuyo.data[i].humid,
          hyakuyo.data[i].lum);
      str += String(buf);
    }
    return str;
}

