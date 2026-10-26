// CAIStateGroup_WanderAroundShops::IsRunnable() -- runnable in work hours when the idle scheduler (if any) prefers this group, a random shop can be picked and is alive, and a pretty thing (the shop itself when it is a shop component holder, else one found near it) is alive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00947280, length 346 bytes (plain `ret`, virtual, bool).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 211 = bit 19 of the bitset dword at +0x38),
//          CTCIdleScheduler::IsPreferredIdle(CAIStateGroupBase*) @ 0x00801BE0, CAIStateGroup_WorkBase::IsInWorkHours
//          @ 0x0091A6F0, the shop picker @ 0x00946F90 (manifest "CAIStateGroup_WanderAroundShops_SelectRandomEnemy") and the
//          pretty-thing finder @ 0x00947110 (manifest "..._FindRandomNPC"), CBaseIntelligentPointer ctor 0x00A01B10 /
//          GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (vtable 0x01238C6C = CIntelligentPointer<CThing>).
// Shape: the scheduler lookup is the out-parameter GetInterface whose hit path alone calls IsPreferredIdle; both pointer
//          assignments from a raw CThing* go through the implicit CIntelligentPointer conversion temporary (ctor, SetPItem,
//          inline operator= naming the item: `CThing* item = other.GetPItem(); SetPItem(item);`, dtor); the same-type `PrettyThing = Shop` is the bare inline
//          operator=. IS_THING_ALIVE is the two-GetPItem inline test; the shop's type-144 test (bit 16 of +0x30) is a third
//          GetPItem. Separate `mov al,1` / `xor al,al` exits (nested success path).
// Layout: engine/CAIStateGroup_WanderAroundShops.h (Shop +0x14, PrettyThing +0x1c, NextFrameBuy +0x24) over the landed
//          CAIStateGroupBase model (the header flattens the vptr, so the base is modelled locally).
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

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
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    __forceinline CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { T* item = other.GetPItem(); SetPItem(item); return *this; }

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIStateGroupBase;
class CTCIdleScheduler { public:                       // interface 211
    bool IsPreferredIdle(CAIStateGroupBase* group);   // 0x00801BE0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};
struct CAIStateGroup_WanderAroundShops : CAIStateGroupBase {
    CIntelligentPointer<CThing> Shop;              // +0x14
    CIntelligentPointer<CThing> PrettyThing;       // +0x1c
    long NextFrameBuy;                          // +0x24
    bool IsInWorkHours();                       // 0x0091A6F0
    CThing* SelectRandomShop();                 // 0x00946F90
    CThing* SelectPrettyThing(CThing* shop);    // 0x00947110
    virtual bool IsRunnable();
};

bool CAIStateGroup_WanderAroundShops::IsRunnable()
{
    CTCIdleScheduler* scheduler;
    if (PBrain->PCreature->GetInterface(&scheduler, 211)) {
        if (!scheduler->IsPreferredIdle(this))
            return false;
    }
    if (IsInWorkHours()) {
        Shop = SelectRandomShop();
        if (IS_THING_ALIVE(Shop)) {
            if (Shop.GetPItem()->HasTC(144))
                PrettyThing = Shop;
            else
                PrettyThing = SelectPrettyThing(Shop.GetPItem());
            if (IS_THING_ALIVE(PrettyThing))
                return true;
        }
    }
    return false;
}