// CAIStateGroup_BanditSitRoundCampfire::OnReplace() -- free the campfire seat, and if the bandit is sitting, schedule the next sit, ask the brain to stand up and drop the sitting mode; re-draw the weapon if it was carried
// Leaf: ?OnReplace@CAIStateGroup_BanditSitRoundCampfire@@QAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): the inline Random() reads the range into a local after GFROR13 and before
//   the seed store (the store may alias the range global, so source order fixes the schedule: load, compare, store, div).
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
long __fastcall GFROR13(unsigned long value);    // 0x00497890

class CCreatureActionBase;
class CThingCreatureBase : public CThing { public:
    bool IsFreeToPerformAction(const CCreatureActionBase& action);   // 0x00662210
    void UnsheatheDefaultWeapon();              // 0x00664A50
};
class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();   // slot 0 (retail scalar deleting dtor; was placeholder v00); 0x00A01C10
    CThing* m_pItem;
    CBaseObject* GetPItem() const;              // 0x00A01B50 ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    __forceinline T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(CCreatureActionBase*);
    CCreatureActionBase* m_pObject;
};
struct CCountedPointer_CCreatureActionBase {
    CCreatureActionBase* m_pObject;
    SCountBlock*         m_pCount;
    void Reset(CCreatureActionBase* object);    // 0x00668D90
    __forceinline ~CCountedPointer_CCreatureActionBase()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
#include "engine/C3DVector.h"
class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v0a();
    virtual CCreatureActionBase* Clone() const; // slot 11 (+0x2c)
};
class CNavigationInput { public:
    C3DVector Destination;
    float Proximity;
    long MovementType;
    float Speed;
};
class CBrainOutput { public:
    unsigned long                       Types;            // +0x00
    long                                LookFlags;        // +0x04
    CNavigationInput                    NavigationInput;  // +0x08
    CCountedPointer_CCreatureActionBase PAction;          // +0x20
    C3DVector                           MoveToPos;        // +0x28
    C3DVector                           LookAtPos;        // +0x34
    CBaseIntelligentPointer             LookAtThing;      // +0x40
    CBrainOutput();                             // 0x00835620
    ~CBrainOutput() { }
    void SetAction(const CCreatureActionBase& action)
    {
        Types |= 8;
        PAction.Reset(action.Clone());
    }
};

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    virtual ~CCombatActionBase();                       // 0x0062E900
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};
struct CCreatureAction_StandUp : CCombatActionBase {   // vtable 0x01276C2C
    virtual void v00();
    long m_114;
    bool m_118;
    CCreatureAction_StandUp(CThingCreatureBase* owner) : CCombatActionBase(owner, 50, CSoundPair()), m_114(0), m_118(false) { }
};
extern CCharString g_SitStandUpAnim;            // 0x013BB708
extern unsigned long g_SitDelayBase;            // 0x013BB70C
extern unsigned long g_SitDelayRange;           // 0x013BB710

struct CCreatureMode { virtual void v00(); virtual long GetType() const; };
class CTCCreatureModeManager { public:
    CCreatureMode* GetCurrentMode();            // 0x0073A6E0
    void RemoveMode(unsigned long type);        // 0x0073ABA0 (Ego_r.pdb: unsigned long)
};
struct CTCUserActionPointBase {
public:
    void SetActionPointAsFree(long a0);   // 0x007FEF40 (Ego_r.pdb ?SetActionPointAsFree@CTCUserActionPointBase@@QAEXJ@Z)
public:
};
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870
class CAIBrain { public:
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
    void SetOutput(const CBrainOutput& output); // 0x0088B8F0
};
class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
protected:
    const CWorld& PeekWorld() const;            // 0x008FCFA0
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0
public:
    unsigned long Random(const unsigned long& range) const
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24a1 + 0x24df;
        unsigned long r = GFROR13(*seed);
        unsigned long n = range;
        *seed = r;
        return n ? r % n : 0;
    }
};
struct CAIStateGroup_BanditSitRoundCampfire : CAIStateGroupBase {
    CIntelligentPointer<CThing> PCampfire;      // +0x14
    long SeatPosIndex;                          // +0x1c
    bool GotSeatPos;                            // +0x20
    bool Sitting;                               // +0x21
    unsigned long FrameToNextSitOnSeat;         // +0x24
    unsigned long FrameToNextCheckIsRunnable;   // +0x28
    bool WasCarryingWeapon;                     // +0x2c
    void OnReplace();
};

void CAIStateGroup_BanditSitRoundCampfire::OnReplace()
{
    if (GotSeatPos && PCampfire.GetPItem() && !PCampfire.GetPItem()->IsDead()) {
        CTCUserActionPointBase* point;
        CThing* campfire = PCampfire.GetPItem();
        if (campfire->HasTC(TCI_SINGLE_USER_ACTION_POINTS))
            point = (CTCUserActionPointBase*)((CTCUserActionPointBase*)&campfire->GetTC(TCI_SINGLE_USER_ACTION_POINTS));
        ((CTCUserActionPointBase*)point)->SetActionPointAsFree(SeatPosIndex);
    }
    CTCCreatureModeManager* modes;
    CThingCreatureBase* creature = GetCreature();
    if (creature->HasTC(TCI_ENTITY_MODE_MANAGER))
        modes = (CTCCreatureModeManager*)((CTCCreatureModeManager*)&creature->GetTC(TCI_ENTITY_MODE_MANAGER));
    if (Sitting || modes->GetCurrentMode()->GetType() == 10) {
        PeekWorld();
        unsigned long delay = Random(g_SitDelayRange);
        FrameToNextSitOnSeat = CWorld::GetFrame() + g_SitDelayBase + delay;
        CBrainOutput output;
        CCreatureAction_StandUp action(GetCreature());
        action.SetCombatAnimation(g_SitStandUpAnim);
        if (GetCreature()->IsFreeToPerformAction(action)) {
            output.SetAction(action);
            PBrain->SetOutput(output);
        }
        modes->RemoveMode(10);
        Sitting = false;
    }
    if (WasCarryingWeapon)
        GetCreature()->UnsheatheDefaultWeapon();
}