// CAIStateGroup_UsesSeat::AllocChairSittingMode() const -- allocate the chair-sitting creature mode: the plain chair sit for a creature under script supervision, else a random pick of three variants (3:2:1)
// Leaf: ?AllocChairSittingMode@CAIStateGroup_UsesSeat@@MBEPAVCModeSitBase@NCreatureMode@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00938280, 224 bytes of code + a 24-byte jump table, plain `ret` (Ego_r.pdb
//   ?AllocChairSittingMode@CAIStateGroup_UsesSeat@@MBEPAVCModeSitBase@NCreatureMode@@XZ, protected virtual const).
// Callees: CThing::IsUnderScriptSupervision() const 0x004C7E90 (on the inline GetCreature()), CAIStateGroupBase::GetPWorldSeed()
//   const 0x008FCFD0, GFROR13 0x00497890 (the inline GFRandom), operator new 0x00BFEA1A (0x40 bytes each),
//   NCreatureMode::CModeSitChair(CThing&) 0x0073FEA0, CModeSitChairBored(CThing&) 0x009380E0, CModeSitChairSleeping(CThing&)
//   0x009381B0 (Ego_r.pdb ctor manglings; catalog names).
// Shape: one result pointer zeroed up front and returned once; the supervised case is the else arm (`if (!supervised) switch
//   (GFRandom(6, seed)) {...} else mode = new CModeSitChair`); the jump table maps 0-2 chair, 3-4 bored, 5 sleeping.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_SCRIPTSUPERVISION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"

long __fastcall GFROR13(unsigned long value);                       // 0x00497890 ?GFROR13@@YIJK@Z
__forceinline unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)   // ?GFRandom@@YIKKPAK@Z (inline in retail)
{
    *seed = *seed * 0x24A1 + 0x24DF;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}
namespace NCreatureMode {                                            // no shared header for the sit modes
class CModeSitBase { unsigned char _body[0x40]; };
class CModeSitChair : public CModeSitBase { public: CModeSitChair(CThing& thing); };                  // 0x0073FEA0
class CModeSitChairBored : public CModeSitBase { public: CModeSitChairBored(CThing& thing); };        // 0x009380E0
class CModeSitChairSleeping : public CModeSitBase { public: CModeSitChairSleeping(CThing& thing); };  // 0x009381B0
}
class CAIStateGroup_UsesSeat : public CAIStateGroupBase {           // no retail-branch shared header
protected:
    virtual NCreatureMode::CModeSitBase* AllocChairSittingMode() const;
};

NCreatureMode::CModeSitBase* CAIStateGroup_UsesSeat::AllocChairSittingMode() const
{
    NCreatureMode::CModeSitBase* mode = 0;
    if (!GetCreature().IsUnderScriptSupervision()) {
        switch (GFRandom(6, GetPWorldSeed())) {
        case 0:
        case 1:
        case 2:
            mode = new NCreatureMode::CModeSitChair(GetCreature());
            break;
        case 3:
        case 4:
            mode = new NCreatureMode::CModeSitChairBored(GetCreature());
            break;
        case 5:
            mode = new NCreatureMode::CModeSitChairSleeping(GetCreature());
            break;
        }
    } else
        mode = new NCreatureMode::CModeSitChair(GetCreature());
    return mode;
}