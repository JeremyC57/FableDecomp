#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __cdecl
#endif
static int g_extCopies = 0;   // out-of-line copy constructors the retail body calls
static int g_links = 0;       // CBaseIntelligentPointer::SetPItem calls
namespace N004ebdf1 {
struct CParentDefClassBase {
    virtual void v00() { }
    unsigned char _body[0x38];                   // +0x04 def-class base body
    CParentDefClassBase() { std::memset(_body, 0, sizeof(_body)); }
    CParentDefClassBase(const CParentDefClassBase& other) { std::memcpy(_body, other._body, sizeof(_body)); ++g_extCopies; }   // polymorphic: copy the body only
};
struct C2DVector { float X, Y; };
struct C2DVectorVector {                         // STLport vector<C2DVector>, 12 bytes
    C2DVector* _M_start;
    C2DVector* _M_finish;
    C2DVector* _M_end_of_storage;
    C2DVectorVector() { std::memset(this, 0, sizeof(C2DVectorVector)); }
    C2DVectorVector(const C2DVectorVector& other) { std::memcpy(this, &other, sizeof(C2DVectorVector)); ++g_extCopies; }
};
struct CCardPositionsDef : CParentDefClassBase { // vtable 0x012439DC
    C2DVectorVector Offset;                      // +0x3C
    virtual void v00() { }
};
void __fastcall _Construct(CCardPositionsDef* p, const CCardPositionsDef& value)
{
    if (p != 0)
        new (p) CCardPositionsDef(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N004ebdf1;
    CCardPositionsDef src;
    src._body[3] = 9;
    src.Offset._M_start = reinterpret_cast<C2DVector*>(0x30);
    src.Offset._M_end_of_storage = reinterpret_cast<C2DVector*>(0x48);
    CCardPositionsDef* slot = reinterpret_cast<CCardPositionsDef*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 2 && g_links == 0;
    if (!(slot->_body[3] == src._body[3])) ok = false;
    if (!(slot->Offset._M_start == src.Offset._M_start)) ok = false;
    if (!(slot->Offset._M_end_of_storage == src.Offset._M_end_of_storage)) ok = false;
    slot->v00();                                        // the copy installed a working vtable
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 2 || g_links != 0) ok = false;
    if (!ok) { std::printf("004ebdf1_TEST FAIL\n"); return 1; }
    std::printf("004ebdf1_TEST PASS\n");
    return 0;
}