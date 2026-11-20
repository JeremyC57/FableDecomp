// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Naming (NF157, 2026-11-20; PRIORITY q3943): the base-class destructor 0x008E31A0 was the free stand-in
//   ?CModeDragonFireBeam_Destruct@@YIXPAX@Z; it is now called qualified as NCreatureMode::CModeDragonHover::~CModeDragonHover
//   (FableWin.pdb ??1CModeDragonHover@NCreatureMode@@UAE@XZ 0x024BF860; the landed self row 0x008E31A0's leaf). CModeDragonHover has no
//   shared header under rebuild/include: it is modelled locally on the shared engine/CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE over
//   engine/CTCCreatureModeManager.h's nested CModeBase), the shape of the landed sibling 0x008E31A0 (members: Ego_r.pdb layout); its
//   destructor is declared only, so the qualified call stays out of line. This row's own leaf is still the free stand-in (its retail
//   name is ??_GCModeDragonFireBeam@NCreatureMode@@UAEPAXI@Z, FableWin.pdb 0x02865520; see the NF157 NAME_FIXES).
// Leaf: ?CModeDragonFireBeam_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
#pragma optimize("t", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMODESTANDBASE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CModeStandBase.h"

class CThing;

namespace NCreatureMode {
class CModeDragonHover : public CModeStandBase {           // vtable 0x0126845C; Ego_r sizeof 0x80
public:
    CModeDragonHover(CThing& thing);                        // ??0CModeDragonHover@NCreatureMode@@QAE@AAVCThing@@@Z
    virtual ~CModeDragonHover();                            // 0x008E31A0 ??1CModeDragonHover@NCreatureMode@@UAE@XZ
    virtual unsigned long GetType() const;
    CAnimFilterOfGroup          Filter;                     // +0x68
    CIntelligentPointer<CThing> PTarget;                    // +0x74
    bool                        UseHeadTurning;             // +0x7C
};
FABLE_STATIC_ASSERT(sizeof(CModeDragonHover) == 0x80);
}

void* __fastcall CModeDragonFireBeam_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(mode) + 132))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    static_cast<NCreatureMode::CModeDragonHover*>(mode)->NCreatureMode::CModeDragonHover::~CModeDragonHover();
    if (flags & 1)
        operator delete(mode);
    return mode;
}
