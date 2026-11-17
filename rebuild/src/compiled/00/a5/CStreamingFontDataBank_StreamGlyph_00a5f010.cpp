// CStreamingFontDataBank::StreamGlyph(void* buffer, unsigned long offset, unsigned long size, bool synchronous) -- read glyph data from the font bank file: an asynchronous request (returned as a counted pointer to its data) when streaming is enabled and the caller allows it, else a blocking read and a null pointer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5F010, 118 bytes; `ret 14h` (hidden result + four arguments).
// Callees: GFGetSystemManager @ 0x009A4EC0 (__fastcall), the streaming entry's asynchronous read request @ 0x009D6060 (landed as
//          the descriptive ?BeginReadData@CBankFileAsyncEntry@@..; CCountedPointer by hidden pointer) and its blocking read
//          CBankFileAsyncEntry::ReadDataNonAsync @ 0x009D52B0 (Ego_r.pdb + FableWin.pdb ?ReadDataNonAsync@CBankFileAsyncEntry@@QAEXKPAXK@Z).
// Layout: CStreamingFontDataBank::streamingEntry (CCountedPointer<CBankFileAsyncEntry>, Ego_r.pdb) at retail +0x188 (engine/
//         CStreamingFontDataBank.h, flattened: streamingEntry_Data); the system manager's streaming settings at +0x84 with the
//         async-streaming flag at +0x38 (named descriptively).
// NF79 (round 7, q10958): the object at +0x188 is the streaming entry, a CBankFileAsyncEntry (no shared header; local declaration
//         with the two members called), not a CBankFile: 0x009D52B0 binds ReadDataNonAsync (was ?Read@CBankFile@@QAEXKPAXK@Z) and
//         0x009D6060 its landed spelling (was ?ReadAsync@CBankFile@@); CStreamingFontDataBank is the shared header's.
// Shape: the asynchronous call passes the size twice and a priority of 4; the null result is CCountedPointer's inline ctor.
// Leaf: ?StreamGlyph@CStreamingFontDataBank@@QAE?AV?$CCountedPointer@VCBankFileAsyncData@@@@PAXKK_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
#define FABLE_RETAIL_CSTREAMINGFONTDATABANK
#include "engine/CCountedPointer.h"
#include "engine/EThreadedFilePriority.h"
#include "engine/CStreamingFontDataBank.h"
class CBankFileAsyncData;
class CBankFileAsyncEntry { public:                // no shared header (Ego_r.pdb: vptr, BankFile +4, Index +8, PaddedSize +0xC)
    CCountedPointer<CBankFileAsyncData> BeginReadData(unsigned long offset, void* buffer, unsigned long size,
                                                      unsigned long bufferSize, EThreadedFilePriority priority);   // 0x009D6060
    void ReadDataNonAsync(unsigned long offset, void* buffer, unsigned long size);   // 0x009D52B0
};
struct CStreamingSettings {
    unsigned char _pad00[0x38];
    bool          AsyncStreaming;                  // +0x38
};
class CSystemManager { public:
    unsigned char       _pad00[0x84];
    CStreamingSettings* Streaming;                 // +0x84
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0

CCountedPointer<CBankFileAsyncData> CStreamingFontDataBank::StreamGlyph(void* buffer, unsigned long offset, unsigned long size, bool synchronous)
{
    CSystemManager* system = GFGetSystemManager();
    if (!synchronous && system->Streaming->AsyncStreaming)
        return streamingEntry_Data->BeginReadData(offset, buffer, size, size, THREADED_FILE_PRIORITY_HIGH);
    streamingEntry_Data->ReadDataNonAsync(offset, buffer, size);
    return CCountedPointer<CBankFileAsyncData>();
}