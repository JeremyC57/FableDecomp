// Behaviour test for 0x00A9D270 ~C3DMeshFileEngineMaterialChunk: the MapFilenames block is released (deleted when its count
// reaches 0, the pointer cleared), then Name, then the super-chunk base.
#include <cstdio>
#include <cstring>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
class CIVCountedPointeeBase {
public:
    CIVCountedPointeeBase() : IVRefCount(1) { }
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const { Log("D"); }
    long Release() const { return --IVRefCount; }
    mutable unsigned long IVRefCount;
};
class CIVCountedPointer {
public:
    CIVCountedPointer() : Object(0) { }
    ~CIVCountedPointer() { Release(); }
    void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CIVCountedPointeeBase* Object;
};
class CCharString { public: ~CCharString() { Log("N"); } };
class CChunkedFileSuperChunk { public: virtual ~CChunkedFileSuperChunk() { Log("S"); } };
class C3DMeshFileEngineMaterialChunk : public CChunkedFileSuperChunk {
public:
    long              MaterialIndex;
    CCharString       Name;
    CIVCountedPointer MapFilenames;
};
int main()
{
    CIVCountedPointeeBase shared;
    shared.IVRefCount = 2;
    g_log[0] = 0;
    { C3DMeshFileEngineMaterialChunk chunk; chunk.MapFilenames.Object = &shared; }
    if (std::strcmp(g_log, "NS") != 0 || shared.IVRefCount != 1) { std::printf("00a9d270_TEST FAIL\n"); return 1; }
    g_log[0] = 0;
    { C3DMeshFileEngineMaterialChunk chunk; chunk.MapFilenames.Object = &shared; }
    if (std::strcmp(g_log, "DNS") != 0 || shared.IVRefCount != 0) { std::printf("00a9d270_TEST FAIL\n"); return 1; }
    g_log[0] = 0;
    { C3DMeshFileEngineMaterialChunk chunk; }
    if (std::strcmp(g_log, "NS") != 0) { std::printf("00a9d270_TEST FAIL\n"); return 1; }
    std::printf("00a9d270_TEST PASS\n");
    return 0;
}