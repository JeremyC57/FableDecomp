// CAIStateGroupNameTag constructor for "SG_GAZE_AT_HOME"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FDBF4, oracle length 19 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push esi ; push -1 ; push offset "SG_GAZE_AT_HOME" ; mov esi,ecx ; call CCharString::CCharString(char*,long) ; mov eax,esi ; pop esi ; ret
// The manifest labels this body `CActionDoCreatureAction::GetActionName` -- that label is
// propagated across 747 byte-identical bodies and is NOT trusted. The bytes are a constructor:
// ecx is the object, its first member (a CCharString) is built from the string literal with
// length -1 (= strlen), and `this` is returned in eax. The literal "SG_GAZE_AT_HOME" is a AI state-group name,
// so this is the no-argument constructor of a small name-tag object for that class/group.
// CCharString::CCharString(char*, long) @ 0x0099EBF0 is a trusted (landed, PASS) identity.
// Shape template (landed byte-exact): CUserProfileManager_GetAutoSaveFileName_0041a060.cpp.

struct CCharString {
    void* PStringData;                              // +0x00 CCharString::PStringData
    CCharString(const char* text, long length);     // 0x0099EBF0
};

struct CAIStateGroupNameTag {
    CCharString Name;                               // +0x00
    CAIStateGroupNameTag();
};

CAIStateGroupNameTag::CAIStateGroupNameTag()
    : Name("SG_GAZE_AT_HOME", -1)
{
}
