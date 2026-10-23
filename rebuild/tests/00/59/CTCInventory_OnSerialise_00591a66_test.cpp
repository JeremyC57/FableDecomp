// Behaviour test for 00591a66: final.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CTCInventory::OnSerialise(CPersistContext*, bool)
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventory::OnSerialise(CPersistContext* context, bool inGameSave) -- serialises the base inventory, then the quick-access slots as def NAMES: saving converts each set slot's def index to its name, loading maps the names back to indices
// Leaf: ?OnSerialise@CTCInventory@
// Retail 0x00591A66, 281 bytes, `ret 8`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: CTCInventoryBase::OnSerialise(CPersistContext*, bool) 0x005BFD5E, CPersistContext::IsSaving 0x004AEF50 (manifest
// "IsBinary"), vector<CCharString>::reserve(unsigned long) 0x004BA7A0, CTCBase::GetDefinitionManager 0x006869D0,
// CDefinitionManager::GetDefNameFromGlobalIndex(long) 0x009ACCC0 (CDefString by hidden pointer), the CDefString ->
// CCharString conversion 0x00415DD0 (a twin of the landed operator CCharString() 0x00415D70), vector<CCharString>::
// push_back 0x0044BFF0, ~CCharString 0x0099EAE0, CPersistContext::Transfer(const char*, vector<CCharString>*) 0x0049B8D0,
// CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) 0x009AD410, ~vector<CCharString> 0x00414EA0.
// Shape: `if (context->IsSaving()) { vector names; ... } else { vector names; ... }` -- ONE vector local per arm, with the
// STLport ctor zeroing start, finish, end_of_storage in init-list order. VC7.1 hoists the two identical ctor store runs
// above the `je` (in reverse order: end, finish, start) and tests IsSaving with `test al,al` because the compare is lowered
// before the hoisted zero register exists; a single vector declared before the test gives `cmp al,bl`. Saving reserves the slot count, pushes the name of every positive slot (the
// converted CCharString temporary homed in the dead `inGameSave` slot, the CDefString result in a frame word) and
// transfers; loading transfers first and writes each looked-up index into the slot (the slot address is formed before
// the lookup call).
// Layout (retail): QuickAccessItems vector<long> +0x148.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharString {
    void* m_pStringData;
    CCharString(const CCharString& other);
    ~CCharString();                                    // 0x0099EAE0
};
namespace N_00415dd0 { struct CDefString {
    void* m_pData;
    CDefString(const CDefString& other);
    operator CCharString() const;                      // 0x00415DD0
}; } using N_00415dd0::CDefString;
struct SAllocator { SAllocator() {} };
template <class T> struct _STLP_alloc_proxy {          // the end-of-storage pointer over an empty allocator
    T _M_data;
    _STLP_alloc_proxy(const SAllocator&, T p) : _M_data(p) {}
};
struct CCharStringVector {                             // vector<CCharString> (12 bytes)
    CCharString* _M_start;
    CCharString* _M_finish;
    _STLP_alloc_proxy<CCharString*> _M_end_of_storage;
    CCharStringVector(const SAllocator& a = SAllocator()) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
    ~CCharStringVector();                              // 0x00414EA0
    CCharString* begin() { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CCharString& operator[](unsigned long n) { return *(begin() + n); }
    void reserve(unsigned long count);                 // 0x004BA7A0
    void push_back(const CCharString& value);          // 0x0044BFF0
};
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    long& operator[](unsigned long n) { return *(begin() + n); }
};
struct CPersistContext {
    bool IsSaving();                                   // 0x004AEF50
    void Transfer(const char* name, CCharStringVector* value);   // 0x0049B8D0
};
struct CDefinitionManager {
    CDefString GetDefNameFromGlobalIndex(long index);  // 0x009ACCC0
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CTCInventoryBase_Retail {
    void OnSerialise(CPersistContext* context, bool inGameSave);   // 0x005BFD5E
    CDefinitionManager* GetDefinitionManager() const;  // 0x006869D0 (CTCBase)
};
struct CTCInventory : CTCInventoryBase_Retail {
    unsigned char _pad_0x000[0x148];
    CLongVector QuickAccessItems;                      // +0x148
    void OnSerialise(CPersistContext* context, bool inGameSave);
};

void CTCInventory::OnSerialise(CPersistContext* context, bool inGameSave)
{
    CTCInventoryBase_Retail::OnSerialise(context, inGameSave);
    if (context->IsSaving()) {
        CCharStringVector names;
        names.reserve(QuickAccessItems.size());
        for (unsigned long i = 0; i < QuickAccessItems.size(); ++i)
            if (QuickAccessItems[i] > 0)
                names.push_back(GetDefinitionManager()->GetDefNameFromGlobalIndex(QuickAccessItems[i]));
        context->Transfer("QuickAccessItemNames", &names);
    } else {
        CCharStringVector names;
        context->Transfer("QuickAccessItemNames", &names);
        for (unsigned long i = 0; i < names.size(); ++i)
            QuickAccessItems[i] = GetDefinitionManager()->GetDefGlobalIndexFromName(names[i]);
    }
}

// ---- stubs ----
static char g_log[4096];
static void logf(const char* fmt, long a = 0, long b = 0) { char t[256]; _snprintf(t, sizeof t, fmt, a, b); strcat(g_log, t); strcat(g_log, ";"); }
static bool g_saving;
static CDefString MakeDef(long v) { void* p = (void*)(size_t)v; return *(CDefString*)&p; }   // CDefString is one pointer
static long g_strings = 0;      // live CCharString blocks
CCharString::CCharString(const CCharString& other) { m_pStringData = other.m_pStringData; ++g_strings; }
CCharString::~CCharString() { --g_strings; }
CDefString::CDefString(const CDefString& other) { m_pData = other.m_pData; }
CDefString::operator CCharString() const { CCharString s(*(const CCharString*)this); logf("conv(%ld)", (long)(size_t)m_pData); return s; }
CCharStringVector::~CCharStringVector() { logf("~vector(%ld)", (long)size()); for (unsigned long i = 0; i < size(); ++i) _M_start[i].~CCharString(); free(_M_start); }
void CCharStringVector::reserve(unsigned long count) { logf("reserve(%ld)", (long)count); _M_start = _M_finish = (CCharString*)malloc(sizeof(CCharString) * (count + 8)); _M_end_of_storage._M_data = _M_start + count + 8; }
void CCharStringVector::push_back(const CCharString& value) { logf("push_back(%ld)", (long)(size_t)value.m_pStringData); _M_finish->m_pStringData = value.m_pStringData; ++g_strings; ++_M_finish; }
bool CPersistContext::IsSaving() { logf("IsSaving"); return g_saving; }
void CPersistContext::Transfer(const char* name, CCharStringVector* value)
{
    logf("Transfer(%ld)", (long)value->size());
    CHECK(strcmp(name, "QuickAccessItemNames") == 0);
    if (!g_saving) {            // loading: fill the vector with two names
        value->reserve(2);
        CCharString a = MakeDef(11); CCharString b = MakeDef(22);
        value->push_back(a); value->push_back(b);
    }
}
CDefString CDefinitionManager::GetDefNameFromGlobalIndex(long index) { logf("GetDefName(%ld)", index); return MakeDef(index * 10); }
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) { logf("GetDefIndex(%ld)", (long)(size_t)name.m_pStringData); return (long)(size_t)name.m_pStringData / 11 + 100; }
static CDefinitionManager g_manager;
void CTCInventoryBase_Retail::OnSerialise(CPersistContext* context, bool inGameSave) { logf("Base::OnSerialise(%ld,%ld)", (long)(size_t)context, (long)inGameSave); }
CDefinitionManager* CTCInventoryBase_Retail::GetDefinitionManager() const { logf("GetDefinitionManager"); return &g_manager; }

int main()
{
    CTCInventory* inv = (CTCInventory*)calloc(1, sizeof(CTCInventory));
    CPersistContext ctx;
    long slots[4] = { 3, 0, 7, -1 };
    inv->QuickAccessItems._M_start = slots; inv->QuickAccessItems._M_finish = slots + 4; inv->QuickAccessItems._M_end_of_storage = slots + 4;

    // saving: positive slots are converted to names and pushed, zero/negative ones skipped
    g_saving = true; g_log[0] = 0;
    inv->OnSerialise(&ctx, true);
    CHECK(strstr(g_log, "Base::OnSerialise(") == g_log);
    CHECK(strstr(g_log, "IsSaving;reserve(4);GetDefinitionManager;GetDefName(3);conv(30);push_back(30);GetDefinitionManager;GetDefName(7);conv(70);push_back(70);Transfer(2);~vector(2);") != 0);
    CHECK(strstr(g_log, "GetDefIndex") == 0);
    CHECK(g_strings == 0);
    CHECK(slots[0] == 3 && slots[2] == 7);

    // loading: Transfer fills two names, each is mapped back into the slot
    g_saving = false; g_log[0] = 0;
    inv->OnSerialise(&ctx, false);
    CHECK(strstr(g_log, "Base::OnSerialise(") == g_log);
    CHECK(strstr(g_log, "IsSaving;Transfer(0);reserve(2);conv(11);conv(22);push_back(11);push_back(22);GetDefinitionManager;GetDefIndex(11);GetDefinitionManager;GetDefIndex(22);~vector(2);") != 0);
    CHECK(strstr(g_log, "GetDefName") == 0);
    CHECK(slots[0] == 101 && slots[1] == 102 && slots[2] == 7);
    CHECK(g_strings == 0);

    if (fails == 0) printf("00591a66_TEST PASS\n"); else printf("00591a66_TEST FAIL\n");
    return fails != 0;
}