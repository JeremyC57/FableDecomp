// CAudioDeviceOpenAL::GetSampleData(CAudioSample* sample, void** data) -- device slot 19: the sample's playable PCM through its CSampleData (+0x30), converting a mono sample on first use
// Leaf: ?GetSampleData@CAudioDeviceOpenAL
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C73340, length 62 bytes, ret 8. Callee: CSampleData::MakeStereo
//   0x00C4DE00. Slot 19 (+0x4C) of the OpenAL device vtable 0x012B8D48 (the table that follows "/OpenAL32.dll").
// Layout (descriptive; libpk audio, no PDB/RTTI): the sample keeps its CSampleData at +0x30 (see 0x00C732F0, the
//   same body out of line): source PCM +0x30, converted PCM +0x34, flag byte +0x38, descriptor +0x3C.
// Shape: a virtual member that never touches `this`; the CSampleData accessor is an inline member here, so VC7.1
//   forms `esi = sample + 0x30` after the descriptor load.
struct CSampleSource {
    unsigned long m_format;                // +0x00
    unsigned long m_channels;              // +0x04
    unsigned long m_08;
    unsigned long m_length;                // +0x0C
    unsigned long m_10;
    unsigned long m_14;
    void* m_pData;                         // +0x18
};

struct CSampleData {
    void* m_pSource;                       // +0x00
    void* m_pConverted;                    // +0x04
    unsigned char m_flags;                 // +0x08
    unsigned char _pad09[3];
    CSampleSource* m_pDescriptor;          // +0x0C
    long MakeStereo();                     // 0x00C4DE00
    long GetPlaybackData(void** data)
    {
        if (m_pDescriptor->m_channels == 2) {
            *data = m_pSource;
            return 0;
        }
        long result = 0;
        if (!(m_flags & 2))
            result = MakeStereo();
        *data = m_pConverted;
        return result;
    }
};

struct CAudioSample {
    unsigned char _pad00[0x30];
    CSampleData m_data;                    // +0x30
};

struct CAudioDeviceOpenAL {
    virtual long GetSampleData(CAudioSample* sample, void** data);   // slot 19 in retail; one slot here
};

long CAudioDeviceOpenAL::GetSampleData(CAudioSample* sample, void** data)
{
    return sample->m_data.GetPlaybackData(data);
}