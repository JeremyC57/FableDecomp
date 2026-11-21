// Behaviour test for 0x004333B3 vector<CCharString>::_M_fill_insert (STLport 4.6, the shared headers): in-place paths, x_copy
// lifetime and the overflow hand-off.
// Test (NF212, 2026-11-21): the row on the shared headers (fable_string.h, stlport/fable_containers.h, as the row source); the
//   harness defines the CCharString members the header declares (live copies counted), the declaration-only
//   uninitialized_fill_n (0x00433504) and _M_insert_overflow (0x00433530). Prints the catalog PassPattern.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_STLP_FALSE_TAG_POD
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_DECL_ONLY
#include "fable_string.h"
#include "stlport/fable_containers.h"
namespace std {
template<> CCharString* uninitialized_fill_n<CCharString*, unsigned int, CCharString>(CCharString*, unsigned int, const CCharString&);
template<> void vector<CCharString>::_M_insert_overflow(CCharString*, const CCharString&, const __false_type&, unsigned int, bool);
}
template void std::vector<CCharString>::_M_fill_insert(CCharString*, unsigned int, const CCharString&);

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static long gLive;
static int gOverflows;
static unsigned long IdOf(const CCharString& s) { void* p; memcpy(&p, &s, sizeof p); return (unsigned long)(size_t)p; }
static void SetId(CCharString* s, unsigned long id) { void* p = (void*)(size_t)id; memcpy(s, &p, sizeof p); }
CCharString::CCharString() { SetId(this, 0); }
CCharString::CCharString(const char* text, long) { SetId(this, (unsigned long)atoi(text)); ++gLive; }
CCharString::CCharString(const CCharString& other) { SetId(this, IdOf(other)); if (IdOf(other)) ++gLive; }
CCharString::~CCharString() { if (IdOf(*this)) --gLive; SetId(this, 0); }
const CCharString& CCharString::operator=(const CCharString& other)
{
    if (IdOf(*this)) --gLive;
    SetId(this, IdOf(other));
    if (IdOf(other)) ++gLive;
    return *this;
}
namespace std {
template<> CCharString* uninitialized_fill_n<CCharString*, unsigned int, CCharString>(CCharString* first, unsigned int n, const CCharString& x)
{
    for (; n > 0; --n, ++first) new (first) CCharString(x);
    return first;
}
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

// Test helper (not a model): the derived class reaches the protected _Vector_base words; it adds no members.
struct CTestVector : std::vector<CCharString> {
    CCharString*& Start() { return this->_M_start; }
    CCharString*& Finish() { return this->_M_finish; }
    CCharString*& EndOfStorage() { return this->_M_end_of_storage._M_data; }
};

static void Init(CTestVector& v, int size, int cap)
{
    CCharString* p = cap ? (CCharString*)malloc(cap * sizeof(CCharString)) : 0;
    char text[16];
    for (int i = 0; i < size; ++i) { sprintf(text, "%d", i * 10 + 1); new (p + i) CCharString(text); }
    v.Start() = p; v.Finish() = p + size; v.EndOfStorage() = p + cap;
}
static void Kill(CTestVector& v)
{
    for (CCharString* p = v.Start(); p != v.Finish(); ++p) p->~CCharString();
    free(v.Start());
    v.Start() = v.Finish() = v.EndOfStorage() = 0;
}
static int Check(CTestVector& v, int size, int pos, int n, unsigned long fillv)
{
    if ((int)(v.Finish() - v.Start()) != size + n) return 0;
    for (int i = 0; i < size + n; ++i) {
        unsigned long expect = i < pos ? (unsigned long)(i * 10 + 1) : (i < pos + n ? fillv : (unsigned long)((i - n) * 10 + 1));
        if (IdOf(v.Start()[i]) != expect) return 0;
    }
    return 1;
}
static int Run(int size, int cap, int pos, int n)
{
    int ok;
    {
        CTestVector v;
        Init(v, size, cap);
        CCharString x("777");
        v._M_fill_insert(v.Start() + pos, n, x);
        ok = Check(v, size, pos, n, 777);
        Kill(v);
    }
    return ok;
}

int main()
{
    int ok = 1;
    ok &= Run(5, 10, 1, 2);
    ok &= Run(5, 10, 3, 4);
    ok &= Run(5, 10, 5, 3);
    ok &= Run(5, 10, 2, 0);
    int before = gOverflows;
    ok &= Run(5, 6, 1, 3);
    if (gOverflows != before + 1) ok = 0;
    if (gLive != 0) ok = 0;
    if (!ok) { printf("004333b3_TEST FAIL\n"); return 1; }
    printf("004333b3_TEST PASS\n");
    return 0;
}
