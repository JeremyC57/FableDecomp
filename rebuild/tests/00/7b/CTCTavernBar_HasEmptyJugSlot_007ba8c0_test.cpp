// Behaviour test for CTCTavernBar (0x007ba8c0): slot walk, jug filter, ACTION_POINT_*_0<n> naming, first claimed point wins.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __declspec(x)
#define __forceinline inline
#define __fastcall
#endif
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                // +0x20
    CInterfaceMap  m_interfaces;                    // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
class CThingObject : public CThing { };
struct C3DVector { float X; float Y; float Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CCharString {
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
    CCharString& operator+=(const char* text);      // 0x0099F100
};
CCharString __fastcall GFIntToCharString(long value);   // 0x0099F830
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +4
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingObject>, vtable 0x0125A858
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.CBaseIntelligentPointer::GetPItem()); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct CJugVector {                                 // retail STLport vector<CIntelligentPointer<CThingObject> > (12 bytes)
    CIntelligentPointer<CThingObject>* first;
    CIntelligentPointer<CThingObject>* last;
    CIntelligentPointer<CThingObject>* endOfStorage;
    unsigned long size() const { return last - first; }
    const CIntelligentPointer<CThingObject>& operator[](unsigned long i) const { return first[i]; }
};
struct CTCJug {                                     // interface 0xBB
    unsigned char _pad00[0x0c];
    void* Contents;                                 // +0x0c (descriptive)
    bool Filled;                                    // +0x10 (descriptive)
};
struct CTCNamedUserActionPoints {                   // interface 0x2E
    bool GetFreeNamedActionPoint(CThing& user, C3DVector& pos, CRightHandedSet& rh, CCharString name, long& pointIndex, long flags = 0, bool claim = true);   // 0x007FF190
};
struct CTCTavernBar {
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CJugVector Jugs;                                // +0x0c (PDB +0x0c; retail vector 12 bytes)
    __forceinline CIntelligentPointer<CThingObject> GetJug(long index) const
    {
        if ((unsigned long)index >= Jugs.size())
            return CIntelligentPointer<CThingObject>(0);
        CIntelligentPointer<CThingObject>* first = Jugs.first;
        return first[index];
    }
    void OnActionUse(long pointIndex);              // 0x007BA4F0
    bool HasEmptyJugSlot(CThing& user);
};

bool CTCTavernBar::HasEmptyJugSlot(CThing& user)
{
    CTCNamedUserActionPoints* points;
    if (ParentThing->HasTC(0x2e))
        points = ParentThing->GetTC<CTCNamedUserActionPoints>(0x2e);
    C3DVector pos;
    CRightHandedSet rh;
    long pointIndex;
    for (long slot = 0; (unsigned long)slot < Jugs.size(); ++slot) {
        CIntelligentPointer<CThingObject> jug = GetJug(slot);
        if (jug.GetPItem()) {
            CThingObject* item = jug.GetPItem();
            if (!item->HasTC(0xbb))
                continue;
            CTCJug* tc = item->GetTC<CTCJug>(0xbb);
            if (!tc->Filled || tc->Contents)
                continue;
        }
        CCharString unused;
        CCharString name("ACTION_POINT_BARMAN");
        name += "_0";
        name += GFIntToCharString(slot + 1);
        if (points->GetFreeNamedActionPoint(user, pos, rh, name, pointIndex)) {
            OnActionUse(pointIndex);
            return true;
        }
    }
    return false;
}

// ---- stubs ----
CThing g_barThing;
CTCNamedUserActionPoints g_points;
static char g_tried[16][64];
static int g_nTried = 0;
static const char* g_accept = "";
static long g_released = -1;
static CThingObject g_jugThings[4];
static CTCJug g_jugTCs[4];
static CInterfaceMap::SEntry g_entries[4][1];
CCharString::CCharString() { PStringData = 0; }
CCharString::CCharString(const char* text, long) { char* p = (char*)std::malloc(64); std::strcpy(p, text); PStringData = p; }
CCharString::CCharString(const CCharString& other) { char* p = (char*)std::malloc(64); std::strcpy(p, other.PStringData ? (const char*)other.PStringData : ""); PStringData = p; }
CCharString::~CCharString() { std::free(PStringData); }
CCharString& CCharString::operator+=(const CCharString& other) { std::strcat((char*)PStringData, (const char*)other.PStringData); return *this; }
CCharString& CCharString::operator+=(const char* text) { std::strcat((char*)PStringData, text); return *this; }
CCharString __fastcall GFIntToCharString(long value) { char b[16]; std::sprintf(b, "%ld", value); return CCharString(b); }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::v00() { }
bool CTCNamedUserActionPoints::GetFreeNamedActionPoint(CThing&, C3DVector&, CRightHandedSet&, CCharString name, long& pointIndex, long, bool)
{
    std::strcpy(g_tried[g_nTried++], (const char*)name.PStringData);
    if (std::strcmp((const char*)name.PStringData, g_accept) == 0) { pointIndex = 40 + g_nTried; return true; }
    return false;
}
static void SetupJug(int i, bool present, bool filled, void* contents)
{
    g_jugTCs[i].Filled = filled; g_jugTCs[i].Contents = contents;
    for (int w = 0; w < 9; ++w) g_jugThings[i].m_presentBits[w] = 0;
    g_jugThings[i].m_presentBits[0xbb >> 5] = present ? (1UL << (0xbb & 31)) : 0;
    g_entries[i][0].type = 0xbb; g_entries[i][0].component = (CTCBase*)&g_jugTCs[i];
    g_jugThings[i].m_interfaces.first = g_entries[i]; g_jugThings[i].m_interfaces.last = g_entries[i] + 1;
}
static CInterfaceMap::SEntry g_barEntries[1];
static void SetupBar(CTCTavernBar& bar, CIntelligentPointer<CThingObject>* jugs, int n)
{
    for (int w = 0; w < 9; ++w) g_barThing.m_presentBits[w] = 0;
    g_barThing.m_presentBits[0x2e >> 5] = 1UL << (0x2e & 31);
    g_barEntries[0].type = 0x2e; g_barEntries[0].component = (CTCBase*)&g_points;
    g_barThing.m_interfaces.first = g_barEntries; g_barThing.m_interfaces.last = g_barEntries + 1;
    bar.ParentThing = &g_barThing;
    bar.Jugs.first = jugs; bar.Jugs.last = jugs + n; bar.Jugs.endOfStorage = jugs + n;
}
static int fails = 0;
static void check(bool c, const char* what) { if (!c) { std::printf("FAIL %s\n", what); ++fails; } }
static void Reset(const char* accept) { g_nTried = 0; g_accept = accept; g_released = -1; }
void CTCTavernBar::v00() { }
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = 0; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
template <class T> void CIntelligentPointer<T>::v00() { }
void CTCTavernBar::OnActionUse(long pointIndex) { g_released = pointIndex; }
int main()
{
    CTCTavernBar bar; CThing user;
    CIntelligentPointer<CThingObject> jugs[2] = { CIntelligentPointer<CThingObject>(0), CIntelligentPointer<CThingObject>(0) };
    SetupBar(bar, jugs, 2);
    SetupJug(0, false, true, 0); jugs[0].SetPItem(&g_jugThings[0]);      // jug without component: skipped
    Reset("ACTION_POINT_BARMAN_02");
    bool ok = bar.HasEmptyJugSlot(user);
    check(ok && g_nTried == 1 && g_released == 41, "free point found and released");
    Reset("none");
    ok = bar.HasEmptyJugSlot(user);
    check(!ok && g_nTried == 1 && g_released == -1, "none free");
    if (fails == 0) std::printf("007ba8c0_TEST PASS\n");
    return fails;
}