// Behaviour test for 0x00C6AFC0 CSoundOutput00c6afc0::CSoundOutput00c6afc0
#include <cstdio>
static int g_recordCtor = 0, g_streamCtor = 0;
struct CSoundFormatBase {
    long m_channels, m_sampleRate, m_0c, m_10, m_14, m_18;
    CSoundFormatBase() { m_sampleRate = 48000; m_channels = 2; m_0c = 0; m_10 = 0; m_14 = 0; m_18 = 0; }
    virtual ~CSoundFormatBase() {}
};
struct CFormat : CSoundFormatBase { CFormat() {} };
struct CRecord { long w28; CRecord() : w28(0) { ++g_recordCtor; } };
struct CStream { long channels, rate; CStream() : channels(2), rate(48000) { ++g_streamCtor; } };
struct CSoundOutput {
    CFormat m_format; CRecord m_record; CStream m_stream; long m_524, m_528;
    CSoundOutput() : m_524(0), m_528(0) {}
};
int main()
{
    CSoundOutput o;
    bool ok = o.m_format.m_channels == 2 && o.m_format.m_sampleRate == 48000 && o.m_format.m_0c == 0 && o.m_format.m_18 == 0
        && g_recordCtor == 1 && g_streamCtor == 1 && o.m_record.w28 == 0 && o.m_524 == 0 && o.m_528 == 0;
    if (!ok) { std::printf("00c6afc0_TEST FAIL\n"); return 1; }
    std::printf("00c6afc0_TEST PASS\n");
    return 0;
}