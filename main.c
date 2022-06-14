#include <16F877a.h>
#include <stdio.h>

#device ADC=10

#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock = 20M)
#use RS232(BAUD=115200, XMIT = PIN_C6, RCV = PIN_C7)

#use fast_io(D)
#include <math.h>
#include <string.h>

#DEFINE LCD_RS PIN_D1
#DEFINE LCD_RW PIN_D2
#DEFINE LCD_E  PIN_D0

#DEFINE LCD_D4 PIN_D4
#DEFINE LCD_D5 PIN_D5
#DEFINE LCD_D6 PIN_D6
#DEFINE LCD_D7 PIN_D7


// Cac ham cho LCD
void LCD_Command(unsigned char c) {
    OUTPUT_d(c & 0xf0);
    OUTPUT_LOW(LCD_RS);
    OUTPUT_HIGH(LCD_E);

    DELAY_MS(1);

    OUTPUT_LOW(LCD_E);

    DELAY_MS(30);

    OUTPUT_d((c & 0x0f) << 4 );
    OUTPUT_LOW(LCD_RS);
    OUTPUT_HIGH(LCD_E);

    DELAY_MS(1);

    OUTPUT_LOW(LCD_E);

    DELAY_MS(30);
}

void LCD_Show(unsigned char d) {
    OUTPUT_d(d & 0xf0);
    OUTPUT_HIGH(LCD_RS);
    OUTPUT_HIGH(LCD_E);

    DELAY_MS(1);
    OUTPUT_LOW(LCD_E);
    DELAY_MS(30);

        OUTPUT_d((d & 0x0f) << 4 );
    OUTPUT_HIGH(LCD_RS);
    OUTPUT_HIGH(LCD_E);

    DELAY_MS(1);
    OUTPUT_LOW(LCD_E);

    DELAY_MS(30);

}
void LCD_init(void) {
    set_tris_d(0x00);
    DELAY_MS(30);

    OUTPUT_LOW(LCD_RS);
    OUTPUT_HIGH(LCD_D4);
    OUTPUT_HIGH(LCD_D5);
    OUTPUT_LOW(LCD_D6);
    OUTPUT_LOW(LCD_D7);
    OUTPUT_HIGH(LCD_E);
    DELAY_MS(1);
    OUTPUT_LOW(LCD_E);
    
    DELAY_MS(30);
    
    OUTPUT_HIGH(LCD_D4);
    OUTPUT_HIGH(LCD_D5);
    OUTPUT_LOW(LCD_D6);
    OUTPUT_LOW(LCD_D7);
    OUTPUT_HIGH(LCD_E);
    DELAY_MS(1);
    OUTPUT_LOW(LCD_E);

    DELAY_MS(30);

    OUTPUT_HIGH(LCD_D4);
    OUTPUT_HIGH(LCD_D5);
    OUTPUT_LOW(LCD_D6);
    OUTPUT_LOW(LCD_D7);
    OUTPUT_HIGH(LCD_E);
    DELAY_MS(1);
    OUTPUT_LOW(LCD_E);

    DELAY_MS(30);

    OUTPUT_HIGH(LCD_D5);
    OUTPUT_LOW(LCD_D4);
    OUTPUT_LOW(LCD_D6);
    OUTPUT_LOW(LCD_D7);
    OUTPUT_HIGH(LCD_E);
    DELAY_MS(1);
    OUTPUT_LOW(LCD_E);

    DELAY_MS(30);

    LCD_Command(0x28);  // Che do 4 bit
    LCD_Command(0x0c);  // Display On cursor OFF
    LCD_Command(0x06);  // Auto Increment cursor
    LCD_Command(0x80);  // Dua con tro ve dau dong thu nhat
}

void LCD_Cursor(char x, char y){

    if (x==0) {
        LCD_Command(0x80 + (y % 16));
        return;
    }
    if (x==1) {
    LCD_Command(0xC0 + (y % 16));
    return;
    }
    if (x==2) {
    LCD_Command(0x94 + (y % 16));
    return;
    }

    LCD_Command(0xD4 + (y % 16));
}

void LCD_Clear(void){
    LCD_Command(0x01);
    DELAY_MS(1);
}

void LCD_Print1kytu(unsigned char value) {
    LCD_Show(value);
}


void LCD_Print1String(char *s) {
    int i;
    for (i=0; i<strlen(s); i++) {
        LCD_Show(s[i]);
    }
}

//===========================================================================
char *float_to_string (float value, int lam_tron ){   
// Hien thi so thap phan LCD

    float temp_1 = 0, temp_3 = 0;
    int phan_nguyen = 0, temp_2 = 0, phan_thapphan = 0;
    unsigned char result_1[5], result_2[5], result_3[5], result_4[19];
    int count = 0, i = 0, j = 0, m = 0;

    if (value == 0) {
        LCD_Print1kytu(48); return;
    }
        
    if (value < 0) {
        LCD_Print1kytu(45); value *= (-1);
    }
        
    temp_1 = value;
    temp_2 = (int)(value);
    phan_nguyen = temp_2;
    temp_3 = temp_1 - (float)temp_2;

    // Xu ly phan nguyen
    if (phan_nguyen == 0) { 
        result_1[count] = 48;
        count++;
    }
    while (phan_nguyen) {
        result_1[count] = (phan_nguyen % 10) + 48;
        phan_nguyen /= 10;
        count++;
    }
    result_1[count] = '\0';

    int length = strlen(result_1);
    for (i = 0; i < length; i++) {
        result_2[m++] = result_1[length - i - 1];
    }
        
    result_2[m] = 46;
    result_2[m+1] = '\0';

    for (j = 0; j<lam_tron; j++) {
        temp_3 *= 10;
        float temp_4 = temp_3; 
        phan_thapphan = (int)temp_4;
        result_3[j] = phan_thapphan %10 + 48;
        temp_3 = temp_3 - phan_thapphan;
    }
    result_3[j] = '\0';
    result_4[0] = '\0';
    strcat(result_4, result_2);
    strcat(result_4, result_3);
    return result_4;
}

//===========================================================================
// Cac ham cho DHT11
#BIT Data_Pin = 0x06.0                       // Pin mapped to PORTB.0
#BIT Data_Pin_Direction = 0x86.0             // Pin direction mapped to TRISB.0
char message1[] = "Temp = 00.0 C";
char message2[] = "RH   = 00.0 %";
short Time_out;
unsigned int8 T_byte1, T_byte2, RH_byte1, RH_byte2, CheckSum ;

void start_signal() {
    Data_Pin_Direction = 0;              // Configure connection pin as output
    Data_Pin = 0;                        // Connection pin output low
    delay_ms(25);
    Data_Pin = 1;                        // Connection pin output high
    delay_us(30);
    Data_Pin_Direction = 1;              // Configure connection pin as input
}
short check_response(){
    delay_us(40);
    if(!Data_Pin){                     // Read and test if connection pin is low
        delay_us(80);
        if(Data_Pin){                    // Read and test if connection pin is high
            delay_us(50);
            return 1;}
        }
}
unsigned int8 Read_Data(){
    unsigned int8 i, k, _data = 0;     // k is used to count 1 bit reading duration
    if(Time_out)
        break;
    for(i = 0; i < 8; i++){
        k = 0;
        while(!Data_Pin){                          // Wait until pin goes high
            k++;
            if (k > 100) {Time_out = 1; break;}
            delay_us(1);
        }
        delay_us(30);
        if(!Data_Pin)
        bit_clear(_data, (7 - i));               // Clear bit (7 - i)
        else{
            bit_set(_data, (7 - i));                 // Set bit (7 - i)
            while(Data_Pin){                         // Wait until pin goes low
            k++;
            if (k > 100) {Time_out = 1; break;}
            delay_us(1);}
        }
    }
    return _data;
}

int temprh_read() {                                // Initialize LCD module
    Time_out = 0;
    Start_signal();
    if(check_response()){                    // If there is response from sensor
      RH_byte1 = Read_Data();                 // read RH byte1
      RH_byte2 = Read_Data();                 // read RH byte2
      T_byte1 = Read_Data();                  // read T byte1
      T_byte2 = Read_Data();                  // read T byte2
      Checksum = Read_Data();                 // read checksum
      if(Time_out){                           // If reading takes long time
        LCD_Cursor(0,0);                     // Go to column 5 row 1
        LCD_Print1kytu("Time out !");
        return 0;
      }
      else{
       if(CheckSum == ((RH_Byte1 + RH_Byte2 + T_Byte1 + T_Byte2) & 0xFF)){
        message1[7]  = T_Byte1/10  + 48;
        message1[8]  = T_Byte1%10  + 48;
        message1[10] = T_Byte2/10  + 48;
        message2[7]  = RH_Byte1/10 + 48;
        message2[8]  = RH_Byte1%10 + 48;
        message2[10] = RH_Byte2/10 + 48;
        message1[11] = 223;                   // Degree symbol
        LCD_Cursor(0, 0);                     // Go to column 1 row 1
        LCD_Print1String(message1);           // Display message1
        LCD_Cursor(1, 0);                     // Go to column 1 row 2
        LCD_Print1String(message2);           // Display message2
        return 1;

       }
        else {
          LCD_Cursor(0, 0);                   // Go to column 1 row 1
          LCD_Print1kytu("Sum Error!");
          return 0;
        }
      }
    }
    else {
      LCD_Clear();             // LCD clear
      LCD_Cursor(0, 0);           // Go to column 1 row 1
      LCD_Print1kytu("No response");
      return 0;
    }
  }
//===========================================================================

#BIT DIR_LED_PM = 0x86.1
#BIT DATA_LED_PM = 0x06.1             // Pin direction mapped to TRISB.0

void ADC_init() {
   setup_adc (ADC_CLOCK_INTERNAL);
   setup_adc_ports(AN0_AN1_AN2_AN3_AN4_AN5);
}
 
float ADC_read(int i) {    // doc gia tri AN0 va AN1
   float value;
   set_adc_channel (i);
   delay_us(20);
   value = read_adc();
   return value;
   }

//============================================================================
//Doc cam bien UV
float convertUV(float uvLevel, float ref){
//Do thi cua UV: 15mW/cm2 (Max) tai 3V, 0 (Min) tai 1V
   float inMin = 1;
   float inMax = 3;
   float outMin = 0.0;
   float outMax = 15.0;
   float uvVoltage = 3.3/ref * uvLevel;
   return (uvVoltage - inMin)*(outMax - outMin)/(inMax - inMin) + outMin;
}
float UV_read(void) {
    int uvLevel = ADC_read(1);
    int ref = ADC_read(3);
    return convertUV(uvLevel, ref);
}

//============================================================================
float PM_read(void) {
   float value = 0;
   OUTPUT_HIGH(DATA_LED_PM);
   delay_us(320);
   value = ADC_read(0);
   OUTPUT_LOW(DATA_LED_PM);
   delay_us(40);
   delay_us(9680);
   float result = 0.17*(value*(5.0/1024)) - 0.1;
   return result;
}
//============================================================================
// Giao tiep ESP8266

void init_esp(void) {// XANH DUONG - GND, TRANG - 3V3, TIM-RXD, XAM-TXD
   delay_ms(2000);
   printf("AT"); putc(13); putc(10);
   delay_ms(500);
   printf("ATE1"); putc(13); putc(10);
   delay_ms(500);
   printf("AT+CWMODE=3"); putc(13); putc(10);
   delay_ms(2000);
   printf("AT+CIPMUX=1"); putc(13); putc(10);
   delay_ms(2000);
   printf("AT+CWJAP=\"Quang Thai\",\"05062000\""); putc(13); putc(10);
   delay_ms(3000);
}  

void send_esp(char *humid_val, char *temp_val, char *uv_val, char *pm_val) {
// GET /getdata.php?humidity=23&temperature=15&uv=21.2348&pm=3.4578 HTTP/1.1<CR><LF>Host: thuctap203.000webhostapp.com<CR><LF><CR><LF>;
   delay_ms(4000);
   printf("AT+CIPSTART=4,\"TCP\",\"thuctap203.000webhostapp.com\",80"); putc(13); putc(10);
   delay_ms(2000);
   printf("AT+CIPSEND=4,109"); putc(13); putc(10);
   delay_ms(2000);
   printf("GET /getdata.php?");
   printf("humidity="); printf("%s", humid_val); printf("&");
   printf("temperature="); printf("%s", temp_val); printf("&");
   printf("pm="); printf("%s", pm_val); printf("&");
   printf("uv="); printf("%s", uv_val); printf(" ");
   printf("HTTP/1.1"); putc(13); putc(10);
   printf("Host: thuctap203.000webhostapp.com"); 
   putc(13); putc(10);
   putc(13); putc(10);
   delay_ms(2000);
   //printf("AT+CIPCLOSE"); putc(13); putc(10);
}      

//============================================================================

int main (void) {
   LCD_init();
   LCD_Clear();
   LCD_Cursor(0,0); LCD_Print1kytu("Dang khoi tao");
   ADC_init();
   init_esp();
   LCD_Clear();
   LCD_Cursor(2,0); LCD_Print1kytu("Dust ="); LCD_Cursor(2,14); LCD_Print1kytu("mg/m3");
   LCD_Cursor(3,0); LCD_Print1kytu("UV   ="); LCD_Cursor(3,14); LCD_Print1kytu("mW/cm2");
   int tem_rh = 0;
   char temper_out[3];
   char humid_out[3];
   char pm_out[8];
   char uv_out[8];
   while(1) {
   //Doc DHT11
      tem_rh = temprh_read();
            if (tem_rh == 1) {
            temper_out[0] = message1[7];
            temper_out[1] = message1[8];
            temper_out[2] = '\0';
            delay_ms(1000);
            humid_out[0] = message2[7];
            humid_out[1] = message2[8];
            humid_out[2] = '\0';
            delay_ms(1000);
            
            strcpy(uv_out, float_to_string(UV_read(), 2));
            LCD_Cursor(3,7); LCD_Print1String(uv_out);
            delay_ms(1000);
            strcpy(pm_out, float_to_string(PM_read(), 2));
            LCD_Cursor(2,7); LCD_Print1String(pm_out);
            
            send_esp(humid_out, temper_out, uv_out, pm_out);
            delay_ms(5000);
            }
   }
}