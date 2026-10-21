// Behaviour test for 005ee3b3: src/005ee3b3.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCInventoryAbilities::GetMiscThingsDef()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryAbilities::GetMiscThingsDef() -- look up the "UI_MISC_THINGS_DEF" definition's index and return a counted pointer to that def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE3B3, length 101 bytes, `ret 4` (hidden result pointer; `this` in ecx
//   unused).
// Identity: called with ecx = the component and a frame result slot from CTCInventoryAbilities code (0x005EEEB5,
//   0x005F1B69, ... ; 0x005EE418 is CTCInventoryAbilities::FrameUpdate), so it is a member of that TU's class; the
//   catalog label "CCreatureAction_CastStrengthMultiStrikeProper" is propagated. Ego_r.pdb names it
//   `?GetMiscThingsDef@CTCInventoryAbilities@@ABE?BV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@XZ`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CGameDefinitionManager::Get @ 0x0044C6B0 (returns
//          the global manager), CDefinitionManager::GetDefGlobalIndex(const CCharString&, bool) @ 0x009AD390 (manifest
//          label "ActualToUniqueIndex" is propagated), CCharString::~CCharString @ 0x0099EAE0,
//          CDefinitionManager::GetDef<T>(long, CDefPointer<T>*) @ 0x0042B312.
// Shape: a size-optimised frame-pointer TU (`optimize("y",off)` + `("s",on)`: `push ebp ; mov ebp,esp ... leave`,
//   `and dword ptr [ebp-4],0`); the name string lives in its own block and its frame word is reused by the def
//   pointer local (zeroed by its inline constructor); `return def;` copy-constructs the hidden result (add-ref) and the
//   local's destructor releases -- one shared null test with both `inc`/`dec [ecx+4]` kept, slot 1 on zero.
// Leaf: GetMiscThingsDef
// Fix (retry 10): the def local's release stays register-held once GetDef<T> has its same-TU operator=-shaped body
//   (defreal), and CGameDefinitionManager::Get is the `__fastcall` static returning a reference (a plain static Get()
//   hoisted `lea eax,[ebp-4]` above `push 1`).
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CDefBase {                           // counted game definition: vtable + reference count
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long m_refCount;                        // +4
};

template <class T>
struct CDefPointer {
    T* m_pDef;
    __forceinline CDefPointer() : m_pDef(0) { }
    __forceinline CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) ++m_pDef->m_refCount; }
    __forceinline ~CDefPointer() { if (m_pDef) { if (--m_pDef->m_refCount == 0) m_pDef->Destroy(); } }
};

struct CUIMiscThingsDef : CDefBase { };

struct CDefinitionManager {
    long GetDefGlobalIndex(const CCharString& name, bool mustExist);          // 0x009AD390
    template <class T> bool GetDef(long index, CDefPointer<T>* result);        // 0x0042B312 (one instantiation)
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long p0, CDefPointer<T>* out)
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)

struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};

struct CTCInventoryAbilities {
    CDefPointer<CUIMiscThingsDef> GetMiscThingsDef() const;
};

CDefPointer<CUIMiscThingsDef> CTCInventoryAbilities::GetMiscThingsDef() const
{
    long index;
    {
        CCharString name("UI_MISC_THINGS_DEF", -1);
        index = CGameDefinitionManager::Get().GetDefGlobalIndex(name, true);
    }
    {
        CDefPointer<CUIMiscThingsDef> def;      // sibling block: shares the string's frame word
        CGameDefinitionManager::Get().GetDef(index, &def);
        return def;
    }
}

// ---- stubs and driver ----
void CDefBase::v00() {}
static const char* g_text = 0; static int g_strDtor = 0;
CCharString::CCharString(const char* text, long length) { g_text = text; m_pData = (void*)text; }
CCharString::~CCharString() { ++g_strDtor; }
static CGameDefinitionManager g_gdm;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_gdm; }
static const char* g_asked = 0; static bool g_must = false; static long g_index = 0; static long g_defIndex = 0;
long CDefinitionManager::GetDefGlobalIndex(const CCharString& name, bool mustExist) { g_asked = (const char*)name.m_pData; g_must = mustExist; return g_index; }
static CUIMiscThingsDef g_def; static void* g_found = 0;
void* FindDef_s7r_0() { return g_found; }
static int g_released = 0; void ReleaseDef_s7(void* def) { ++g_released; }
static int g_destroyed = 0;
void CDefBase::Destroy() { ++g_destroyed; }
int main()
{
    CTCInventoryAbilities inv;
    g_index = 9; g_found = &g_def; g_def.m_refCount = 1;
    {
        CDefPointer<CUIMiscThingsDef> r = inv.GetMiscThingsDef();
        CHECK(strcmp(g_text, "UI_MISC_THINGS_DEF") == 0); CHECK(g_asked == g_text); CHECK(g_must == true); CHECK(g_strDtor == 1);
        CHECK(r.m_pDef == &g_def); CHECK(g_def.m_refCount == 1); CHECK(g_destroyed == 0);   // +1 for the result, -1 for the local
        r.m_pDef = 0;
    }
    g_found = 0;
    {
        CDefPointer<CUIMiscThingsDef> r = inv.GetMiscThingsDef();
        CHECK(r.m_pDef == 0); CHECK(g_strDtor == 2); CHECK(g_destroyed == 0);
    }
    g_found = &g_def; g_def.m_refCount = 0;          // a def nobody else holds: the result keeps it alive
    {
        CDefPointer<CUIMiscThingsDef> r = inv.GetMiscThingsDef();
        CHECK(r.m_pDef == &g_def); CHECK(g_def.m_refCount == 0); CHECK(g_destroyed == 1);   // the local's release hit zero
        r.m_pDef = 0;
    }
    if (fails == 0) printf("005ee3b3_TEST PASS\n"); else printf("005ee3b3_TEST FAIL\n");
    return fails;
}