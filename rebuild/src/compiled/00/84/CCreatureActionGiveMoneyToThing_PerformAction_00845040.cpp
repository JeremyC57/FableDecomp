// CCreatureAction_GiveMoneyToThing::PerformAction() -- take up to Amount gold from the creature's hero stats, post a received-money event to the receiver and, for a qualifying villager receiver, post the opinion deed (skipped for a married wife whose gift reaction is not running)
// Leaf: ?PerformAction@CCreatureAction_GiveMoneyToThing@@UAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00845040, 547 bytes, thiscall virtual, plain `ret`.
// Identity: Ego_r.pdb / FableWin.pdb ?PerformAction@CCreatureAction_GiveMoneyToThing@@UAEXXZ (manifest label agrees); Ego_r
//   CCreatureAction_GiveMoneyToThing {PReceivingThing CIntelligentPointer<CThing> +0xB4, Amount +0xBC} = retail +0xA8 / +0xB0
//   (CCreatureActionBase is 0xC smaller in retail: three STLport containers).
// Callees (every spelling = the Ego_r.pdb symbol, checked with objdump): CBaseIntelligentPointer::GetPItem() const 0x00A01B50,
//   CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound 0x0040F020 (inside the forced-inline CThing::GetTC<T>),
//   CTCHeroStats::AddMoney(long, bool) 0x0057B327, operator new 0x00BFEA1A,
//   NEntityEvents::CEventReceivedMoney::CEventReceivedMoney(CThing&, long) 0x0084AE00, CTCEntityEvents::AddEvent(CEventBase*)
//   0x008113C0, CThingCreatureBase::IsPlayerCharacter() const (vtable slot 77, +0x134), CTCWife::IsMarriedToThePlayer() const
//   0x006CEC80, CTCWife::ReceivedNiceGiftIsOpinionReactionHappening(long) 0x006D0630, CThing::GetPos() const 0x004C73D0,
//   CTCHeroOpinionDeedLog::PostDeed(EOpinionDeedType, const C3DVector&, const CThing*, CThingAICreature*,
//   EOpinionPostDeedType, bool, bool, const CFaction*, bool, long) 0x006C6990.
// Interface types (GetInterfaceType): CTCHeroStats 4, CTCEntityEvents 0x42, CTCHeroOpinionDeedLog 0x51, CTCWife 0xD7; the two plain
//   presence tests are 0xB4 (must be absent) and 0x4F (must be present).
// Shape: the landed CCreatureAction shared model (0x00849E80 / 0x00848500 family) with LowerBound declared only and the
//   GetInterface ternary store (`*out = ((it == end || type < it->first) ? end : it)->second`, EXTRA_RULES); HasTC forced inline
//   like the 501 landed HasTC rows (plain inline runs out of VC7.1's per-caller inline budget here: 567 bytes). The entity-events
//   lookup result is ignored (the absent path reads the never-written out variable, retail's `mov ebp,[esp+14h]`); the amount is
//   STLport std::min(gold, Amount) by reference; receiver and creature are named locals read before the PostDeed constants.
//   Probe history (8 probes) in probe/00845040.log; the prior's 552v547 constant-4 register residue is gone.
// ---- Shared model (retail layouts; names from Ego_r.pdb / FableWin.pdb) ----
class CBaseObject;
class CBaseObjectPointer;
class CTCBase;
class CTCPhysicsBase;
#include "engine/C3DVector.h"
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class vector {              // retail STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    _Tp& operator[](unsigned int n) { return *(this->_M_start + n); }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    CArray<value_type> Data;                                     // +0x00
    TCompare           Compare;                                  // +0x0C
    bool               Dirty;                                    // +0x0D
    value_type* LowerBound(const K& key);                        // 0x0040F020
    const value_type* LowerBound(const K& key) const;            // 0x004365B0
};


// Definition objects: CIVCountedPointeeBase (vptr, IVRefCount +0x04; slot 1 Delete) -> CResource (0x1C) -> CDefClassBase
// (PDefManager +0x1C). Virtual slots from FableWin.pdb in declaration order (the landed CCreatureAction_Drink::Initialise
// 0x00847AC0 calls slot 14 GetPSubDefInfo const).
class CCharString;
class CDefinitionManager;
class CDefString {
public:
    CDefString() : TablePos(0) { }
    CCharString GetString() const;                               // inline over g_DefStringTable (where used)
    long TablePos;                                               // +0x00
};
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();                            // 0
    virtual void Delete() const;                                 // 1
    void AddRef() const { ++IVRefCount; }
    long Release() const
    {
        long count = --IVRefCount;
        if (count == 0)
            Delete();
        return count;
    }
    mutable long IVRefCount;                                     // +0x04
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { if (Object) Object->Release(); }
    void Release()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                Object->AddRef();
        }
        return *this;
    }
    T* operator->() const { return Object; }
    T& operator*() const { return *Object; }
};
class CResource : public CIVCountedPointeeBase {
public:
    virtual bool UnloadResource();                               // 2
    virtual unsigned long GetUnloadableMemory() const;           // 3
    virtual bool IsResourceLoaded() const;                       // 4
    virtual void OnResourceLoaded();                             // 5
    virtual bool CanAbortResourceLoad(long reason) const;        // 6
    virtual void OnResourceLoadAborted();                        // 7
    virtual unsigned long GetDebugMemoryInUse() const;           // 8
    virtual void ResetDebugMemoryInUse();                        // 9
    unsigned char _pad08[0x1C - 0x08];
};
class CDefClassBase : public CResource {
public:
    class CSubDefInfo {
    public:
        long GlobalIndex;
    };
    virtual CDefString GetInstantiationName() const;             // 10
    virtual void SetInstantiationName(CDefString name);          // 11
    virtual void AddSubDef(const CCharString& name, long globalIndex, long flags);   // 12
    virtual void RemoveSubDef(const CCharString& name);          // 13
    // const / non-const overloads take their slots in reverse declaration order: const is slot 14 (+0x38).
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString& name);                   // 15
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString& name) const;       // 14
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;
    CDefinitionManager* PDefManager;                             // +0x1C
};
class CThingBaseDef : public CDefClassBase { };
class CThing {                                                   // partial view, retail offsets
public:
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    // CThing vtable: slot 0 is the virtual destructor; slots 1-76 are placeholders (not called here, names not modelled);
    // CThingCreatureBase::IsPlayerCharacter is slot 77.
    virtual ~CThing();
    virtual void ThingVSlot01(); virtual void ThingVSlot02(); virtual void ThingVSlot03(); virtual void ThingVSlot04(); virtual void ThingVSlot05(); virtual void ThingVSlot06();
    virtual void ThingVSlot07(); virtual void ThingVSlot08(); virtual void ThingVSlot09(); virtual void ThingVSlot10(); virtual void ThingVSlot11(); virtual void ThingVSlot12();
    virtual void ThingVSlot13(); virtual void ThingVSlot14(); virtual void ThingVSlot15(); virtual void ThingVSlot16(); virtual void ThingVSlot17(); virtual void ThingVSlot18();
    virtual void ThingVSlot19(); virtual void ThingVSlot20(); virtual void ThingVSlot21(); virtual void ThingVSlot22(); virtual void ThingVSlot23(); virtual void ThingVSlot24();
    virtual void ThingVSlot25(); virtual void ThingVSlot26(); virtual void ThingVSlot27(); virtual void ThingVSlot28(); virtual void ThingVSlot29(); virtual void ThingVSlot30();
    virtual void ThingVSlot31(); virtual void ThingVSlot32(); virtual void ThingVSlot33(); virtual void ThingVSlot34(); virtual void ThingVSlot35(); virtual void ThingVSlot36();
    virtual void ThingVSlot37(); virtual void ThingVSlot38(); virtual void ThingVSlot39(); virtual void ThingVSlot40(); virtual void ThingVSlot41(); virtual void ThingVSlot42();
    virtual void ThingVSlot43(); virtual void ThingVSlot44(); virtual void ThingVSlot45(); virtual void ThingVSlot46(); virtual void ThingVSlot47(); virtual void ThingVSlot48();
    virtual void ThingVSlot49(); virtual void ThingVSlot50(); virtual void ThingVSlot51(); virtual void ThingVSlot52(); virtual void ThingVSlot53(); virtual void ThingVSlot54();
    virtual void ThingVSlot55(); virtual void ThingVSlot56(); virtual void ThingVSlot57(); virtual void ThingVSlot58(); virtual void ThingVSlot59(); virtual void ThingVSlot60();
    virtual void ThingVSlot61(); virtual void ThingVSlot62(); virtual void ThingVSlot63(); virtual void ThingVSlot64(); virtual void ThingVSlot65(); virtual void ThingVSlot66();
    virtual void ThingVSlot67(); virtual void ThingVSlot68(); virtual void ThingVSlot69(); virtual void ThingVSlot70(); virtual void ThingVSlot71(); virtual void ThingVSlot72();
    virtual void ThingVSlot73(); virtual void ThingVSlot74(); virtual void ThingVSlot75(); virtual void ThingVSlot76();
    unsigned char      _pad04[0x10 - 0x04];
    unsigned char      ThingType;                                // +0x10 (descriptive: a byte type code, 1 here)
    unsigned char      _pad11[0x20 - 0x11];
    unsigned long      UsingComponents[9];                       // +0x20 std::bitset<274>
    TCMap              Components;                               // +0x44
    unsigned char      UpdateComponentsList[4];                  // +0x54 std::list<CTCBase*>
    unsigned char      SerialisationSectionName[4];              // +0x58 CCharString
    void*              PTCDriver;                                // +0x5C CTCDriverBase*
    CTCPhysicsBase*    PhysicsTC;                                // +0x60
    void*              PTCGraphicAppearance;                     // +0x64
    long               WorldFrameCreatedAt;                      // +0x68
    unsigned long      ThingTypeFlags;                           // +0x6C
    CDefPointer<const CThingBaseDef> PBaseDef;                   // +0x70
    unsigned char      _pad74[0x90 - 0x74];
    signed char        PlayerNumber;                             // +0x90
    unsigned char      Dead : 1;                                 // +0x91 bit 0
    bool IsDead() const { return Dead; }
    const CThingBaseDef& PeekBaseDef() const { return *PBaseDef; }
    unsigned long GetPlayerNumber() const { return PlayerNumber; }
    const C3DVector& GetPos() const;                             // 0x004C73D0
    long GetJoystickDeviceNumber() const;                        // 0x004C7CA0
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }         // inline (Ego_r ?GetPhysicsTC@CThing@@QAEPAVCTCPhysicsBase@@XZ)
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    __forceinline bool HasTC(ETCInterfaceType type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC(T::StatGetInterfaceType()))
            return false;
        ETCInterfaceType key = T::StatGetInterfaceType();
        TCMap* map = &Components;
        TCMap::value_type* it = map->LowerBound(key);
        *out = (T*)((it == map->Data.end() || T::StatGetInterfaceType() < it->first) ? map->Data.end() : it)->second;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const
    {
        if (!HasTC(T::StatGetInterfaceType()))
            return false;
        ETCInterfaceType key = T::StatGetInterfaceType();
        const TCMap* map = &Components;
        const TCMap::value_type* it = map->LowerBound(key);
        if (it == map->Data.end() || T::StatGetInterfaceType() < it->first)
            it = map->Data.end();
        *out = (const T*)it->second;
        return true;
    }
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CCreatureActionBase;
class CGameDefinitionManager;
class CWorldMap;
class CThingCreatureBase : public CThingGameObject {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);    // 0x006644F0
    virtual bool IsPlayerCharacter() const = 0;                  // slot 77 (+0x134): CThingPlayerCreature 0x0052ABE0 true, CThingAICreature 0x00835940 false
};

class CBaseIntelligentPointer {                                  // vptr + PData (8 bytes)
public:
    CBaseIntelligentPointer();                                   // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                          // 0x00A01C10
    CBaseObject* GetPItem() const;                               // 0x00A01B50
    void SetPItem(CBaseObject* item);                            // 0x00A01B90
    CBaseObjectPointer* PData;                                   // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    explicit CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    virtual ~CIntelligentPointer();
    T* Get() const { return (T*)GetPItem(); }
    T* operator->() const { return (T*)GetPItem(); }
    T& operator*() const { return *(T*)GetPItem(); }
};

class CCreatureActionBase {                                      // retail sizeof 0xA8 (Ego_r 0xB4: three STL containers)
public:
    virtual ~CCreatureActionBase();
    unsigned char                           _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;           // +0x08
    long             TotalTime;                                  // +0x10
    long             ActionTime;                                 // +0x14
    long             AnimDelayTime;                              // +0x18
    long             FramesRunning;                              // +0x1C
    long             InterruptionGroupID;                        // +0x20
    long             InterruptionPriority;                       // +0x24
    long             ActionID;                                   // +0x28
    CIntelligentPointer<CThing> PItemRemoved;                    // +0x2C
    unsigned char    _pad34[0x60 - 0x34];
    bool             AddedActionAnim;                            // +0x60
    bool             Finished;                                   // +0x61
    bool             PerformedAction;                            // +0x62
    bool             Initialised;                                // +0x63
    unsigned char    _pad64[0xA8 - 0x64];
    bool HasPerformedAction() const;                             // 0x006925A0
    long GetTotalTimeWorldFrames() const;                        // 0x006925B0
    long GetFramesRunning() const;                               // 0x006925C0
    long GetFramesRemaining() const;                             // 0x006925D0
protected:
    const CGameDefinitionManager& PeekDefinitionManager() const; // 0x00692910
    CWorldMap& GetWorldMap();                                    // 0x00692C10 (folded with the const overload)
public:
};
// ---- end shared model ----
void* __cdecl operator new(unsigned int size);                   // 0x00BFEA1A
class CFaction;
class CThingAICreature;
class CTCHeroStats {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x04; }
    unsigned char _pad00[0x3c];
    long Money;                                                  // +0x3C (Ego_r +0x44)
    long GetMoney() const { return Money; }
    void AddMoney(long amount, bool notify);                     // 0x0057B327
};
namespace NEntityEvents {
class CEventBase { };
class CEventReceivedMoney : public CEventBase {
public:
    CEventReceivedMoney(CThing& giver, long amount);             // 0x0084AE00
    unsigned char _body[0x14];
};
}
class CTCEntityEvents {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x42; }
    void AddEvent(NEntityEvents::CEventBase* event);             // 0x008113C0
};
enum EOpinionDeedType { };
enum EOpinionPostDeedType { };
class CTCHeroOpinionDeedLog {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x51; }
    long PostDeed(EOpinionDeedType type, const C3DVector& pos, const CThing* target, CThingAICreature* victim,
                  EOpinionPostDeedType postType, bool b1, bool b2, const CFaction* faction, bool b3, long extra);   // 0x006C6990
};
class CTCWife {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0xd7; }
    bool IsMarriedToThePlayer() const;                           // 0x006CEC80
    bool ReceivedNiceGiftIsOpinionReactionHappening(long a);     // 0x006D0630
};
namespace std {
template <class _Tp> inline const _Tp& min(const _Tp& __a, const _Tp& __b) { return __b < __a ? __b : __a; }
}
class CCreatureAction_GiveMoneyToThing : public CCreatureActionBase {
public:
    CIntelligentPointer<CThing> PReceivingThing;                 // +0xA8
    long                        Amount;                          // +0xB0
    virtual void PerformAction();
};

void CCreatureAction_GiveMoneyToThing::PerformAction()
{
    if (PReceivingThing.Get() == 0 || PReceivingThing->IsDead())
        return;
    CTCHeroStats* stats;
    if (!PCreature->GetTC(&stats))
        return;
    long amount = std::min(stats->GetMoney(), Amount);
    stats->AddMoney(-amount, true);
    CTCEntityEvents* events;
    PReceivingThing->GetTC(&events);
    events->AddEvent(new NEntityEvents::CEventReceivedMoney(*PCreature, amount));
    CTCHeroOpinionDeedLog* deeds;
    if (!PCreature->GetTC(&deeds))
        return;
    if (PReceivingThing->ThingType == 1 && !PReceivingThing->HasTC((ETCInterfaceType)0xb4) && PReceivingThing->HasTC((ETCInterfaceType)0x4f)) {
        CTCWife* wife;
        if (PCreature->IsPlayerCharacter() && PReceivingThing->GetTC(&wife)) {
            if (wife->IsMarriedToThePlayer() && !wife->ReceivedNiceGiftIsOpinionReactionHappening(-1))
                return;
        }
        CThing* receiver = PReceivingThing.Get();
        CThingCreatureBase* creature = PCreature.Get();
        deeds->PostDeed((EOpinionDeedType)0x3d, creature->GetPos(), receiver, (CThingAICreature*)PReceivingThing.Get(),
                        (EOpinionPostDeedType)1, false, false, 0, false, 0);
    }
}