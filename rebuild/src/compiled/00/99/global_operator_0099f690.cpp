// operator+(const char* left, const CCharString& right) -- concatenate a C string and a CCharString into a new CCharString
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099F690, 160 bytes. FableWin.pdb ??H@YI?AVCCharString@@PBDABV0@@Z: a free
// `__fastcall` returning CCharString by value (hidden result pointer in ecx, left in edx, right on the stack, `ret 4`).
// Callees: operator new @ 0x00BFEA1A (0x11-byte CCharStringData), CBasicString<char>::CBasicString(const char*) @
// 0x009A0590, CCharString::operator+=(const CCharString&) @ 0x0099F0A0, CCharString::UnassignString() @ 0x0099E9B0
// (inside the inlined copy and the local's destructor).
// Globals: g_CCharStringInstanceCount @ 0x013BD800 (+1 per constructor, -1 per destructor).
// Shape: `CCharString result(left); result += right; return result;` -- the (const char*, long = -1) constructor is
// inlined with its AllocStringData (the landed 0x0099EA60 body; length -1 keeps only the `new CCharStringData(text)`
// arm: operator new, null test, CBasicString(text), NoRefs = 1); no NRVO, so the return copies through the inline
// copy constructor (zero, count++, self test, out-of-line UnassignString, share + NoRefs++ at +0xD) and the local's
// inline destructor calls UnassignString and decrements the count.
// Layout (landed CCharString model): CCharStringData = packed CBasicString<char> (13 bytes) + NoRefs +0xD.
// Leaf: ??H@YI?AVCCharString
extern long g_CCharStringInstanceCount;                 // 0x013BD800

#pragma pack(push, 1)
struct CBasicStringChar {                               // CBasicString<char>, 13 bytes
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength    : 31;                   // +0x08
    unsigned long m_reserved    : 1;                    // +0x08 bit 31
    unsigned char UseFastExtend : 1;                    // +0x0c bit 0
    CBasicStringChar(const char* text);                 // 0x009A0590
    CBasicStringChar(const char* text, unsigned long length);   // 0x009A05E0
};
class CCharStringData { public:                                // 17 bytes
    CBasicStringChar Data;                              // +0x00
    long             NoRefs;                            // +0x0d
    CCharStringData(const char* text) : Data(text), NoRefs(1) { }
    CCharStringData(const char* text, unsigned long length) : Data(text, length), NoRefs(1) { }
};
#pragma pack(pop)

class CCharString
{
public:
    CCharStringData* PStringData;                       // +0x00
    CCharString(const char* text, long length = -1) : PStringData(0)
    {
        ++g_CCharStringInstanceCount;
        if (text != 0 && *text != 0)
            PStringData = AllocStringData(text, length);
    }
    CCharString(const CCharString& other) : PStringData(0)
    {
        ++g_CCharStringInstanceCount;
        if (&other != this) {
            if (PStringData != 0)
                UnassignString();
            CCharStringData* data = other.PStringData;
            if (data != 0) {
                PStringData = data;
                ++data->NoRefs;
            }
        }
    }
    ~CCharString() { UnassignString(); --g_CCharStringInstanceCount; }
    CCharStringData* AllocStringData(const char* text, long length)   // 0x0099EA60 (inlined here)
    {
        if (length == -1)
            return new CCharStringData(text);
        return new CCharStringData(text, length);
    }
    void UnassignString();                              // 0x0099E9B0
    const CCharString& operator+=(const CCharString& other);   // 0x0099F0A0
};

CCharString __fastcall operator+(const char* left, const CCharString& right)
{
    CCharString result(left);
    result += right;
    return result;
}