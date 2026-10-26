#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; std::printf("check failed line %d\n", __LINE__); } } while (0)
// ---- the candidate model (copied from src/00b19c20.cpp) ----
// NParticleEngine::CPSCLight::Kill() -- vtable slot 32: kill the light: clear its engine primitive handle (the primitive is told when this was its last handle, then the counted pointer is released)
// Leaf: ?Kill@CPSCLight@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B19C20, 58 bytes (+6 int3 padding).
// Name: FableWin.pdb ?Kill@CPSCLight@NParticleEngine@@UAEXXZ (the only no-argument void virtual CPSCLight adds beyond
// EnterTimelineEnd, slot 25 0x00B1AE60); slot 32 of the vtable 0x012A0624 (rtti-vtables.tsv).
// Callees: the primitive's virtual slots 3 (+0x0C, told its last handle goes away, with the handle) and 1 (+0x04, destroy
// on the last reference).
// Layout: as the landed ~CPSCLight 0x00B1AEB0: PrimHandle (CEnginePrimitiveHandle: vptr + CIVCountedPointer Data) +0x58.
// Shape: the handle's inline Clear() from the landed destructor, verbatim: the null-safe `GetRefCount() == 1` test
// (handle address formed in esi first), then `Data = 0` = the counted release plus the second unconditional zero
// store. Plain `ret`.
class CBaseClass { public:
    virtual void v00();
    ~CBaseClass();                           // 0x0099A2E0
};

class CEnginePrimitiveHandle;

class CEngineInternalPrimitiveBase { public:        // the referenced primitive: count at +4
    virtual void v00();
    virtual void Destroy();                  // slot 1 (+0x04)
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3 (+0x0c)
    long m_handleCount;                      // +0x04
};

struct CIVCountedPointer {                   // the counted primitive pointer (as the landed handle ??_G 0x0041C500)
    CEngineInternalPrimitiveBase* Object;
    long GetRefCount() const { return Object ? Object->m_handleCount : 0; }
    __forceinline void Release()
    {
        CEngineInternalPrimitiveBase* object = Object;
        if (object) {
            if (--object->m_handleCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(CEngineInternalPrimitiveBase* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->m_handleCount;
        return *this;
    }
    __forceinline ~CIVCountedPointer() { Release(); }
};

class CEnginePrimitiveHandle : public CBaseClass {   // vtable 0x0122F598
public:
    CIVCountedPointer Data;                  // +0x04
    virtual void v00();
    __forceinline void Clear()
    {
        if (Data.GetRefCount() == 1)
            Data.Object->OnLastHandleReleased(this);
        Data = 0;
    }
};

namespace NParticleEngine {
class CPSCLight { public:                          // vtable 0x012A0624
    virtual void v00();
    unsigned char          _body[0x58 - 4];  // the component base's state and the light parameters
    CEnginePrimitiveHandle PrimHandle;       // +0x58
    virtual void Kill();
};

void CPSCLight::Kill()
{
    PrimHandle.Clear();
}
} // namespace NParticleEngine

// ---- stubs for the external callees ----

CBaseClass::~CBaseClass() {}
void CBaseClass::v00() {}
static int g_destroyed = 0;
static int g_lastReleased = 0;
static CEnginePrimitiveHandle* g_releasedBy = 0;
void CEngineInternalPrimitiveBase::v00() {}
void CEngineInternalPrimitiveBase::Destroy() { ++g_destroyed; }
void CEngineInternalPrimitiveBase::v02() {}
void CEngineInternalPrimitiveBase::OnLastHandleReleased(CEnginePrimitiveHandle* handle) { ++g_lastReleased; g_releasedBy = handle; }
void CEnginePrimitiveHandle::v00() {}
void NParticleEngine::CPSCLight::v00() {}

static void run()
{

    CEngineInternalPrimitiveBase primitive;
    primitive.m_handleCount = 1;
    NParticleEngine::CPSCLight light;
    light.PrimHandle.Data.Object = &primitive;
    light.Kill();                              // the last handle: notified, released, destroyed
    CHECK(g_lastReleased == 1 && g_releasedBy == &light.PrimHandle);
    CHECK(g_destroyed == 1 && primitive.m_handleCount == 0 && light.PrimHandle.Data.Object == 0);
    primitive.m_handleCount = 2;
    light.PrimHandle.Data.Object = &primitive;
    light.Kill();                              // another handle remains: just released
    CHECK(g_lastReleased == 1 && g_destroyed == 1 && primitive.m_handleCount == 1 && light.PrimHandle.Data.Object == 0);
    light.Kill();                              // empty handle: nothing happens
    CHECK(g_lastReleased == 1 && g_destroyed == 1 && primitive.m_handleCount == 1);
}

int main()
{
    run();
    if (!g_ok) { std::printf("00b19c20_TEST FAIL\n"); return 1; }
    std::printf("00b19c20_TEST PASS\n");
    return 0;
}