// ForEach_AddDummyObjectName_00a72f40 (0x00A72F40)
// Leaf: ?ForEach_AddDummyObjectName_00a72f40@@YI?AUCAddName@@PBUSDummyObject@@0U1@@Z
// Test (NF208, 2026-11-20): the row on the shared headers (fable_string.h, stlport/fable_containers.h); the harness defines the
//   CCharString members the header declares (live copies counted) and the declaration-only _M_insert_overflow, and checks the
//   functor result and the collected names through both push_back paths.
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#include "fable_string.h"
#include "stlport/fable_containers.h"
// The out-of-line growth path (0x00433530, declaration-only in the row) supplied by the harness: double, copy, append, adopt.
namespace std { template<> void vector<CCharString>::_M_insert_overflow(CCharString*, const CCharString&, const __false_type&, unsigned int, bool); }

struct SDummyObject {                           // pair<CCharString, C3DDummyObject>, 56 bytes
    CCharString   m_name;                       // +0x00
    unsigned char m_object[0x34];               // C3DDummyObject
};

struct CAddName {                               // CPhysicsMesh's name-collecting functor (landed callers' model)
    std::vector<CCharString>* m_pNames;
    CAddName(std::vector<CCharString>& names) : m_pNames(&names) { }
    void operator()(const SDummyObject& object) { m_pNames->push_back(object.m_name); }
};

CAddName FABLE_FASTCALL ForEach_AddDummyObjectName_00a72f40(const SDummyObject* first, const SDummyObject* last, CAddName f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static long gLive;
static int gOverflows;
static unsigned long IdOf(const CCharString& s) { void* p; memcpy(&p, &s, sizeof p); return (unsigned long)(size_t)p; }
static void SetId(CCharString* s, unsigned long id) { void* p = (void*)(size_t)id; memcpy(s, &p, sizeof p); }
CCharString::CCharString() { SetId(this, 0); }
CCharString::CCharString(const char* text, long) { SetId(this, (unsigned char)text[0]); ++gLive; }
CCharString::CCharString(const CCharString& other) { SetId(this, IdOf(other)); if (IdOf(other)) ++gLive; }
CCharString::~CCharString() { if (IdOf(*this)) --gLive; SetId(this, 0); }
namespace std {
template<> void vector<CCharString>::_M_insert_overflow(CCharString* position, const CCharString& x, const __false_type&, unsigned int fill_len, bool at_end)
{
    ++gOverflows;
    unsigned int size = (unsigned int)(this->_M_finish - this->_M_start);
    unsigned int len = size + (size > fill_len ? size : fill_len);
    CCharString* fresh = (CCharString*)malloc(len * sizeof(CCharString));
    CCharString* out = fresh;
    for (CCharString* p = this->_M_start; p != position; ++p, ++out) new (out) CCharString(*p);
    for (unsigned int k = 0; k < fill_len; ++k, ++out) new (out) CCharString(x);
    if (!at_end) for (CCharString* p = position; p != this->_M_finish; ++p, ++out) new (out) CCharString(*p);
    for (CCharString* p = this->_M_start; p != this->_M_finish; ++p) p->~CCharString();
    free(this->_M_start);
    this->_M_start = fresh; this->_M_finish = out; this->_M_end_of_storage._M_data = fresh + len;
}
}

static int Run()
{
    static const char* names[5] = { "r", "a", "b", "h", "t" };
    SDummyObject* records = (SDummyObject*)malloc(5 * sizeof(SDummyObject));
    memset(records, 0, 5 * sizeof(SDummyObject));
    for (int i = 0; i < 5; ++i) new (&records[i].m_name) CCharString(names[i]);
    int r = 0;
    {
        std::vector<CCharString> v;
        CAddName f = ForEach_AddDummyObjectName_00a72f40(records, records + 5, CAddName(v));
        if (f.m_pNames != &v || v.size() != 5) r = 1;
        else for (int i = 0; i < 5; ++i) if (IdOf(v[i]) != (unsigned char)names[i][0]) r = 2;
        if (!r && gOverflows != 4) r = 3;                 // capacity 0 -> 1 -> 2 -> 4 -> 8
    }
    for (int i = 0; i < 5; ++i) records[i].m_name.~CCharString();
    free(records);
    return r;
}

int main()
{
    int r = Run();
    if (r) { printf("00a72f40_TEST FAIL %d\n", r); return r; }
    if (gLive != 0) { printf("00a72f40_TEST FAIL leak %ld\n", gLive); return 9; }
    printf("00a72f40_TEST PASS\n");
    return 0;
}
