// CCreatureAction_DragonTakeOffIntoFlying::FinishAction() -- switch the dragon's physics to flying, remove its 0x4D component,
// drop it from the hero's 0x4D list, lift it 31.15 units, push that point to the front of MovementPoints and, when it has
// a creature-mode manager, remember the first point and add a CModeDragonFlying (StartPos, speed 1.0, PEndAction); then the
// base FinishAction
// Leaf: ?FinishAction@CCreatureAction_DragonTakeOffIntoFlying
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Naming (NF144, 2026-11-20; q6408): the local CModeBase / CCountedModePointer / CTCCreatureModeManager stand-ins replaced by the shared
//   engine/CTCCreatureModeManager.h (ADDMODEPOINTER, nested CModeBase) + engine/CCountedPointer.h (CALLED_CTOR); 0x0073ACC0 now binds the Ego_r.pdb
//   ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@Z (was ...QAE_NUCCountedModePointer@@@Z) and
//   0x00718A00 the FableWin.pdb ??0?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@QAE@PAVCModeBase@CTCCreatureModeManager@@@Z (was
//   ??0CCountedModePointer@@QAE@PAUCModeBase@@@Z). The header-less mode class keeps its token and its constructor spelling, now on the shared
//   CModeBase (the landed 0x0096DBC0 shape).
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#include "fable_compiler.h"
class CTCBase;
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"                        // CTCBase interface type (the shared enum: CTCCreatureModeManager.h needs TCI_ENTITY_MODE_MANAGER)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CPhysicsBody {                                 // the thing's physics body at +0x60
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF(30)
    virtual void SetPosition(const C3DVector& position);   // slot 31 (+0x7c)
    VF(32) VF(33) VF(34) VF(35)
    virtual void SetFlying(bool flying);              // slot 36 (+0x90)
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad54[0xc];
    CPhysicsBody* PhysicsBody;                        // +0x60
    CPhysicsBody* GetPhysics() const { return PhysicsBody; }
    const C3DVector& GetPos() const;             // 0x004C73D0
    void RemoveTC(ETCInterfaceType interfaceType);                // 0x004C9840
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
struct CTC4D {                                        // interface 0x4D on the hero
};
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };        // 0x00487DC0
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
  // 0x00449970
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad[0x1c];
    CPlayerManager* PlayerManager;                    // +0x1c
};

struct C3DVectorVector {                              // STLport vector<C3DVector>, 12 bytes in retail
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    C3DVector* begin() { return _M_start; }
    C3DVector& operator[](long i) { return _M_start[i]; }
    C3DVector* insert(C3DVector* where, const C3DVector& value);   // 0x007653B0
};

struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
class CCreatureActionBase;
struct CCountedActionPointer {                        // CCountedPointer<CCreatureActionBase>
    CCreatureActionBase* m_pObject;
    SCountBlock* m_pCount;
    CCountedActionPointer(const CCountedActionPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedActionPointer();
};
#include "engine/CTCCreatureModeManager.h"
struct CModeDragonFlying : CTCCreatureModeManager::CModeBase {
    CModeDragonFlying(CThing* creature, const C3DVectorVector& path, const C3DVector& pos, float speed,
                      CCountedActionPointer action);  // 0x008E1150
    virtual unsigned long GetType() const;
    unsigned char _body[0x94 - 0x1c];                  // sizeof = 0x94
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    void FinishAction();                              // 0x00859A20
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_DragonTakeOffIntoFlying : public CCombatActionBase { public:
    C3DVector             Dest;                       // +0x114 (PDB +0x12C)
    C3DVector             StartPos;                   // +0x120
    C3DVector             PreviousPoint;              // +0x12C
    C3DVectorVector       MovementPoints;             // +0x138
    bool                  UsePreviousPoint;           // +0x144
    CCountedActionPointer PEndAction;                 // +0x148
    void FinishAction();
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00770F20 ?RemoveThingNotSolidTo@CTCSolidToThing@@QAEXPBVCThing@@@Z (was ?RemoveThing@CTC4D@@QAEXPAVCThing@@@Z)
class CTCSolidToThing { public: void RemoveThingNotSolidTo(CThing const*); };
void CCreatureAction_DragonTakeOffIntoFlying::FinishAction()
{
    PCreature.GetPItem()->GetPhysics()->SetFlying(true);
    PCreature.GetPItem()->RemoveTC((ETCInterfaceType)(0x4d));
    CThing* hero = CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature();
    CTC4D* list;
    if (hero->GetInterface(&list, 0x4d))
        reinterpret_cast<CTCSolidToThing*>(list)->RemoveThingNotSolidTo(reinterpret_cast<CThing const*>(PCreature.GetPItem()));
    const C3DVector& position = PCreature.GetPItem()->GetPos();
    C3DVector lifted;
    lifted.X = position.X;
    lifted.Y = position.Y;
    lifted.Z = position.Z + 31.148378f;
    PCreature.GetPItem()->GetPhysics()->SetPosition(lifted);
    MovementPoints.insert(MovementPoints.begin(), lifted);
    CTCCreatureModeManager* modes;
    if (PCreature.GetPItem()->GetInterface(&modes, 0x31)) {
        PreviousPoint = MovementPoints[0];
        modes->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(new CModeDragonFlying(GetCreature(), MovementPoints, StartPos, 1.0f, PEndAction)));
    }
    CCombatActionBase::FinishAction();
}