#include "mbed.h"
#include "hcsr04.h"
#include "ESP8266.h"
#include "SDFileSystem.h"
#include  "FATFileSystem.h"
#include <stdio.h>

SDFileSystem sd(D11, D12, D13, D10, "sd");
HCSR04 sensor(PB_8, PB_9);
Serial pc(USBTX, USBRX);


//wifi UART port and baud rate
ESP8266 wifi(D8, D2, 115200); 

//buffers for wifi library
char resp[1000];
char http_cmd[300], comm[300];

int timeout = 8000; //timeout for wifi commands

//SSID and password for connection
#define SSID "wamae" 
#define PASS "wamae6719"  

//Remote IP
#define IP "184.106.153.149"

//global variable
long distance; 
void readTest2() {
    unsigned char c; // a single byte buffer
 
    sd.mount();
    printf("\r\nreadTest2() fopen /sd/DirX/sdtest.txt\r\n");
    FILE *file = fopen("/sd/DirX/sdtest.txt", "r");
 
    if (file == NULL) {
        printf("\tFailed fopen(...)\r\n");
    }
    else {
        printf("\tSuccess fopen(...)\r\n");
 
      while (c != '\n') {
            c = getc(file); 
            printf("%c\r\n",c);
      }
        fclose(file);
    }
    sd.unmount();
}
//Update key for thingspeak
char* Update_Key = "1LXHUMOQRIL6QX03";
 
//Wifi init function
void wifi_initialize(void){
    
    pc.printf("******** Resetting wifi module ********\r\n");
    wifi.Reset();
    
    //wait for 5 seconds for response, else display no response receiveed
    if (wifi.RcvReply(resp, 5000))    
        pc.printf("%s",resp);    
    else
        pc.printf("No response");
    
    pc.printf("******** Setting Station mode of wifi with AP ********\r\n");
    wifi.SetMode(1);    // set transparent  mode
    if (wifi.RcvReply(resp, timeout))    //receive a response from ESP
        pc.printf("%s",resp);    //Print the response onscreen
    else
        pc.printf("No response while setting mode. \r\n");
    
    pc.printf("******** Joining network with SSID and PASS ********\r\n");
    wifi.Join(SSID, PASS);     
    if (wifi.RcvReply(resp, timeout))    
        pc.printf("%s",resp);   
    else
        pc.printf("No response while connecting to network \r\n");
        
    pc.printf("******** Getting IP and MAC of module ********\r\n");
    wifi.GetIP(resp);     
    if (wifi.RcvReply(resp, timeout))    
        pc.printf("%s",resp);    
    else
        pc.printf("No response while getting IP \r\n");
    
    pc.printf("******** Setting WIFI UART passthrough ********\r\n");
    wifi.setTransparent();          
    if (wifi.RcvReply(resp, timeout))    
        pc.printf("%s",resp);    
    else
        pc.printf("No response while setting wifi passthrough. \r\n");
    wait(1);    
    
    pc.printf("******** Setting single connection mode ********\r\n");
    wifi.SetSingle();             
    wifi.RcvReply(resp, timeout);
    if (wifi.RcvReply(resp, timeout))    
        pc.printf("%s",resp);    
    else
        pc.printf("No response while setting single connection \r\n");
    wait(1);
}

void wifi_send(){
    sensor.start();
    wait_ms(100); 
    long distance=sensor.get_dist_cm();
    pc.printf("distance = %dcm\n",distance);
    
    pc.printf("******** Starting TCP connection on IP and port ********\r\n");
    wifi.startTCPConn(IP,80);    //cipstart
    wifi.RcvReply(resp, timeout);
    if (wifi.RcvReply(resp, timeout))    
        pc.printf("%s",resp);    
    else
        pc.printf("No response while starting TCP connection \r\n");
    wait(0.25);
    
    //create link 
    sprintf(http_cmd,"/update?api_key=%s&field1=%d",Update_Key,distance); 
    pc.printf(http_cmd);
    
    pc.printf("******** Sending URL to wifi ********\r\n");
    wifi.sendURL(http_cmd, comm);   //cipsend and get command
    if (wifi.RcvReply(resp, timeout))    
        pc.printf("%s",resp); 
        pc.printf("Testing Sd card Point");  
    else
        pc.printf("No response while sending URL \r\n");
}

void update_ThingSpeak()
{
        wifi_send();
        wait(0.25);
    
    }
    //to be called when wifi fails to initialize
void use_sdCard(){
    
    }
    int main() {
    pc.baud(115200);
    wifi_initialize();

    update_ThingSpeak();

}
