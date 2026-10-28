// CVoiceParams::SetUnprovenParameter28(float value) -- Change a stored parameter and return an integer changed flag.
// Leaf: ?SetUnprovenParameter28@CVoiceParams@@QAEHM@Z
// Identity: descriptive libpk names, no corresponding PDB name recovery claimed. Own caller00c20eb0 passes the float to field40; original parameter role not established. Manifest CGuiInputProcess::AddToInterface is wrong.
// Layout: no active shared header; untouched prefix40, own float field at40. Return is full-width0/1 as in retail.
class CVoiceParams{public:unsigned char UnmodeledPrefix[40];float UnprovenParameter28;int SetUnprovenParameter28(float);};
int CVoiceParams::SetUnprovenParameter28(float value){if(UnprovenParameter28==value)return 0;UnprovenParameter28=value;return 1;}