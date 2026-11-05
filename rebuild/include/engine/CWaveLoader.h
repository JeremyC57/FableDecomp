#ifndef FABLE_ENGINE_CWAVELOADER_H
#define FABLE_ENGINE_CWAVELOADER_H
// Retail layout (chunk 779, hand-written): Ego_r.pdb @STRUCT CWaveLoader, 1 byte, no members (`this` unused in both landed
// methods). Member set: Ego_r.pdb ??0CWaveLoader@@QAE@XZ, ?LoadWaveFile@CWaveLoader@@QAE?AV?$CCountedPointer@VCWaveFile@@@@AAVCDataInputStream@@@Z
// (retail 0x00AAD650) and ?SaveWaveFile@CWaveLoader@@QAEXABVCWaveFile@@AAVCDataOutputStream@@@Z (retail 0x00AAD150).
// Nested CWaveLoader::CWaveFormatHeader (FableWin.pdb ??0CWaveFormatHeader@CWaveLoader@@QAE@XZ; Ego_r.pdb layout 16 bytes:
// FormatTag +0, Channels +2, SamplesPerSec +4, AvgBytesPerSec +8, BlockAlign +0xc, BitsPerSample +0xe) is the RIFF "fmt "
// chunk body; its constructor zeroes FormatTag only (0x00AAD73C `mov [esp+58h],di`; Channels is read back uninitialised at
// 0x00AAD74C, so no other member is stored).
#include "engine/CCountedPointer.h"
#include "engine/CWaveFile.h"
class CDataInputStream;
class CDataOutputStream;
class CWaveLoader {
public:
    class CWaveFormatHeader {
    public:
        unsigned short FormatTag;           // +0x00
        unsigned short Channels;            // +0x02
        unsigned long SamplesPerSec;        // +0x04
        unsigned long AvgBytesPerSec;       // +0x08
        unsigned short BlockAlign;          // +0x0c
        unsigned short BitsPerSample;       // +0x0e
        CWaveFormatHeader() : FormatTag(0) { }
    };
    CWaveLoader() { }
    CCountedPointer<CWaveFile> LoadWaveFile(CDataInputStream& stream);     // 0x00AAD650
    void SaveWaveFile(const CWaveFile& wave, CDataOutputStream& output);   // 0x00AAD150
};
FABLE_STATIC_ASSERT(sizeof(CWaveLoader::CWaveFormatHeader) == 0x10);
#endif
