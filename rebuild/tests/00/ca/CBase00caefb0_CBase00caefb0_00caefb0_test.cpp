// Behaviour test for 00caefb0: src/00caefb0.cpp copied verbatim; the COM pointer, the member destructor and CBaseObject's destructor
// stubbed so the release (only for a non-null pointer), the member destruction and the base destructor's this are observable.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CBase_00caefb0::~CBase_00caefb0() -- EH-framed destructor of a strmbase-style COM object: release the +0x10 COM pointer (its smart-pointer member's destructor), then the CUnknown base at +4 (its CBaseObject destructor at +8)
// Retry 10b: the COM pointer at +10h is a smart-pointer MEMBER whose inline destructor releases it (the destructor body is empty):
// the release then dereferences the member's `this`, not the destructor's, so VC7.1 keeps the `test esi,esi` null check of
// the base conversions; a release written in the destructor body dereferences `this` and the check is dropped.
// Leaf: ??1CBase_00caefb0
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAEFB0, length 118 bytes.
// Label correction: the catalog's "CVolume::ReleaseVolume" is propagated. Callers: the derived destructors 0x00CAF070
// (`this ? this+4 : 0` then a tail jump here) and 0x00CAFD90 (deletes its +0x1c critical section first), so this is a base
// destructor of the same no-RTTI CUnknown family as the landed CVtable_012bf83c 0x00CAA410.
// Callees: the +0x10 COM pointer's Release (slot 2), CBaseObject::~CBaseObject @ 0x00CACAF0.
// Layout (retail): interface vptr +0, CUnknown {vptr +4, CBaseObject +8, m_cRef +0xc}, COM pointer +0x10.
// Shape: EH state 0 for the body, -1 for the base; retail null-checks both base conversions (`test esi,esi` then
// `lea eax,[esi+4] ; test eax,eax`) before `lea ecx,[eax+4]`.
struct __declspec(novtable) IUnknownRetail {
    virtual long __stdcall QueryInterface(const void* iid, void** object) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
struct __declspec(novtable) INonDelegatingUnknownRetail {
    virtual long __stdcall NonDelegatingQueryInterface(const void* iid, void** object) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};
namespace N_00cacaf0 { class CBaseObject {                                // strmbase object counter base (4 bytes in this build)
public:
    const char* m_pName;
    ~CBaseObject();                                // 0x00CACAF0
}; } using N_00cacaf0::CBaseObject;
class __declspec(novtable) CUnknown : public INonDelegatingUnknownRetail, public CBaseObject {
public:
    long m_cRef;                                   // +0x08 within CUnknown
    virtual ~CUnknown() {}
};
struct CComPointer_Retail {                       // inline smart pointer: releases its interface
    IUnknownRetail* p;
    ~CComPointer_Retail() { if (p) p->Release(); }
};
class __declspec(novtable) CBase_00caefb0 : public IUnknownRetail, public CUnknown {
public:
    CComPointer_Retail m_pObject;                  // +0x10
    ~CBase_00caefb0();
};
CBase_00caefb0::~CBase_00caefb0()
{
}
// ---- end source ----
static int g_seq = 0; static int g_release = -1, g_base = -1; static void* g_baseThis = 0;
struct CTestObject : IUnknownRetail {
    long __stdcall QueryInterface(const void*, void**) { return 0; }
    unsigned long __stdcall AddRef() { return 1; }
    unsigned long __stdcall Release() { g_release = g_seq++; return 0; }
};
CBaseObject::~CBaseObject() { g_base = g_seq++; g_baseThis = this; }
int main()
{
    static unsigned char buf[sizeof(CBase_00caefb0)];
    CHECK(sizeof(CBase_00caefb0) == 0x14);
    CTestObject obj;
    memset(buf, 0, sizeof(buf)); *(IUnknownRetail**)(buf + 0x10) = &obj;
    CBase_00caefb0* p = (CBase_00caefb0*)buf;
    p->CBase_00caefb0::~CBase_00caefb0();
    CHECK(g_release == 0 && g_base == 1 && g_baseThis == (void*)(buf + 8));
    g_seq = 0; g_release = g_base = -1;
    memset(buf, 0, sizeof(buf));
    p->CBase_00caefb0::~CBase_00caefb0();
    CHECK(g_release == -1 && g_base == 0 && g_baseThis == (void*)(buf + 8));
    if (fails == 0) printf("00caefb0_TEST PASS\n"); else printf("00caefb0_TEST FAIL\n");
    return fails;
}