#ifndef FABLE_ENGINE_CWAVEFILE_H
#define FABLE_ENGINE_CWAVEFILE_H
// Retail layout (chunk 779, hand-written): Ego_r.pdb CWaveFile {Channels +0, SampleLength +4, SamplingFrequency +8,
// CArray<short> AudioData +0xc} -- 0x1c in the PDB with its 16-byte debug vector, 0x18 in retail (STLport 4.6 vector, 12 bytes:
// `push 18h ; call operator new` at 0x00AADB59, the landed ConvertToMono 0x00AAD5D0 and SaveWaveFile 0x00AAD150 models).
// Member set: Ego_r.pdb (ConvertToMono, ConvertSamplingRate, GetByteLength, Initialise(K,K,ABV CArray<short>), PeekAudioData)
// + FableWin.pdb (GetChannels QBEFXZ, GetSampleLength, GetSamplingFrequency). Default constructor: expanded at 0x00AADB69
// (AudioData's three pointers, Channels and SampleLength stored zero; SamplingFrequency left alone).
#include "engine/CArray.h"
class CWaveFile {
public:
    unsigned long Channels;                 // +0x00
    unsigned long SampleLength;             // +0x04
    unsigned long SamplingFrequency;        // +0x08
    CArray<short> AudioData;                // +0x0c
    CWaveFile() { Channels = 0; SampleLength = 0; }   // body assignments: AudioData is zeroed first at 0x00AADB69
    void Initialise(unsigned long channels, unsigned long samplingFrequency, const CArray<short>& audioData);
    void ConvertToMono();                   // 0x00AAD5D0
    void ConvertSamplingRate(unsigned long samplingFrequency);
    unsigned long GetByteLength() const;
    short GetChannels() const;
    unsigned long GetSampleLength() const;
    unsigned long GetSamplingFrequency() const;
    const short* PeekAudioData() const;
};
FABLE_STATIC_ASSERT(sizeof(CWaveFile) == 0x18);
#endif
