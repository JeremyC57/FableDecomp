#pragma optimize("s",on)
// BeginInputLoading — forwards to a member of a fixed global object with a fixed
// name argument. __stdcall, one stack arg (ret 4); ecx comes from the global, so the
// function ignores any incoming this.
class CVersion;
struct CLoader { void Begin_009f5e10(const CVersion& version, int arg); };
class CInputManager;
extern CInputManager* GPInputManager;                   // 0x013B8388 ?GPInputManager@@3PAVCInputManager@@A
extern CVersion GVersion;  // 0x013B85E4 ?GVersion@@3VCVersion@@A (Ego_d.pdb public; initializer 0x01219F40)
void __stdcall BeginInputLoading(int arg) { ((CLoader*)GPInputManager)->Begin_009f5e10(GVersion, arg); }