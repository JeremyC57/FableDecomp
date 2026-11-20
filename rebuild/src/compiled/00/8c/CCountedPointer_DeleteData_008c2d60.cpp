// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<NCreatureMode::CModeJackOfBladesScream>) the counted pointer's per-type deleter:
//   `delete (T*)data`, null-guarded, through the object's virtual (scalar deleting) destructor.
// Leaf: ?DeleteData@?$CCountedPointer@VCModeJackOfBladesScream@NCreatureMode@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C2D60, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member
//   (data in ecx); only its address is used: CCreatureAction_JackScream::Initialise 0x008C0D10 stores it as the count block's DeleteFunc
//   (`mov [eax+4], 0x008C2D60`, absolute reference at 0x008C0DB8) when it builds its CCountedPointer<NCreatureMode::CModeJackOfBladesScream>.
// Naming (NF157, 2026-11-20; PRIORITY q11880 / q11887): was the behavioural free stand-in ?DeleteData@@YIXPAUIData@@@Z (catalog
//   _global::DeleteViaVtable0_008c2d60). Two sources: FableWin.pdb 0x02A148C0 ?DeleteData@?$CCountedPointer@VCModeJackOfBladesScream@NCreatureMode@@@@SIXPAX@Z
//   (no Ego_r copy), and the body = engine/CCountedPointer.h DeleteData on a polymorphic T, stored by 0x008C0D10's inline (T*) constructor of
//   that exact instance (RTTI of the mode: .?AVCModeJackOfBladesScream@NCreatureMode@@, vtable 0x01267D5C). 0x008C0D10 binds this spelling in
//   the same hand-back.
// Shape: the explicit instantiation emits the header body as a pick-any COMDAT (the 0x005ECF50 pattern), so it links beside 0x008C0D10's
//   implicit COMDAT of the same instance. The mode class has no shared header (engine/CCreatureAction_JackScream.h only forward-declares it):
//   a header-less local model on the shared nested CTCCreatureModeManager::CModeBase (its virtual destructor is slot 0), the landed
//   0x008C0D10 shape.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCCreatureModeManager.h"

namespace NCreatureMode {
class CModeJackOfBladesScream : public CTCCreatureModeManager::CModeBase {   // vtable 0x01267D5C; retail sizeof 0xbc
    unsigned char _body[0xbc - 0x1c];
public:
    virtual unsigned long GetType() const;
};
}

template void FABLE_FASTCALL CCountedPointer<NCreatureMode::CModeJackOfBladesScream>::DeleteData(void*);
