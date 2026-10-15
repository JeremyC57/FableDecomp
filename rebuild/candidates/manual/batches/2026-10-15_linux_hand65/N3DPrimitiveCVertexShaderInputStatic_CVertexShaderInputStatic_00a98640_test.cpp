#include <cstdio>
#include <cstdlib>
#include <cstring>
// CVertexShaderInputStatic destructor: the D3D declaration array freed, the stream infos destroyed then freed, then the CBaseClass destructor
static char g_log[32]; static long g_destroyed;
static void Log(const char* s) { std::strcat(g_log, s); }
struct CBaseClass { virtual void v00() { } ~CBaseClass() { Log("B"); } };
struct CStreamInfo { long stream; long stride; };
static void DestroyRange(CStreamInfo* first, CStreamInfo* last) { g_destroyed += (long)(last - first); Log("r"); }
struct CStreamInfoArray {
    CStreamInfo* m_pFirst; CStreamInfo* m_pLast; CStreamInfo* m_pEnd;
    CStreamInfoArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    ~CStreamInfoArray() { DestroyRange(m_pFirst, m_pLast); if (m_pFirst != 0) { std::free(m_pFirst); Log("s"); } }
};
struct D3DVERTEXELEMENT9 { unsigned short Stream, Offset; unsigned char Type, Method, Usage, UsageIndex; };
struct CVertexElementArray {
    D3DVERTEXELEMENT9* m_pFirst; D3DVERTEXELEMENT9* m_pLast; D3DVERTEXELEMENT9* m_pEnd;
    CVertexElementArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    ~CVertexElementArray() { if (m_pFirst) { std::free(m_pFirst); Log("d"); } }
};
struct CVertexShaderInput : CBaseClass {
    CStreamInfoArray DataStreams; CVertexElementArray D3DDeclaration;
    virtual void v00() { }
    ~CVertexShaderInput() { }
};
struct CVertexShaderInputStatic : CVertexShaderInput { virtual void v00() { } ~CVertexShaderInputStatic() { } };
int main()
{
    {
        CVertexShaderInputStatic input;
        input.DataStreams.m_pFirst = (CStreamInfo*)std::malloc(3 * sizeof(CStreamInfo)); input.DataStreams.m_pLast = input.DataStreams.m_pFirst + 3;
        input.D3DDeclaration.m_pFirst = (D3DVERTEXELEMENT9*)std::malloc(4 * sizeof(D3DVERTEXELEMENT9));
    }
    if (std::strcmp(g_log, "drsB") != 0 || g_destroyed != 3) { std::printf("00a98640_TEST FAIL\n"); return 1; }
    std::printf("00a98640_TEST PASS\n");
    return 0;
}
