#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#define free test_free
#define malloc test_malloc
// Behaviour test for 00af1480: the reconstructed source below (copied verbatim from src/) runs against stub callees.
// NParticleEngine::CParticleEmitterInfo::GetEmitterTemplate(CCountedPointer<CParticleEmitter>&, bool) -- create and start loading the emitter data on first use; hand out its emitter template (building it first if needed) once the data has finished loading, or after forcing the load to complete when asked to; false when the template is not available yet
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF1480, length 204 bytes (manifest label correct).
// Callees: operator new @ 0x00BFEA1A (0x30 bytes), CResource::CResource @ 0x009FBEC0 (the inlined CEmitterData
//          constructor stores vtable 0x0129F354 = NParticleEngine::CParticleEmitterInfo::CEmitterData),
//          CEmitterData::BeginLoading @ 0x00AF1360, BuildEmitterTemplate @ 0x00AF12A0 (x2),
//          CBankFileAsyncData::IsFinished @ 0x009D54E0, WaitUntilFinished @ 0x009D5B60,
//          CCountedPointer<CParticleEmitter>::operator= @ 0x00AE96E0 (x2; labelled
//          "CCountedPointer<CDiskFileWin32>::operator=", a byte-twin), the data's virtual slot 1 (Destroy).
// Layout: the info's first word is an intrusive pointer to its CEmitterData (a CResource: reference count at +4,
// slot 1 destroys); CEmitterData = CResource (0x1c) + FileData (counted pointer, +0x1c) + EmitterTemplate
// (counted pointer, +0x24) + Owner (+0x2c), as the landed BeginLoading 0x00AF1360.
// Shape: assigning a raw pointer to the intrusive pointer releases the old object (Destroy at the last
// reference, then null) and stores the new one without an extra reference; BeginLoading runs on the new
// object directly. A template that already exists skips the "has the file finished" test.
void* __cdecl operator new(size_t size);     // 0x00BFEA1A

struct CResource {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long           RefCount;                       // +0x04
    unsigned char  _body[0x14];                    // +0x08 ResourceList, Prev/NextResource, ResourceSize, LastUsedFrame
    CResource();                                   // 0x009FBEC0
    void Release() { if (--RefCount == 0) Destroy(); }
};

struct CBankFileAsyncData {
    bool IsFinished();                             // 0x009D54E0
    bool WaitUntilFinished();                      // 0x009D5B60
};

struct CCPPointerInfo;
template<class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00AE96E0 (T = CParticleEmitter)
    T* operator->() const { return Data; }
};

template<class T> struct CIVCountedPointer {       // intrusive: the object carries its own count
    T* Object;
    void ReleaseRef() { if (Object) { Object->Release(); Object = 0; } }
    CIVCountedPointer& operator=(T* p) { ReleaseRef(); Object = p; return *this; }
    T* operator->() const { return Object; }
};

namespace NParticleEngine {
class CParticleEmitter;

class CParticleEmitterInfo {
public:
    struct CEmitterData : CResource {              // vtable 0x0129F354
        CCountedPointer<CBankFileAsyncData> FileData;          // +0x1c
        CCountedPointer<CParticleEmitter>   EmitterTemplate;   // +0x24
        CParticleEmitterInfo*               Owner;             // +0x2c
        CEmitterData(CParticleEmitterInfo* owner) : CResource() { Owner = owner; }
        virtual void Destroy();
        void BeginLoading();                       // 0x00AF1360
        void BuildEmitterTemplate();               // 0x00AF12A0
    };
    CIVCountedPointer<CEmitterData> Data;          // +0x00
    bool GetEmitterTemplate(CCountedPointer<CParticleEmitter>& out, bool forceLoad);
};

bool CParticleEmitterInfo::GetEmitterTemplate(CCountedPointer<CParticleEmitter>& out, bool forceLoad)
{
    if (Data.Object == 0) {
        CEmitterData* data = new CEmitterData(this);
        Data = data;
        data->BeginLoading();
    }
    if (Data.Object) {
        if (Data->EmitterTemplate.Data != 0 || (Data->FileData.Data != 0 && Data->FileData->IsFinished())) {
            if (Data->EmitterTemplate.Data == 0)
                Data->BuildEmitterTemplate();
            out = Data->EmitterTemplate;
            return true;
        }
    }
    if (forceLoad) {
        Data->FileData->WaitUntilFinished();
        Data->BuildEmitterTemplate();
        out = Data->EmitterTemplate;
        return true;
    }
    return false;
}
}

// ---- stubs and driver ----
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }
static unsigned char g_heap[256];
static unsigned int g_newSize = 0;
void* __cdecl operator new(size_t size) { g_newSize = (unsigned int)size; return g_heap; }
CResource::CResource() : RefCount(1) { Log("CResource"); }
void CResource::v00() {}
void CResource::Destroy() { Log("Destroy"); }
static bool g_finished = false;
bool CBankFileAsyncData::IsFinished() { Log("IsFinished"); return g_finished; }
bool CBankFileAsyncData::WaitUntilFinished() { Log("Wait"); g_finished = true; return true; }
static CBankFileAsyncData g_fileData;
static NParticleEngine::CParticleEmitter* g_template = (NParticleEngine::CParticleEmitter*)0x1234;
void NParticleEngine::CParticleEmitterInfo::CEmitterData::Destroy() { Log("DataDestroy"); }
void NParticleEngine::CParticleEmitterInfo::CEmitterData::BeginLoading() { Log("Begin"); FileData.Data = &g_fileData; }
void NParticleEngine::CParticleEmitterInfo::CEmitterData::BuildEmitterTemplate() { Log("Build"); EmitterTemplate.Data = g_template; }
template<class T> CCountedPointer<T>& CCountedPointer<T>::operator=(const CCountedPointer<T>& o) { Log("Assign"); Data = o.Data; Info = o.Info; return *this; }
int main()
{
    using NParticleEngine::CParticleEmitterInfo;
    CParticleEmitterInfo info;
    info.Data.Object = 0;
    CCountedPointer<NParticleEngine::CParticleEmitter> out;
    // first call: creates the data, starts loading; not finished and not forced -> false
    bool r1 = info.GetEmitterTemplate(out, false);
    bool ok = !r1 && info.Data.Object == (CParticleEmitterInfo::CEmitterData*)(void*)g_heap && g_newSize == sizeof(CParticleEmitterInfo::CEmitterData) &&
              info.Data.Object->Owner == &info && out.Data == 0 &&
              std::strcmp(g_log, "CResource,Begin,IsFinished,") == 0;
    g_log[0] = 0;
    // forced: waits, builds and hands out the template
    bool r2 = info.GetEmitterTemplate(out, true);
    ok = ok && r2 && out.Data == g_template && std::strcmp(g_log, "IsFinished,Wait,Build,Assign,") == 0;
    g_log[0] = 0;
    // template exists: straight hand-out, no file test
    out.Data = 0;
    bool r3 = info.GetEmitterTemplate(out, false);
    ok = ok && r3 && out.Data == g_template && std::strcmp(g_log, "Assign,") == 0;
    if (!ok) { std::printf("00af1480_TEST FAIL\n"); return 1; }
    std::printf("00af1480_TEST PASS\n");
    return 0;
}
