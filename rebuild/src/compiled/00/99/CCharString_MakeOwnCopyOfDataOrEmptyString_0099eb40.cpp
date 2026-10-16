// CCharString::MakeOwnCopyOfDataOrEmptyString() -- copy-on-write for a shared block (clone the text, drop the old reference); an unassigned string gets a fresh empty block with a 0x20-byte growth reserve
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099EB40, 176 bytes incl. padding (plain `ret`).
// Callees: operator new @ 0x00BFEA1A (17-byte block, null-checked), CBasicString<char>::CBasicString(const char*) @ 0x009A0590,
//          CBasicString<char>::Initialise(long length, unsigned long growth) @ 0x009A0390 (as in the CCharString::LoadFromFile
//          oracle 0x0099F2E0), CCharString::UnassignString @ 0x0099E9B0.
// Layout: the landed CCharString block model (Ego_r.pdb): CBasicString<char> {PData +0, StringLength +4, DataLength:31 +8
//          (bit 31 untouched), UseFastExtend:1 +0xc} then NoRefs +0xd.
// Shape: the shared-block arm is the landed MakeOwnCopyOfData 0x0099EAF0 body; the empty arm inlines the block's
//          (length, growth) constructor (`__forceinline`: zero words, bitfield read/modify/write, then Initialise(0, 0x20)),
//          NoRefs = 1 after it; both arms publish the (possibly null) block last.
#pragma pack(push, 1)
struct CBasicStringChar {                     // CBasicString<char>, 13 bytes
    char*         PData;                      // +0x00
    unsigned long StringLength;               // +0x04
    unsigned long DataLength    : 31;         // +0x08 bits 0..30
    unsigned long m_reserved    : 1;          // +0x08 bit 31 (not initialised here)
    unsigned char UseFastExtend : 1;          // +0x0c bit 0
    CBasicStringChar(const char* text);       // 0x009A0590
    __forceinline CBasicStringChar(long length, unsigned long growth)
        : PData(0), StringLength(0), DataLength(0), UseFastExtend(1)
    {
        Initialise(length, growth);
    }
    void Initialise(long length, unsigned long growth);   // 0x009A0390
};
struct CCharStringData {                      // 17 bytes
    CBasicStringChar Data;                    // +0x00
    long             NoRefs;                  // +0x0d
    CCharStringData(const char* text) : Data(text), NoRefs(1) { }
    __forceinline CCharStringData(long length, unsigned long growth) : Data(length, growth), NoRefs(1) { }
};
#pragma pack(pop)

class CCharString {
public:
    CCharStringData* PStringData;             // +0x00
    void UnassignString();                    // 0x0099E9B0
    void MakeOwnCopyOfDataOrEmptyString();
};

void CCharString::MakeOwnCopyOfDataOrEmptyString()
{
    CCharStringData* data = PStringData;
    if (data != 0) {
        if (data->NoRefs > 1) {
            const char* text = data->Data.PData;
            CCharStringData* copy = new CCharStringData(text);
            UnassignString();
            PStringData = copy;
        }
    } else {
        PStringData = new CCharStringData(0, 0x20);
    }
}