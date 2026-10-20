// CStreamingFontDataBank::InitStreamingEntry(CSymbolMap& symbols) -- look up the "GlyphData" entry of the font bank and keep its asynchronous bank entry for streaming the glyphs
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5EF50, 178 bytes (+ int3 padding), `ret 4` (PDB
//   `?InitStreamingEntry@CStreamingFontDataBank@@QAEXAAVCSymbolMap@@@Z`; the symbol map is unused in the release build).
// Leaf: InitStreamingEntry
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("GlyphData" @ 0x0129D5A4), ~CCharString @ 0x0099EAE0,
//   CBankFile::FindIndexBySymbol(const CCharString&) const @ 0x009CCDC0 (landed), CBankFileAsync::GetAsyncEntry(unsigned long)
//   @ 0x009D58D0 (PDB name; CCountedPointer<CBankFileAsyncEntry> by hidden pointer; landed as "CreateEntry"),
//   operator delete @ 0x00BFE9BC (count blocks).
// Shape: the "GlyphData" conversion temporary dies right after the lookup, so the hidden result of GetAsyncEntry reuses its
//   frame word. The counted-pointer assignment is inline: both source words are read into locals first (ebx/edi), the
//   member block is compared with the new one, the old block released (the member re-read after the decrement), the
//   words stored and the new block add-referenced; then the temporary's destructor releases its own reference (its
//   address escaped into the call, so the block is re-read after the decrement).
// Retail layout: the CBankFileAsync base is 0x164 bytes (engine/CStreamingFontDataBank.h carries the PDB +0x1D8..), so
//   streamingEntry sits at +0x188 (Data) / +0x18C (Info), 0x74 below the header.
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CCPPointerInfo {                                // CCountedPointer's shared block
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4
    void* Data;                                        // +8
};

template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { Release(); }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};

class CSymbolMap;
class CBankFileAsyncEntry;

class CStreamingFontDataBank {
public:
    virtual void v00();
    unsigned char                        _bank[0x184];  // CBankFileAsync part, StateBlock +0x164, Fonts +0x178, Filename +0x184
    CCountedPointer<CBankFileAsyncEntry> streamingEntry;   // +0x188
    unsigned long FindIndexBySymbol(const CCharString& symbol) const;               // 0x009CCDC0 (CBankFile)
    CCountedPointer<CBankFileAsyncEntry> GetAsyncEntry(unsigned long index);       // 0x009D58D0 (CBankFileAsync)
    void InitStreamingEntry(CSymbolMap& symbols);
};

void CStreamingFontDataBank::InitStreamingEntry(CSymbolMap& /*symbols*/)
{
    unsigned long index = FindIndexBySymbol("GlyphData");
    streamingEntry = GetAsyncEntry(index);
}
