// CBankFile::GetEntrySymbol(unsigned long) const -- a copy of the entry's symbol string, or an empty string when the bank has no symbols
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CC1D0, oracle length 57 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCharString::CCharString(const CCharString&) @ 0x0099EC30.
//
// Retail bytes:  mov eax,[ecx+8] ; cmp eax,[ecx+0Ch] ; push esi ; mov esi,[esp+8] ; jne S ; push -1 ; push offset "(Unknown)" ; mov ecx,esi ;
//                call CCharString(const char*, long) ; mov eax,esi ; pop esi ; ret 8 ; S: mov ecx,[ecx+8] ; mov edx,[esp+0Ch] ; lea eax,[ecx+edx*4] ;
//                push eax ; mov ecx,esi ; call CCharString(const CCharString&) ; mov eax,esi ; pop esi ; ret 8
// The string is returned through the hidden result pointer (a class with a user copy constructor),
// `ret 8` popping it and the index.  An empty Symbols array (first == last) yields the literal
// "(Unknown)" with the -1 length (the explicit two-argument constructor), otherwise element `index`
// of the 4-byte-element array is copy-constructed into the result.  The first pointer is re-read
// for the element (retail does not CSE it with the emptiness test), the mark of an element reached
// through the array's iterator object (`*(begin() + index)`) rather than a raw subscript.
struct CCharStringData;

struct CCharString {                    // 4 bytes: the shared string data
    CCharStringData* m_pStringData;
    CCharString(const char* text, long length);   // 0x0099EBF0
    CCharString(const CCharString& other);        // 0x0099EC30
};

// Retail CBankFile layout (see the landed CBankFile::Align @ 0x009CBB90): in the shipped binary every
// CArray / std::vector / map is 4 bytes smaller than in Ego_r.pdb (engine/CBankFile.h), so from +0x38
// on each member sits 0x10 lower than the PDB offset.  The PDB member names are kept at their retail
// offsets; the header itself cannot be included because its offsets disagree with the bytes.
struct CDiskFileCalls;
struct CBankFileEntryUpdateData;

struct CRuntimeData {                   // CBankFile::CRuntimeData, 12 bytes, RuntimeData[0] is unused
    unsigned long DataOffset;           // +0x00
    unsigned long DataSize;             // +0x04
    unsigned char DataType;             // +0x08
    bool          Valid;                // +0x09
    unsigned char _pad[2];
};

struct CPackedUIntArray {               // 16 bytes; m_pData == 0 means "not packed, use RuntimeData"
    unsigned long  m_bitsPerValue;      // +0x00
    unsigned long* m_pData;             // +0x04
    unsigned long  m_count;             // +0x08
    unsigned long  m_capacity;          // +0x0c
    unsigned long operator[](unsigned long index) const;   // 0x00A629F0
    bool IsPacked() const { return m_pData != 0; }
};

template <class T> struct CArray {      // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    struct iterator {                   // the real iterator object: an element reached through it is not CSE'd with the raw loads
        T* m_p;
        iterator(T* p) : m_p(p) {}
        iterator operator+(unsigned long n) const { return iterator(m_p + n); }
        T& operator*() const { return *m_p; }
    };
    bool IsEmpty() const { return m_pFirst == m_pLast; }
    iterator begin() const { return iterator(m_pFirst); }
    long GetCount() const { return m_pLast - m_pFirst; }
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};

struct CBankFile_Retail {        // the retail layout; engine/CBankFile.h (PDB) disagrees with the bytes
    void*                     __vftable;            // +0x00
    unsigned long             Size;                 // +0x04 entry count (index 0 is reserved)
    CArray<CCharString>       Symbols;              // +0x08
    unsigned char             Checksums[0x0c];      // +0x14 CArray<unsigned long>
    CArray<CRuntimeData>      RuntimeData;          // +0x20 1-based: entry i is RuntimeData[i]
    CArray<CBankFileEntryUpdateData*> UpdateData;   // +0x2c 0-based
    CPackedUIntArray          PackedDataOffset;     // +0x38 0-based: entry i is [i-1]
    CPackedUIntArray          PackedDataSize;       // +0x48
    CPackedUIntArray          PackedDataType;       // +0x58
    CPackedUIntArray          PackedValid;          // +0x68
    unsigned long             OpenFlags;            // +0x78
    CDiskFileCalls*           BankFile_Data;        // +0x7c CCountedPointer<CDiskFileWin32>::Data
    void*                     BankFile_Info;        // +0x80
    void*                     Filename_PStringData; // +0x84 CWideString
    void*                     BankHandle_PStringData; // +0x88 CCharString
    bool                      RetailMode;           // +0x8c
    bool                      ReadOnly;             // +0x8d
    bool                      WrittenToFile;        // +0x8e
    bool                      Solid;                // +0x8f
    CCharString GetEntrySymbol(unsigned long index) const;   // 0x009CC1D0
};

CCharString CBankFile_Retail::GetEntrySymbol(unsigned long index) const
{
    if (Symbols.IsEmpty())
        return CCharString("(Unknown)", -1);
    return CCharString(*(Symbols.begin() + index));
}
