// CAIStateGroup_EatAtTavern::IsTime() -- the tavern is staffed and the village says the current time falls in
// the def's eating hours.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096D380, length 197 bytes (+ int3 padding). Vtable slot 15.
// Callees: CAIStateGroup_EatAtTavern::IsStaffed @ 0x0096D2D0, CVectorMap::LowerBound @ 0x004365B0 (inlined
// GetInterface, type 0x23 = CTCVillager), GFGetPVillageInterface @ 0x007EE540 (three times), _ftol2 @ 0x00BFEA70,
// CTCVillage::IsTimeBetween(vector<long>*, long) @ 0x00658620, the def's virtual slot 1 (+4) on the last release.
// Shape: one `&&` return expression whose last operand builds a by-value counted-def temporary (the village's def
// at +0x94, intrusive count at +4): VC7.1 sets a constructed-flag byte (init 0 before IsStaffed, 1 before the
// copy) and the shared epilogue destroys the temporary only when the flag is set; the IsStaffed miss returns al.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
// GetInterface: the inline out-parameter lookup picks the entry with the end/hit ternary (`*out = (it == last || k <
//   it->type ? last : it)->component`), which loads the component straight into the register retail uses.
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual CThing* GetBuilding();              // slot 14 (+0x38) CAIStateGroup_EatAtTavern::GetBuilding @ 0x0096D260
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
};


// Retail builds message filters as an expression chain: `IsOfType(n) && HasExtraData() && IsForThing(t)`.
// Each `operator&&` returns a CFilter_Bind2/CFilter_Bind3 holding POINTERS to its operands (the filter
// temporaries live in the caller's frame until the call returns; the empty one-byte filter has a user
// constructor so it takes no store and sits in the high byte of a slot), and FindMostRecentMessage takes the
// bound filter by const reference (one `lea ; push`) -- the landed 0x00910D50 / 0x00939D70 model.
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

class CMessageEventFilter_HasExtraData { public:
    CMessageEventFilter_HasExtraData() { }
};

struct CMessageEventFilter_IsForThing {
    CThing* m_pThing;
    CMessageEventFilter_IsForThing(CThing* thing) : m_pThing(thing) { }
};

class CMessageEventFilter_IsOriginatedByThing { public:
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF2B0 (0x006AF2B0 / 0x006AF290 twins, out of line, `ret 4`)
};

template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B, class C> class CFilter_Bind3 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }

class CMessageEvent;
class CMessageEventManager;
class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
    CWorld* GetPThingWorld();                   // 0x008FCFA0
};

class CVillageDef { public:
    virtual void v00();
    virtual void Release();                     // slot 1 (+4) on the last reference
    long m_refs;                                // +0x04 intrusive reference count
    unsigned char _pad[0x7c];
    unsigned char m_eatingTimes[0x10];          // +0x84 vector<long>
};

struct CVillageDefPointer {
    CVillageDef* m_p;
    CVillageDefPointer(const CVillageDefPointer& o) : m_p(o.m_p) { if (m_p) ++m_p->m_refs; }
    ~CVillageDefPointer() { if (m_p && --m_p->m_refs == 0) m_p->Release(); }
    CVillageDef* operator->() const { return m_p; }
};

class CTCVillage { public:
    unsigned char _pad[0x94];
    CVillageDefPointer m_def;                   // +0x94
    CVillageDefPointer GetDef() const { return m_def; }
    bool IsTimeBetween(void* times, long time); // 0x00658620
};
struct CTCVillager;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

struct CAIStateGroup_EatAtTavern : CAIStateGroupBaseW {
    unsigned char _pad14[0x34];
    float m_time;                               // +0x48
    bool IsStaffed();                           // 0x0096D2D0
    virtual bool IsTime();                      // slot 15
};

bool CAIStateGroup_EatAtTavern::IsTime()
{
    CTCVillager* villager;
    if (!IsStaffed())
        return false;
    return PBrain->PCreature->GetInterface(&villager, 0x23)
        && GFGetPVillageInterface(villager) != 0
        && GFGetPVillageInterface(villager)->IsTimeBetween(GFGetPVillageInterface(villager)->GetDef()->m_eatingTimes, (long)m_time);
}
// ---- harness ----
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
CAIStateGroupBase::~CAIStateGroupBase() { }
void CAIStateGroupBase::v01() { } void CAIStateGroupBase::v02() { } void CAIStateGroupBase::v03() { }
void CAIStateGroupBase::v04() { } void CAIStateGroupBase::v05() { } void CAIStateGroupBase::v06() { }
void CAIStateGroupBase::v07() { } void CAIStateGroupBase::v08() { } void CAIStateGroupBase::Cleanup() { }
void CAIStateGroupBase::OnReplace() { } bool CAIStateGroupBase::IsRunnable() { return false; }
void CAIStateGroupBase::Update() { } void CAIStateGroupBase::v13() { }
CThing* CAIStateGroupBase::GetBuilding() { return 0; }
void CAIStateGroupBase::v15() { } void CAIStateGroupBase::v16() { } void CAIStateGroupBase::v17() { }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
static int g_releases = 0;
void CVillageDef::v00() { }
void CVillageDef::Release() { ++g_releases; }
static long g_villageMem[(sizeof(CTCVillage) + 3) / 4];   // CVillageDefPointer has no default constructor
static CTCVillage& g_village = *(CTCVillage*)g_villageMem; static bool g_haveVillage = true; static int g_villageCalls = 0;
static const void* g_villagerArg = 0;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager) { ++g_villageCalls; g_villagerArg = villager; return g_haveVillage ? &g_village : 0; }
static void* g_timesArg = 0; static long g_timeArg = -1; static bool g_between = true;
bool CTCVillage::IsTimeBetween(void* times, long time) { g_timesArg = times; g_timeArg = time; return g_between; }
static bool g_staffed = true;
bool CAIStateGroup_EatAtTavern::IsStaffed() { return g_staffed; }
static CVillageDef g_def;
static unsigned char g_villager[4];
static CThing g_creature;
static CAIBrain g_brain;
static CInterfaceMap::SEntry g_entries[2];
int main()
{
    g_def.m_refs = 1;
    g_village.m_def.m_p = &g_def;
    g_entries[0].type = 0x23; g_entries[0].component = (CTCBase*)g_villager;
    g_entries[1].type = 0x31; g_entries[1].component = 0;
    g_creature.m_interfaces.first = g_entries; g_creature.m_interfaces.last = g_entries + 2;
    g_creature.m_presentBits[0x23 >> 5] = 1UL << (0x23 & 31);
    g_brain.PCreature = &g_creature;
    CAIStateGroup_EatAtTavern s;
    s.PBrain = &g_brain;
    s.m_time = 13.75f;
    CHECK(s.IsTime() == true);
    CHECK(g_villageCalls == 3 && g_villagerArg == g_villager && g_timesArg == g_def.m_eatingTimes && g_timeArg == 13);
    CHECK(g_def.m_refs == 1 && g_releases == 0);
    g_between = false;                              // outside the eating hours
    CHECK(s.IsTime() == false && g_def.m_refs == 1);
    g_between = true; g_def.m_refs = 0;             // the temporary holds the last reference: released
    CHECK(s.IsTime() == true && g_releases == 1 && g_def.m_refs == 0);
    g_def.m_refs = 1; g_haveVillage = false; g_villageCalls = 0;   // no village
    CHECK(s.IsTime() == false && g_villageCalls == 1 && g_releases == 1);
    g_haveVillage = true; g_creature.m_presentBits[0x23 >> 5] = 0; g_villageCalls = 0;   // not a villager
    CHECK(s.IsTime() == false && g_villageCalls == 0);
    g_creature.m_presentBits[0x23 >> 5] = 1UL << (0x23 & 31); g_staffed = false;     // tavern not staffed
    CHECK(s.IsTime() == false && g_villageCalls == 0);
    if (fails == 0) printf("0096d380_TEST PASS\n");
    return fails;
}