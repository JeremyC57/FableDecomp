// CHitParameters::AddDamageWithAttribute(EDamageAttribute attribute, float damage) -- when the damage is above -1e-4, insert (attribute, damage) into DamageMap (+0x30; _Rb_tree::insert_unique 0x004619BA, the result discarded)
// Leaf: ?AddDamageWithAttribute@CHitParameters@@QAEXW4EDamageAttribute@@M@Z
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00868CA0, length 64 bytes.
// The chunk label "CIME::SetEditBoxRegion" is propagated; named by address. Float 0x0129BA3C (1e-4f, negated with `fchs`). Shape: a 16-byte frame (the pair at +0, the iterator at +8), `add ecx,30h`, `ret 8`.
// NF61 (round 7, q10559): moved onto engine/CHitParameters.h (FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP: the real std::map<EDamageAttribute, float>
//   DamageMap +0x30 on stlport/fable_map.h; FABLE_RETAIL_CHITPARAMETERS_ADDDAMAGE: the PDB declaration). The insert now binds NF58's
//   _Rb_tree::insert_unique 0x004619BA; the self leaf is the Ego_r.pdb / FableWin.pdb spelling (enum attribute, was `long`).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
#define FABLE_RETAIL_CHITPARAMETERS_ADDDAMAGE
#include "engine/CThing.h"   // CIntelligentPointer<CThing> members need the complete CThing (retail CIntelligentPointer branch)
#include "engine/CHitParameters.h"

extern const float DELTA_NOTIONAL_ZERO;           // 0x0129BA3C (1e-4f), ?DELTA_NOTIONAL_ZERO@@3MB

void CHitParameters::AddDamageWithAttribute(EDamageAttribute attribute, float damage)
{
    if (-DELTA_NOTIONAL_ZERO < damage)
        DamageMap.insert(std::map<EDamageAttribute, float>::value_type(attribute, damage));
}
