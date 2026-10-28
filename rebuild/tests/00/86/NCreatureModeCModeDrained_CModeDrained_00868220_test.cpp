#include <cstdio>
#include <cstring>
static int g_log[64];
static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
class CThing { public: int id; };
class CCharString { public:
    const char* p;
    CCharString() : p(0) {}
    CCharString(const char* t, long = -1) : p(t) {}
    const CCharString& operator=(const CCharString& o) { p = o.p; return *this; }
    unsigned long GetLength() const { return p ? (unsigned long)std::strlen(p) : 0; }
    const char* GetPString() const { return p ? p : ""; }
};
static unsigned long Calc(unsigned long seed, const void* d, unsigned long n)
{
    const unsigned char* c = (const unsigned char*)d;
    unsigned long h = seed;
    for (unsigned long i = 0; i < n; ++i) h = h * 33 + c[i];
    return h;
}
class CAnimFilterOfGroup { public:
    CCharString   Group;
    unsigned long GroupCRC;
    CAnimFilterOfGroup() : Group(), GroupCRC(0) {}
    CAnimFilterOfGroup(const CCharString& g) : Group() { Group = g; GroupCRC = Calc(0, g.GetPString(), g.GetLength()); }
    void SetGroup(const CCharString& g) { Group = g; GroupCRC = Calc(0, g.GetPString(), g.GetLength()); }
};
static bool FilterIs(const CAnimFilterOfGroup& f, const char* s)
{
    return f.Group.p != 0 && std::strcmp(f.Group.p, s) == 0 && f.GroupCRC == Calc(0, s, (unsigned long)std::strlen(s));
}
struct CIP {
    CThing* PItem;
    int     built;
    CIP() : PItem(0), built(1) {}
    CIP(CThing* t) : PItem(0), built(1) { PItem = t; }
};
template <class T> struct CVector { T* a; T* b; T* c; CVector() : a(0), b(0), c(0) {} };
template <class T> class CDefPointer { public: T* Object; CDefPointer() : Object(0) {} };
class C3DVector { public: float X, Y, Z; };
struct CModeStandBase {
    CThing* ParentThing;
    CModeStandBase(CThing* t) : ParentThing(t) { Log(1); }
};
// behaviour copy of NCreatureMode::CModeDrained::CModeDrained(CThing*, CThing*): the drainer's current mode type is looked up through its interface 0x31
struct CModeBase { virtual unsigned long GetModeType() { return 7; } virtual ~CModeBase() {} };
struct CMode9 : CModeBase { unsigned long GetModeType() { return 9; } };
class CTCCreatureModeManager { public: CModeBase* current; CModeBase* GetCurrentMode() { return current; } };
struct SEntry { long type; void* component; };
struct CDrainer : CThing {
    unsigned long m_presentBits[9]; SEntry entries[2]; SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType; SEntry* it = LowerBound(&type);
            if (it == last || interfaceType < it->type) it = last;
            *ppOut = (T*)it->component; return true;
        }
        return false;
    }
};
struct CModeDrained : CModeStandBase {
    CAnimFilterOfGroup Filter; CIP PDrainer; unsigned long DrainerMode;
    CModeDrained(CThing* thing, CDrainer* drainer) : CModeStandBase(thing), Filter("DRAINED"), PDrainer(drainer)
    {
        CTCCreatureModeManager* modes;
        drainer->GetInterface(&modes, 0x31);
        DrainerMode = modes->GetCurrentMode()->GetModeType();
    }
};
int main()
{
    CMode9 mode; CTCCreatureModeManager mgr; mgr.current = &mode;
    CDrainer d; std::memset(d.m_presentBits, 0, sizeof(d.m_presentBits)); d.m_presentBits[1] = 0x20000;
    d.entries[0].type = 0x10; d.entries[0].component = 0; d.entries[1].type = 0x31; d.entries[1].component = &mgr;
    d.first = d.entries; d.last = d.entries + 2;
    CThing t; CModeDrained m(&t, &d);
    if (g_n != 1 || !FilterIs(m.Filter, "DRAINED") || m.PDrainer.PItem != &d || m.DrainerMode != 9)
    { std::printf("00868220_TEST FAIL\n"); return 1; }
    std::printf("00868220_TEST PASS\n");
    return 0;
}