// Behaviour test for 00749f00: src/00749f00.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCCreatureGenerator::Validate(CCharString& error) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCreatureGenerator::Validate(CCharString& error) const -- false (with the reason in error) when no creature families are set, or when a family generates hidden without hidden start positions, uses the unsupported generator-animation type, or generates in the open without open positions
// Leaf: Validate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00749F00, 512 bytes (`ret 4`, 0x18-byte frame, no EH frame).
//         Ego_r.pdb: `?Validate@CTCCreatureGenerator@@UBE_NAAVCCharString@@@Z` (vtable slot 17).
// Callees: CCharString::operator=(const char*) @ 0x0099EFE0, operator=(const CCharString&) @ 0x0099EFB0, operator+=(const char*)
//          @ 0x0099F100, ~CCharString @ 0x0099EAE0, operator+(const char*, const CCharString&) @ 0x0099F690 (__fastcall, hidden
//          result in ecx); CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDef<CCreatureGenerationFamilyDef>
//          @ 0x0074D220 (catalog "GetDef<CThingPhysicalSwitchDef>", one body of the shared template); the def's virtual slot 10
//          GetDefName (+0x28, CDefString by value) and slot 1 Destroy; CDefStringTable::GetString(long) @ 0x009D49B0 on
//          g_DefStringTable 0x013CA828; the STLport random-access __find<long> @ 0x0074C9B0 (__fastcall, value and tag by reference).
// Layout: retail CTCCreatureGenerator: CreatureFamilies (CArray<CDefIndex>) begin +0x0c / end +0x10, OpenPositions begin +0x60 /
//         end +0x64, HiddenPositions begin +0x6c / end +0x70 (12-byte retail vectors; Ego_r.pdb +0x70 / +0x80 with 16-byte ones).
//         CCreatureGenerationFamilyDef GenerationTypes begin +0x50 / end +0x54 (PDB +0x58).
// Strings: 0x01268BD8, 0x01268BC0, 0x01268B98, 0x01268B50, 0x01268B00.
// Shape: the family name is a named CCharString local in its own block (both temporaries destroyed right after the
//        assignment, the name pushed by `lea`); the def is a CDefPointer local filled by a same-TU GetDef stand-in and released
//        inline at each exit without a null test; the __find instantiation is compiled in this TU with its real read-only body,
//        so the type range is read once (end into edi before begin into ebp: `last` declared first); value 2 lives in its own
//        slot set once before the loop, values 3/0/1 reuse the def's frame word, and the empty iterator tag is homed in the
//        (still live) error argument slot; the family index is passed through CDefIndex's conversion to long.
class CCharString { public:
    void* PStringData;
    ~CCharString();                                                  // 0x0099EAE0
    const CCharString& operator=(const char* text);                        // 0x0099EFE0
    const CCharString& operator=(const CCharString& other);                // 0x0099EFB0
    CCharString& operator+=(const char* text);                       // 0x0099F100
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
struct CDefStringTable {
    CCharString GetString(long id);                                  // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                             // 0x013CA828
class CDefString { public:                                                  // a def name: its offset into the def string table
    long m_index;
    CDefString(const CDefString& other) : m_index(other.m_index) { }
};
enum ECreatureGeneratorGenerateType {
    GENERATE_TYPE_0 = 0,                                             // needs open positions
    GENERATE_TYPE_1 = 1,                                             // needs open positions
    GENERATE_TYPE_2 = 2,                                             // needs hidden start positions
    GENERATE_GENERATOR_ANIMATION = 3                                 // not supported
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };
// The out-of-line STLport __find instantiation (retail 0x0074C9B0) is compiled in this TU: its body (the landed
// global_Find0074c9b0 four-way unrolled scan) only reads memory, so VC7.1 keeps the range in registers across the calls.
#pragma auto_inline(off)
template <class T> __declspec(noinline) const T* __fastcall __find(const T* first, const T* last, const T& value,
                                                                   const random_access_iterator_tag&)
{
    long count;
    for (count = (last - first) >> 2; 0 < count; --count) {
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == value) return first;
        ++first;
    case 2:
        if (*first == value) return first;
        ++first;
    case 1:
        if (*first == value) return first;
    case 0:
    default:
        break;
    }
    return last;
}
#pragma auto_inline(on)
template <class T> inline const T* find(const T* first, const T* last, const T& value)
{
    return __find(first, last, value, random_access_iterator_tag());
}

template <class T> struct CVector {                                  // STLport vector (12 bytes retail)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
};
class C3DVector { public: float X, Y, Z; };
class CDefIndex { public: long Index; operator long() const { return Index; } };

class CCreatureGenerationFamilyDef { public:
    virtual void v00();
    virtual void Destroy();                                          // slot 1
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetDefName() const;                           // slot 10 (+0x28)
    long RefCount;                                                   // +0x04
    unsigned char _pad08[0x48];
    CVector<long> GenerationTypes;                                   // +0x50
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
void* FindDef_CDefinitionManager_GetDef_0();                         // stand-in callee of the stand-in body below
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CCreatureGenerationFamilyDef>* out) const;   // 0x0074D220
};
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CCreatureGenerationFamilyDef>* out) const
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CTCCreatureGenerator {
public:
    virtual void v00();
    void*                   ParentThing;                             // +0x04 (CTCBase)
    long                    m_tcBase08;                              // +0x08
    CVector<CDefIndex>      CreatureFamilies;                        // +0x0c
    unsigned char           _pad18[0x48];
    CVector<C3DVector>      OpenPositions;                           // +0x60
    CVector<C3DVector>      HiddenPositions;                         // +0x6c
    CDefinitionManager* GetDefinitionManager() const;               // 0x006869D0 (CTCBase)
    virtual bool Validate(CCharString& error) const;
};

bool CTCCreatureGenerator::Validate(CCharString& error) const
{
    if (CreatureFamilies.empty()) {
        error = "no creature families specified - what am I supposed to generate?";
        return false;
    }
    for (const CDefIndex* it = CreatureFamilies.begin(); it != CreatureFamilies.end(); ++it) {
        CDefPointer<CCreatureGenerationFamilyDef> def;
        GetDefinitionManager()->GetDef(*it, &def);
        {
            CCharString name = g_DefStringTable.GetString(def->GetDefName().m_index);
            error = "creature family " + name;
        }
        const long* last = def->GenerationTypes.end();
        const long* first = def->GenerationTypes.begin();
        if (find(first, last, (long)GENERATE_TYPE_2) != last && HiddenPositions.empty()) {
            error += " needs to have hidden start positions";
            return false;
        }
        if (find(first, last, (long)GENERATE_GENERATOR_ANIMATION) != last) {
            error += " has generate type GENERATE_GENERATOR_ANIMATION and it's not supported";
            return false;
        }
        if (find(first, last, (long)GENERATE_TYPE_0) != last && OpenPositions.empty()) {
            error += " needs to create creatures in the open and I can't find any navigable positions";
            return false;
        }
        if (find(first, last, (long)GENERATE_TYPE_1) != last && OpenPositions.empty()) {
            error += " needs to create creatures in the open and I can't find any navigable positions";
            return false;
        }
    }
    return true;
}

// ---- stubs and driver ----
#include <stdlib.h>
// string stubs: PStringData is a heap copy (leaked on purpose: the model has no user copy constructor)
static int g_strDtors = 0;
static char* Dup(const char* a, const char* b) { size_t n = strlen(a), m = b ? strlen(b) : 0; char* p = (char*)malloc(n + m + 1); memcpy(p, a, n); if (b) memcpy(p + n, b, m); p[n + m] = 0; return p; }
static const char* Text(const CCharString& s) { return s.PStringData ? (const char*)s.PStringData : ""; }
CCharString::~CCharString() { ++g_strDtors; }
const CCharString& CCharString::operator=(const char* text) { PStringData = Dup(text, 0); return *this; }
const CCharString& CCharString::operator=(const CCharString& other) { PStringData = Dup(Text(other), 0); return *this; }
CCharString& CCharString::operator+=(const char* text) { PStringData = Dup(Text(*this), text); return *this; }
CCharString __fastcall operator+(const char* left, const CCharString& right) { CCharString r; r.PStringData = Dup(left, Text(right)); return r; }
CDefStringTable g_DefStringTable;
static const char* g_names[] = { "Bandits", "Wolves", "Hobbes" };
CCharString CDefStringTable::GetString(long id) { CCharString r; r.PStringData = Dup(g_names[id], 0); return r; }

static long g_nameIds[4];
static int g_destroys = 0, g_getDefs = 0, g_nameCalls = 0;
static CCreatureGenerationFamilyDef* g_defQueue[4];
void CCreatureGenerationFamilyDef::v00() {}
void CCreatureGenerationFamilyDef::Destroy() { ++g_destroys; }
void CCreatureGenerationFamilyDef::v02() {} void CCreatureGenerationFamilyDef::v03() {} void CCreatureGenerationFamilyDef::v04() {}
void CCreatureGenerationFamilyDef::v05() {} void CCreatureGenerationFamilyDef::v06() {} void CCreatureGenerationFamilyDef::v07() {}
void CCreatureGenerationFamilyDef::v08() {} void CCreatureGenerationFamilyDef::v09() {}
static CCreatureGenerationFamilyDef g_defs[2];
CDefString CCreatureGenerationFamilyDef::GetDefName() const { ++g_nameCalls; return *(const CDefString*)&g_nameIds[this - g_defs]; }
void* FindDef_CDefinitionManager_GetDef_0() { return g_defQueue[g_getDefs++]; }
static CDefinitionManager g_manager;
CDefinitionManager* CTCCreatureGenerator::GetDefinitionManager() const { return &g_manager; }
void CTCCreatureGenerator::v00() {}

static long g_types0[8], g_types1[8];
static CDefIndex g_families[2];
static C3DVector g_positions[1];
static void SetTypes(int d, const long* t, int n) { long* a = d ? g_types1 : g_types0; for (int i = 0; i < n; ++i) a[i] = t[i];
    g_defs[d].GenerationTypes._M_start = a; g_defs[d].GenerationTypes._M_finish = a + n; g_defs[d].GenerationTypes._M_end_of_storage = a + n; }
static void Positions(CTCCreatureGenerator& g, bool open, bool hidden)
{
    g.OpenPositions._M_start = g_positions; g.OpenPositions._M_finish = g_positions + (open ? 1 : 0);
    g.HiddenPositions._M_start = g_positions; g.HiddenPositions._M_finish = g_positions + (hidden ? 1 : 0);
}
static bool Run(CTCCreatureGenerator& g, int families, CCharString& error)
{
    g_getDefs = g_destroys = g_nameCalls = 0; g_defs[0].RefCount = 2; g_defs[1].RefCount = 2;
    g_defQueue[0] = &g_defs[0]; g_defQueue[1] = &g_defs[1];
    g.CreatureFamilies._M_start = g_families; g.CreatureFamilies._M_finish = g_families + families; g.CreatureFamilies._M_end_of_storage = g_families + 2;
    error.PStringData = (void*)"untouched";
    return g.Validate(error);
}
int main()
{
    static CTCCreatureGenerator gen; CCharString error;
    g_families[0].Index = 10; g_families[1].Index = 11; g_nameIds[0] = 0; g_nameIds[1] = 2;
    long open0[] = { 0 }, open1[] = { 1 }, hidden[] = { 2 }, anim[] = { 5, 3 }, longHidden[] = { 7, 7, 7, 7, 7, 2 }, other[] = { 4, 5, 6, 7, 8 };

    // no families
    Positions(gen, true, true);
    CHECK(!Run(gen, 0, error) && strcmp(Text(error), "no creature families specified - what am I supposed to generate?") == 0 && g_getDefs == 0);
    // one valid family: the error text still names it, the def reference is dropped
    SetTypes(0, open0, 1);
    CHECK(Run(gen, 1, error) && strcmp(Text(error), "creature family Bandits") == 0 && g_getDefs == 1 && g_defs[0].RefCount == 1 && g_destroys == 0);
    // last reference -> Destroy
    g_defs[0].RefCount = 1; g_getDefs = 0; g_defQueue[0] = &g_defs[0]; error.PStringData = 0;
    CHECK(gen.Validate(error) && g_defs[0].RefCount == 0 && g_destroys == 1);
    // hidden generation without hidden positions
    SetTypes(0, hidden, 1); Positions(gen, true, false);
    CHECK(!Run(gen, 1, error) && strcmp(Text(error), "creature family Bandits needs to have hidden start positions") == 0 && g_defs[0].RefCount == 1);
    Positions(gen, false, true);
    CHECK(Run(gen, 1, error));
    // the unrolled find reaches the sixth element
    SetTypes(0, longHidden, 6); Positions(gen, true, false);
    CHECK(!Run(gen, 1, error) && strcmp(Text(error), "creature family Bandits needs to have hidden start positions") == 0);
    SetTypes(0, other, 5);
    CHECK(Run(gen, 1, error));
    // generator animation is never supported
    SetTypes(0, anim, 2); Positions(gen, true, true);
    CHECK(!Run(gen, 1, error) && strcmp(Text(error), "creature family Bandits has generate type GENERATE_GENERATOR_ANIMATION and it's not supported") == 0 && g_defs[0].RefCount == 1);
    // open generation (type 0 or 1) without open positions
    SetTypes(0, open0, 1); Positions(gen, false, true);
    CHECK(!Run(gen, 1, error) && strcmp(Text(error), "creature family Bandits needs to create creatures in the open and I can't find any navigable positions") == 0);
    SetTypes(0, open1, 1);
    CHECK(!Run(gen, 1, error) && strcmp(Text(error), "creature family Bandits needs to create creatures in the open and I can't find any navigable positions") == 0);
    // two families: the first passes, the second fails and is named
    SetTypes(0, open0, 1); SetTypes(1, hidden, 1); Positions(gen, true, false);
    CHECK(!Run(gen, 2, error) && strcmp(Text(error), "creature family Hobbes needs to have hidden start positions") == 0 && g_getDefs == 2 && g_nameCalls == 2);
    CHECK(g_defs[0].RefCount == 1 && g_defs[1].RefCount == 1 && g_destroys == 0);
    SetTypes(1, open1, 1);
    CHECK(Run(gen, 2, error) && strcmp(Text(error), "creature family Hobbes") == 0);
    if (fails == 0) printf("00749f00_TEST PASS\n");
    return fails;
}
