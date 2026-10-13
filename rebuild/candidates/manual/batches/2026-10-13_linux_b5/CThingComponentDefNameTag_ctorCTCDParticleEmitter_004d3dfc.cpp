// CThingComponentDefNameTag constructor for "CTCDParticleEmitter"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3DFC, oracle length 19 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; push -1 ; push offset "CTCDParticleEmitter" ; mov esi,ecx ; call CCharString::CCharString(char*,long) ; mov eax,esi ; pop esi ; ret
// One of the 748 bodies the manifest labels `CActionDoCreatureAction::GetActionName` (propagated,
// not trusted): a constructor building the CCharString at `this` from the literal with length -1
// (strlen) and returning `this`. The literal is a thing-component-def class name, so this is the
// name tag for that def class. CCharString::CCharString(char*, long) @ 0x0099EBF0 is landed-trusted.
struct CCharString {
    void* PStringData;                              // +0x00 CCharString::PStringData
    CCharString(const char* text, long length);     // 0x0099EBF0
};

struct CThingComponentDefNameTag {
    CCharString Name;                               // +0x00
    CThingComponentDefNameTag();
};

CThingComponentDefNameTag::CThingComponentDefNameTag()
    : Name("CTCDParticleEmitter", -1)
{
}
