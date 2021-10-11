/* Stereo peak meter example, assumes Audio adapter but just uses terminal so no more parts required.

This example code is in the public domain
*/

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            audioInput1;    //xy=139.01040649414062,124
AudioAmplifier           amp1;           //xy=295.01042556762695,223.01042366027832
AudioEffectChorus        chorus1;        //xy=387.0104064941406,281
AudioEffectFreeverb      freeverb1;      //xy=390.0104064941406,166
AudioAmplifier           amp3;           //xy=453.01041412353516,224.0104103088379
AudioMixer4              mixerOut;       //xy=611.0104064941406,141
AudioAmplifier           amp_out;           //xy=772.0104217529297,142.01043224334717
AudioOutputI2S           audioOutput1;   //xy=976.0104751586914,144.00000762939453
AudioConnection          patchCord1(audioInput1, 1, freeverb1, 0);
AudioConnection          patchCord2(audioInput1, 1, chorus1, 0);
AudioConnection          patchCord3(audioInput1, 1, mixerOut, 0);
AudioConnection          patchCord4(audioInput1, 1, amp1, 0);
AudioConnection          patchCord5(amp1, amp3);
AudioConnection          patchCord6(chorus1, 0, mixerOut, 3);
AudioConnection          patchCord7(freeverb1, 0, mixerOut, 1);
AudioConnection          patchCord8(amp3, 0, mixerOut, 2);
AudioConnection          patchCord9(mixerOut, amp_out);
AudioConnection          patchCord10(amp_out, 0, audioOutput1, 0);
AudioConnection          patchCord11(amp_out, 0, audioOutput1, 1);
AudioControlSGTL5000     audioShield;    //xy=144.01040649414062,51
// GUItool: end automatically generated code

// Available analog pins
// pins 14,16,17,22
//      A0,A2,A3,A8
//int Pot1 = A0;
//int Pot2 = A2;
//int Pot3 = A3;
//int Pot4 = A8;

// Available Encoder pins: 0,1,2,3,4,5,9,14,16,17,22
// Select FX: 0,1
// P1: 2,3
// P2: 4,5
// MIX: 14,16
// NOT USED
// LED: 9
// P3: 17,22
#include <Encoder.h>
Encoder knob_select(0, 1);
Encoder knob_mix(14, 16); // Dry=-1.0 ... 0.0=50% ... Wet=+1.0
// Encoder knob_p1(2, 3);
// Encoder knob_p2(4, 5);
// Encoder knob_p3(17, 22);

#include "OutputMixer.h"
#include "FxController.h"
#define NUMPARMS 2

OutputMixer outputMixer(mixerOut);
int fxpins[] = {2, 3, 4, 5};
FxController fxc(4, NUMPARMS, fxpins);

// Number of samples in each delay line
#define CHORUS_DELAY_LENGTH (64 * AUDIO_BLOCK_SAMPLES)
// Allocate the delay lines for left and right channels
short delayline[CHORUS_DELAY_LENGTH];

const int myInput = AUDIO_INPUT_LINEIN;
// const int myInput = AUDIO_INPUT_MIC;

const int led0 = 13; // Teensy on-board LED
const int led1 = 17; // Pot 6
const int led2 = 22; // Pot 6
//bool led1_state = true;
//bool led2_state = true;
int32_t last_select_val, last_mix_val = 0;

struct Display
{
    Display(int inPin1, int inPin2): pin1(inPin1), pin2(inPin2) 
    {
        pinMode(pin1, OUTPUT);
        digitalWrite(pin1, false); // set the LED on

        pinMode(pin2, OUTPUT);
        digitalWrite(pin2, false); // set the LED on

    }
    void set(int val)
    {
        digitalWrite(pin1, val & 0x01);
        digitalWrite(pin2, (val>>1) & 0x01);
    }

    int pin1;
    int pin2;
};
Display ledDisplay(led1, led2);


float wsArray[] = {-1.0, 0.0, 1.0};

void setup()
{
    AudioMemory(6);
    audioShield.enable();
    audioShield.inputSelect(myInput);
    // audioShield.volume(0.5);
    audioShield.lineInLevel(0);   // 3.12 Vp-p
    audioShield.lineOutLevel(13); // 3.16 Vp-p
    Serial.begin(9600);

    fxc.setActiveChannel(0);
    // FX1 for channel 0 is Master volume
    audioShield.volume(0.5);
    fxc.setParameterForChan(0, 0, 20);

    // Default FX settings
    freeverb1.roomsize(0.5);
    freeverb1.damping(0.2);
    fxc.setParameterForChan(1, 0, 0.5 * 40);
    fxc.setParameterForChan(1, 1, 0.2 * 40);

    // waveshape1.shape(wsArray, 3);
    fxc.setParameterForChan(2, 0, 4);

    // 2 = number of "voices" in the chorus which INCLUDES the original voice
    chorus1.begin(delayline, CHORUS_DELAY_LENGTH, 4);
    fxc.setParameterForChan(3, 0, 4 * 4);

    // initialize the digital pin as an output.
    pinMode(led0, OUTPUT);
    digitalWrite(led0, true); // set the LED on

    // Keep current channel wet/dry mix value
    outputMixer.setChannel(0);
    int32_t mix_val = 40 * outputMixer.getWetDryRatio();
    knob_mix.write(mix_val);
    last_mix_val = mix_val;
    outputMixer.printAllRatios();

    amp_out.gain(20);
}

elapsedMillis fps;
uint8_t cnt = 0;
void loop()
{
    if (fps > 10)
    {
        fps = 0;
        // set fx selection (counts by 4)
        int32_t raw_select_val = knob_select.read();
        int32_t select_val = int32_t(raw_select_val / 4.0) % 4; // only 4 fx channels supported
        if (select_val < 0)
        {
            select_val += 4;
        }
        if (last_select_val != select_val)
        {
            last_select_val = select_val;
            outputMixer.setChannel(select_val);
            fxc.setActiveChannel(select_val);
            Serial.print("fx select: ");
            Serial.print(select_val);
            Serial.print(", raw: ");
            Serial.println(raw_select_val);
            fxc.printAllSettings();
            digitalWrite(led0, select_val == 0); // set the LED on when no fx selected

            // Keep current channel wet/dry mix value
            int32_t mix_val = 40 * outputMixer.getWetDryRatio();
            knob_mix.write(mix_val);
            last_mix_val = mix_val;
            outputMixer.setWetDryRatio(outputMixer.getWetDryRatio());
            ledDisplay.set(select_val);
        }

        if (fxc.updateParameters())
        {
            fxc.printAllSettings();
            if (select_val == 0) // bypass
            {
                int32_t p1 = fxc.getParameter(0);
                if (p1 % 4 == 0)
                {
                    p1 /= 4;
                    p1 = constrain(p1, 0, 20.0);
                    audioShield.volume(p1/20.0);
                    fxc.setParameter(0, p1*4);
                    Serial.print("vol: ");
                    Serial.println(p1/20.0);
                }
            }
            else if (select_val == 1)
            {
                float p1 = fxc.getParameter(0) / 40.0;
                float p2 = fxc.getParameter(1) / 40.0;

                p1 = constrain(p1, 0, 1.0);
                p2 = constrain(p2, 0, 1.0);
                fxc.setParameter(0, p1*40);
                fxc.setParameter(1, p2*40);

                freeverb1.roomsize(p1);
                freeverb1.damping(p2);
                Serial.print("p1: ");
                Serial.print(p1);
                Serial.print(" p2: ");
                Serial.println(p2);
            }
            else if (select_val == 2)
            {
                int32_t p1 = fxc.getParameter(0);
                if (p1 % 4 == 0)
                {
                    p1 /= 4;
                    p1 = constrain(p1, 1, 100);
                    fxc.setParameter(0, p1*4);

                    amp1.gain(2*10*p1);
                    amp3.gain(1.0/(10*p1));
                    // Map 1 to {-1,1} and 10 to {-0.1, 0.1}
                    // wsArray[1] = p1/20;
                    // waveshape1.shape(wsArray, 3);
                    Serial.print("p1: ");
                    Serial.println(p1);

                    ledDisplay.set(p1);
                }
            }
            else if (select_val == 3)
            {
                int32_t p1 = fxc.getParameter(0);
                if (p1 % 4 == 0)
                {
                    p1 /= 4;
                    p1 = constrain(p1, 1, 8);
                    fxc.setParameter(0, p1*4);

                    chorus1.voices(p1);
                    Serial.print("p1: ");
                    Serial.println(p1);

                    ledDisplay.set(p1);
                }
            }
        }

        // set wet/dry mix
        int32_t mix_val = knob_mix.read();
        mix_val = constrain(mix_val, -40, 40); // will actually set -1.0 to 1.0
        if (mix_val % 4 == 0)
        {
            if (last_mix_val != mix_val)
            {
                last_mix_val = mix_val;
                
                float v = floor(mix_val / 40.0 * 10)/10.0;
                knob_mix.write(v*40);
                outputMixer.setWetDryRatio(v);

                Serial.print("Mix set: ");
                Serial.println(v);
                outputMixer.printAllRatios();
            
                ledDisplay.set(mix_val>>2);
            }
            else
            {
                knob_mix.write(mix_val);
            }
        }
    }
}
