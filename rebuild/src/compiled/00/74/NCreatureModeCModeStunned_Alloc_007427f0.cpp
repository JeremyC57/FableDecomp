// NCreatureMode::CModeStunned::Alloc(CThing& thing) -- the mode registry factory: a new CModeStunned for `thing` (operator new(0x84) 0x00BFEA1A,
//   null-checked, then the constructor 0x00740160), returned as its CTCCreatureModeManager::CModeBase
// Leaf: ?Alloc@CModeStunned@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007427F0, oracle length 34 bytes.
// Retail bytes:  push esi ; push 0x84 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CModeStunned::CModeStunned ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Identity: Ego_r.pdb and FableWin.pdb ?Alloc@CModeStunned@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z (public static
//   __fastcall); its retail reference is the mode registry 0x0073B130 (abs 0x0073BB08), whose landed row already binds this spelling here. The
//   constructor is ??0CModeStunned@NCreatureMode@@QAE@AAVCThing@@@Z (Ego_r + FableWin), 0x00740160 (stores the RTTI vftable 0x01267C74
//   .?AVCModeStunned@NCreatureMode@@, ret 4), respelt to that form in the same unit (NF463).
// Naming (NF463, queue nf455 identity line 0x00740160): was the generated free function CModeStunned_Alloc(CThing*) calling the constructor as a
//   global-namespace (CThing*) form ??0CModeStunned@@QAE@PAVCThing@@@Z; same bytes.
// Model (NF463, the NF462 bM pattern, ruling r460a / r462a): CModeStunned and its direct base NCreatureMode::CModeOutOfActionBase have no shared
//   header (ghidra_out/rtti_bases.tsv: .?AVCModeStunned@NCreatureMode@@ : CModeOutOfActionBase@0 : CModeStandBase@0 : CTCCreatureModeManager::CModeBase@0),
//   so both are local classes deriving from the shared engine/CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE), over the containers-free view of
//   engine/CTCCreatureModeManager.h (FABLE_RETAIL_CTCCREATUREMODEMANAGER_NO_CONTAINERS: no /DEFAULTLIB:"libcp", r450a). Declared: the ctor, the GetType
//   override (CModeBase slot 1 is pure; the header's default unsigned long spelling) and Alloc; own members opaque. No cast: CModeBase is at offset 0.
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_NO_CONTAINERS
#define FABLE_RETAIL_CMODESTANDBASE
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
namespace NCreatureMode {
class CModeOutOfActionBase : public CModeStandBase {
public:
    CModeOutOfActionBase(CThing& thing);
private:
    unsigned char Members[0x78 - sizeof(CModeStandBase)];  // RecoveryCount +0x68, Recoverable +0x6c, PEmitter +0x6d (Ego_r.pdb; sizeof 0x78)
};
class CModeStunned : public CModeOutOfActionBase {
public:
    CModeStunned(CThing& thing);                        // 0x00740160
    virtual unsigned long GetType() const;
    static CTCCreatureModeManager::CModeBase* __fastcall Alloc(CThing& thing);
private:
    unsigned char Members[0x84 - 0x78];                 // Filter +0x78 (CAnimFilterOfGroup); sizeof(CModeStunned) = 0x84
};
CTCCreatureModeManager::CModeBase* __fastcall CModeStunned::Alloc(CThing& thing)
{
    return new CModeStunned(thing);
}
}
