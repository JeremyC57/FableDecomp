// Behaviour test for CEngineSkyRenderer::BuildStarFieldVB (0x00B61FF0): with the cache file present the star count
// is read, the vertex buffer initialised for StarCount 24-byte vertices, filled from the file and unlocked; the file
// is closed. Without the file nothing happens.
#include <cstdio>
#include <cstring>

struct CWideString { const char* Path; };
static bool g_exists = false;
bool PathExists(const CWideString&) { return g_exists; }
struct CASuspendableProcess;
enum FileAccessType { FILE_ACCESS_READ = 1 };
static unsigned char g_fileData[64]; static int g_filePos = 0; static int g_closes = 0; static long g_lastFlags = 0;
struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() {} };
struct CDiskFileWin32 : CBaseClassNonCopyable {
    CWideString Pathname;
    CDiskFileWin32(const CWideString& pathname, FileAccessType, unsigned long flags) { Pathname = pathname; g_filePos = 0; g_lastFlags = (long)flags; }
    virtual void Close() { ++g_closes; }
    virtual void Read(void* buffer, long size, CASuspendableProcess*) { std::memcpy(buffer, g_fileData + g_filePos, size); g_filePos += size; }
    ~CDiskFileWin32() { Close(); }
};
enum EVertexType { VERTEX_TYPE_STAR_FIELD = 0x42 };
enum EVertexBufferPool { VERTEX_BUFFER_POOL_MANAGED = 1 };
struct CStarFieldVertexBufferCalls {
    long Type, Count, Stride; int Locked; unsigned char Data[48];
    bool Initialise(EVertexType type, long count, long stride, unsigned long, EVertexBufferPool) { Type = type; Count = count; Stride = stride; return true; }
    void* Lock(unsigned long, unsigned long, unsigned long) { ++Locked; return Data; }
    void Unlock() { --Locked; }
};
struct CSkyRendererMethods {
    CStarFieldVertexBufferCalls VB; long StarCount; CWideString Path;
    CStarFieldVertexBufferCalls& StarField() { return VB; }
    const CWideString& StarVBFilePath() const { return Path; }
    void BuildStarFieldVB();
};
void CSkyRendererMethods::BuildStarFieldVB()
{
    if (PathExists(StarVBFilePath())) {
        CDiskFileWin32 file(StarVBFilePath(), FILE_ACCESS_READ, 2);
        file.Read(&StarCount, 4, 0);
        StarField().Initialise(VERTEX_TYPE_STAR_FIELD, StarCount, 0x18, 0, VERTEX_BUFFER_POOL_MANAGED);
        long bytes = StarCount * 0x18;
        void* vertices = StarField().Lock(0, 0, 0);
        file.Read(vertices, bytes, 0);
        StarField().Unlock();
    }
}
int main()
{
    CSkyRendererMethods sky; sky.VB.Type = 0; sky.VB.Locked = 0; sky.StarCount = -1; sky.Path.Path = "stars.vb";
    sky.BuildStarFieldVB();
    if (sky.StarCount != -1 || g_closes != 0) { std::printf("00b61ff0_TEST FAIL\n"); return 1; }
    long two = 2; std::memcpy(g_fileData, &two, 4);
    for (int i = 0; i < 48; ++i) g_fileData[4 + i] = (unsigned char)(i + 1);
    g_exists = true; sky.StarCount = 0;   // the file supplies the low 4 bytes (long is wider on 64-bit hosts)
    sky.BuildStarFieldVB();
    if (sky.StarCount != 2 || sky.VB.Type != 0x42 || sky.VB.Count != 2 || sky.VB.Stride != 0x18 || sky.VB.Locked != 0 ||
        sky.VB.Data[0] != 1 || sky.VB.Data[47] != 48 || g_filePos != 52 || g_closes != 1 || g_lastFlags != 2) { std::printf("00b61ff0_TEST FAIL\n"); return 1; }
    std::printf("00b61ff0_TEST PASS\n");
    return 0;
}
