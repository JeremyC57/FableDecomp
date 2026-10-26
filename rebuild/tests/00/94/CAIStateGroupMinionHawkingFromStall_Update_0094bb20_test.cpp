// behaviour test for 0x0094BB20 CAIStateGroup_MinionHawkingFromStall::Update (body verbatim, callees stubbed)
#include <stdio.h>
#include <string.h>
// CAIStateGroup_MinionHawkingFromStall::Update() -- walk to the stall (1.5m), shout the shop's hawking speech at it and pick up a new customer; schedule the next hawk 75..149 frames ahead and terminate
// Grade: RELOCATION_MATCH -- the presence test reads the bit word into a local and shifts it (`mov ecx,[eax+30h] ; shr ecx,10h ; and cl,1`; the masked form narrows to a byte load); the thing's def is a CDefPointer member (+0x70) reached through operator-> on a named shopThing local, which loads it into eax ahead of the holder's address
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094BB20, length 384 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x90 = CTCShopKeeper, bit 16 of +0x30 as a
//          bitset::test byte), CTCShopKeeper::GetShopTC @ 0x00798F50 / AcquireNewCustomer @ 0x00799900,
//          CDefClassBase::GetSubDef<...> @ 0x005130A0 on the shop thing's def (+0x04 -> +0x70), CBaseIntelligentPointer
//          ctor/GetPItem/SetPItem (by-value pointers: CIntelligentPointer<const CThing> vtable 0x01237F1C,
//          CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8), CAIStateGroupBase::AIAction_MoveToThingProximity
//          @ 0x0097E380 / AIAction_SpeakAISpeechToTarget @ 0x009812C0, GetPWorld @ 0x008FCFC0, GetPWorldSeed @ 0x008FCFD0,
//          GFROR13 @ 0x00497890, CWorld::GetWorldFrame @ 0x0049D870 (static), Terminate @ 0x008FCF40; the def's virtual
//          slot 1 (destroy on the last release).
// Shape: the keeper out pointer shares the key's home; GetSubDef is the same-TU noinline stand-in of the landed CTCShop rows; the def holder is a function-scope CDefPointer whose
//          destructor releases without a null test after Terminate; the speech id is read once into a dead slot.
// Layout: no engine header for this class; modelled locally like the landed OnReplace 0x0094BAD0 (next hawk frame
//          +0x14, PStall +0x18); the sub-def's +0x2c speech id is descriptive.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    void Release() { if (--m_refCount == 0) Destroy(); }
};
struct CHawkingDef : CDefPointee {
    unsigned char _pad08[0x2c - 0x08];
    unsigned long HawkSpeech;                   // +0x2c (descriptive)
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CHawkingDef>& out);   // 0x005130A0
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CHawkingDef>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.Object = (CHawkingDef*)def;
    return true;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x70 - 0x4c];
    CDefPointer<CDefClassBase> PDef;            // +0x70
    bool TestTC(long interfaceType) const       // a bool masked test: VC7.1 narrows it to `shr ; and cl,1`
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
};
class CTCShop { public:
    void* __vftable;
    CThing* PThing;                             // +0x04
};
class CTCShopKeeper { public:                          // interface 0x90
    CTCShop* GetShopTC();                       // 0x00798F50
    void AcquireNewCustomer();                  // 0x00799900
};
class CWorld { public:
    static long GetWorldFrame();                // 0x0049D870
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    __forceinline CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    __forceinline CIntelligentPointer(const CBaseIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
class CThingCreatureBase;
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
    CThing* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24A1U + 0x24DFU;
        *seed = GFROR13(*seed);
        return *seed % range;
    }
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointer<const CThing> target, float proximity,
        bool* reached, long movement, bool a, bool b, bool c, bool d, float speed);   // 0x0097E380
    long AIAction_SpeakAISpeechToTarget(long repeat, CIntelligentPointer<CThingCreatureBase> speaker,
        CIntelligentPointer<const CThing> target, unsigned long speech, bool interrupt, long priority);   // 0x009812C0
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_MinionHawkingFromStall : CAIStateGroupBase {
    long NextHawkFrame;                         // +0x14 (descriptive)
    CBaseIntelligentPointer PStall;             // +0x18
    void Update();
};

void CAIStateGroup_MinionHawkingFromStall::Update()
{
    CTCShopKeeper* keeper;
    GetCreature()->GetInterfaceT(&keeper, 0x90);
    CTCShop* shop = keeper->GetShopTC();
    CThing* shopThing = shop->PThing;
    {
    CDefPointer<CHawkingDef> def;
    shopThing->PDef->GetSubDef(def);
    unsigned long speech = def->HawkSpeech;
    bool reached;
    if (AIAction_MoveToThingProximity(1, CIntelligentPointer<const CThing>(PStall), 1.5f, &reached, 3,
            true, false, true, true, 0.0f) == 2) {
        AIAction_SpeakAISpeechToTarget(1, CIntelligentPointer<CThingCreatureBase>((CThingCreatureBase*)GetCreature()),
            CIntelligentPointer<const CThing>(PStall), speech, false, 0x93);
        keeper->AcquireNewCustomer();
    }
    NextHawkFrame = GetPWorld()->GetWorldFrame() + Random(75) + 75;
    Terminate();
    }
}


// ---- stubs ----
static long g_lbKey; static int g_lbOffset;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { g_lbKey = *type; return first + g_lbOffset; }
static CTCShop* g_shop; static CTCShopKeeper* g_shopThis; static int g_acquired; static CTCShopKeeper* g_acqThis;
CTCShop* CTCShopKeeper::GetShopTC() { g_shopThis = this; return g_shop; }
void CTCShopKeeper::AcquireNewCustomer() { g_acquired++; g_acqThis = this; }
static CHawkingDef* g_subDef; static const void* g_subOwner;
void* Find_CDefClassBase_GetSubDef(const void* owner) { g_subOwner = owner; return g_subDef; }
static int g_destroyed;
void CDefPointee::v00() { }
void CDefPointee::Destroy() { g_destroyed++; }
static long g_worldFrame = 1000;
long CWorld::GetWorldFrame() { return g_worldFrame; }
unsigned long __fastcall GFROR13(unsigned long value) { return (value >> 13) | (value << 19); }
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = 0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
void CBaseIntelligentPointer::v00() { }
template <class T> void CIntelligentPointer<T>::v00() { }
template class CIntelligentPointer<const CThing>;
template class CIntelligentPointer<CThingCreatureBase>;
static CWorld g_world;
CWorld* CAIStateGroupBase::GetPWorld() { return &g_world; }
static unsigned long g_seed;
unsigned long* CAIStateGroupBase::GetPWorldSeed() { return &g_seed; }
static long g_moveResult; static int g_moves; static CThing* g_moveTarget; static float g_moveProx;
static long g_moveArgs[4];
long CAIStateGroupBase::AIAction_MoveToThingProximity(long repeat, CIntelligentPointer<const CThing> target, float proximity,
    bool* reached, long movement, bool a, bool b, bool c, bool d, float speed)
{
    g_moves++; g_moveTarget = target.GetPItem(); g_moveProx = proximity;
    g_moveArgs[0] = repeat; g_moveArgs[1] = movement; g_moveArgs[2] = a * 8 + b * 4 + c * 2 + d; g_moveArgs[3] = (speed == 0.0f) && reached != 0;
    return g_moveResult;
}
static int g_speaks; static CThing* g_speaker; static CThing* g_speakTarget; static unsigned long g_speech; static long g_speakPrio; static long g_speakRepeat; static bool g_speakInt;
long CAIStateGroupBase::AIAction_SpeakAISpeechToTarget(long repeat, CIntelligentPointer<CThingCreatureBase> speaker,
    CIntelligentPointer<const CThing> target, unsigned long speech, bool interrupt, long priority)
{
    g_speaks++; g_speaker = speaker.GetPItem(); g_speakTarget = target.GetPItem(); g_speech = speech; g_speakInt = interrupt; g_speakPrio = priority; g_speakRepeat = repeat;
    return 0;
}
static int g_terminated;
void CAIStateGroupBase::Terminate() { g_terminated++; }
CAIStateGroupBase::~CAIStateGroupBase() { }

static int fails;
static void check(int c, const char* w) { if (!c) { fails++; printf("FAIL %s\n", w); } }

static double g_creatureBuf[0x20], g_shopThingBuf[0x20], g_stallBuf[0x20], g_keeperBuf[4], g_endKeeperBuf[4];
static CDefClassBase g_shopClassDef;

static unsigned long ExpectedRandom(unsigned long seed, unsigned long range, unsigned long* newSeed)
{
    seed = seed * 0x24A1U + 0x24DFU;
    seed = (seed >> 13) | (seed << 19);
    *newSeed = seed;
    return seed % range;
}

int main()
{
    CThing* creature = (CThing*)g_creatureBuf;
    CThing* shopThing = (CThing*)g_shopThingBuf;
    CThing* stall = (CThing*)g_stallBuf;
    CTCShopKeeper* keeper = (CTCShopKeeper*)g_keeperBuf;
    CTCShopKeeper* endKeeper = (CTCShopKeeper*)g_endKeeperBuf;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x90; entries[0].component = (CTCBase*)keeper;
    entries[1].type = 0; entries[1].component = (CTCBase*)endKeeper;
    creature->m_interfaces.first = entries; creature->m_interfaces.last = entries + 1;
    creature->m_presentBits[0x90 >> 5] = 1UL << (0x90 & 31);
    shopThing->PDef.Object = &g_shopClassDef;
    static CTCShop shop; shop.PThing = shopThing; g_shop = &shop;
    static CHawkingDef hawk; hawk.HawkSpeech = 0x1234; hawk.m_refCount = 1; g_subDef = &hawk;
    static CAIBrain brain; brain.PCreature = creature;
    CAIStateGroup_MinionHawkingFromStall* g = new CAIStateGroup_MinionHawkingFromStall;
    g->PBrain = &brain; g->PStall.m_pItem = stall;

    // 1: reached the stall -> hawk, acquire, schedule, terminate, last release destroys the sub-def
    g_seed = 7; g_moveResult = 2;
    g->Update();
    unsigned long seed1; unsigned long r1 = ExpectedRandom(7, 75, &seed1);
    check(g_lbKey == 0x90 && g_shopThis == keeper && g_subOwner == &g_shopClassDef, "lookup");
    check(g_moves == 1 && g_moveTarget == stall && g_moveProx == 1.5f && g_moveArgs[0] == 1 && g_moveArgs[1] == 3 && g_moveArgs[2] == 0xb && g_moveArgs[3] == 1, "move args");
    check(g_speaks == 1 && g_speaker == creature && g_speakTarget == stall && g_speech == 0x1234 && !g_speakInt && g_speakPrio == 0x93 && g_speakRepeat == 1, "speak args");
    check(g_acquired == 1 && g_acqThis == keeper, "acquire");
    check(g_seed == seed1 && g->NextHawkFrame == (long)(1000 + r1 + 75), "next hawk frame");
    check(g_terminated == 1 && g_destroyed == 1 && hawk.m_refCount == 0, "terminate + destroy");

    // 2: not there yet -> no speech, no customer; shared sub-def survives
    hawk.m_refCount = 2; g_moveResult = 1; g_worldFrame = 5000; g_seed = 0xdeadbeef;
    g->Update();
    unsigned long seed2; unsigned long r2 = ExpectedRandom(0xdeadbeef, 75, &seed2);
    check(g_moves == 2 && g_speaks == 1 && g_acquired == 1, "not reached");
    check(g->NextHawkFrame == (long)(5000 + r2 + 75) && g_terminated == 2 && g_destroyed == 1 && hawk.m_refCount == 1, "shared def kept");

    // 3: lookup lands on the end slot -> the end slot's keeper
    hawk.m_refCount = 1; g_moveResult = 2; g_lbOffset = 1;
    g->Update();
    check(g_shopThis == endKeeper && g_acqThis == endKeeper && g_destroyed == 2, "end pick");
    // 4: lookup lands on a greater key -> end slot
    hawk.m_refCount = 1; g_lbOffset = 0; entries[0].type = 0x91;
    g->Update();
    check(g_shopThis == endKeeper && g_acqThis == endKeeper, "greater-key pick");
    // 5: hit again
    hawk.m_refCount = 1; entries[0].type = 0x90;
    g->Update();
    check(g_shopThis == keeper && g_acqThis == keeper, "hit pick");
    // 6: presence bit clear -> no lookup (keeper is whatever the slot held; the stubs ignore it)
    hawk.m_refCount = 1; g_lbKey = 0; creature->m_presentBits[0x90 >> 5] = 0; int acq = g_acquired;
    g->Update();
    check(g_lbKey == 0 && g_acquired == acq + 1 && g_terminated == 6, "presence clear");
    // 7: other bits of the word do not count
    creature->m_presentBits[0x90 >> 5] = ~(1UL << (0x90 & 31)); hawk.m_refCount = 1;
    g->Update();
    check(g_lbKey == 0, "other bits");
    if (!fails) printf("0094bb20_TEST PASS\n");
    return fails;
}