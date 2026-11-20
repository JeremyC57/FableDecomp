// CCreatureAction_DragonBurninateInto::Initialise() -- animation "INTO_BURNINATE", base Initialise, then push a new CModeDragonStrafeBurninate(owner, the movement points, previous point, burn start/stop, flame start/end) onto the owner's mode manager
// Leaf: ?Initialise@CCreatureAction_DragonBurninateInto@@UAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F4320, length 218 bytes, plain `ret` (virtual, no arguments).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("INTO_BURNINATE" @ 0x012884F0), SetCombatAnimation 0x008574D0,
// CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020
// (inlined GetInterface 0x31 = CTCCreatureModeManager), operator new 0x00BFEA1A (0xc8 bytes),
// CModeDragonStrafeBurninate::CModeDragonStrafeBurninate(CThing*, const vector<C3DVector>&, const C3DVector&, float, float,
// const C3DVector&, const C3DVector&) 0x008E1580, CCountedPointer<CModeBase>::CCountedPointer(CModeBase*) 0x00718A00,
// CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) 0x0073ACC0.
// Shape: the owner argument goes through the intelligent pointer's inline `operator CThing*()` (an inline call argument is
// evaluated before the plain member arguments: `call GetPItem` precedes the pushes; a direct `.GetPItem()` is fetched last);
// the mode-manager GetInterface out-parameter form;
// (tested here, as in 0x008C1E80).
// Layout: retail PreviousPoint +0x114, MovementPoints (std::vector<C3DVector>, 12 bytes) +0x120, BurnStart +0x12c, BurnStop
// +0x130, FlameStartPos +0x134, FlameEndPos +0x140 (PDB +0x12c.. with a 16-byte vector).
// Naming (NF144, 2026-11-20; q6408): the local empty `struct CModeBase { };` and the local CTCCreatureModeManager replaced by the shared
//   engine/CTCCreatureModeManager.h (ADDMODEPOINTER, nested CModeBase); 0x0073ACC0 now binds the Ego_r.pdb
//   ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@Z (was ...QAE_NV?$CCountedPointer@UCModeBase@@@@@Z) and
//   0x00718A00 the FableWin.pdb ??0?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@QAE@PAVCModeBase@CTCCreatureModeManager@@@Z (was
//   ??0?$CCountedPointer@UCModeBase@@@@QAE@PAUCModeBase@@@Z). The header-less mode class keeps its token and its constructor spelling, now on
//   the shared CModeBase (the landed 0x0096DBC0 shape), its pad resized so sizeof stays the retail allocation.
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase : public CThing { public: };

class CBaseObject;
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return ((CThing*)GetPItem()); }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
    CThingCreatureBase* GetPItem() const { return (CThingCreatureBase*)CBaseIntelligentPointer::GetPItem(); }
};

#include "engine/C3DVector.h"
struct CVectorC3DVector {                       // std::vector<C3DVector> (retail 12 bytes)
    C3DVector* _First;
    C3DVector* _Last;
    C3DVector* _End;
};

#include "engine/CTCCreatureModeManager.h"
#include "engine/CCountedPointer.h"
struct CModeDragonStrafeBurninate : CTCCreatureModeManager::CModeBase {
    unsigned char _pad[0xc8 - 0x1c];
    CModeDragonStrafeBurninate(CThing* owner, const CVectorC3DVector& points, const C3DVector& previous, float burnStart, float burnStop,
                               const C3DVector& flameStart, const C3DVector& flameEnd);   // 0x008E1580
    virtual unsigned long GetType() const;
};

class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_DragonBurninateInto : public CCombatActionBase {
public:
    C3DVector        PreviousPoint;             // +0x114 (PDB +0x12c)
    CVectorC3DVector MovementPoints;            // +0x120 (PDB +0x138)
    float            BurnStart;                 // +0x12c (PDB +0x148)
    float            BurnStop;                  // +0x130 (PDB +0x14c)
    C3DVector        FlameStartPos;             // +0x134 (PDB +0x150)
    C3DVector        FlameEndPos;               // +0x140 (PDB +0x15c)
    virtual void Initialise();
};

void CCreatureAction_DragonBurninateInto::Initialise()
{
    {
        CCharString name("INTO_BURNINATE");
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modeManager;
    if (((CThing*)PCreature.GetPItem())->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager))
        modeManager->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(new CModeDragonStrafeBurninate(PCreature, MovementPoints, PreviousPoint,
                                                                                       BurnStart, BurnStop, FlameStartPos, FlameEndPos)));
}