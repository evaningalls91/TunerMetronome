#include "string.h"
using namespace std;
#include <IRremote.h>
const int RECV_PIN = 3;
IRrecv irrecv(RECV_PIN);
decode_results results;
#include <LiquidCrystal.h>
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//User can select specific note and frequency
int note = 0;
String notes[]={"A ", "Bb", "B ", "C ", "C#", "D ", "Eb", "E ", "F ",
"F#", "G ", "G#"};
int frequencies[]={880, 932, 988, 1047, 1109, 1172, 1245, 1319, 1397,
1480, 1568, 1661};
//User selects tempo and time signature
int bpm = 120;
int beats = 4;
//Parameter select
bool tuner = true;
bool boolTempo = true;
//Play or pause
bool playing = false;
//Tracks timing for metronome
int delayTime = 0;
unsigned long prevTime = 0;
unsigned long currTime = 0;
//Counts beats during metronome play
int beatsCount = 0;
//Stores input codes for each remote key
unsigned long codes[] =
{
//On/Off
3125149440,
//Vol+
3108437760,
//Func/Stop
3091726080,
//Back
3141861120,
//PlayPause
3208707840,
//Forward
3158572800,
//Down
4161273600,
//Vol-
3927310080,
//Up
4127850240,
//0
3910598400,
//EQ
3860463360,
//StRept
4061003520,
//1
4077715200,
//2
3877175040,
//3
2707357440,
//4
4144561920,
//5
1888238952,
//6
2774204160,
//7
3175284480,
//8
2907897600,
//9
3041591040
};
void metronomeLaunch()
{
//clear screen and reset values
lcd.clear();
bpm = 120;
beats = 4;
boolTempo = true;
playing = false;
while(true) {
//if tone enabled, play short pulses at specified bpm.
if(playing) {
//The millis() function will enable multitasking.
currTime = millis();
delayTime = (60000/bpm);
//Code counts how many beats have been played and resets at end of
measure.
//If beatsCount is zero, that note will be accented at 2x frequency.
if(currTime - prevTime >= delayTime)
{
if(beatsCount == 0)
tone (13, 1760, 100);
else
tone(13, 880, 100);
beatsCount++;
prevTime = currTime;
if (beatsCount == beats)
beatsCount = 0;
}
}
//Prints new tempo
lcd.print("Tempo: ");
lcd.setCursor(7,0);
lcd.print(bpm);
if(bpm<100)
lcd.print(" ");
//Prints new number of beats
lcd.setCursor(0,1);
lcd.print("Beats: ");
lcd.print(beats);
if(beats<10)
lcd.print(" ");
switch (input()) {
case 2:
//Return to main menu
programSelect();
break;
case 3:
//decrease beats OR tempo
if(boolTempo) {
if(bpm > 60)
bpm -= 5;
} else {
if(beats > 1)
beats-=1;
}
break;
case 4:
// play and pause
if(!playing){ }
else { }
playing = !playing;
break;
case 5:
// increase beats OR tempo
if(boolTempo) {
if(bpm < 240)
bpm += 5;
} else {
if(beats < 16)
beats+=1;
}
break;
case 6:
//select beats
boolTempo = false;
break;
case 8:
//select tempo
boolTempo = true;
break;
}
//point arrow at selected parameter
if(boolTempo){
lcd.setCursor(11,1);
lcd.print(" ");
lcd.setCursor(11,0);
} else {
lcd.setCursor(11, 0);
lcd.print(" ");
lcd.setCursor(11, 1);
}
lcd.print("<");
//print signing emoji
if(playing){
lcd.setCursor(13,0);
lcd.print("OoO");
} else {
lcd.setCursor(13,0);
lcd.print("O_O");
}
}
}
void tunerLaunch() {
//clear lcd and reset values
lcd.clear();
note = 0;
playing = false;
//Print Note: NoteName
lcd.print("Note: ");
lcd.setCursor(6,0);
lcd.print(notes[0]);
//Print singing
lcd.setCursor(13,0);
lcd.print("O_O");
//Print Freq: Frequency
lcd.setCursor(0,1);
lcd.print("Freq: ");
lcd.setCursor(6,1);
lcd.print(frequencies[0]);
lcd.print("Hz");
while(true){
//if tuner is enabled, play specified note
if (playing)
{
tone(13,frequencies[note]);
playing = false;
}
switch (input()) {
case 2:
//return to main menu
programSelect();
break;
//decrease note value
case 3:
note--;
break;
case 4:
//play and pause
playing = !playing;
break;
case 5:
//increase note value
note++;
break;
}
//Print new note
lcd.setCursor(6,0);
lcd.print(notes[note]);
//Print new frequency
lcd.setCursor(6,1);
lcd.print(frequencies[note]);
lcd.print("Hz");
//Print singing emoji
if(playing){
lcd.setCursor(13,0);
lcd.print("OoO");
} else {
lcd.setCursor(13,0);
lcd.print("O_O");
}
}
}
int input()
{
if(irrecv.decode())
{
//continue looking for input
irrecv.resume();
for (int i = 0; i < 21; i++)
{
// if data matches exsisting code in array, return that key value
if(codes[i]==irrecv.decodedIRData.decodedRawData)
{
return i;
}
}
}
}
void programSelect() {
// Main menu display
lcd.clear();
lcd.print("Tuner:");
lcd.setCursor(0,1);
lcd.print("Metronome:");
while(true) {
switch (input()) {
case 4:
//if user hits enter
if(tuner)
tunerLaunch();
else
metronomeLaunch();
break;
case 6:
// if user hits up
tuner = false;
break;
case 8:
// if user hits down
tuner = true;
break;
}
//Point arrow at selected option
if(tuner) {
//erase(11,1,3);
lcd.setCursor(14,1);
lcd.print(" ");
lcd.setCursor(14,0);
} else {
//erase(7,0,3);
lcd.setCursor(14,0);
lcd.print(" ");
lcd.setCursor(14,1);
}
lcd.print("<-");
}
}
void setup()
{
//BuzzerPin
pinMode(13, OUTPUT);
//Enable IR Receiver
irrecv.enableIRIn();
//Start LCD
lcd.begin(16, 2);
}
void loop()
{
//Go to main menu
programSelect();
}