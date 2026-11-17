// CAIStateGroup_NymphFloatAround::IsRunnable() -- make sure the creature has a nymph component (add "CTCNymph" when missing) and report whether it has one now
// Leaf: ?IsRunnable@CAIStateGroup_NymphFloatAround@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) 0x0091C0D0, 81 bytes, thiscall plain `ret` (virtual slot 11).
// Names: Ego_r.pdb / Ego_d.pdb ?IsRunnable@CAIStateGroup_NymphFloatAround@@UAE_NXZ (Ego_d ai_state_group_nymph_attacks.obj, first
//   function; no named locals: one expression per statement). Callees: CThing::HasTC (inline bitset test, TCI_NYMPH 245 = bit 21
//   of the dword at +0x3c), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 (the "CTCNymph" temporary),
//   CThing::AddTC 0x004C9D60. Creature through the inline CAIStateGroupBase::GetCreature (PBrain +0x18).
// Shape: matched at the first grade once ported onto the shared headers (the parked draft was a local class model).
//   The class has no shared header; it is declared over engine/CAIStateGroupBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
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
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"

class CAIStateGroup_NymphFloatAround : public CAIStateGroupBase {
public:
    virtual bool IsRunnable();
};

bool CAIStateGroup_NymphFloatAround::IsRunnable()
{
    if (!GetCreature().HasTC(TCI_NYMPH))
        GetCreature().AddTC("CTCNymph", false, 0);
    return GetCreature().HasTC(TCI_NYMPH);
}
