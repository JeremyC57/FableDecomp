#include <cstdio>
#include <cstring>
struct SStageSettings94 { unsigned char _data[0xc0]; };
struct SStageSettings144 { unsigned char _data[0x40]; };
struct SReverbSettings {                           // the processor's current / target settings block
    unsigned char     _combs[0x88];                // +0x00 (the four 0x14-byte comb settings sit at +0x38)
    SStageSettings94  Stage94;                     // +0x88
    SStageSettings144 Stage144;                    // +0x148
};
struct CDelayLine {
    float*        m_pBuffer;
    unsigned long m_length;
    unsigned long m_position;
};
struct CReverbStage94 {                            // 0xb0-byte part at +0x94 (ctor 0x00C6A280)
    unsigned char _pad00[0xa8];
    long          m_firstElement;                  // +0xa8 first blend element this stage owns
    long          m_endElement;                    // +0xac one past its last
    bool Covers(long element) const { return element >= m_firstElement && element < m_endElement; }
    void Blend(const SStageSettings94* current, const SStageSettings94* target, CDelayLine* line, float* output,
               const float* const* inputs, long blendElement, unsigned long count, unsigned long blendPosition);   // 0x00C75E90
    void Process(const SStageSettings94* current, CDelayLine* line, float* output, const float* const* inputs,
                 unsigned long count);                                                                         // 0x00C75CE0
};
struct CReverbStage144 {                           // 0x9c-byte part at +0x144 (ctor 0x00C75980)
    unsigned char _pad00[0x94];
    long          m_firstElement;                  // +0x94
    long          m_endElement;                    // +0x98
    bool Covers(long element) const { return element >= m_firstElement && element < m_endElement; }
    void Blend(const SStageSettings144* current, const SStageSettings144* target, CDelayLine* line, float* output,
               const float* const* inputs, long blendElement, unsigned long count, unsigned long blendPosition);   // 0x00C75AB0
    void Process(const SStageSettings144* current, CDelayLine* line, float* output, const float* const* inputs,
                 unsigned long count);                                                                         // 0x00C75A20
};
struct CReverbProcessor {                          // vtable 0x012B9594 (ctor 0x00C6A650, dtor 0x00C6A780)
    void*            _vtable;                      // +0x00
    SReverbSettings* m_pSettings;                  // +0x04
    SReverbSettings* m_pTargetSettings;            // +0x08
    unsigned char    _pad0c[0x18 - 0x0c];
    long             m_blendElement;               // +0x18 the element being cross-faded
    unsigned long    m_blendPosition;              // +0x1c
    unsigned char    _pad20[0x60 - 0x20];
    CDelayLine       m_line60;                     // +0x60
    unsigned char    _pad6c[0x94 - 0x6c];
    CReverbStage94   m_stage94;                    // +0x94
    CReverbStage144  m_stage144;                   // +0x144
    unsigned char    _pad1e0[0x288 - 0x1e0];
    float*           m_pOutput;                    // +0x288
    void ProcessStage144(const float* const* inputs, unsigned long count);
};

void CReverbProcessor::ProcessStage144(const float* const* inputs, unsigned long count)
{
    CReverbStage144* stage = &m_stage144;
    SReverbSettings* settings = m_pSettings;
    SReverbSettings* target = m_pTargetSettings;
    if (stage->Covers(m_blendElement))
        stage->Blend(&settings->Stage144, &target->Stage144, &m_line60, m_pOutput, inputs, m_blendElement, count, m_blendPosition);
    else
        stage->Process(&settings->Stage144, &m_line60, m_pOutput, inputs, count);
}
static int g_blend, g_process;
static const void* g_cur; static const void* g_tgt; static CDelayLine* g_line; static float* g_out;
static const float* const* g_in; static long g_elem; static unsigned long g_count, g_pos;
void CReverbStage144::Blend(const SStageSettings144* current, const SStageSettings144* target, CDelayLine* line, float* output,
    const float* const* inputs, long blendElement, unsigned long count, unsigned long blendPosition)
{ ++g_blend; g_cur = current; g_tgt = target; g_line = line; g_out = output; g_in = inputs; g_elem = blendElement; g_count = count; g_pos = blendPosition; }
void CReverbStage144::Process(const SStageSettings144* current, CDelayLine* line, float* output, const float* const* inputs, unsigned long count)
{ ++g_process; g_cur = current; g_line = line; g_out = output; g_in = inputs; g_count = count; }
static int fail() { std::printf("00c6a200_TEST FAIL\n"); return 1; }
int main()
{
    static CReverbProcessor p;
    static SReverbSettings cur, tgt;
    static float out[4];
    const float* chans[2] = { out, out + 2 };
    std::memset(&p, 0, sizeof(p));
    p.m_pSettings = &cur; p.m_pTargetSettings = &tgt; p.m_pOutput = out;
    p.m_stage144.m_firstElement = 2; p.m_stage144.m_endElement = 5;
    p.m_blendPosition = 77;
    p.m_blendElement = 1;                       // below the range: plain process
    p.ProcessStage144(chans, 128);
    if (g_process != 1 || g_blend != 0 || g_cur != &cur.Stage144 || g_line != &p.m_line60 || g_out != out || g_in != chans || g_count != 128) return fail();
    p.m_blendElement = 5;                       // the end is exclusive
    p.ProcessStage144(chans, 64);
    if (g_process != 2 || g_blend != 0 || g_count != 64) return fail();
    p.m_blendElement = 2;                       // inside: cross-fade current -> target
    p.ProcessStage144(chans, 32);
    if (g_blend != 1 || g_process != 2 || g_cur != &cur.Stage144 || g_tgt != &tgt.Stage144 || g_elem != 2 || g_count != 32 || g_pos != 77) return fail();
    std::printf("00c6a200_TEST PASS\n");
    return 0;
}