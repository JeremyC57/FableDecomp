// CCharString::CCharString(CAFile&, long length) -- construct a string from a file: null block, bump the
// live-instance counter, (dead) release of the null block, then LoadFromFile(file, 0, length).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099FA50, length 52 bytes.
// Callees: CCharString::UnassignString @ 0x0099E9B0, CCharString::LoadFromFile @ 0x0099F2E0.
// Globals: g_CCharStringInstanceCount @ 0x013BD800. Same prologue shape as the landed (const char*, long)
// constructor @ 0x0099EBF0: the `if (PStringData) UnassignString();` is an inlined Unassign() helper
// that VC7.1 keeps even though the member was just zeroed.
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
class CAFile;
class CASuspendableProcess;
extern long g_CCharStringInstanceCount;         // 0x013BD800

class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    CCharString(CAFile& file, long length);
    void UnassignString();                      // 0x0099E9B0
    void LoadFromFile(CAFile& file, CASuspendableProcess* process, long length); // 0x0099F2E0
};

CCharString::CCharString(CAFile& file, long length)
    : PStringData(0)
{
    ++g_CCharStringInstanceCount;
    if (PStringData != 0)
        UnassignString();
    LoadFromFile(file, 0, length);
}