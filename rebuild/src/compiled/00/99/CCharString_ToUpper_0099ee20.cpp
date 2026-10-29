// CCharString::ToUpper() const -- copy of the string with every ASCII (< 0x80) character passed through toupper (copy-on-write: a shared block is cloned first)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099EE20, length 278 bytes (hidden result pointer, `ret 4`).
// Callees: CCharString::UnassignString @ 0x0099E9B0 (landed; its body is repeated above under
//          `auto_inline(off)` so VC7.1 knows what it clobbers), operator new @ 0x00BFEA1A, CBasicString<char>
//          (const char*) @ 0x009A0590, toupper @ 0x00BFED16, operator delete[] @ 0x00BFEB1C, operator delete @ 0x00BFE9BC;
//          data: the CCharString instance counter @ 0x013BD800.
// Layout: CCharString is a handle on the 17-byte packed CCharStringData {CBasicString<char> (PData, StringLength,
// DataLength:31, UseFastExtend:1), NoRefs +0xd} (the landed MakeOwnCopyOfData 0x0099EAF0 model).
// Shape: `CCharString result(*this); result.MakeOwnCopyOfData(); <loop>; return result;` -- the copy
// constructor, MakeOwnCopyOfData and the destructor are inline (the destructor's release repeats
// UnassignString's body inline), the return copy is a second copy-construction into the hidden result
// (VC7.1 has no NRVO). With the callee body in the TU the handle stays in esi across UnassignString and the
// local's last stores are dropped. The loop condition `i < result.GetLength()` (inline null-tested getter)
// keeps retail's top-tested, non-rotated loop: a plain `for` over StringLength is rotated (`test ; jle` +
// bottom `jl`), even written with `for(;;) break` or a `goto`. The character is read through a named element pointer
// `char* p = PData + i` (tested and passed to toupper): that keeps retail's unfused `mov eax,[esi] ; add eax,edi ;
// mov al,[eax]`, while the store re-indexes PData (`mov edx,[esi] ; mov [edi+edx],al`).
extern "C" int __cdecl toupper(int c);                 // 0x00BFEE10
void __cdecl operator delete(void* block);             // 0x00BFE9BC (operator new 0x00BFEA1A is the implicit declaration)
void __cdecl operator delete[](void* block);           // 0x00BFEB1C

extern long g_CCharStringInstanceCount;                // 0x013BD800

#pragma pack(push, 1)
struct CBasicStringChar {                              // CBasicString<char>, 13 bytes
    char*         PData;                               // +0x00
    unsigned long StringLength;                        // +0x04
    unsigned long DataLength : 31;                     // +0x08
    bool          UseFastExtend : 1;                   // +0x0b..0x0c
    CBasicStringChar(const char* text);                // 0x009A0590
    ~CBasicStringChar()
    {
        if (PData) {
            delete[] PData;
            PData = 0;
        }
        StringLength = 0;
        DataLength = 0;
    }
};
class CCharStringData { public:                               // 17 bytes
    CBasicStringChar Data;                             // +0x00
    long             NoRefs;                           // +0x0d
    CCharStringData(const char* text) : Data(text), NoRefs(1) { }
};
#pragma pack(pop)

class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other) : PStringData(0)
    {
        ++g_CCharStringInstanceCount;
        if (&other != this) {
            if (PStringData)
                UnassignString();
            if (other.PStringData) {
                PStringData = other.PStringData;
                ++PStringData->NoRefs;
            }
        }
    }
    ~CCharString()
    {
        if (PStringData) {
            if (--PStringData->NoRefs <= 0)
                delete PStringData;
        }
        --g_CCharStringInstanceCount;
    }
    protected: void UnassignString(); public:                             // 0x0099E9B0
    void MakeOwnCopyOfData()
    {
        CCharStringData* data = PStringData;
        if (data != 0 && data->NoRefs > 1) {
            const char* text = data->Data.PData;
            CCharStringData* copy = new CCharStringData(text);
            UnassignString();
            PStringData = copy;
        }
    }
    long GetLength() const { return PStringData ? (long)PStringData->Data.StringLength : 0; }
    const char& GetChar(long i) const { return PStringData->Data.PData[i]; }
    CCharString ToUpper() const;
};

#pragma auto_inline(off)
void CCharString::UnassignString()
{
    CCharStringData* p = PStringData;
    if (p != 0) {
        if (--p->NoRefs <= 0) {
            CCharStringData* data = PStringData;
            if (data != 0)
                delete data;
        }
        PStringData = 0;
    }
}
#pragma auto_inline(on)

CCharString CCharString::ToUpper() const
{
    CCharString result(*this);
    result.MakeOwnCopyOfData();
    if (result.PStringData) {
        for (long i = 0; i < result.GetLength(); ++i) {
            char* p = result.PStringData->Data.PData + i;
            if (*(unsigned char*)p < 0x80)
                result.PStringData->Data.PData[i] = (char)toupper(*p);
        }
    }
    return result;
}