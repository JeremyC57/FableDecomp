#include <cstdio>
#include <cstdlib>
#include <cstring>

// Behaviour test for 0x00C579C0 CVtable_012b8ed4::ReleaseBuffer: lazy entry-point resolution (own resolver vs the DLL
// export path), the call order stop / detach / delete source, delete buffers, freeing the name array, flag clearing.
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static unsigned int g_detachedSource = 0, g_deletedSource = 0, g_deletedCount = 0;
static const unsigned int* g_deletedBuffers = 0;
static void alSourcei(unsigned int source, int param, int value) { if (param == 0x1009 && value == 0) { g_detachedSource = source; Log("sourcei"); } }
static void alDeleteSources(int count, const unsigned int* sources) { if (count == 1) g_deletedSource = *sources; Log("delsrc"); }
static void alDeleteBuffers(int count, const unsigned int* buffers) { g_deletedCount = count; g_deletedBuffers = buffers; Log("delbuf"); }
typedef void (*PFN_alDeleteBuffers)(int, const unsigned int*);
typedef void (*PFN_alDeleteSources)(int, const unsigned int*);
typedef void (*PFN_alSourcei)(unsigned int, int, int);
static int g_dllLookups = 0;
static void* DllGetProcAddress(void*, const char* name)
{
    ++g_dllLookups;
    if (!std::strcmp(name, "alSourcei")) return (void*)alSourcei;
    if (!std::strcmp(name, "alDeleteSources")) return (void*)alDeleteSources;
    if (!std::strcmp(name, "alDeleteBuffers")) return (void*)alDeleteBuffers;
    return 0;
}

struct CAudioContext {
    void* Module;
    PFN_alDeleteBuffers pfnDeleteBuffers;
    PFN_alDeleteSources pfnDeleteSources;
    PFN_alSourcei pfnSourcei;
    bool UseALGetProcAddress;
    int ownLookups;
    void* GetProcAddress(const char* name) { ++ownLookups; return DllGetProcAddress(0, name); }
    void SourceStop(unsigned int) { Log("stop"); }
    void Sourcei(unsigned int source, int param, int value)
    {
        if (!pfnSourcei)
            pfnSourcei = (PFN_alSourcei)(UseALGetProcAddress ? GetProcAddress("alSourcei") : DllGetProcAddress(Module, "alSourcei"));
        pfnSourcei(source, param, value);
    }
    void DeleteSources(int count, const unsigned int* sources)
    {
        if (!pfnDeleteSources)
            pfnDeleteSources = (PFN_alDeleteSources)(UseALGetProcAddress ? GetProcAddress("alDeleteSources") : DllGetProcAddress(Module, "alDeleteSources"));
        pfnDeleteSources(count, sources);
    }
    void DeleteBuffers(int count, const unsigned int* buffers)
    {
        if (!pfnDeleteBuffers)
            pfnDeleteBuffers = (PFN_alDeleteBuffers)(UseALGetProcAddress ? GetProcAddress("alDeleteBuffers") : DllGetProcAddress(Module, "alDeleteBuffers"));
        pfnDeleteBuffers(count, buffers);
    }
};
static CAudioContext g_AudioContext;
static int g_frees = 0;
static void PKDelete(void* p) { ++g_frees; std::free(p); Log("free"); }

struct CStream {
    unsigned int* m_pBuffers;
    unsigned int  m_source;
    bool          m_sourceCreated;
    bool          m_buffersCreated;
    unsigned int  m_bufferCount;
    void ReleaseBuffer()
    {
        if (m_sourceCreated) {
            g_AudioContext.SourceStop(m_source);
            g_AudioContext.Sourcei(m_source, 0x1009, 0);
            g_AudioContext.DeleteSources(1, &m_source);
            m_sourceCreated = false;
        }
        if (m_buffersCreated) {
            g_AudioContext.DeleteBuffers(m_bufferCount, m_pBuffers);
            m_buffersCreated = false;
        }
        if (m_pBuffers) {
            PKDelete(m_pBuffers);
            m_pBuffers = 0;
        }
        m_bufferCount = 0;
    }
};

int main()
{
    int bad = 0;
    std::memset(&g_AudioContext, 0, sizeof(g_AudioContext));
    CStream s;
    s.m_pBuffers = (unsigned int*)std::malloc(4 * sizeof(unsigned int));
    unsigned int* names = s.m_pBuffers;
    s.m_source = 77; s.m_sourceCreated = true; s.m_buffersCreated = true; s.m_bufferCount = 4;
    s.ReleaseBuffer();                                       // DLL export path
    bad |= std::strcmp(g_log, "stop;sourcei;delsrc;delbuf;free;") != 0;
    bad |= g_detachedSource != 77 || g_deletedSource != 77 || g_deletedCount != 4 || g_deletedBuffers != names;
    bad |= s.m_pBuffers != 0 || s.m_bufferCount != 0 || s.m_sourceCreated || s.m_buffersCreated || g_dllLookups != 3;
    g_log[0] = 0;
    s.m_pBuffers = (unsigned int*)std::malloc(sizeof(unsigned int)); s.m_bufferCount = 1; s.m_sourceCreated = true; s.m_source = 5;
    s.ReleaseBuffer();                                       // cached entry points, buffers flag clear: no delete-buffers
    bad |= std::strcmp(g_log, "stop;sourcei;delsrc;free;") != 0 || g_dllLookups != 3 || g_frees != 2 || g_deletedSource != 5;
    g_log[0] = 0;
    std::memset(&g_AudioContext, 0, sizeof(g_AudioContext)); g_AudioContext.UseALGetProcAddress = true;
    s.m_buffersCreated = true; s.m_bufferCount = 2;
    s.ReleaseBuffer();                                       // own resolver path, null array: no free
    bad |= std::strcmp(g_log, "delbuf;") != 0 || g_AudioContext.ownLookups != 1 || g_frees != 2 || g_deletedCount != 2;
    if (bad) { std::printf("00c579c0_TEST FAIL\n"); return 1; }
    std::printf("00c579c0_TEST PASS\n");
    return 0;
}