// NCreatureMode::CModeJackOfBladesScream::Alloc(CThing&) -- factory: new CModeJackOfBladesScream(owner)
// Leaf: ?Alloc@CModeJackOfBladesScream@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00742880, oracle length 34 bytes.
// Retail bytes:  push esi ; push 0xbc ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CModeJackOfBladesScream::CModeJackOfBladesScream ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// A __fastcall factory: allocates sizeof(CModeJackOfBladesScream) = 0xbc bytes (operator new @ 0x00BFEA1A), null-checks the block and runs
// the constructor @ 0x00740450 on it with the argument, returning the new object (or null).
// Naming (NF157, 2026-11-20; PRIORITY q11883 / q11887): was the free stand-in ?CModeJackOfBladesScream_Alloc@@YIPAUCModeJackOfBladesScream@@PAVCThing@@@Z
//   over a global `struct CModeJackOfBladesScream` calling ??0CModeJackOfBladesScream@@QAE@PAVCThing@@@Z. Now the static member as Ego_r.pdb
//   0x00755920 and FableWin.pdb 0x024BB030 spell it (?Alloc@CModeJackOfBladesScream@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z;
//   its only landed referencer, CTCCreatureModeManager::RegisterModes 0x0073B130, already binds that), in the shape of the landed sibling
//   0x00742460 (CModeBasic::Alloc); the constructor 0x00740450 binds Ego_r.pdb 0x00754400 / FableWin.pdb 0x024BB0C0
//   ??0CModeJackOfBladesScream@NCreatureMode@@QAE@AAVCThing@@@Z. The mode class has no shared header (engine/CCreatureAction_JackScream.h only
//   forward-declares it): a header-less local model on the shared nested CTCCreatureModeManager::CModeBase (the landed 0x008C0D10 / 0x0096DBC0
//   shape), padded to the retail allocation 0xbc.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"

namespace NCreatureMode {
class CModeJackOfBladesScream : public CTCCreatureModeManager::CModeBase {   // vtable 0x01267D5C; retail sizeof 0xbc
    unsigned char _body[0xbc - 0x1c];
public:
    CModeJackOfBladesScream(CThing& owner);                                     // 0x00740450
    virtual unsigned long GetType() const;
    static CTCCreatureModeManager::CModeBase* __fastcall Alloc(CThing& owner);
};
FABLE_STATIC_ASSERT(sizeof(CModeJackOfBladesScream) == 0xbc);

CTCCreatureModeManager::CModeBase* __fastcall CModeJackOfBladesScream::Alloc(CThing& owner)
{
    return new CModeJackOfBladesScream(owner);
}
}
