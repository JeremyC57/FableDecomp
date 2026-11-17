// CHitParameters::ClearAllDamage() -- empty the damage map (+0x30)
// Leaf: ?ClearAllDamage@CHitParameters@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00868A40, 52 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb 0x0083EC50 / FableWin.pdb 0x02810E60 ?ClearAllDamage@CHitParameters@@QAEXXZ; it sits among CHitParameters' damage
//   methods (HasDamageAttribute 0x008688C0, AddDamageWithAttribute 0x00868CA0) and CTCCombat::SetStruck(..., CHitParameters&) 0x00734C60 calls
//   it on its hit parameters; Ego_r.pdb CHitParameters::DamageMap is map<EDamageAttribute, float> at +0x30.
// Callees: the map's tree _M_erase 0x00450903 (STLport private member, AAE); the clear itself is STLport's inline map/tree clear.
// Port (NF81, round 7, q10986 binder): the local std (allocator / less / pair / _Rb_tree / map / set) and CHitParameters models are gone; the
//   row is on engine/CHitParameters.h (DAMAGEMAP: std::map<EDamageAttribute,float> from stlport/fable_map.h; the 0x00812170 define block) with
//   the NF81 overlay opt-in FABLE_RETAIL_CHITPARAMETERS_CLEARALLDAMAGE (the PDB declaration). The tree's _M_erase is now bound under its
//   STLport private spelling (the local model had declared it public: QAE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
#define FABLE_RETAIL_CHITPARAMETERS_CLEARALLDAMAGE   // NF81 overlay: ?ClearAllDamage@CHitParameters@@QAEXXZ
#include "fable_compiler.h"
#include <stddef.h>
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CMaterialDef.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CHitParameters.h"

void CHitParameters::ClearAllDamage()
{
    DamageMap.clear();
}
