// CVoiceParams::SetGainScale(float value) -- Change a stored parameter and return an integer changed flag.
// Leaf: ?SetGainScale@CVoiceParams@@QAEHM@Z
// Identity: descriptive libpk names, no corresponding PDB name recovery claimed. Own caller00c20c80 and neighboring GetGain00c34440 multiply field48 with Gain44. Manifest CGuiInputProcess::AddToInterface is wrong.
// Layout: no active shared header; untouched prefix48, own float field at48. Return is full-width0/1 as in retail.
class CVoiceParams{public:unsigned char UnmodeledPrefix[48];float GainScale;int SetGainScale(float);};
int CVoiceParams::SetGainScale(float value){if(GainScale==value)return 0;GainScale=value;return 1;}