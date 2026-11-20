// std::vector<CCharString>::push_back (0x0044BFF0)
// Leaf: ?push_back@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAEXABVCCharString@@@Z
// Test (NF208, 2026-11-20): the row on the shared headers (fable_string.h, stlport/fable_containers.h); the harness defines the
//   CCharString members the header declares (live copies counted) and the declaration-only _M_insert_overflow (0x00433530), and
//   checks both push_back paths. Prints the catalog PassPattern.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#include "fable_string.h"
#include "stlport/fable_containers.h"
namespace std { template<> void vector<CCharString>::_M_insert_overflow(CCharString*, const CCharString&, const __false_type&, unsigned int, bool); }
template void std::vector<CCharString>::push_back(const CCharString&);

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static long gLive;
static int gOverflows, gBadArgs;
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
    if (position != this->_M_finish || fill_len != 1 || !at_end) gBadArgs = 1;
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
    CCharString a("a"), b("b"), c("c");
    std::vector<CCharString> v;
    v.push_back(a);                      // overflow: capacity 1
    v.push_back(b);                      // overflow: capacity 2
    v.push_back(c);                      // overflow: capacity 4
    v.push_back(a);                      // in place
    if (gBadArgs || gOverflows != 3 || v.size() != 4) return 1;
    if (IdOf(v[0]) != 'a' || IdOf(v[1]) != 'b' || IdOf(v[2]) != 'c' || IdOf(v[3]) != 'a') return 2;
    if (gLive != 7) return 3;
    return 0;
}

int main()
{
    int r = Run();
    if (r || gLive != 0) { printf("STD_VECTOR_PUSH_BACK_FAIL %d %ld\n", r, gLive); return 1; }
    puts("STD_VECTOR_PUSH_BACK_PASS");
    return 0;
}
