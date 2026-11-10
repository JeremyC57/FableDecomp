// CCombatSequenceDef::CCombatSequenceDef() -- the def constructor: the out-of-line CParentDefClassBase() 0x00430370, the CCombatSequenceDef vtable, then the members' constructors
// Leaf: ??0CCombatSequenceDef@@QAE@XZ
// Retail: Fable.exe @ 0x004588A5. Identity: the vtable it stores, 0x01234BBC, is RTTI .?AVCCombatSequenceDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??0CCombatSequenceDef@@QAE@XZ.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CCombatSequenceDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CCombatSequenceDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "engine/EActionRegisteredType.h"
class CPersistContext;
enum ECombatSequenceInterruptionType { };                                   // enumerators not needed here (no engine header)
enum ECombatSequenceIsValidType { };                                   // enumerators not needed here (no engine header)
enum ECombatSequenceOnStartModuleType { };                                   // enumerators not needed here (no engine header)
enum ECombatSequenceOnStopModuleType { };                                   // enumerators not needed here (no engine header)
enum ECombatSequenceType { };                                   // enumerators not needed here (no engine header)

class CCombatSequenceDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CCombatSequenceDef();                                   // 0x004588A5
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    long ValidCombatZones;
    ECombatSequenceType Type;
    ECombatSequenceInterruptionType InterruptionType;
    ECombatSequenceIsValidType IsValidType;
    ECombatSequenceOnStartModuleType OnStartType;
    ECombatSequenceOnStopModuleType OnStopType;
    std::vector<EActionRegisteredType> ActionList;
    float SecondsToPredictTargetPosition;
    bool PreferIfValid;
};

CCombatSequenceDef::CCombatSequenceDef()
{
}
