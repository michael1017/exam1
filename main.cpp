// note
// mode : HZ : T
// 0    : 5  : 200 ms
// 1    : 10 : 100 ms
// 2    : 50 : 20  ms
#include "mbed.h"
#include "uLCD_4DGL.h"

uLCD_4DGL uLCD(D1, D0, D2);  // serial tx, serial rx, reset pin;

DigitalIn BtnSelect(D10);
DigitalIn BtnUp(D11);
DigitalIn BtnDown(D12);

DigitalOut Writing(LED2);

AnalogOut WaveOut(D13);
AnalogIn WaveIn(D7);

int nowType, nowSelect;
double increaseDelta, decreaseDelta, data[1300];
const int S = 9;
int TypeToFreq[5] = {80, 40 ,20, 10};
int wave_gen_ter = 1;
int sample_ter = 1;
int wave_gen = 0;
int sample = 0;

Thread thread1;
Thread thread2;

EventQueue eventQueue;

void uLCDControl(void);
void uLCDInit(void);
bool BtnControl(void);
void Init(void);
void WaveGen(void);
void ShowData(void);
void Reset(void);
//void WaitAnimation(void);

void NewWaveGen(void) {
    if (wave_gen_ter == 1) {
        return;
    }
    if (wave_gen > 1000) {
        wave_gen_ter = 1;
        WaveOut = 0;
        return ;
    }
    if (wave_gen_ter < TypeToFreq[nowType]) {
        WaveOut = wave_gen_ter * increaseDelta;
    } else if (wave_gen_ter < 240 - TypeToFreq[nowType]) {
        WaveOut = 1;
    } else if (wave_gen_ter < 240) {
        WaveOut = (240 - wave_gen_ter) * decreaseDelta;
    } else {
        WaveOut = 0;
        wave_gen_ter = -1;
    }
    wave_gen_ter += 1;
}
void NewSampleData(void) {
    if (sample_ter == 1) {
        return ;
    }
    if (sample > 1000) {
        sample_ter = 1;
        return ;
    }
    data[sample++] = WaveIn;
}

void triger(void) {
    wave_gen_ter = 1;
    sample_ter = 1;
}

int main(void) {
    int temp;
    Init();
    while(1) {
        Reset();
        while(1) {
            temp = BtnControl();
            if (temp) break;
        }
        
        increaseDelta = 1.0 / TypeToFreq[nowType];
        decreaseDelta = 1.0 / TypeToFreq[nowType];


        Thread eventThread(osPriorityNormal);
        eventThread.start(callback(&eventQueue, &EventQueue::dispatch_forever));
        eventQueue.call(&WaveGen);

        ThisThread::sleep_for(1s);
        Writing = 1;

        ShowData();
    }
    return 0;
}
void uLCDControl(void) {
    uLCD.locate(3,1);
    uLCD.textbackground_color(0xFFFFFF);
    uLCD.color(0x00FF00);
    uLCD.printf("1");

    uLCD.locate(3,3);
    uLCD.textbackground_color(0xFFFFFF);
    uLCD.color(0x00FF00);
    uLCD.printf("1/2");

    uLCD.locate(3,5);
    uLCD.textbackground_color(0xFFFFFF);
    uLCD.color(0x00FF00);
    uLCD.printf("1/4");

    uLCD.locate(3,7);
    uLCD.textbackground_color(0xFFFFFF);
    uLCD.color(0x00FF00);
    uLCD.printf("1/8");

    uLCD.locate(3,1 + nowType*2);
    uLCD.textbackground_color(0xFFFFFF * nowSelect);
    uLCD.color(0xFF00FF);
    if (nowType == 0) {
        uLCD.printf("1");
    } else if (nowType == 1){
        uLCD.printf("1/2");
    } else if (nowType == 2){
        uLCD.printf("1/4");
    } else {
        uLCD.printf("1/8");
    }
}
void uLCDInit(void) {
    uLCD.background_color(0xFFFFFF);
    uLCD.text_width(3); 
    uLCD.text_height(3);
    uLCD.cls();
}
bool BtnControl(void) {
    if (BtnSelect) {
        nowSelect = 1;
        uLCDControl();
    } else if (BtnDown) {
        nowType = nowType == 3 ? 0 : nowType+1;
        nowSelect = 0;
        uLCDControl();
    } else if (BtnUp) {
        nowType = nowType == 0 ? 3 : nowType-1;
        nowSelect = 0;
        uLCDControl();
    } else {
        nowSelect = 0;
    }

    return nowSelect;
}
void Init(void) {
    nowType = 1;
    nowSelect = 0;
    increaseDelta = 1.0 / TypeToFreq[nowType];
    decreaseDelta = 1.0 / TypeToFreq[nowType];
    sample = 0;
    Writing = 0;
    uLCDInit();
}
void WaveGen(void) {
    increaseDelta = 1.0 / TypeToFreq[nowType];
    decreaseDelta = 1.0 / TypeToFreq[nowType];

    while (sample < 1000) {
        for (double i=0; i<1; i+= increaseDelta) {
            WaveOut = i;
            ThisThread::sleep_for(1ms);
            data[sample++] = WaveIn;
        }
        for (int i=0; i<240 - 2*TypeToFreq[nowType]; i++) {
            ThisThread::sleep_for(1ms);
            data[sample++] = WaveIn;
        }
        for (double i=1; i>0; i-= decreaseDelta) {
            WaveOut = i;
            ThisThread::sleep_for(1ms);
            data[sample++] = WaveIn;
        }
    }
}
void ShowData(void) {
    for (int i=0; i<1000; i++) {
        printf("%lf\n", data[i]);
    }
}
void Reset(void) {
    nowSelect = 0;
    Writing = 0;
    sample = 0;
    wave_gen_ter = 1;
    sample_ter = 1;
    uLCD.cls();
    uLCDControl();
}
