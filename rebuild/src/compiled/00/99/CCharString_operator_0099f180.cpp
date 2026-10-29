// CCharString::operator+=(char) -- append one character (copy-on-write; an empty string becomes a 1-char string)
// Leaf: ??YCCharString@@QAEABV0@D@Z
// Retail: Fable.exe @ 0x0099F180, 104 bytes (+int3). PDB ??YCCharString@@QAEABV0@D@Z
// LABEL CORRECTION: the manifest files this as CWideString "operator+=(wchar_t)"; the body is CCharString's: it calls
// CCharString::MakeOwnCopyOfData 0x0099EAF0, allocates the 17-byte CCharStringData (0x11) and sets NoRefs at +0xD.
// Callees: CCharString::MakeOwnCopyOfData 0x0099EAF0, CBasicString<char>::Append(char, long count) 0x009A0460
// (label "String_FillRange"), operator new 0x00BFEA1A, CBasicString<char>::CBasicString(const char*) 0x009A0590.
// Model as in the landed 0x0099F040 operator=(char): 4-byte handle on a packed 17-byte block
// {PData +0, StringLength +4, DataLength:31 +8, UseFastExtend:1 +0xC, NoRefs +0xD}.
// Shape: with data, unshare and append the character once; without, `return *this = c;` through an inline copy of
// operator=(char) (its unassign folds away on the null arm); inlining puts the {c, 0} buffer in the dead argument
// slot, which a buffer written directly in this body does not get.
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    CBasicStringChar(const char* text);         // 0x009A0590
    void Append(char c, long count);            // 0x009A0460
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
    CCharStringData(const char* text) : Data(text), NoRefs(1) { }
};
#pragma pack(pop)

class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    void MakeOwnCopyOfData();                   // 0x0099EAF0
    protected: void UnassignString(); public:                      // 0x0099E9B0
    const CCharString& operator+=(char c);
    // inline copy of operator=(char) 0x0099F040 for the empty-string arm (the null test of the unassign folds away)
    __forceinline const CCharString& operator=(char c)
    {
        char text[2];
        text[0] = c;
        text[1] = '\0';
        if (PStringData != 0)
            UnassignString();
        if (c != '\0')
            PStringData = new CCharStringData(text);
        return *this;
    }
};

const CCharString& CCharString::operator+=(char c)
{
    if (PStringData != 0)
    {
        MakeOwnCopyOfData();
        PStringData->Data.Append(c, 1);
        return *this;
    }
    return *this = c;
}