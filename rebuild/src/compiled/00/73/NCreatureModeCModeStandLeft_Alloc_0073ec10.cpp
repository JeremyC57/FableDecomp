// NCreatureMode::CModeStandLeft::Alloc(CThing& thing) -- the mode registry factory: a new CModeStandLeft for `thing` (operator new(0x84) 0x00BFEA1A,
//   null-checked, then the constructor 0x0073E410), returned as its CTCCreatureModeManager::CModeBase
// Leaf: ?Alloc@CModeStandLeft@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073EC10, oracle length 34 bytes.
// Retail bytes:  push esi ; push 0x84 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CModeStandLeft::CModeStandLeft ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CModeStandLeft@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z (public static
//   __fastcall); its retail reference is the mode registry 0x0073B130, whose landed row already binds this spelling here. The constructor is
//   ??0CModeStandLeft@NCreatureMode@@QAE@AAVCThing@@@Z (Ego_r + FableWin), 0x0073E410, whose landed row defines it.
// Naming (NF460 / NF462, queue nf453, ruling r460a): was the generated free function CModeStandLeft_Alloc(CThing*) calling the constructor as a global-namespace (CThing*) form; same bytes.
// Model (NF462, ruling r460a): CModeStandLeft has no shared header; it derives from its RTTI direct base CModeStandBase (ghidra_out/rtti_bases.tsv:
//   .?AVCModeStandLeft@NCreatureMode@@ : CModeStandBase@0 : CTCCreatureModeManager::CModeBase@0; its ctor 0x0073E410 calls CModeStandBase() 0x00861E00
//   first) from the shared engine/CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE), over the containers-free view of engine/CTCCreatureModeManager.h
//   (FABLE_RETAIL_CTCCREATUREMODEMANAGER_NO_CONTAINERS: no /DEFAULTLIB:"libcp", r450a). Declared: the ctor, the GetType override (CModeBase
//   slot 1 is pure; the header's default unsigned long spelling) and Alloc; own members opaque. No cast: CModeBase is at offset 0.
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_NO_CONTAINERS
#define FABLE_RETAIL_CMODESTANDBASE
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
namespace NCreatureMode {
class CModeStandLeft : public CModeStandBase {
public:
    CModeStandLeft(CThing& thing);                      // 0x0073E410
    virtual unsigned long GetType() const;
    static CTCCreatureModeManager::CModeBase* __fastcall Alloc(CThing& thing);
private:
    unsigned char Members[0x84 - sizeof(CModeStandBase)];   // sizeof(CModeStandLeft) = 0x84
};
CTCCreatureModeManager::CModeBase* __fastcall CModeStandLeft::Alloc(CThing& thing)
{
    return new CModeStandLeft(thing);
}
}
