// CBasicString<char>::~CBasicString() -- frees the character buffer (array delete) and zeroes the string length and the 31-bit data length, keeping the UseFastExtend bit
// Leaf: ??1?$CBasicString@D@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A0190, 44 bytes (+ int3 padding). Callee: operator delete[] @ 0x00BFEB1C (MSVCR71 ??_V).
// Identity: the manifest label LHHttpDocumentParts::~LHHttpDocumentParts is propagated (that class has no +8 word; its own
// destructor is 0x0083EA10). The row has no callers (dead out-of-line copy) and sits among the CBasicString<char> members of
// one TU (Truncate 0x009A0080, ExtendData 0x009A01C0, SetString 0x009A0390); the same body is inlined in the CCharString
// release 0x0099E9B0 (CCharStringData = CBasicString<char> + owner count at +0x0D). Its byte twin 0x009A0430 is modelled
// as CCharStringData::~CCharStringData (the wrapper's destructor, whose only work is this member's), so each address keeps
// its own symbol; which of the two retail copies is which is not proven.
// Layout: Ego_r.pdb CBasicString<char>, packed 13 bytes (the landed Truncate 0x009A0080 model): +0 PData, +4 StringLength,
// +8 DataLength:31 (its storage word's top bit is preserved by the `and 0x80000000` read-modify-write), +0xC UseFastExtend:1.
void __cdecl operator delete[](void* block);       // 0x00BFEB1C

#pragma pack(push, 1)
template <class T> class CBasicString { public:
    T*            PData;                          // +0x00
    unsigned long StringLength;                   // +0x04
    unsigned long DataLength : 31;                // +0x08
    bool          UseFastExtend : 1;              // +0x0C
    ~CBasicString()
    {
        if (PData) {
            delete[] PData;
            PData = 0;
        }
        StringLength = 0;
        DataLength = 0;
    }
};
#pragma pack(pop)

template class CBasicString<char>;