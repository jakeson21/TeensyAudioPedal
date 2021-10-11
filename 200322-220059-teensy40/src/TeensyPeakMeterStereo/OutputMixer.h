#ifndef OUTPUTMIXER_H
#define OUTPUTMIXER_H


#include <Audio.h>

class OutputMixer
{
public:
    OutputMixer(AudioMixer4& mixer)
    : myMixer(mixer) 
    { }
    ~OutputMixer(){}

    void bypass(bool enableBypass)
    {
        if (enableBypass)
        {
            // Set output mixer to FX bypass
            myMixer.gain(0,1.0);
            myMixer.gain(1,0.0);
            myMixer.gain(2,0.0);
            myMixer.gain(3,0.0);
        }
        else
        {
            updateMixer();
        }
    }

    void setWetDryRatio(float inRatio)
    {
        // Leave mixRatios[mixChannel=0] always -1.0
        //if (mixChannel!=0)
        //{
            inRatio = constrain(inRatio, -1.0, 1.0);
            mixRatios[mixChannel] = inRatio;
            updateMixer();
        //}
    }

    float getWetDryRatio()
    {
        return mixRatios[mixChannel];
    }

    void printAllRatios()
    {
        Serial.print("Mix: ");
        for (size_t n=0; n<4; n++)
        {
            Serial.print(mixRatios[n]);
            Serial.print(" ");
        }
        Serial.println("");
    }

    void setChannel(uint8_t inChannel)
    {
        mixChannel = constrain(inChannel, 0, 3);
        updateMixer();
    }

    void setVolume(float inVolume)
    {
        volume = constrain(inVolume, 0, 1.0);
        updateMixer();
    }

protected:
    AudioMixer4& myMixer;
    float mixRatios[4] = {-1.0, -0.0, -0.0, -0.0}; // Dry=-1 ... 0=50% ... Wet=+1
    uint8_t mixChannel = 0;
    float volume = 1.0;

    void updateMixer()
    {
        // If No FX selected, set pass through gain.
        if (mixChannel==0)
        {
            myMixer.gain(0, mixRatios[0] * volume);
            for (size_t n=1; n<4; n++)
            {
                myMixer.gain(n, 0.0);
            }
        }
        // Set FX gain
        else
        {
            for (size_t n=1; n<4; n++)
            {
                if (n==mixChannel)
                {
                    float dry, wet;
                    dry = fabsf((mixRatios[n] - 1) / 2.0);
                    wet = 1.0 - dry;
                    myMixer.gain(0, dry * volume);
                    myMixer.gain(mixChannel, wet * volume);
                    Serial.print("dry/wet: ");
                    Serial.print(dry * volume);
                    Serial.print("/");
                    Serial.println(wet * volume);
                }
                // Zero out gain for non-selected FX
                else { myMixer.gain(n, 0.0); }
            }
        }
    }
};

#endif