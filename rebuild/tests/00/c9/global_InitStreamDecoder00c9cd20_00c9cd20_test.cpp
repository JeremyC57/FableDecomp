#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test: InitStreamDecoder_00c9cd20 clears the decoder, derives the main / lead / tail sample counts from the
// stream timing, and when any filter is active sizes the frame ring from the longest span (history and band tables
// only for the active filters); otherwise it allocates one buffer; every allocated buffer is initialised
static int g_inits = 0;

struct CByteBuffer_00c8f080 {                      // 0x14 bytes
    long m_0, m_4;
    unsigned char* m_pData;                        // +0x08
    unsigned char* m_pCursor;                      // +0x0c
    long m_capacity;                               // +0x10
    void Init() { ++g_inits; m_capacity = 0x10; }
};
struct STiming_00c9cd20 {                          // 7 doubles
    double m_scale;                                // +0x00
    double m_ratio;                                // +0x08
    double m_extra;                                // +0x10
    double m_d18;                                  // +0x18
    double m_d20;                                  // +0x20
    double m_d28;                                  // +0x28
    double m_d30;                                  // +0x30
};
struct SStream_00c9cd20 {
    long             m_blockSize;                  // +0x00
    unsigned char    _pad[0xd1c];
    STiming_00c9cd20 m_timing;                     // +0xd20
};
struct SStreamContext_00c9cd20 {
    long              m_0, m_4;
    long              m_rate;                      // +0x08
    unsigned char     _pad[0x10];
    SStream_00c9cd20* m_pStream;                   // +0x1c
};
struct SStreamDecoder_00c9cd20 {                   // 0x68 bytes
    void*                 m_pFrames;               // +0x00 count x 0x3c
    void*                 m_pFrameIndex;           // +0x04 count x 4
    long                  m_count;                 // +0x08
    long                  m_0c;
    long                  m_history;               // +0x10
    void*                 m_pHistory;              // +0x14
    long                  m_18;
    long                  m_historyOff;            // +0x1c
    long                  m_20, m_24;
    long                  m_main;                  // +0x28
    long                  m_lead;                  // +0x2c
    void*                 m_pTable;                // +0x30
    void*                 m_pBandsA;               // +0x34
    void*                 m_pBandsB;               // +0x38
    long                  m_3c, m_40;
    long                  m_bandsOff;              // +0x44
    long                  m_48;
    long                  m_tail;                  // +0x4c
    long                  m_50, m_54;
    double                m_historyGain;           // +0x58
    CByteBuffer_00c8f080* m_pBuffers;              // +0x60 count x 0x14
    void*                 m_pBufferState;          // +0x64 count x 0x20
};

void InitStreamDecoder_00c9cd20(SStreamContext_00c9cd20* context, SStreamDecoder_00c9cd20* decoder)
{
    SStream_00c9cd20* stream = context->m_pStream;
    STiming_00c9cd20* timing = &stream->m_timing;
    memset(decoder, 0, sizeof(*decoder));
    if (timing == 0)
        return;
    decoder->m_main = (long)(context->m_rate * timing->m_scale);
    decoder->m_lead = (long)(context->m_rate * timing->m_ratio * timing->m_scale);
    unsigned long tail = (long)(context->m_rate * timing->m_extra);
    decoder->m_tail = tail;
    unsigned long span = decoder->m_main - decoder->m_lead;
    if (span > tail)
        tail = span;
    long total = decoder->m_lead + tail;
    if (total > 0 && (timing->m_d28 > 0.0 || timing->m_d30 > 0.0 || timing->m_d20 > 0.0 || timing->m_d18 > 0.0)) {
        long count = total / (stream->m_blockSize >> 1) + 3;
        decoder->m_history = 15;
        decoder->m_count = count;
        decoder->m_pFrames = calloc(count, 0x3c);
        decoder->m_pFrameIndex = calloc(count, 4);
        if ((timing->m_d28 > 0.0 || timing->m_d30 > 0.0) && timing->m_scale > 0.0) {
            decoder->m_pHistory = calloc(15, 4);
            decoder->m_historyGain = 7.0;
        } else {
            decoder->m_historyOff = -1;
        }
        if ((timing->m_d18 > 0.0 || timing->m_d20 > 0.0) && timing->m_extra > 0.0) {
            decoder->m_pTable = calloc(0x3a2, 4);
            decoder->m_pBandsA = calloc(0x1f, 4);
            decoder->m_pBandsB = calloc(0x1f, 4);
        } else {
            decoder->m_bandsOff = -1;
        }
        decoder->m_pBuffers = (CByteBuffer_00c8f080*)calloc(count, 0x14);
        decoder->m_pBufferState = calloc(count, 0x20);
        for (long i = 0; i < count; ++i)
            decoder->m_pBuffers[i].Init();
        return;
    }
    decoder->m_pBuffers = (CByteBuffer_00c8f080*)calloc(1, 0x14);
    decoder->m_pBufferState = calloc(1, 0x20);
    decoder->m_pBuffers->Init();
}

static void FreeAll(SStreamDecoder_00c9cd20& d)
{
    std::free(d.m_pFrames); std::free(d.m_pFrameIndex); std::free(d.m_pHistory); std::free(d.m_pTable);
    std::free(d.m_pBandsA); std::free(d.m_pBandsB); std::free(d.m_pBuffers); std::free(d.m_pBufferState);
}
int main()
{
    static SStream_00c9cd20 stream;
    SStreamContext_00c9cd20 context;
    std::memset(&context, 0, sizeof(context));
    context.m_pStream = &stream;
    context.m_rate = 1000;
    stream.m_blockSize = 100;
    STiming_00c9cd20& t = stream.m_timing;
    t.m_scale = 0.5; t.m_ratio = 0.2; t.m_extra = 0.3; t.m_d18 = 0.0; t.m_d20 = 0.0; t.m_d28 = 1.0; t.m_d30 = 0.0;
    SStreamDecoder_00c9cd20 d;
    std::memset(&d, 0x5a, sizeof(d));
    g_inits = 0;
    InitStreamDecoder_00c9cd20(&context, &d);
    bool ok = d.m_main == 500 && d.m_lead == 100 && d.m_tail == 300 && d.m_count == 13 && d.m_history == 15
        && d.m_pHistory != 0 && d.m_historyGain == 7.0 && d.m_bandsOff == -1 && d.m_pTable == 0 && g_inits == 13
        && d.m_pBuffers[12].m_capacity == 0x10 && d.m_historyOff == 0;
    FreeAll(d);
    t.m_d28 = 0.0; t.m_d20 = 2.0; t.m_extra = 0.0;
    g_inits = 0;
    InitStreamDecoder_00c9cd20(&context, &d);
    ok = ok && d.m_count == 13 && d.m_pHistory == 0 && d.m_historyOff == -1 && d.m_bandsOff == -1 && g_inits == 13;
    FreeAll(d);
    t.m_d20 = 0.0;
    g_inits = 0;
    InitStreamDecoder_00c9cd20(&context, &d);
    ok = ok && d.m_count == 0 && d.m_pFrames == 0 && d.m_pBuffers != 0 && g_inits == 1 && d.m_main == 500;
    FreeAll(d);
    if (!ok) { std::printf("00c9cd20_TEST FAIL\n"); return 1; }
    std::printf("00c9cd20_TEST PASS\n");
    return 0;
}