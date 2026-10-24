// Behaviour test for 0x00459AF3 CPlayerInventoryDef::CPlayerInventoryDef (source body copied; base and range-ctor stubs).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
namespace std {
template <class T> struct allocator {
    allocator() { }
};

template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector(unsigned int n, const T& value, const A& a = A());                       // 0x00451583 (vector<bool>)
    template <class InputIter> vector(InputIter first, InputIter last, const A& a = A());   // 0x00453659 (vector<long>, int)
};

template <> __declspec(noinline) vector<bool, allocator<bool> >::vector(unsigned int n, const bool& value, const allocator<bool>&)
{
    _M_start = (bool*)malloc(n);
    _M_end_of_storage = _M_start + n;
    for (unsigned int i = 0; i < n; ++i)
        _M_start[i] = value;
    _M_finish = _M_end_of_storage;
}
}

static int g_base = 0, g_range = 0;
namespace std {
template <> template <> vector<long, allocator<long> >::vector(int first, int last, const allocator<long>&)
{
    ++g_range;
    _M_start = (long*)malloc(first * sizeof(long));
    for (int i = 0; i < first; ++i) _M_start[i] = last;
    _M_finish = _M_end_of_storage = _M_start + first;
}
}

class CParentDefClassBase {                        // 0x3C bytes in retail
public:
    virtual void v000();
    unsigned long _body[0x0E];
    CParentDefClassBase();                         // 0x00430370
};

class CPlayerInventoryDef : public CParentDefClassBase {   // vtable 0x01235274
public:
    virtual void v000();
    std::vector<long> Slot;                        // +0x3C
    std::vector<long> MaxInSlot;                   // +0x48
    std::vector<long> InitialAmount;               // +0x54
    std::vector<bool> IsUsable;                    // +0x60
    long              InitialSelectedSlot;         // +0x6C
    CPlayerInventoryDef();
};

CPlayerInventoryDef::CPlayerInventoryDef()
    : Slot(6, 0), MaxInSlot(6, 0), InitialAmount(6, 0), IsUsable(6, false), InitialSelectedSlot(0)
{
}

CParentDefClassBase::CParentDefClassBase() { ++g_base; for (int i = 0; i < 0x0E; ++i) _body[i] = 0; }
void CParentDefClassBase::v000() { }
void CPlayerInventoryDef::v000() { }
int main()
{
    CPlayerInventoryDef* d = new CPlayerInventoryDef;
    if (g_base != 1 || g_range != 3) { printf("00459af3_TEST FAIL 1\n"); return 1; }
    std::vector<long>* v[3] = { &d->Slot, &d->MaxInSlot, &d->InitialAmount };
    for (int k = 0; k < 3; ++k) {
        if (v[k]->_M_finish - v[k]->_M_start != 6) { printf("00459af3_TEST FAIL 2\n"); return 1; }
        for (int i = 0; i < 6; ++i) if (v[k]->_M_start[i] != 0) { printf("00459af3_TEST FAIL 3\n"); return 1; }
    }
    if (d->IsUsable._M_finish - d->IsUsable._M_start != 6) { printf("00459af3_TEST FAIL 4\n"); return 1; }
    for (int i = 0; i < 6; ++i) if (d->IsUsable._M_start[i] != false) { printf("00459af3_TEST FAIL 5\n"); return 1; }
    if (d->InitialSelectedSlot != 0) { printf("00459af3_TEST FAIL 6\n"); return 1; }
    printf("00459af3_TEST PASS\n");
    return 0;
}