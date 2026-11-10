// LHA::CEventPlayOptions::CEventPlayOptions() -- initialize event playback options and resource defaults
// Leaf: ??0CEventPlayOptions@LHA@@QAE@XZ
// Retail: 0x00C0A790,106 bytes,thiscall
#include "engine/LHA_CEventPlayOptions.h"
LHA::CEventPlayOptions::CEventPlayOptions():EmitterId((unsigned int)-1),UserData(0),Gain(1),Pitch(1),Is3D(false),Pan(0),HeadRelative(false),WhichParams(0),WhichCallbacks(0),WhichGainSendFaders(0),WhichPitchSendFaders(0){}