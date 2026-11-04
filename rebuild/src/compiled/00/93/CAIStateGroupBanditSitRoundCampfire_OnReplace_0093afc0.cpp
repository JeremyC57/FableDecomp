// CAIStateGroup_BanditSitRoundCampfire::OnReplace() -- free the campfire seat, and if the bandit is sitting, schedule the next sit, ask the brain to stand up and drop the sitting mode; re-draw the weapon if it was carried
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): the inline Random() reads the range into a local after GFROR13 and before
//   the seed store (the store may alias the range global, so source order fixes the schedule: load, compare, store, div).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
long __fastcall GFROR13(unsigned long value);    // 0x00497890

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)key);
        return (it == map->end() || type < it->first ? map->end() : it)->second;
    }
};
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
class C3DVector { public: float X, Y, Z; };
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
class CCharString { public: void* PStringData; };
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
    if (GotSeatPos && PCampfire.GetPItem() && !(PCampfire.GetPItem()->m_flags91 & 1)) {
        CTCUserActionPointBase* point;
        CThing* campfire = PCampfire.GetPItem();
        if (campfire->HasTC(0x2d))
            point = (CTCUserActionPointBase*)campfire->GetTC(0x2d);
        ((CTCUserActionPointBase*)point)->SetActionPointAsFree(SeatPosIndex);
    }
    CTCCreatureModeManager* modes;
    CThingCreatureBase* creature = GetCreature();
    if (creature->HasTC(0x31))
        modes = (CTCCreatureModeManager*)creature->GetTC(0x31);
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