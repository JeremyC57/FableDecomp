// CCombatTypeDef::~CCombatTypeDef() -- the compiler-generated destructor: the members' destructors in reverse order (STLport vectors free their storage), then the out-of-line ~CParentDefClassBase 0x00430300 as the tail jump; no vtable store
// Leaf: ??1CCombatTypeDef@@UAE@XZ
#pragma optimize("s",on)
// Retail: Fable.exe @ 0x00455DFF. Identity: its only caller is the scalar deleting destructor in slot 0 of vtable 0x01233744,
//   RTTI .?AVCCombatTypeDef@@ : CParentDefClassBase (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??1CCombatTypeDef@@UAE@XZ.
// Callees: free 0x00BFEA14 (the STLport malloc allocator), CParentDefClassBase::~CParentDefClassBase 0x00430300 (Ego_r.pdb / FableWin.pdb ??1CParentDefClassBase@@UAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CCombatTypeDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CCombatTypeDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CParentDefClassBase.h"
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/ECombatCreatureType.h"
#include "engine/ECombatStrikeRecoilStyle.h"
class CPersistContext;

class CCombatTypeDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CCombatTypeDef();                                   // 0x00455DFF
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    std::vector<long> DefenderCombatRingOccupationNumbers;
    long DefenderWheelOuterRingStartIndex;
    bool DefenderWheelAllowAttackerStrafing;
    std::vector<CCharString> CombatSequence;
    std::vector<long> CombatSequenceDef;
    bool PreferOuterRing;
    float FrontFarLimit;
    float FrontMiddleLimit;
    float FrontNearLimit;
    float SideLimit;
    float RearLimit;
    float PreferredCombatDistance;
    std::map<ECombatCreatureType, long> AttackPatterns;
    ECombatCreatureType CombatCreatureType;
    bool AlwaysAttackIfNearest;
    bool KnockdownCausesStun;
    ECombatStrikeRecoilStyle RecoilStyle;
    bool CanBeKnockedDown;
    float ComboContinueSpeed;
    float CombatFOV;
    bool StartsAsReadyForMelee;
};

// Emission harness (not a retail function): the class declares no destructor (retail's stores no vtable); a qualified
// call makes VC7.1 emit the implicit destructor's COMDAT, as the deleting destructor's call does in retail.
void DestroyCCombatTypeDef(CCombatTypeDef* def)
{
    def->CCombatTypeDef::~CCombatTypeDef();
}
