// Behaviour test for 0x00A3ACC0 CCompressorZlib::~CCompressorZlib: with a session open, a compressing stream gets deflateEnd,
// a decompressing one inflateEnd and its buffer pointers / counts cleared; the flag drops; then OwnWorkspace is freed and
// ~CBaseClass runs. With no session nothing is ended.
#include <cstdio>
#include <cstdlib>
struct z_stream_s { unsigned char* next_in; unsigned int avail_in; unsigned long total_in; unsigned char* next_out; unsigned int avail_out; };
static int g_deflateEnds = 0, g_inflateEnds = 0, g_frees = 0, g_bases = 0;
static z_stream_s g_seen;
static int z_deflateEnd(z_stream_s*) { ++g_deflateEnds; return 0; }
static int z_inflateEnd(z_stream_s*) { ++g_inflateEnds; return 0; }
class CBaseClass { public: virtual ~CBaseClass() { ++g_bases; } };
class CACompressor : public CBaseClass { public:
    void* Workspace; unsigned long WorkspaceSize; bool CompressionOn; unsigned char* OwnWorkspace;
    CACompressor() : OwnWorkspace(0) { }
    ~CACompressor() { if (OwnWorkspace != 0) { std::free(OwnWorkspace); ++g_frees; } }
};
class CCompressorZlib : public CACompressor { public:
    z_stream_s Stream; bool ZlibInitialised, Compressing, ErrorFlag;
    void SetupStream(z_stream_s& s, const void* in, unsigned long inLen, void* out, unsigned long outLen)
    { s.next_in = (unsigned char*)in; s.next_out = (unsigned char*)out; s.avail_in = inLen; s.avail_out = outLen; }
    void FinishCompressStream() { if (ZlibInitialised) { z_deflateEnd(&Stream); ZlibInitialised = false; } }
    void FinishDecompressStream() { if (ZlibInitialised) { z_inflateEnd(&Stream); SetupStream(Stream, 0, 0, 0, 0); ZlibInitialised = false; g_seen = Stream; } }
    ~CCompressorZlib() { if (ZlibInitialised) { if (Compressing) FinishCompressStream(); else FinishDecompressStream(); } }
};
static CCompressorZlib* Make(bool init, bool compressing)
{
    CCompressorZlib* z = new CCompressorZlib;
    z->ZlibInitialised = init; z->Compressing = compressing;
    z->Stream.next_in = (unsigned char*)1; z->Stream.next_out = (unsigned char*)2; z->Stream.avail_in = 3; z->Stream.avail_out = 4;
    z->OwnWorkspace = (unsigned char*)std::malloc(16);
    return z;
}
int main()
{
    delete Make(true, true);
    if (g_deflateEnds != 1 || g_inflateEnds != 0 || g_frees != 1 || g_bases != 1) { std::printf("00a3acc0_TEST FAIL\n"); return 1; }
    delete Make(true, false);
    if (g_inflateEnds != 1 || g_seen.next_in != 0 || g_seen.next_out != 0 || g_seen.avail_in != 0 || g_seen.avail_out != 0 || g_frees != 2) {
        std::printf("00a3acc0_TEST FAIL\n"); return 1;
    }
    delete Make(false, true);
    if (g_deflateEnds != 1 || g_inflateEnds != 1 || g_frees != 3 || g_bases != 3) { std::printf("00a3acc0_TEST FAIL\n"); return 1; }
    std::printf("00a3acc0_TEST PASS\n");
    return 0;
}