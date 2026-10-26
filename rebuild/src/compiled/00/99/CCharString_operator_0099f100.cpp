// CCharString::operator+=(const char* text) -- append a C string: an existing block is made private (copy-on-write) and the text appended in place; an empty handle gets a new one-owner block holding the text (nothing for a null or empty text)
// Leaf: ??YCCharString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099F100, length 122 bytes (ret 4).
// Callees: CCharString::MakeOwnCopyOfData @ 0x0099EAF0 (landed), CBasicString<char>::AppendData(const char*,
// unsigned long) @ 0x009A04E0, operator new @ 0x00BFEA1A (0x11 bytes), CBasicString<char>::CBasicString(const char*)
// @ 0x009A0590.
// Layout (the landed CCharString block model): CBasicString<char> (13 bytes, packed) then NoRefs +0xd.
// Shape: the narrow twin of the landed CWideString::operator+=(const wchar_t*) (0x0099B940): the append path re-reads
// the block into a local after MakeOwnCopyOfData (loaded ahead of the scan) and open-codes strlen as the guarded
// do/while (`mov bl,[eax+edx+1] ; inc eax` with an aligned loop head); the empty path allocates the block with `new CCharStringData(text)` (NoRefs = 1 after the out-of-line base
// constructor) and publishes it, 0 when the allocation failed.
#pragma pack(push, 1)
struct CBasicStringChar {                     // CBasicString<char>, 13 bytes
    char*         PData;                      // +0x00
    unsigned long StringLength;               // +0x04
    unsigned long DataLength;                 // +0x08 (DataLength:31 + a flag bit)
    unsigned char UseFastExtend;              // +0x0c (bit 0)
    CBasicStringChar(const char* text);       // 0x009A0590
    void AppendData(const char* text, unsigned long length);   // 0x009A04E0
};
class CCharStringData { public:                      // 17 bytes
    CBasicStringChar Data;                    // +0x00
    long             NoRefs;                  // +0x0d
    CCharStringData(const char* text) : Data(text), NoRefs(1) { }
};
#pragma pack(pop)

class CCharString {
public:
    CCharStringData* PStringData;             // +0x00
    void MakeOwnCopyOfData();                 // 0x0099EAF0
    const CCharString& operator+=(const char* text);
};

const CCharString& CCharString::operator+=(const char* text)
{
    if (PStringData != 0) {
        MakeOwnCopyOfData();
        CCharStringData* data = PStringData;
        unsigned long length = 0;
        if (text[length] != 0) {
            do {
                ++length;
            } while (text[length] != 0);
        }
        data->Data.AppendData(text, length);
    } else if (text != 0 && *text != 0) {
        PStringData = new CCharStringData(text);
    }
    return *this;
}