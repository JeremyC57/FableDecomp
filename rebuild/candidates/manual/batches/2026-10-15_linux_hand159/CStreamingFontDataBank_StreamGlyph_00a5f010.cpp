// CStreamingFontDataBank::StreamGlyph(void* buffer, unsigned long offset, unsigned long size, bool synchronous) -- read glyph data from the font bank file: an asynchronous request (returned as a counted pointer to its data) when streaming is enabled and the caller allows it, else a blocking read and a null pointer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5F010, 118 bytes; `ret 14h` (hidden result + four arguments).
// Callees: GFGetSystemManager @ 0x009A4EC0 (__fastcall), the bank file's asynchronous read request @ 0x009D6060 (catalog
//          "CBankFileAsyncEntry_SetupReadRequest"; CCountedPointer by hidden pointer) and blocking read @ 0x009D52B0 (catalog
//          "CEditTransactionBase::ActionSetEngineTheme" is propagated).
// Layout: the bank file pointer at retail +0x188 of CStreamingFontDataBank; the system manager's streaming settings at +0x84
//         with the async-streaming flag at +0x38 (named descriptively).
// Shape: the asynchronous call passes the size twice and a priority of 4; the null result is CCountedPointer's inline ctor.
// Leaf: StreamGlyph
struct CCPPointerInfo;
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
struct CBankFileAsyncData;
struct CBankFile {
    CCountedPointer<CBankFileAsyncData> ReadAsync(unsigned long offset, void* buffer, unsigned long size,
                                                  unsigned long bufferSize, long priority);   // 0x009D6060
    void Read(unsigned long offset, void* buffer, unsigned long size);   // 0x009D52B0
};
struct CStreamingSettings {
    unsigned char _pad00[0x38];
    bool          AsyncStreaming;                  // +0x38
};
struct CSystemManager {
    unsigned char       _pad00[0x84];
    CStreamingSettings* Streaming;                 // +0x84
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
class CStreamingFontDataBank {
public:
    unsigned char _pad000[0x188];
    CBankFile*    BankFile;                        // +0x188
    CCountedPointer<CBankFileAsyncData> StreamGlyph(void* buffer, unsigned long offset, unsigned long size, bool synchronous);
};

CCountedPointer<CBankFileAsyncData> CStreamingFontDataBank::StreamGlyph(void* buffer, unsigned long offset, unsigned long size, bool synchronous)
{
    CSystemManager* system = GFGetSystemManager();
    if (!synchronous && system->Streaming->AsyncStreaming)
        return BankFile->ReadAsync(offset, buffer, size, size, 4);
    BankFile->Read(offset, buffer, size);
    return CCountedPointer<CBankFileAsyncData>();
}
