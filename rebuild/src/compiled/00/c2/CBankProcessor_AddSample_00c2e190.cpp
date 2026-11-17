// CBankProcessor::AddSample(const SSampleHeader* header) -- build the audio format of one bank sample record (wave format tag 1 PCM -> codec 0, 2 ADPCM -> 2, 0x69 Xbox ADPCM -> 1; any other tag logs "Unknown sample format for index <n>" and skips it), convert its loop points from samples to bytes, create the sample through the manager and register it under its index
// Leaf: AddSample
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2E190, length 224 bytes (219 + int3 padding), __fastcall, `ret 4`.
// Caller: the landed 0x00C2E270 (CChildListLoader::AddChildren: one call per item of the bank's sample list, stopping when
//   the log at +8 reports a failure); its owner is the bank processor built at 0x00C2E0C0 and run by 0x00C2EB00.
// Callees: the log's AddMessage @ 0x00C307E0 (a new 12-byte message node registered through the log's slot 9; returns its
//   stream), the libpk stream's `__fastcall operator<<` (text @ 0x00C0DEC0, unsigned @ 0x00C0DF10), CAudioFormat::SamplesToBytes
//   @ 0x00C2D820 (twice), the manager's CScriptHost::CreateSample(format, offset) @ 0x00C0E700 (`new` of a 0x24-byte sample, ctor 0x00C1D880),
//   the sample map's CSampleIndexMap::Add(log, sample, index) @ 0x00C3B0E0 (logs a duplicate index, else stores an 8-byte node).
// Name: libpk, no PDB; descriptive (the catalog's `C3DAnimationSequence_ProcessSampleFormatChunk` is propagated). The
//   tags are the WAVEFORMATEX ones (WAVE_FORMAT_PCM 1, WAVE_FORMAT_ADPCM 2, WAVE_FORMAT_XBOX_ADPCM 0x69).
// Shape: the format's inline constructor stores the codec word BEFORE the channel word (`mov [esp+1Ah],bx ; mov
//   [esp+18h],cx`), so it assigns in that order in its body (an init list follows declaration order and swaps them); the
//   switch lowers to `dec ; je ; dec ; je ; sub 67h ; je` with the logged default inline and its own epilogue; the loop
//   start clamps branch-free (`setl ; dec ; and`), the end (+1) through ebx; the created sample is a named local, so the
//   index is read after CreateSample returns (an inline call argument loads it first).
// NF65 (round 7): ported onto libpk/CBankProcessor.h (BANK_LOAD_WRAPPER: CBankProcessor, CSampleIndexMap::Add via
//   FABLE_RETAIL_CSAMPLEINDEXMAP_ADD; overlay opt-ins FABLE_RETAIL_SSAMPLEHEADER_FORMAT (the record's format fields) and
//   FABLE_RETAIL_CBANKPROCESSOR_ADDSAMPLE (the member, m_pManager typed CScriptHost*)), libpk/CScriptHost.h (CLoadLog::AddMessage; overlay
//   opt-in FABLE_RETAIL_CSCRIPTHOST_CREATESAMPLE) and engine/CString.h; the local CString / CLoadLog / SSampleHeader / CSampleManager /
//   CSampleMap / CBankProcessor models are gone. CAudioFormat has no shared header and stays local. The sample map call now binds
//   ?Add@CSampleIndexMap@@QAEXPAVCLoadLog@@PAXK@Z (was ?Add@CSampleMap@@QAEXPAUCLoadLog@@PAXK@Z), the manager call the landed self spelling
//   ?CreateSample@CScriptHost@@QAEPAUCSample@@PBUCAudioFormat@@K@Z (was ?CreateSample@CSampleManager@@...).
#define FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#define FABLE_RETAIL_CSAMPLEINDEXMAP_ADD
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_SSAMPLEHEADER_FORMAT
#define FABLE_RETAIL_CBANKPROCESSOR_ADDSAMPLE
#define FABLE_RETAIL_CSCRIPTHOST_CREATESAMPLE
struct CAudioFormat;
#include "libpk/CBankProcessor.h"

struct CAudioFormat {                        // 20 bytes (the CAudioFormat of 0x00C2D790..0x00C2D9A0)
    unsigned long  m_length;                // +0x00 data length in bytes
    unsigned long  m_loopStart;             // +0x04 loop start in bytes
    unsigned long  m_loopEnd;               // +0x08 loop end in bytes
    unsigned short m_channels;              // +0x0C
    unsigned short m_codec;                 // +0x0E 0 = PCM, 1 = Xbox ADPCM, 2 = ADPCM
    unsigned long  m_sampleRate;            // +0x10
    CAudioFormat(unsigned long length, unsigned short channels, unsigned long sampleRate)
    {
        m_length = length;
        m_loopStart = 0;
        m_loopEnd = 0;
        m_codec = 0;
        m_channels = channels;
        m_sampleRate = sampleRate;
    }
    unsigned long SamplesToBytes(unsigned long samples);   // 0x00C2D820
};

void CBankProcessor::AddSample(const SSampleHeader* header)
{
    CAudioFormat format(header->m_length, header->m_channels, header->m_sampleRate);
    switch (header->m_formatTag) {
    case 1:    format.m_codec = 0; break;
    case 2:    format.m_codec = 2; break;
    case 0x69: format.m_codec = 1; break;
    default:
        m_pLog->AddMessage() << "Unknown sample format for index " << header->m_number;
        return;
    }
    unsigned long loopStart = header->m_loopStart < 0 ? 0 : header->m_loopStart;
    unsigned long loopEnd = header->m_loopEnd < 0 ? 0 : header->m_loopEnd + 1;
    format.m_loopStart = format.SamplesToBytes(loopStart);
    format.m_loopEnd = format.SamplesToBytes(loopEnd);
    CSample* sample = m_pManager->CreateSample(&format, header->m_dataOffset);
    m_samples.Add(m_pLog, sample, header->m_number);
}