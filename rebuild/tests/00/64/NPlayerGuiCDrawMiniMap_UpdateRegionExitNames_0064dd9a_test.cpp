// Behaviour test for 0x0064DD9A NPlayerGui::CDrawMiniMap::UpdateRegionExitNames (links the REAL row object, with the same-TU COMDATs
// it carries: GetAllThings bodies, the HasTCOfLeafType filter body, list / vector / pair bodies, bitset test, CVectorMap LowerBound).
// The engine callees outside the TU are defined here over raw native-layout storage; every offset is one the row (or its COMDATs)
// reads: CDrawMiniMap World +0x0C (0x0064DDB7), RegionExits {start, finish, end} +0x154 (0x0064DDA5); CWorld PeekWorldMap vslot 13
// (+0x34, 0x0064DDBC), thing search tools +0x20 (0x0064DDF1); CRegion Name +0x18 (0x0064DDC4); CThing UsingComponents bitset +0x20
// (interface 100 = byte +0x2C bit 4), Components {start, finish} +0x44 / +0x48 (0x0064DF12), PTCDriver +0x5C, CurrentMapNumber +0x8E
// (0x0064DE0F), scan flags byte +0x91 (IncludeInThingScans 0x04, Activated 0x08); CTCDRegionExit Active +0x0C (0x0064DE34);
// CTCActionUseScriptedHook Usable +0x0C (0x0064DF39), PEntranceConnectedTo +0x20 (0x0064DF3F, PItem at +4 behind its vptr), class
// type name through vslot 22 (+0x58).
// Expected: RegionExits cleared, then for every scan-visible thing with a region-exit TC on a map of this region whose exit is active,
// {position (+ text offset X/Y looked up by the exit's connecting-region name), display name}; then the same for every thing whose 0xC2
// component is exactly a CTCActionUseScriptedHook, usable, with an entrance connected, using the entrance names. List order kept.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
inline void* operator new(unsigned int, void* where) { return where; }

// ---- CCharString (one data pointer; data block {char* text +0, length +4, ..., refs +0x0D}) ----
template<class T> class CBasicString { protected: static long __fastcall Compare(const T* a, const T* b); };
template<> long __fastcall CBasicString<char>::Compare(const char* a, const char* b) { return strcmp(a, b); }
struct StrData { char* text; unsigned long length; unsigned char pad[0x0D - 8]; long refs; unsigned char tail[3]; };
static int g_strings;   // live string data blocks
class CCharString {
public:
    StrData* d;
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
    ~CCharString();
    const CCharString& operator=(const CCharString& other);
};
static StrData* MakeData(const char* text) { StrData* d = (StrData*)calloc(1, sizeof(StrData)); d->length = (unsigned long)strlen(text);
    d->text = (char*)malloc(d->length + 1); memcpy(d->text, text, d->length + 1); d->refs = 1; ++g_strings; return d; }
static void FreeData(StrData* d) { if (d) { free(d->text); free(d); --g_strings; } }
CCharString::CCharString(const char* text, long) { d = MakeData(text); }
CCharString::CCharString(const CCharString& other) { d = MakeData(other.d->text); }
CCharString::~CCharString() { FreeData(d); d = 0; }
const CCharString& CCharString::operator=(const CCharString& other) { if (this != &other) { FreeData(d); d = MakeData(other.d->text); } return *this; }

class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; };
class CBaseObject;
class CBaseIntelligentPointer { public: CBaseObject* GetPItem() const; };
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return *(CBaseObject* const*)((const char*)this + 4); }

static int failures;
static void check(bool ok, const char* what) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s\n", what); } }
template<class T> static void put(void* base, int off, T v) { memcpy((unsigned char*)base + off, &v, sizeof v); }
template<class T> static T get(const void* base, int off) { T v; memcpy(&v, (const unsigned char*)base + off, sizeof v); return v; }

// ---- things ----
enum { NT = 12 };
static union { unsigned char b[NT][0x98]; double align; } things;
struct ExitTC { unsigned char b[0x38]; };
static ExitTC exits[NT];
static const char* exit_names[NT];       // GetRegionName / GetRegionEntranceName
static const char* display_names[NT];    // GetDisplayName / GetRegionEntranceDisplayName
static C3DVector positions[NT];
static int Index(const void* thing) { return (int)(((const unsigned char*)thing - things.b[0]) / 0x98); }
static int ExitIndex(const void* tc) { return (int)((const ExitTC*)tc - exits); }

class CThing { public: const C3DVector& GetPos() const; };
const C3DVector& CThing::GetPos() const { return positions[Index(this)]; }

class CTCDRegionExit { public: const CCharString& GetRegionName() const; const CCharString& GetDisplayName() const; };
static CCharString* exit_name_str[NT];
static CCharString* display_str[NT];
static int name_calls, display_calls;
const CCharString& CTCDRegionExit::GetRegionName() const { ++name_calls; return *exit_name_str[ExitIndex(this)]; }
const CCharString& CTCDRegionExit::GetDisplayName() const { ++display_calls; return *display_str[ExitIndex(this)]; }

// scripted hooks: real objects with a vtable (class type name at slot 22)
class FakeHook {
public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {} virtual void v16() {} virtual void v17() {}
    virtual void v18() {} virtual void v19() {} virtual void v20() {} virtual void v21() {}
    virtual CCharString GetClassTypeName() const { return CCharString(class_name); }
    unsigned char pad04[8];        // +0x04
    bool Usable;                   // +0x0C
    unsigned char pad0d[0x13];     // +0x0D
    void* entrance_vptr;           // +0x20 CIntelligentPointer<CThing>
    void* entrance;                // +0x24 its PItem
    const char* class_name;
    int index;
};
static FakeHook hooks[NT];
class CTCActionUseScriptedHook { public: const CCharString& GetRegionEntranceName() const; const CCharString& GetRegionEntranceDisplayName() const; };
const CCharString& CTCActionUseScriptedHook::GetRegionEntranceName() const { ++name_calls; return *exit_name_str[((const FakeHook*)this)->index]; }
const CCharString& CTCActionUseScriptedHook::GetRegionEntranceDisplayName() const { ++display_calls; return *display_str[((const FakeHook*)this)->index]; }

class CThingFilter_HasTC { public: bool operator()(const CThing& thing) const; long Type; };
static long filter_type_seen = -1;
static bool HasBit(const void* thing, long type) { return (get<unsigned char>(thing, 0x20 + type / 8) >> (type % 8)) & 1; }
bool CThingFilter_HasTC::operator()(const CThing& thing) const { filter_type_seen = Type; return HasBit(&thing, Type); }

// the class list PeekClassList returns: an STLport list<CClass*> {head node}, node {next, prev, data}
struct Node { Node* next; Node* prev; void* data; };
static Node head, nodes[NT];
static struct { Node* node; } class_list = { &head };
class CClass;
namespace std { template<class T> class allocator; template<class T, class A> class list; }
class CThingSearchTools { protected: const std::list<CClass*, std::allocator<CClass*> >& PeekClassList() const; };
const std::list<CClass*, std::allocator<CClass*> >& CThingSearchTools::PeekClassList() const
{ return *(const std::list<CClass*, std::allocator<CClass*> >*)(const void*)&class_list; }

class CWorldMap { public: long GetRegionNumberFromName(const CCharString& name) const; long GetRegionNumberMapIsIn(long map) const; };
static unsigned char world_map_storage[16];
static const void* name_receiver; static const char* name_asked;
long CWorldMap::GetRegionNumberFromName(const CCharString& name) const { name_receiver = this; name_asked = name.d->text; return strcmp(name.d->text, "Region1") == 0 ? 1 : 99; }
long CWorldMap::GetRegionNumberMapIsIn(long map) const { return this == (const void*)world_map_storage ? map / 10 : 77; }

class CRegion { public: bool GetMiniMapRegionExitTextOffset(const CCharString& exit_name, C2DVector& offset) const; };
static union { unsigned char b[0x58]; double align; } region;
static int offset_calls;
bool CRegion::GetMiniMapRegionExitTextOffset(const CCharString& exit_name, C2DVector& offset) const
{
    ++offset_calls;
    if (this != (const void*)region.b) return false;
    if (strcmp(exit_name.d->text, "R_A") == 0) { offset.X = 1.5f; offset.Y = -2.5f; return true; }
    if (strcmp(exit_name.d->text, "E_C") == 0) { offset.X = 3.0f; offset.Y = 4.0f; return true; }
    offset.X = 100.0f; offset.Y = 100.0f;   // written even when not found: a row that ignores the result would add it
    return false;
}

class FakeWorld {
public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void* GetWorldMap() { return 0; }                                          // slot 12
    virtual const void* PeekWorldMap() const { return world_map_storage; }             // slot 13
    unsigned char pad04[0x1C];
    void* search_tools;                                                                // +0x20
};
static FakeWorld world;
static unsigned char search_tools_storage[16];

namespace NPlayerGui {
class CDrawMiniMap { public: static void Run(void* self, const CRegion& r) { ((CDrawMiniMap*)self)->UpdateRegionExitNames(r); }
protected: void UpdateRegionExitNames(const CRegion& region); };
}
static union { unsigned char b[0x160]; double align; } minimap;

struct Label { C3DVector pos; CCharString name; };   // pair<C3DVector,CCharString>, 0x10 bytes

enum Kind { NONE, EXIT, HOOK };
static void MakeThing(int i, Kind kind, long map, bool on, bool connected, const char* ename, const char* dname, bool scan = true, const char* cls = "CTCActionUseScriptedHook")
{
    unsigned char* t = things.b[i];
    memset(t, 0, 0x98);
    put<short>(t, 0x8e, (short)map);
    put<unsigned char>(t, 0x91, scan ? 0x0C : 0x08);
    positions[i].X = 10.0f * i; positions[i].Y = 10.0f * i + 1; positions[i].Z = 10.0f * i + 2;
    exit_names[i] = ename; display_names[i] = dname;
    exit_name_str[i] = new CCharString(ename ? ename : "?"); display_str[i] = new CCharString(dname ? dname : "?");
    if (kind == EXIT) {
        t[0x20 + 100 / 8] |= 1 << (100 % 8);
        memset(&exits[i], 0, sizeof exits[i]);
        exits[i].b[0x0C] = on;
        put<void*>(t, 0x5C, &exits[i]);
    } else if (kind == HOOK) {
        t[0x20 + 194 / 8] |= 1 << (194 % 8);
        FakeHook& h = hooks[i];
        h.Usable = on; h.entrance_vptr = 0; h.entrance = connected ? (void*)&things.b[(i + 1) % NT] : 0; h.class_name = cls; h.index = i;
        // Components: sorted {key, CTCBase*} pairs, the hook between two other keys
        static unsigned long comp[NT][6];
        comp[i][0] = 5; comp[i][1] = 0; comp[i][2] = 194; comp[i][3] = (unsigned long)&h; comp[i][4] = 300; comp[i][5] = 0;
        put<void*>(t, 0x44, comp[i]); put<void*>(t, 0x48, comp[i] + 6); put<void*>(t, 0x4C, comp[i] + 6);
    }
}

int main()
{
    put<void*>(minimap.b, 0x0C, &world);
    world.search_tools = search_tools_storage;
    CCharString region_name("Region1");
    memcpy(region.b + 0x18, &region_name, 4);   // Name +0x18 shares region_name's data block
    // things (list order 0..11)
    MakeThing(0, EXIT, 10, true, false, "R_A", "A Disp");          // active, offset (1.5, -2.5)
    MakeThing(1, EXIT, 20, true, false, "R_A", "wrong region");    // region 2
    MakeThing(2, EXIT, 11, false, false, "R_A", "inactive");       // inactive exit
    MakeThing(3, EXIT, 11, true, false, "R_B", "B Disp");          // active, no offset
    MakeThing(4, HOOK, 10, true, true, "E_C", "C Disp");           // usable + connected, offset (3, 4)
    MakeThing(5, HOOK, 10, true, false, "E_C", "unconnected");     // no entrance
    MakeThing(6, HOOK, 12, false, true, "E_C", "unusable");        // not usable
    MakeThing(7, HOOK, 21, true, true, "E_C", "wrong region");     // region 2
    MakeThing(8, HOOK, 19, true, true, "E_D", "D Disp");           // usable + connected, no offset
    MakeThing(9, HOOK, 10, true, true, "E_C", "other class", true, "CTCOther");   // 0xC2 component of another class
    MakeThing(10, EXIT, 10, true, false, "R_A", "not scanned", false);           // excluded from thing scans
    MakeThing(11, NONE, 10, true, false, 0, 0);                    // no component
    head.next = &nodes[0]; head.prev = &nodes[NT - 1];
    for (int i = 0; i < NT; ++i) { nodes[i].data = things.b[i]; nodes[i].next = i + 1 < NT ? &nodes[i + 1] : &head; nodes[i].prev = i ? &nodes[i - 1] : &head; }
    // RegionExits starts with two stale labels
    Label* old = (Label*)malloc(4 * sizeof(Label));
    for (int k = 0; k < 2; ++k) { old[k].pos.X = old[k].pos.Y = old[k].pos.Z = -1.0f; new (&old[k].name) CCharString("stale"); }
    put<Label*>(minimap.b, 0x154, old); put<Label*>(minimap.b, 0x158, old + 2); put<Label*>(minimap.b, 0x15C, old + 4);
    int strings_before = g_strings;

    NPlayerGui::CDrawMiniMap::Run(minimap.b, *(const CRegion*)(const void*)region.b);

    check(name_receiver == world_map_storage, "GetRegionNumberFromName on the world map");
    check(name_asked && strcmp(name_asked, "Region1") == 0, "region number asked for the region's name");
    check(filter_type_seen == 100, "first search filters on TCI_REGION_EXIT (100)");
    Label* s = get<Label*>(minimap.b, 0x154); Label* f = get<Label*>(minimap.b, 0x158);
    const int expect[4] = { 0, 3, 4, 8 };
    const float ox[4] = { 1.5f, 0.0f, 3.0f, 0.0f }, oy[4] = { -2.5f, 0.0f, 4.0f, 0.0f };
    check(f - s == 4, "four labels (stale ones cleared)");
    for (int k = 0; k < 4 && k < f - s; ++k) {
        int i = expect[k];
        check(s[k].pos.X == positions[i].X + ox[k], "label X (+ text offset X)");
        check(s[k].pos.Y == positions[i].Y + oy[k], "label Y (+ text offset Y)");
        check(s[k].pos.Z == positions[i].Z, "label Z unchanged");
        check(strcmp(s[k].name.d->text, display_names[i]) == 0, "label name = display name");
        if (failures && k < 4) printf("  label %d: %g %g %g %s\n", k, s[k].pos.X, s[k].pos.Y, s[k].pos.Z, s[k].name.d->text);
    }
    check(offset_calls == 4 && name_calls == 4 && display_calls == 4, "one name / offset / display lookup per accepted thing");
    check(g_strings == strings_before - 2 + 4, "temporaries destroyed, stale labels destroyed");
    printf(failures ? "0064dd9a_TEST FAIL\n" : "0064dd9a_TEST PASS\n");
    return failures ? 1 : 0;
}