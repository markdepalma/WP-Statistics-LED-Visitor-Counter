#include <HttpClient.h>
#include <adafruit-led-backpack.h>

unsigned int nextTime = 0;
HttpClient http;

http_header_t headers[] = {
    //  { "Content-Type", "application/json" },
    //  { "Accept" , "application/json" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

http_request_t request;
http_response_t response;

Adafruit_7segment matrix = Adafruit_7segment();

int visitorCount;
bool initialRun = true;
bool displayDot = true;
bool matrixBrigthtnessFadeDown;

//Settings
bool matrixBrightnessFade = true;
int marixFadeDelay = 100; //100 is a good starting point
int matrixBrightness = 15; //Max is 15
const int getInterval = 10000;

void setup() {
    matrix.begin(0x70);
}

void loop() {
    if (initialRun == true) {
        matrix.writeDigitNum(0, 0, true);
        matrix.writeDigitNum(1, 0, true);
        matrix.drawColon(true);
        matrix.writeDigitNum(3, 0, true);
        matrix.writeDigitNum(4, 0, true);
        matrix.writeDisplay();
        
        initialRun = false;
        nextTime = millis() + getInterval;
    }
    
    matrix.setBrightness(matrixBrightness);
    
    if (matrixBrightnessFade == true) {
        if (matrixBrigthtnessFadeDown == false) {
            if (matrixBrightness > 0) {
                matrixBrightness = matrixBrightness - 1;
            }
            else {
                matrixBrigthtnessFadeDown = true;
            }
        }
        
        if (matrixBrigthtnessFadeDown == true) {
            if (matrixBrightness < 15) {
                matrixBrightness = matrixBrightness + 1;
            }
            else {
                matrixBrigthtnessFadeDown = false;
            }
        }
        
        delay(marixFadeDelay);
    }
    
    if (nextTime > millis()) {
        return;
    }
    
    request.hostname = "blog.markdepalma.com";
    request.port = 80;
    request.path = "/_custom/visitors.php";
    
    http.get(request, response, headers);
    
    if (response.status == 200) {
        visitorCount = response.body.toInt();
        
        displayDot = !displayDot;
        
        matrix.writeDigitNum(0, (visitorCount / 1000), false);
        matrix.writeDigitNum(1, (visitorCount / 100) % 10, false);
        matrix.drawColon(false);
        matrix.writeDigitNum(3, (visitorCount / 10) % 10, false);
        matrix.writeDigitNum(4, visitorCount % 10, displayDot);
        
        matrix.writeDisplay();
    }
    
    nextTime = millis() + getInterval;
}
