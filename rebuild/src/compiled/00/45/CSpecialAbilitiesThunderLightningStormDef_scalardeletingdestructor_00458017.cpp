// CSpecialAbilitiesThunderLightningStormDef::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor: the implicit ~CSpecialAbilitiesThunderLightningStormDef expanded (the members' destructors, then the out-of-line ~CParentDefClassBase 0x00430300), then operator delete when bit 0 of the flags is set
// Leaf: ??_GCSpecialAbilitiesThunderLightningStormDef@@UAEPAXI@Z
#pragma optimize("s",on)
// Retail: Fable.exe @ 0x00458017. Identity: slot 0 of vtable 0x01234814, RTTI .?AVCSpecialAbilitiesThunderLightningStormDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv; the catalog name agrees).
// Callees: CParentDefClassBase::~CParentDefClassBase 0x00430300 (Ego_r.pdb / FableWin.pdb ??1CParentDefClassBase@@UAE@XZ), operator delete 0x00BFE9BC; the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CSpecialAbilitiesThunderLightningStormDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CSpecialAbilitiesThunderLightningStormDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CParentDefClassBase.h"
#include "fable_string.h"
#include "engine/CDefString.h"
class CPersistContext;

class CSpecialAbilitiesThunderLightningStormDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    long NumBeams;
    float StartRadius;
    float EndRadius;
    float SkyBeamStartTime;
    float MainBeamsStartTime;
    float BeamRotationStartSpeed;
    float BeamRotationEndSpeed;
    float TotalLifetime;
    float Damage;
    CDefString InitSound;
    CDefString LoopingSound;
};

// Emission harness (not a retail function): constructing one emits the vftable and its deleting destructor.
CDefClassBase* EmitCSpecialAbilitiesThunderLightningStormDef()
{
    return new CSpecialAbilitiesThunderLightningStormDef;
}
