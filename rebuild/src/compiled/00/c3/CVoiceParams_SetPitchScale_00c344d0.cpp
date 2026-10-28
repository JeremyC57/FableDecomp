// CVoiceParams::SetPitchScale(float value) -- Change a stored parameter and return an integer changed flag.
// Leaf: ?SetPitchScale@CVoiceParams@@QAEHM@Z
// Identity: descriptive libpk names, no corresponding PDB name recovery claimed. Own caller00c20c50 and neighboring GetPitch00c34410 multiply field56 with Pitch52. Manifest CGuiInputProcess::AddToInterface is wrong.
// Layout: no active shared header; untouched prefix56, own float field at56. Return is full-width0/1 as in retail.
class CVoiceParams{public:unsigned char UnmodeledPrefix[56];float PitchScale;int SetPitchScale(float);};
int CVoiceParams::SetPitchScale(float value){if(PitchScale==value)return 0;PitchScale=value;return 1;}