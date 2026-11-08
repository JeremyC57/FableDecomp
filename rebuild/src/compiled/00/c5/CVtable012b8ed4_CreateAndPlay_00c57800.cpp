// CVtable_012b8ed4::CreateAndPlay(unsigned long channels, unsigned long bytes) -- set up the OpenAL streaming voice: pick the 16-bit AL format and block size for the channel count (QUAD / 5.1 / 7.1 through alGetEnumValue), generate the source (not looping) and one buffer name per 128 bytes; false on an unknown layout or any AL error
// Leaf: ?CreateAndPlay@CVtable_012b8ed4@@UAE_NKK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C57800, 448 bytes = 0x192 of code + `mov edi,edi` pad + 8-entry jump table (one
// function); thiscall `ret 8`; slot 3 of the OpenAL streaming-buffer vtable 0x012B8ED4 (data ref 0x012B8EE0) and of the
// derived 0x012B8E20 (0x012B8E2C).
// Identity: the landed slot members of 0x012B8ED4 (Initialize 0x00C57460 already lists slot 3 = 0x00C57800 as
// CreateAndPlay; ReleaseBuffer 0x00C579C0 frees exactly what this allocates: the source +0x608, the buffer names +0x604,
// count +0x618) and the DirectSound twin CDSoundStreamBuffer::CreateAndPlay 0x00C57380 (same slot, same `ret 8`). The
// class name is the landed descriptive one (libpk, no PDB/RTTI). Formats: AL_FORMAT_MONO16 0x1101 / STEREO16 0x1103 and the
// "AL_FORMAT_QUAD16" / "AL_FORMAT_51CHN16" / "AL_FORMAT_71CHN16" enum names (0x012B8F24 / 0x012B8F10 / 0x012B8EFC);
// block sizes 0x100 per channel; AL_LOOPING 0x1007 set to 0.
// Callees (all on the global context 0x0143BDB0, out of line): CAudioContext::GetEnumValue 0x00C57730, GetError
// 0x00C4D960, GenSources 0x00C574B0, Sourcei 0x00C57550, GenBuffers 0x00C4D870; CodaMalloc / CodaFree 0x00C3C360 /
// 0x00C3C370.
// Shape: the three enum arms share the `mov [esi+60Ch],eax` tail; the zero register (ebp) is the AL_FALSE argument and the
// three cleared counters.
#include "engine/CodaMemory.h"

struct CAudioContext {
    long GetEnumValue(const char* name);                    // 0x00C57730
    long GetError();                                        // 0x00C4D960
    void GenSources(long count, unsigned long* sources);    // 0x00C574B0
    void Sourcei(unsigned long source, long param, long value);   // 0x00C57550
    void GenBuffers(long count, unsigned long* buffers);    // 0x00C4D870
};
extern CAudioContext g_AudioContext;                        // 0x0143BDB0

class CVtable_012b8ed4 {
public:
    virtual void v00();
    virtual bool Initialize(const void* device, void* window, bool multithreaded);   // slot 1: 0x00C57460
    virtual void v02();
    virtual bool CreateAndPlay(unsigned long channels, unsigned long bytes);         // slot 3
    unsigned char  _pad04[0x600];
    unsigned long* m_pBuffers;                      // +0x604 OpenAL buffer names
    unsigned long  m_source;                        // +0x608 OpenAL source name
    long           m_format;                        // +0x60c AL_FORMAT_*
    unsigned long  m_blockBytes;                    // +0x610
    bool           m_hasSource;                     // +0x614
    bool           m_created;                       // +0x615
    unsigned char  _pad616[2];
    unsigned long  m_bufferCount;                   // +0x618
    unsigned long  m_queuedCount;                   // +0x61c
    unsigned long  m_620;                           // +0x620
    unsigned long  m_624;                           // +0x624
};

bool CVtable_012b8ed4::CreateAndPlay(unsigned long channels, unsigned long bytes)
{
    switch (channels) {
    case 1:
        m_format = 0x1101;                          // AL_FORMAT_MONO16
        m_blockBytes = 0x100;
        break;
    case 2:
        m_format = 0x1103;                          // AL_FORMAT_STEREO16
        m_blockBytes = 0x200;
        break;
    case 4:
        m_format = g_AudioContext.GetEnumValue("AL_FORMAT_QUAD16");
        m_blockBytes = 0x400;
        break;
    case 6:
        m_format = g_AudioContext.GetEnumValue("AL_FORMAT_51CHN16");
        m_blockBytes = 0x600;
        break;
    case 8:
        m_format = g_AudioContext.GetEnumValue("AL_FORMAT_71CHN16");
        m_blockBytes = 0x800;
        break;
    default:
        return false;
    }
    if (m_format == 0)
        return false;
    g_AudioContext.GetError();
    g_AudioContext.GenSources(1, &m_source);
    if (g_AudioContext.GetError() != 0)
        return false;
    g_AudioContext.Sourcei(m_source, 0x1007, 0);    // AL_LOOPING, AL_FALSE
    m_hasSource = true;
    m_bufferCount = bytes >> 7;
    m_pBuffers = (unsigned long*)CodaMalloc(m_bufferCount * 4);
    if (m_pBuffers == 0)
        return false;
    g_AudioContext.GetError();
    g_AudioContext.GenBuffers(m_bufferCount, m_pBuffers);
    if (g_AudioContext.GetError() != 0) {
        CodaFree(m_pBuffers);
        m_pBuffers = 0;
        return false;
    }
    m_queuedCount = 0;
    m_620 = 0;
    m_624 = 0;
    m_created = true;
    return true;
}