// CCharString::SaveToFile(CAFile&) -- write the raw characters (strlen bytes, no terminator) to the file
// through CAFile's virtual Write(const void*, long, long) (vtable slot 4), third argument 0.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099E5F0, length 42 bytes.
// Callees: CAFile vtable slot 4 (+0x10). strlen is the VC7.1 intrinsic (lea esi,[eax+1] ; loop).
// An unassigned string writes nothing.
#include <string.h>
// CCharString model (Ego_r.pdb layout, byte-exact with retail): a 4-byte handle on a 17-byte shared
// block. CBasicString<char> is {PData +0, StringLength +4, DataLength:31 +8, UseFastExtend:1 +0xc}
// (13 bytes, packed) and CCharStringData appends the reference count NoRefs at +0xd.
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08 (the top bit is never touched)
    bool          UseFastExtend : 1;            // +0x0c
    CBasicStringChar(const char* text);         // 0x009A0590 (manifest: CCharString_InitFromCStr)
    void Truncate(unsigned long length);        // 0x009A0080
    void Insert(long position, const char* text); // 0x009A06B0
    bool operator==(const char* text) const;    // 0x004115A0
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
    CCharStringData(const char* text) : Data(text), NoRefs(1) { }
};
#pragma pack(pop)
class CAFile { public:
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2();
    virtual void Slot3();
    virtual void Write(const void* data, long size, long flags);   // slot 4 (+0x10)
};

class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    void SaveToFile(CAFile& file) const;
};

void CCharString::SaveToFile(CAFile& file) const
{
    CCharStringData* data = PStringData;
    if (data != 0) {
        const char* text = data->Data.PData;
        file.Write(text, strlen(text), 0);
    }
}