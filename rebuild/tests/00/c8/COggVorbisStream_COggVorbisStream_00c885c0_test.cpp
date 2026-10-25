// Behaviour test for 00c885c0: COggVorbisStream::~COggVorbisStream -- ov_clear only when opened, then the reader, then the base
#include <cstdio>
#include <cstring>
static char g_seq[32] = "";
static void Log(const char* s) { std::strcat(g_seq, s); }
struct OggVorbis_File { __int64 _body[0x2c0 / 8]; };
static int __fastcall ov_clear(OggVorbis_File* vf) { Log("C"); std::memset(vf, 0, sizeof *vf); return 0; }
class PKCBufferedReader { public: PKCBufferedReader() { } ~PKCBufferedReader() { Log("R"); } virtual void v00() { } unsigned char _body04[0x6c]; };
class IAudioStream { public: virtual ~IAudioStream() { Log("B"); } };
class COggVorbisStream : public IAudioStream {
public:
    COggVorbisStream(long opened) : m_valid(0), m_opened(opened) { }
    virtual ~COggVorbisStream() { if (m_opened) ov_clear(&m_vf); }
    PKCBufferedReader m_reader; long m_valid; long m_opened; OggVorbis_File m_vf;
};
int main()
{
    IAudioStream* s = new COggVorbisStream(1);
    delete s;
    int ok = std::strcmp(g_seq, "CRB") == 0;
    g_seq[0] = 0; s = new COggVorbisStream(0); delete s;
    ok = ok && std::strcmp(g_seq, "RB") == 0 && sizeof(COggVorbisStream) == 0x340;
    std::printf(ok ? "00c885c0_TEST PASS\n" : "00c885c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
