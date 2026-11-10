// CSpecialAbilitiesUnholyPowerDef::~CSpecialAbilitiesUnholyPowerDef() -- the compiler-generated destructor: the members' destructors in reverse order (STLport vectors free their storage), then the out-of-line ~CParentDefClassBase 0x00430300 as the tail jump; no vtable store
// Leaf: ??1CSpecialAbilitiesUnholyPowerDef@@UAE@XZ
#pragma optimize("s",on)
// Retail: Fable.exe @ 0x004582D6. Identity: its only caller is the scalar deleting destructor in slot 0 of vtable 0x012348E4,
//   RTTI .?AVCSpecialAbilitiesUnholyPowerDef@@ : CParentDefClassBase (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??1CSpecialAbilitiesUnholyPowerDef@@UAE@XZ.
// Callees: free 0x00BFEA14 (the STLport malloc allocator), CParentDefClassBase::~CParentDefClassBase 0x00430300 (Ego_r.pdb / FableWin.pdb ??1CParentDefClassBase@@UAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CSpecialAbilitiesUnholyPowerDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CSpecialAbilitiesUnholyPowerDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CDefString.h"
class CPersistContext;

class CSpecialAbilitiesUnholyPowerDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CSpecialAbilitiesUnholyPowerDef();                                   // 0x004582D6
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    std::vector<CDefString> StartPowerupSound;
    std::vector<CDefString> LoopingPowerupSound;
    std::vector<CDefString> ReleaseSound;
    std::vector<CDefString> ReleaseLoopSound;
    std::vector<CDefString> FinishSound;
    std::vector<CDefString> CancelSound;
    float RandomFactorForNumSecsBetweenDamageHit;
    std::vector<float> NumSecsBetweenDamageHit;
    std::vector<float> DamagePerHit;
    std::vector<float> DurationOfSpellSecs;
    std::vector<float> RadiusOfEffect;
    std::vector<float> EnvChangeSecs;
    std::vector<float> EnvReturnSecs;
    bool DoLifting;
    float VelocityOfLift;
    float DistanceToLift;
    std::vector<float> StaminaCostPerSec;
    std::vector<float> DamageTakenMultiplier;
    float SmallRingRadius;
    float LargeRingRadius;
};

// Emission harness (not a retail function): the class declares no destructor (retail's stores no vtable); a qualified
// call makes VC7.1 emit the implicit destructor's COMDAT, as the deleting destructor's call does in retail.
void DestroyCSpecialAbilitiesUnholyPowerDef(CSpecialAbilitiesUnholyPowerDef* def)
{
    def->CSpecialAbilitiesUnholyPowerDef::~CSpecialAbilitiesUnholyPowerDef();
}
