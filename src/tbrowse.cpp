#include <stdio.h>
#include "tbrowse.h"

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
  fprintf(stderr, "A different kind of browser. \n");
  return new TBrowse (audioMaster);
}


TBrowse::TBrowse (audioMasterCallback audioMaster)
  : AudioEffectX (audioMaster, PROGS, PARAMS)
{
  int i;
  programs = new TBrowseProgram[numPrograms];
  fs = getSampleRate();
  if(programs)setProgram(0);
  setNumInputs(1);	// 1 channel
  setNumOutputs(1);	// input & output
  setUniqueID('vic');	// this should be unique
  resume ();		// flush buffer
}


TBrowse::~TBrowse ()
{
  if (programs) delete[] programs;
}


void TBrowse::setProgram (VstInt32 program)
{
  TBrowseProgram* ap = &programs[program];
  curProgram = program;
}

void TBrowse::setProgramName (char *name)
{
  strcpy(programs[curProgram].name, name);
}

void TBrowse::resume ()
{
  AudioEffectX::resume();
}

void TBrowse::getProgramName (char *name)
{
  // Redo, not necessary..
  if (!strcmp (programs[curProgram].name, "N/A"))
    sprintf (name, "%s %d", programs[curProgram].name, curProgram + 1);
  else
    strcpy (name, programs[curProgram].name);
}


bool TBrowse::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
  if (index < PROGS)
    {
      strcpy (text, programs[index].name);
      return true;
    }
  return false;
}



void TBrowse::setParameter (VstInt32 index, float value)
{
  TBrowseProgram &ap = programs[curProgram];

  //switch (index)
}


float TBrowse::getParameter (VstInt32 index)
{
  float param = 0;

  //switch (index)

  return param;
}


void TBrowse::getParameterName (VstInt32 index, char *label)
{
  strcpy(label, "Reserved");
  //switch (index)
}


void TBrowse::getParameterDisplay (VstInt32 index, char *text)
{
  //float2string ((pitch*1.5+0.5), text, kVstMaxParamStrLen);
  //switch (index)
}

void TBrowse::getParameterLabel (VstInt32 index, char *label)
{
  strcpy (label, ".");
  //switch (index)
}

bool TBrowse::getEffectName (char* name)
{
  strcpy (name, "TBrowse");
  return true;
}

bool TBrowse::getProductString (char* text)
{
  strcpy (text, "TBrowse");
  return true;
}


bool TBrowse::getVendorString (char* text)
{
  strcpy (text, "By Viktor Ringdahl");
  return true;
}

void TBrowse::processReplacing (float** inputs, float** outputs, VstInt32 vecframes)
{
  float* in = inputs[0];
  float* out = outputs[0];

  for(int i=0; i < vecframes; i++){
    out[i] = in[i];
  }

}
