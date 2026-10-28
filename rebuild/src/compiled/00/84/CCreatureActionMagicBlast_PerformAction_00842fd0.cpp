// CCreatureAction_MagicBlast::PerformAction() -- with an enemy set, look up the "SHOT_SUPER_FIRE" definition index in the definition manager and, if it exists, create that shot from the creature with the creature's shot accuracy (speeds 1 / 1, no target, no combat ability, no position or frame)
// Leaf: ?PerformAction@CCreatureAction_MagicBlast@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00842FD0, 188 bytes, thiscall, `ret`. Vtable 0x0129935C slot 3.
// Identity (proven): RTTI vtable 0x0129935C (.?AVCCreatureAction_MagicBlast@@) slot 3 (PerformAction in every
//   CCreatureActionBase vtable); Ego_r.pdb / FableWin.pdb hold ?PerformAction@CCreatureAction_MagicBlast@@UAEXXZ; the literal
//   0x012743C4 is "SHOT_SUPER_FIRE".
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CCharString::CCharString(const char*, long) 0x0099EBF0,
//   CCreatureActionBase::PeekDefinitionManager() const 0x00692910 (protected; Ego_r donor order PeekDefinitionManager,
//   PeekPlayer, GetPlayer, GetWorld = retail 0x00692910/20/30/40, each forwarding to the CThing getter of the same name),
//   CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) const 0x009AD410, CCharString::~CCharString()
//   0x0099EAE0, CThingCreatureBase::GetShotAccuracyPercentage() const 0x00663B10, NThingShot::CreateShot(long, CThing*,
//   CThing*, const CThing*, float, float, float, const CDefPointer<const CMeleeCombatAbilityDef>&, const C3DVector*,
//   const CRightHandedSet*) 0x007C0AD0 (__fastcall, PDB).
// Layout: Ego_r.pdb CCreatureAction_MagicBlast adds PEnemy (CIntelligentPointer<const CThingCreatureBase>) at PDB +0xB4 =
//   retail +0xA8; PCreature +0x08 (engine/CCreatureActionBase.h). Modelled locally: the engine header is flat.
// Shape: the landed CCreatureAction_TrollProjectileAttack::PerformAction 0x008C6430 (named accuracy long, zeroed CDefPointer
//   temporary passed by reference and released after the call through CIVCountedPointeeBase slot 1).
#define CDefinitionManager CDefinitionManagerPdb
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
#define CIVCountedPointeeBase CIVCountedPointeeBasePdb
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CThingCreatureBase CThingCreatureBasePdb
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
class CBaseObject;
class C3DVector;
class CRightHandedSet;
class CMeleeCombatAbilityDef;
class CCharString {
public:
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    void* m_pStringData;
};
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                             // slot 1
    mutable unsigned long IVRefCount;                        // +0x04
    long Release() const { long result = (long)--IVRefCount; if (result == 0) Delete(); return result; }
};
template <class T> class CDefPointer {
public:
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    CIVCountedPointeeBase* Object;
};
class CDefinitionManager {
public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { };
class CThing { };
class CThingCreatureBase : public CThing {
public:
    long GetShotAccuracyPercentage() const;                  // 0x00663B10
};
namespace NThingShot {
CThing* __fastcall CreateShot(long defIndex, CThing* shooter, CThing* origin, const CThing* target, float accuracy, float a,
    float b, const CDefPointer<const CMeleeCombatAbilityDef>& ability, const C3DVector* position,
    const CRightHandedSet* frame);                           // 0x007C0AD0
}
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
    virtual void PerformAction() = 0;
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;       // +0x08
    unsigned char _pad10[0xa8 - 0x10];
protected:
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x00692910
};
class CCreatureAction_MagicBlast : public CCreatureActionBase {
public:
    virtual void PerformAction();
    CIntelligentPointer<const CThingCreatureBase> PEnemy;    // +0xA8
};

void CCreatureAction_MagicBlast::PerformAction()
{
    if (PEnemy.GetPItem()) {
        long shotIndex;
        {
            CCharString name("SHOT_SUPER_FIRE");
            shotIndex = PeekDefinitionManager().GetDefGlobalIndexFromName(name);
        }
        if (shotIndex > 0) {
            long accuracy = PCreature.GetPItem()->GetShotAccuracyPercentage();
            NThingShot::CreateShot(shotIndex, PCreature.GetPItem(), 0, 0, (float)accuracy, 1.0f, 1.0f,
                CDefPointer<const CMeleeCombatAbilityDef>(), 0, 0);
        }
    }
}