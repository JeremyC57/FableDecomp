// CAIStateGroup_MeleeCombatNearbyBase::RequestMoney() -- when the enemy is the living hero and its fine dialogue is allowed, show the fine for the creature's village's current crimes (with the hero's interface-4 amount), then yield the brain, refresh the best enemy and terminate unless still runnable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093D580, length 408 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::GetPPlayerManager @ 0x008FCF90, CPlayerManager::GetMainPlayer @ 0x00449970,
// CPlayer::GetPControlledCreature @ 0x00487DC0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PEnemy +0x14), CVectorMap::LowerBound
// @ 0x0040F020 (inlined lookups: 0xF3 = CTCFineDialogue on the hero, 0x23 = CTCVillager, interface 4 on the hero,
// 8 = CTCTargetingAI), CTCFineDialogue::IsAllowed @ 0x007E2790, GFGetPVillageInterface @ 0x007EE540 (twice; manifest
// "IS_THING_ALIVE<..>" propagated), CTCVillage::GetFineAmountForCurrentCrimes @ 0x0065A41C, CTCFineDialogue::Show @ 0x007E27F0,
// the brain's virtual slot 2 (Yield), CTCTargetingAI::UpdatePBestEnemyCreatureForAttack @ 0x007F6200, own virtual slot 14
// IsStillRunnable(bool), Terminate @ 0x008FCF40.
// Shape: the fine dialogue, interface-4 and targeting lookups are the out-parameter GetInterface (component kept in eax;
// the fine dialogue's out word in the frame), the villager lookup is HasTC + the ternary GetTC; the callee-saved pushes
// of edi (hero map) and ebp (fine amount) sink into the branches that need them.
// Layout: engine/CAIStateGroup_MeleeCombatNearbyBase.h flattens the base; local model with its PEnemy (+0x14).
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
    }
};
class CThingPlayerCreature : public CThing { };

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

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                   // 0x00449970
};

class CTCVillage { public:
    long GetFineAmountForCurrentCrimes();       // 0x0065A41C
};
struct CTCVillager;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

class CTCFineDialogue { public:                        // interface 0xF3
    bool IsAllowed();                           // 0x007E2790
    void Show(long fine, long amount, CTCVillage* village);     // 0x007E27F0
};
struct CTCHeroInterface4 {                      // interface 4 on the hero
    unsigned char _pad[0x3c];
    long m_amount;                              // +0x3c
};
class CTCTargetingAI { public:                         // interface 8
    void UpdatePBestEnemyCreatureForAttack(bool force);     // 0x007F6200
};

class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
    unsigned char _pad[0x14];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13();
    virtual bool IsStillRunnable(bool checkEnemy);  // slot 14 (+0x38)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase {
    CIntelligentPointer<CThing> PEnemy;             // +0x14
    void RequestMoney();
};

void CAIStateGroup_MeleeCombatNearbyBase::RequestMoney()
{
    CThing* hero = GetPPlayerManager()->GetMainPlayer().GetPControlledCreature();
    CTCFineDialogue* fine;
    if (hero != 0 && !(hero->m_flags91 & 1) && hero == PEnemy.GetPItem() && hero->GetInterface(&fine, 0xf3)) {
        if (fine->IsAllowed()) {
            CThing* creature = PBrain->PCreature;
            if (creature->HasTC(0x23)) {
                CTCVillager* villager = creature->GetTC<CTCVillager>(0x23);
                if (GFGetPVillageInterface(villager) != 0) {
                    CTCVillage* village = GFGetPVillageInterface(villager);
                    long amount = village->GetFineAmountForCurrentCrimes();
                    if (amount > 0) {
                        long heroAmount = 0;
                        CTCHeroInterface4* tc4;
                        if (hero->GetInterface(&tc4, 4))
                            heroAmount = tc4->m_amount;
                        fine->Show(amount, heroAmount, village);
                        PBrain->Yield();
                        CTCTargetingAI* targeting;
                        if (PBrain->PCreature->GetInterface(&targeting, 8))
                            targeting->UpdatePBestEnemyCreatureForAttack(true);
                        if (!IsStillRunnable(true))
                            Terminate();
                    }
                }
            }
        }
    }
}