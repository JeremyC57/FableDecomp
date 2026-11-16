#ifndef FABLE_LIBPK_UNPROVEN_AUDIO_CHANNEL_BANK_H
#define FABLE_LIBPK_UNPROVEN_AUDIO_CHANNEL_BANK_H
#ifdef FABLE_RETAIL_AUDIO_CHANNEL_BANK_PASS23
#include "engine/CString.h"
// Descriptive names retained consistently after searches of all three PDB sets.
// Libpk has no original symbol coverage in those PDBs or the CODA SDK.
// Layout: native getters C0AF90/C0B060 index52 bytes, use floats0/4 and byte48.
// Constructor C0B200 initializes16records,8aux floats,16+8realCString names.
class CUnprovenAngularChannel {
public:
 float First,Second;
 unsigned char Unrecovered08[40];
 bool WrapAngle;
};
// Layout: native9-slot vtable12b1e84; slots below independently read in full.
// C0B200 constructor and C0A9A0 destructor prove strings364/3a4 and owner3c4.
// Method names and unavailable member names are descriptive, not PDB recoveries.
class CUnprovenAudioChannelBank {
public:
 explicit CUnprovenAudioChannelBank(void* owner);
 ~CUnprovenAudioChannelBank();
 virtual void SetChannel(unsigned int,float,float);
 virtual float ReadSecond(unsigned int);
 virtual float ReadFirst(unsigned int);
 virtual unsigned int GetChannelCount();
 virtual void SetChannelName(unsigned int,const char*);
 virtual void SetAuxiliaryValue(unsigned int,float);
 virtual float GetAuxiliaryValue(unsigned int);
 virtual unsigned int GetAuxiliaryCount();
 virtual void SetAuxiliaryName(unsigned int,const char*);
 CUnprovenAngularChannel Channels[16];
 float AuxiliaryValues[8];
 CString ChannelNames[16];
 CString AuxiliaryNames[8];
 void* Owner;
};
#endif
#endif
