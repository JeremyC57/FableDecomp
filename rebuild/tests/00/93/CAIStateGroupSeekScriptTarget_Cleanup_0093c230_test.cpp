// CAIStateGroup_SeekScriptTarget::Cleanup() -- if the script target is still alive and has interface 0x36, tell that component the
// creature stopped seeking it (thiscall 0x00747B80 with the creature and 0); then clear PTarget.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093C230, length 169 bytes (no padding: the next function follows).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times: IS_THING_ALIVE then the thing),
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x36 = bit 22 of the bitset dword at +0x24),
// the component's RemoveSeeker @ 0x00747B80, the converting CIntelligentPointer temporary from 0 (vtable 0x01238C6C):
// ctor @ 0x00A01B10, SetPItem @ 0x00A01B90 (twice), GetPItem @ 0x00A01B50, dtor @ 0x00A01C10.
// Shape: IS_THING_ALIVE(PTarget) inlined (two GetPItem calls), a third GetPItem feeding the out-parameter
// GetInterface (`test dword [eax+24h]`) whose entry pick is one end/hit ternary, so the component loads straight into
// ecx between the two brain loads; `PTarget = 0` through the temporary in the key slot.
// The engine header flattens the base, so the class is modelled locally; PTarget is +0x14.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointerCtor& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CTCSeekTarget {                          // the interface-0x36 component
    void RemoveSeeker(CThing* seeker, long flags);  // 0x00747B80
};

struct CAIStateGroup_SeekScriptTarget : CAIStateGroupBase {
    CIntelligentPointerThing PTarget;            // +0x14
    virtual void Cleanup();
};

void CAIStateGroup_SeekScriptTarget::Cleanup()
{
    if (IS_THING_ALIVE(PTarget)) {
        CThing* enemy = PTarget.GetPItem();
        CTCSeekTarget* seek;
        if (enemy->GetInterface(&seek, 0x36))
            seek->RemoveSeeker(PBrain->PCreature, 0);
    }
    PTarget = 0;
}
// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
void CBaseIntelligentPointerCtor::v00() { }
CBaseIntelligentPointerCtor::CBaseIntelligentPointerCtor() { m_pItem = 0; }
CThing* CBaseIntelligentPointerCtor::GetPItem() const { return m_pItem; }
static int g_sets = 0;
void CBaseIntelligentPointerCtor::SetPItem(CThing* item) { ++g_sets; m_pItem = item; }
CBaseIntelligentPointerCtor::~CBaseIntelligentPointerCtor() { }
static int g_removes = 0; static CThing* g_seeker = 0; static long g_flags = -1; static const void* g_seekThis = 0;
void CTCSeekTarget::RemoveSeeker(CThing* seeker, long flags) { ++g_removes; g_seeker = seeker; g_flags = flags; g_seekThis = this; }
static CThing g_enemy, g_me;
static CAIBrain g_brain;
static CTCSeekTarget g_seek;
static CInterfaceMap::SEntry g_entries[2];
static long g_mem[(sizeof(CAIStateGroup_SeekScriptTarget) + 3) / 4];      // the intelligent-pointer member has no default constructor
int main()
{
    g_entries[0].type = 0x10; g_entries[0].component = 0;
    g_entries[1].type = 0x36; g_entries[1].component = (CTCBase*)&g_seek;
    g_enemy.m_interfaces.first = g_entries; g_enemy.m_interfaces.last = g_entries + 2;
    g_enemy.m_presentBits[0x36 >> 5] = 1UL << (0x36 & 31);
    g_brain.PCreature = &g_me;
    CAIStateGroup_SeekScriptTarget* s = (CAIStateGroup_SeekScriptTarget*)g_mem;
    s->PBrain = &g_brain;
    s->PTarget.m_pItem = &g_enemy;
    s->CAIStateGroup_SeekScriptTarget::Cleanup();
    CHECK(g_removes == 1 && g_seeker == &g_me && g_flags == 0 && g_seekThis == &g_seek && s->PTarget.m_pItem == 0);
    s->PTarget.m_pItem = &g_enemy; g_enemy.m_presentBits[0x36 >> 5] = 0;   // no seek component
    s->CAIStateGroup_SeekScriptTarget::Cleanup();
    CHECK(g_removes == 1 && s->PTarget.m_pItem == 0);
    g_enemy.m_presentBits[0x36 >> 5] = 1UL << (0x36 & 31); g_enemy.m_flags91 = 1;   // enemy dead
    s->PTarget.m_pItem = &g_enemy;
    s->CAIStateGroup_SeekScriptTarget::Cleanup();
    CHECK(g_removes == 1 && s->PTarget.m_pItem == 0);
    s->CAIStateGroup_SeekScriptTarget::Cleanup();                               // no enemy at all
    CHECK(g_removes == 1 && s->PTarget.m_pItem == 0);
    if (fails == 0) printf("0093c230_TEST PASS\n");
    return fails;
}