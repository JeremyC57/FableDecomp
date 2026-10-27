// Behaviour test for 0x00C62920 CAudioStreamOwner_00c62920 constructor (standalone copy of the model and logic; callees stubbed)
#include <cstdio>
struct SStreamSegment { unsigned int m_word0, m_word4, m_word8; };
struct IAudioStream { virtual ~IAudioStream() {} int kind; };
struct ToneStream : IAudioStream { ToneStream(float) { kind = 1; } };
struct DecoderStream : IAudioStream { SStreamSegment seg; DecoderStream(const SStreamSegment& s) : seg(s) { kind = 2; } };
struct Source { unsigned int block; unsigned int GetBlockSize() { return block; } };
struct Opener { Source* src; Source* Open(const char*) { return src; } };
static bool g_useTestToneStream;
static IAudioStream* CreateDecoderStream(Source*, const char*, const SStreamSegment& s) { return new DecoderStream(s); }
inline unsigned int AlignUp(unsigned int value, unsigned int alignment) { return (value + alignment - 1) / alignment * alignment; }
struct Owner {
    Source* m_source; IAudioStream* m_stream; int m_position; int m_0c; bool m_finished;
    Owner(Opener* opener, const char* name, const SStreamSegment& segment)
        : m_source(0), m_stream(0), m_position(-1), m_finished(false)
    {
        if (g_useTestToneStream) { m_stream = new ToneStream(800.0f); return; }
        m_source = opener->Open(name);
        if (m_source) {
            SStreamSegment aligned = segment;
            aligned.m_word0 = AlignUp(aligned.m_word0, m_source->GetBlockSize());
            m_stream = CreateDecoderStream(m_source, name, aligned);
        }
    }
};
int main()
{
    Source src = { 2048 };
    Opener op = { &src };
    SStreamSegment seg = { 3000, 7, 9 };
    Owner a(&op, "x.ogg", seg);
    DecoderStream* d = (DecoderStream*)a.m_stream;
    if (!d || d->kind != 2 || d->seg.m_word0 != 4096 || d->seg.m_word4 != 7 || a.m_position != -1 || a.m_finished) { std::printf("00c62920_TEST FAIL\n"); return 1; }
    delete d;
    Opener none = { 0 };
    Owner b(&none, "y", seg);
    if (b.m_stream != 0 || b.m_source != 0) { std::printf("00c62920_TEST FAIL\n"); return 1; }
    g_useTestToneStream = true;
    Owner c(&op, "z", seg);
    if (!c.m_stream || c.m_stream->kind != 1 || c.m_source != 0) { std::printf("00c62920_TEST FAIL\n"); return 1; }
    delete c.m_stream;
    std::printf("00c62920_TEST PASS\n");
    return 0;
}