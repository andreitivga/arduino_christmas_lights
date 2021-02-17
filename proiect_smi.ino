#include <util/delay.h>
#include <IRremote.h>

const int receiver = 5;
const int speakerPin = 4;
const int releu1 = 7;
const int releu2 = 8;

IRrecv irrecv(receiver);
decode_results results;

volatile int stat = 0;
volatile int soundON = 0;
volatile int soundCheck = 1;
volatile int color = 0;
volatile int tempo;

const int length1 = 136;
const char notes1[] = "   GGggfedcGGGdcdcBGAaagfedbaggfeedcGggfedcGGGdcdcBGAaagfedbaggfeedcBcdedGbdBdccBAGABBcdedBgdgfefgagGGggfedcGGGdcdcBGAaagfedbaggfeedc   ";                     
const float beats1[] =  {1, 1, 1, 1.5, 0.5, 1.5, 0.5, 2, 2, 2, 2, 4, 1.5, 0.5, 3, 1, 3, 1, 2, 4, 2, 1.5, 0.5,\
                      2, 2, 2, 6, 1.5, 0.5, 2, 1.5, 0.5, 2, 1.5, 0.5, 6, 2, 1.5, 0.5, 2, 2, 2, 2, 4, 1.5, 0.5, \
                      3, 1, 3, 1, 2, 4, 2, 1.5, 0.5, 2, 2, 2, 6, 1.5, 0.5, 2, 1.5, 0.5, 2, 1.5, 0.5, 6, 1.5, \
                      0.5, 1.5, 0.5, 2, 2, 2, 6, 1.5, 0.5, 2, 1.5, 0.5, 2, 1.5, 0.5, 6, 1.5, 0.5, 2, 1.5, 0.5, 2, 2, \
                      8, 1.5, 0.5, 2, 2, 1.5, 0.5, 6, 1.5, 0.5, 1.5, 0.5, 2, 2, 2, 2, 4, 1.5, 0.5, 2, 1, 2, 1, \
                      2, 4, 2, 1.5, 0.5, 2, 2, 2, 6, 1.5, 0.5, 2, 1.5, 0.5, 2, 1.5, 0.5, 6, 1, 1, 1};
                      

const int length2 = 74; 
const char notes2[] = "   CbagfgafgabgagfefCbagfgafgabgagfefgabgabcgabCDEFEDCCbagfgafDDDDCbagf   ";
const int beats2[] = {1, 1, 1, \
                      3, 1, 2, 2,   2, 2, 2, 2,   1, 1, 1, 1, 3, 1,    2, 2, 4,\
                      3, 1, 2, 2,   2, 2, 2, 2,   1, 1, 1, 1, 3, 1,    2, 2, 4,\
                      3, 1, 2, 2,   3, 1, 2, 2,   1, 1, 2, 1, 1, 2,    2, 2, 4,\
                      3, 1, 2, 2,   2, 2, 2, 2,   1, 1, 1, 1, 3, 1,    2, 2, 4, \
                      1, 1, 1};
                                        


const int length3 = 57;
const char notes3[] = "   eeeeeeegcdefffffeeeeeddedgeeeeeeegcdefffffeeeeggdfc   ";                     
const int beats3[] = {1, 1, 1,\
                      2, 2, 4, 2, 2, 4, 2, 2, 3, 1, 8,\
                      2, 2, 3, 1, 2, 2, 2, 1, 1, 2, 2, 2, 2, 4, 4,\
                      2, 2, 4, 2, 2, 4, 2, 2, 3, 1, 8,\
                      2, 2, 3, 1, 2, 2, 2, 1, 1, 2, 2, 2, 2, 8,\
                      1, 1, 1};                    
                    
// {'G', 'B', 'A', 'c', 'd', 'e', 'f', 's', 'g', 'a', 'v', 'b', 'C', 'D', 'E', 'F' }; // note

void playTone(int tone, int duration)
{
 for (long i = 0; i < duration * 1000L; i += tone * 2)
 {
 digitalWrite(speakerPin, HIGH);
 delayMicroseconds(tone);
 digitalWrite(speakerPin, LOW);
 delayMicroseconds(tone);
 }
}

void playNote(char note, int duration)
{
 const char names[] = {'G', 'A', 'B', 'c', 'd', 'e', 'f', 's', 'g', 'a', 'v', 'b', 'C', 'D', 'E', 'F' }; // note
 const int tones[] = { 2551, 2272, 2032, 1915, 1700, 1519, 1432, 1352, 1275, 1136, 1073, 1014, 956, 852, 758, 716};
 
// frecvente
 for (int i = 0; i < 16; i++)
    if (names[i] == note){
      if (soundON == 1) {
        if (color == 0) {
          digitalWrite(releu1, HIGH);
          digitalWrite(releu2, LOW); 
          playTone(tones[i], duration);
          color = 1;
        }

        else {
          digitalWrite(releu1, LOW);
          digitalWrite(releu2, HIGH); 
          playTone(tones[i], duration);
          color = 0;
        }
      }

      else {
        if (color == 0) {
          digitalWrite(releu1, HIGH);
          digitalWrite(releu2, LOW); 
          delay(duration);
          color = 1;
        }

        else {
          digitalWrite(releu1, LOW);
          digitalWrite(releu2, HIGH); 
          delay(duration);
          color = 0;
        }
      }
    }
}

void check_command()
{
    if (irrecv.decode(&results)) {
            if (results.value == 0xE13DDA28) {stat = 1; tempo = 140;} //sunet1
       else if (results.value == 0xAD586662) {stat = 2; tempo = 100;} //sunet2
       else if (results.value == 0x273009C4) {stat = 3; tempo = 130;} //sunet3
       
       else if (results.value == 0xB9F56762) {stat = 0; } // turn of
       else if (results.value == 0xF4BA2988) {stat = -1;} // turn on
       
       else if (results.value == 0x2340B922) {soundON = 0; soundCheck = 0;} //diez
       else if (results.value == 0x2D25D203) {soundON = 1; soundCheck = 1;} //steluta
       irrecv.resume();
    }
}


void setup()
{
  irrecv.enableIRIn(); 
  pinMode(speakerPin, OUTPUT);
  pinMode(releu1, OUTPUT);
  pinMode(releu2, OUTPUT);
  digitalWrite(releu1, HIGH);
  digitalWrite(releu2, HIGH);
}


void loop() {
  
    check_command();
    
    if (stat == 1) {
      if (soundCheck == 1)
        soundON = 1;
      
      for (int n = 0; n < length1; n++) {
         if (notes1[n] == ' ') 
            delay(beats1[n] * tempo);
            
         else {
            playNote(notes1[n], beats1[n] * tempo); 
         }
         delay(tempo / 2);
         check_command();
         if (stat != 1) break;
      }
    }

    else if (stat == 2) {
      if (soundCheck == 1)
        soundON = 1;
      
      for (int n = 0; n < length2; n++) {
         if (notes2[n] == ' ') 
            delay(beats2[n] * tempo);
            
         else {
            playNote(notes2[n], beats2[n] * tempo); 
         }
         delay(tempo / 2);
         check_command();
         if (stat != 2) break;
      }
    }

    else if (stat == 3) {
      if (soundCheck == 1)
        soundON = 1;
      
      for (int n = 0; n < length3; n++) {
         if (notes3[n] == ' ') 
            delay(beats3[n] * tempo);
            
         else {
            playNote(notes3[n], beats3[n] * tempo); 
         }
         delay(tempo / 2);
         check_command();
         if (stat != 3) break;
      }
    }

    else if (stat == -1) { // starea de turn on
      digitalWrite(releu1, LOW);
      digitalWrite(releu2, LOW); 
    }

    else if (stat == 0) { // starea de reset total
      digitalWrite(releu1, HIGH);
      digitalWrite(releu2, HIGH); 
    }
 } 
