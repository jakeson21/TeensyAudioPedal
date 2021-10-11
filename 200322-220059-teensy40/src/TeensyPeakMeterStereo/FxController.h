#ifndef FXCONTROLLER_H
#define FXCONTROLLER_H

#include <utility>
#include <stdlib.h>     /* malloc, free, rand */
#include <Encoder.h>

typedef Encoder* EncoderPtr;
class FxController
{
public:
    
    FxController(size_t inNumChans, size_t inNumKnobs, int* pin_pairs)
    : pChannels(nullptr), 
      numChans(inNumChans), 
      pKnobs(nullptr), 
      numParms(inNumKnobs), 
      activeFxChan(0)
    { 
        pChannels = new FxChannel[numChans];
        for (size_t n=0; n<numChans; n++)
        {
            pChannels[n].initialize(numParms);
        }

        pKnobs = new EncoderPtr[numParms];
        for (size_t n=0; n<numParms; n++)
        {
            size_t ind = 2*n;
            pKnobs[n] = new Encoder(pin_pairs[ind], pin_pairs[ind+1]);
        }
    }

    ~FxController()
    {
        delete[] pChannels;

        for (size_t n=0; n<numParms; n++)
        {
            delete pKnobs[n];
        }
        delete[] pKnobs;
    }

    void setActiveChannel(size_t channelId)
    {
        if (channelId < numChans)
        {
            activeFxChan = channelId;
            for (size_t n=0; n<numParms; n++)
            {
                // update Encoders to have current channels parameter values
                pKnobs[n]->write(pChannels[activeFxChan].parms[n]);
            }
        }
    }

    bool updateParameters()
    {
        // Read each knob and update current parm values
        bool changed = false;
        for (size_t n=0; n<numParms; n++)
        {
            int32_t val = pKnobs[n]->read();
            if (val != pChannels[activeFxChan].parms[n])
            {
                pChannels[activeFxChan].last_parms[n] = pChannels[activeFxChan].parms[n];
                pChannels[activeFxChan].parms[n] = val;
                changed = true;
            }
        }
        return changed;
    }

    int32_t getParameter(size_t n) 
    {
        if (n<numParms)
            return pChannels[activeFxChan].parms[n];
        else
        {
            return 0;
        }
    }

    bool setParameter(size_t n, int32_t val) 
    {
        if (n<numParms)
        {
            pKnobs[n]->write(val);
            pChannels[activeFxChan].last_parms[n] = val;
            pChannels[activeFxChan].parms[n] = val;
            return true;
        }
        return false;
    }

    bool setParameterForChan(size_t chan, size_t parm, int32_t val) 
    {
        if (chan < numChans && parm < numParms)
        {
            pChannels[chan].last_parms[parm] = val;
            pChannels[chan].parms[parm] = val;
            return true;
        }
        return false;
    }

    void printAllSettings()
    {
        Serial.print("parms: ");
        for (size_t n=0; n<numChans; n++)
        {
            for (size_t m=0; m<numParms; m++)
            {
                Serial.print(pChannels[n].parms[m]);
                Serial.print(" ");
            }
        }
        Serial.println("");
    }

    size_t getNumChans() const { return numChans; }
    size_t getNumFx() const { return numParms; }

protected:
    struct FxChannel {
        FxChannel() : numParms(0) {}
        ~FxChannel()
        {
            delete[] parms;
            delete[] last_parms;
        }
        void initialize(uint8_t n)
        { 
            numParms = n;
            parms = new int32_t[numParms];
            last_parms = new int32_t[numParms];
            memset(parms, 0, sizeof(int32_t)*numParms);
            memset(last_parms, 0, sizeof(int32_t)*numParms);
        }
        uint8_t numParms;
        int32_t* parms;        // stores current setting. Length of knobs.
        int32_t* last_parms;   // stores previous setting, used for change detection. Length of knobs.
    };

    FxChannel* pChannels;
    size_t numChans;
    EncoderPtr * pKnobs;
    size_t numParms;

    size_t activeFxChan;
};

#endif