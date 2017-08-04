#pragma once

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include <string.h>

enum { PROGS = 1, GAIN = 0, PARAMS };

struct TBrowseProgram
{
  TBrowseProgram (){
    strcpy (name, "N/A");
  }
  char name[64];
};

class TBrowse : public AudioEffectX
{
 protected:

  TBrowseProgram* programs;
  float fs;

 public:
  TBrowse (audioMasterCallback audioMaster);
  ~TBrowse ();

  virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
  virtual void setProgram (VstInt32 program);
  virtual void setProgramName (char* name);
  virtual void getProgramName (char* name);
  virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);
  virtual void setParameter (VstInt32 index, float value);
  virtual float getParameter (VstInt32 index);
  virtual void getParameterLabel (VstInt32 index, char* label);
  virtual void getParameterDisplay (VstInt32 index, char* text);
  virtual void getParameterName (VstInt32 index, char* text);
  virtual void resume ();
  virtual bool getEffectName (char* name);
  virtual bool getVendorString (char* text);
  virtual bool getProductString (char* text);
  virtual VstInt32 getVendorVersion () { return 1024; }
  virtual VstPlugCategory getPlugCategory () { return kPlugCategEffect; }




};
