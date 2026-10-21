// InitStreamDecoder_00c9cd20(SStreamContext_00c9cd20* context, SStreamDecoder_00c9cd20* decoder) -- libpk decoder set-up: clear the 0x68-byte decoder, size its frame buffers from the context's rate and the stream's timing parameters and calloc them (with fallback one-entry buffers when nothing is enabled), then initialise every byte buffer
// Leaf: InitStreamDecoder_00c9cd20
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9CD20, 484 bytes (496 with padding), `__fastcall` (context in ecx, decoder in edx).
// Callees: calloc (IAT 0x01440058, kept in a register across the calls), __ftol2 0x00BFEA70, CByteBuffer_00c8f080::Init
//   0x00C90F10 (thiscall, tail-called on the fallback path). Doubles 0.0 0x0122ED70, 7.0 0x012BF330.
// Names: the manifest label "CVsyncCallbackTimerInfo::Initialize" is propagated from the byte-buffer Init (as on the
//   landed InitStreamEntry_00c8f080 0x00C8F080); this is a libpk free function, names are descriptive.
// Layout: context {+0x08 rate (long), +0x1c stream}; stream {+0x00 block size (long), +0xd20 timing (7 doubles)};
//   decoder 0x68 bytes (the offsets below are the retail stores).
#include <string.h>
extern "C" __declspec(dllimport) void* __cdecl calloc(unsigned int count, unsigned int size);   // IAT 0x01440058

struct CByteBuffer_00c8f080 {                      // 0x14 bytes
    long m_0, m_4;
    unsigned char* m_pData;                        // +0x08
    unsigned char* m_pCursor;                      // +0x0c
    long m_capacity;                               // +0x10
    void Init();                                   // 0x00C90F10
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

void __fastcall InitStreamDecoder_00c9cd20(SStreamContext_00c9cd20* context, SStreamDecoder_00c9cd20* decoder)
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