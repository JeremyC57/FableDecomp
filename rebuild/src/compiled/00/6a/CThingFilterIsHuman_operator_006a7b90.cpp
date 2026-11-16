// CThingFilter_IsHuman::operator()(const CThingCreatureBase&) const -- true for a creature whose def type is not NOT_HUMAN
// Leaf: ??RCThingFilter_IsHuman@@QBE_NABVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A7B90, 18 bytes: the creature's out-of-line GetCreatureType 0x00662000, then `neg ; sbb ; neg`
//   (!= NOT_HUMAN), `ret 4`.
// Name: Ego_r.pdb 0x005A3150 + FableWin.pdb 0x01F95C80 ??RCThingFilter_IsHuman@@QBE_NABVCThingCreatureBase@@@Z; callee both PDBs
//   ?GetCreatureType@CThingCreatureBase@@QBE?AW4ECreatureType@@XZ (engine/CThingCreatureBase.h CREATURETYPE); the PDB inline
//   CThingCreatureBase::IsHuman (Ego_r 0x0053CD90) is that test. Landed binders 0x007D4CE0 / 0x00943830 bind this symbol.
//   No shared header for the filter; modelled here. NF33 2026-11-16: was long operator()(CThingCreatureBaseCalls*).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "engine/ECreatureType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
class CThingFilter_IsHuman {
public:
    bool operator()(const CThingCreatureBase& creature) const;
};
bool CThingFilter_IsHuman::operator()(const CThingCreatureBase& creature) const
{
    return creature.GetCreatureType() != NOT_HUMAN;
}
