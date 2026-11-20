// CTCTrapBase::OnKill() -- base OnKill, then kills the trap's obstruction thing if it is still alive
// Leaf: ?OnKill@CTCTrapBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00787D94, length 40 bytes.
// Callees: CTCBase::OnKill @ 0x006868F0 (the base override called directly; manifest "$E2" is a propagated label),
// IS_THING_ALIVE<CIntelligentPointer<CThingPhysical const>> @ 0x004CC340 (`__fastcall`, the pointer in ecx),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80.
// Shape: `add esi,24h` turns `this` into &PObstruction once (this is dead after the base call); `push 1` is evaluated
// before the object expression `PObstruction.GetPItem()`.
// Layout: CTCTrapBase.h is the PDB layout (PObstruction +0x2C); the retail constructor 0x0078918D puts the
// CIntelligentPointer at +0x24 (12-byte STLport vectors), so a local model is used.
// Naming (NF191, 2026-11-20, q4492): the row-local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared
//   engine/CIntelligentPointer.h (members CIntelligentPointer<CThing>, the Ego_r.pdb member type) and the IS_THING_ALIVE template
//   declaration (the out-of-line instance 0x004CC340, as the landed CTCInventoryWeapons rows declare it): the liveness tests bind the
//   FableWin.pdb ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV?$CIntelligentPointer@VCThing@@@@@Z. Other row-local
//   models unchanged (in place).
#include "engine/CThing.h"

class CBaseObject;


#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);   // 0x004CC340

class CTCBase { public:
    virtual void v00();
    unsigned char _pad_0x04[0x8];           // +0x04 owner thing, +0x08
    virtual void OnKill();                  // 0x006868F0
};

class CTCTrapBase : public CTCBase { public:
    unsigned char       _vectors[0x18];     // +0x0C HitThings, ImmuneThings (12-byte vectors)
    CIntelligentPointer<CThing> PObstruction;       // +0x24
    virtual void OnKill();
};

void CTCTrapBase::OnKill()
{
    CTCBase::OnKill();
    if (IS_THING_ALIVE(PObstruction))
        ((CThing*)PObstruction.GetPItem())->Kill(true);
}