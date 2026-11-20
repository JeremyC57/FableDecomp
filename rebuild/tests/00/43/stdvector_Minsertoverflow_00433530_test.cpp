// 00433530 vector<CCharString>::_M_insert_overflow (catalog: std::vector::Reallocate)
// Flags: /O2 /Oy /W3
// Leaf: ?_M_insert_overflow@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@IAEXPAVCCharString@@ABV3@ABU__false_type@2@I_N@Z
// Test (NF204, 2026-11-20): the row on the shared headers (fable_string.h + stlport/fable_containers.h); the harness defines the
//   CCharString constructors / destructor the header declares, with a reference count per string id held by the harness, and reaches
//   the protected member through a derived vector.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
template void std::vector<CCharString>::_M_insert_overflow(CCharString*, const CCharString&, const std::__false_type&, unsigned int, bool);

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static long gRefs[256];
static long gLive;
static unsigned long IdOf(const CCharString& s) { void* p; memcpy(&p, &s, sizeof p); return (unsigned long)(size_t)p; }
static void SetId(CCharString* s, unsigned long id) { void* p = (void*)(size_t)id; memcpy(s, &p, sizeof p); }
CCharString::CCharString() { SetId(this, 0); }
CCharString::CCharString(const char* text, long) { unsigned long id = (unsigned char)text[0]; SetId(this, id); if (id) { ++gRefs[id]; ++gLive; } }
CCharString::CCharString(const CCharString& other) { unsigned long id = IdOf(other); SetId(this, id); if (id) { ++gRefs[id]; ++gLive; } }
CCharString::~CCharString() { unsigned long id = IdOf(*this); if (id) { --gRefs[id]; --gLive; } SetId(this, 0); }

struct CTestVector : std::vector<CCharString> {
    void Overflow(CCharString* pos, const CCharString& x, unsigned int n, bool atEnd) { _M_insert_overflow(pos, x, std::__false_type(), n, atEnd); }
    CCharString* Start() const { return this->_M_start; }
    CCharString* Finish() const { return this->_M_finish; }
    CCharString* EndOfStorage() const { return this->_M_end_of_storage._M_data; }
};

static int Run()
{
    CTestVector v;
    CCharString a("a"), b("b"), c("c"), d("d"), x("x");
    v.push_back(a); v.push_back(b); v.push_back(c); v.push_back(d);
    if (v.Finish() - v.Start() != 4) return 1;
    unsigned long before = (unsigned long)(v.EndOfStorage() - v.Start());
    v.Overflow(v.Start() + 2, x, 2, false);
    const char want[] = { 'a', 'b', 'x', 'x', 'c', 'd' };
    if (v.Finish() - v.Start() != 6) return 2;
    if ((unsigned long)(v.EndOfStorage() - v.Start()) != before + (before > 2 ? before : 2)) return 3;
    for (int i = 0; i < 6; ++i) if (IdOf(v.Start()[i]) != (unsigned char)want[i]) return 4;
    if (gRefs['x'] != 3 || gRefs['a'] != 2 || gRefs['d'] != 2) return 5;
    v.Overflow(v.Finish(), x, 1, true);
    if (v.Finish() - v.Start() != 7 || IdOf(v.Start()[6]) != 'x' || gRefs['x'] != 4) return 6;
    return 0;
}

int main()
{
    int r = Run();
    if (r) { printf("00433530_TEST FAIL %d\n", r); return r; }
    if (gLive != 0) { printf("00433530_TEST FAIL leak %ld\n", gLive); return 9; }
    printf("00433530_TEST PASS\n");
    return 0;
}
