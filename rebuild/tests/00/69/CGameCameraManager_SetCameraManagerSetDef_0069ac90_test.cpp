// Behaviour test for 0069ac90: src/0069ac90.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CGameCameraManager::SetCameraManagerSetDef(const CDefPointer<const CCameraManagerSetDef>& setDef)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CGameCameraManager::SetCameraManagerSetDef(const CDefPointer<const CCameraManagerSetDef>& setDef) -- store the camera manager set def (counted-pointer assignment), then look up the set's main camera manager def and install it (SetCameraManagerDef)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069AC90, length 124 bytes (+ int3 padding). `ret 4`.
// Callees: the old set def's virtual slot 1 (Destroy), CDefinitionManager::GetDef(long, CDefPointer&) @ 0x0069CE80 (the world's
// definition manager at +0x10), CGameCameraManager::SetCameraManagerDef @ 0x0069A850 (landed), the def's virtual slot 1 in the local's
// destructor.
// Layout: retail CGameCameraManager as the landed SetCameraManagerDef 0x0069A850 (World +0x74, PCameraManagerSet +0x130).
// Shape: CDefPointer::operator= inlined exactly as the landed SetCameraManagerDef (compare, release through a local copy, null, copy
// re-read from the reference, add-ref); the def index is read through the ARGUMENT (`mov edx,[edi]`), and the local CDefPointer lives in
// the dead argument slot (`lea ecx,[esp+0Ch]`), zeroed after the GetDef pushes.
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                         // slot 1: runs when the last reference goes
    long m_refCount;                                // +4
    void Release()
    {
        if (DecRefToZero(m_refCount))
            Destroy();
    }
};
struct CCameraManagerSetDef : CDefPointeeBase {
    unsigned char _pad08[0x3c];
    long          MainCameraManagerDef;             // +0x44 global def index of the set's main camera manager def
};
template <class T>
struct CDefPointer {
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    ~CDefPointer()
    {
        if (m_pObject)
            m_pObject->Release();                   // the release lives on the pointee: `this` stays in ecx (`dec [ecx+4] ; jne`)
    }
    T* operator->() const { return m_pObject; }
    void Release()
    {
        T* old = m_pObject;                         // a local copy: retail decrements through the register (`dec [ecx+4] ; jne`)
        if (old) {
            if (DecRefToZero(old->m_refCount)) old->Destroy();
            m_pObject = 0;
        }
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_pObject != other.m_pObject) {
            Release();
            m_pObject = other.m_pObject;            // re-read after Destroy (`mov eax,[edi]`)
            if (m_pObject) ++m_pObject->m_refCount;
        }
        return *this;
    }
};
struct CCameraManagerDef : CDefPointeeBase { };

struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointer<CCameraManagerDef>& out);   // 0x0069CE80 (manifest names the CThingPhysicalSwitchDef instantiation: identical body)
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CCameraManagerDef>& out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)
struct CWorld {
    unsigned char       _pad[0x10];
    CDefinitionManager* m_pDefinitionManager;      // +0x10
};
struct CGameCameraManager {
    void*         __vftable;                        // +0x00
    unsigned char _pad04[0x70];                     // +0x04 Camera, OldCamera
    CWorld*       World;                            // +0x74
    unsigned char _pad78[0x30];                     // +0x78 CamTools, CurrentMode, PendingMode
    bool          NewAutoMode;                      // +0xa8
    bool          ObservedThingInMelee;             // +0xa9
    bool          ObservedThingInBuilding;          // +0xaa
    unsigned char _padab[0x85];                     // +0xab
    CDefPointer<CCameraManagerSetDef> PCameraManagerSet;   // +0x130
    CDefPointer<CCameraManagerDef>    PDef;                // +0x134
    void SetCameraManagerDef(const CDefPointer<CCameraManagerDef>& def);   // 0x0069A850 (landed)
    void SetCameraManagerSetDef(const CDefPointer<CCameraManagerSetDef>& setDef);
};
void* FindRef_CGameCameraManager_SetCameraManagerDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CGameCameraManager::SetCameraManagerDef(const CDefPointer<CCameraManagerDef>& def)
{
    void* p = FindRef_CGameCameraManager_SetCameraManagerDef_0();
    if (!p)
        return;
    if (*(void**)&def != p)
        return;
}
#pragma auto_inline(on)

void CGameCameraManager::SetCameraManagerSetDef(const CDefPointer<CCameraManagerSetDef>& setDef)
{
    PCameraManagerSet = setDef;
    CDefPointer<CCameraManagerDef> def;
    World->m_pDefinitionManager->GetDef(setDef->MainCameraManagerDef, def);
    SetCameraManagerDef(def);
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_destroyed = 0; static int g_setCalls = 0;
void* FindDef_CDefinitionManager_GetDef_0() { return g_def; }
void* FindRef_CGameCameraManager_SetCameraManagerDef_0() { ++g_setCalls; return 0; }
void CDefPointeeBase::v00() {}
void CDefPointeeBase::Destroy() { ++g_destroyed; }
int main()
{
    CCameraManagerSetDef setA; setA.m_refCount = 1; setA.MainCameraManagerDef = 3;
    CCameraManagerSetDef setB; setB.m_refCount = 1; setB.MainCameraManagerDef = 4;
    CCameraManagerDef cd; cd.m_refCount = 2; g_def = &cd;
    CDefinitionManager dm; CWorld world; memset(&world, 0, sizeof(world)); world.m_pDefinitionManager = &dm;
    static CGameCameraManager m; memset(&m, 0, sizeof(m)); m.World = &world;
    {
        CDefPointer<CCameraManagerSetDef> a; a.m_pObject = &setA; ++setA.m_refCount;
        m.SetCameraManagerSetDef(a);
        CHECK(m.PCameraManagerSet.m_pObject == &setA); CHECK(setA.m_refCount == 3); CHECK(g_setCalls == 1); CHECK(cd.m_refCount == 1);
        CDefPointer<CCameraManagerSetDef> b; b.m_pObject = &setB; ++setB.m_refCount;
        m.SetCameraManagerSetDef(b);
        CHECK(m.PCameraManagerSet.m_pObject == &setB); CHECK(setA.m_refCount == 2); CHECK(setB.m_refCount == 3); CHECK(cd.m_refCount == 0); CHECK(g_destroyed == 1);
    }
    CHECK(setA.m_refCount == 1); CHECK(setB.m_refCount == 2);
    m.PCameraManagerSet.m_pObject = 0; m.PDef.m_pObject = 0;
    if (fails == 0) printf("0069ac90_TEST PASS\n"); else printf("0069ac90_TEST FAIL\n");
    return fails;
}