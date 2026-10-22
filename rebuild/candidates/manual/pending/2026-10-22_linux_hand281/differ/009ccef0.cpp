// Grade: DIFFER(620v592) -- register allocation: retail keeps `this` in ebp and the node index in edi (homed for push_back's
//   reference), tests the type against its argument slot and puts the filename temporary and the entry symbol in the dead
//   path-argument slot; ours caches type in ebp, `this` in edi and a zero in ebx (one more frame word)
// CBankFile::FindIndicesByFilename(path, type, excludeUnassigned) const -- every entry index whose lower-cased base filename
//   matches path's (FilenameIndices multimap equal range), optionally restricted to an entry type (-1 = any); with
//   excludeUnassigned, entries whose symbol has no '[' and does not start with "UNASSIGNED" are left out
// Leaf: ?FindIndicesByFilename@CBankFile@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CCEF0, 592 bytes, ret 0x10 (hidden CArray<unsigned long> return); PDB
//   ?FindIndicesByFilename@CBankFile@@QBE?AV?$CArray@K@@ABVCCharString@@J_N@Z.
// Callees: CBankFile::ConvertPathToFilename 0x009CBE10, CCharString::ToLower 0x0099ED10 / FindCharPos 0x0099E5A0 / copy
//   0x0099EC30 / (const char*) 0x0099EBF0 / dtor 0x0099EAE0, FilenameIndices (retail +0xc4) UpperBound 0x009D2B50 / LowerBound
//   0x0042AD34 (as the landed FindIndexByFilename 0x009CCDF0), inline GetEntryDataType (CPackedUIntArray::operator[] 0x00A629F0),
//   inline GetEntrySymbol 0x009CC1D0 ("(Unknown)" 0x0129A03C), strncmp 0x00BFEAF8 ("UNASSIGNED" 0x0129B538), vector<unsigned long>
//   _M_insert_overflow 0x00464ACE, malloc 0x00BFEA0E / memmove 0x00BFEAE6 / free 0x00BFEA14 (inline copy into the return value).
// Tried: StartsWith vs strncmp != 0 skip flag (fixed the bl polarity), named __true_type tag (no zero store), STLport-shaped
//   vector copy constructor.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);   // 0x00BFEAE6
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int n);       // 0x00BFEAF8
#pragma function(strncmp)
struct CCharStringData { const char* Text; long Length; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& other);             // 0x0099EC30
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString ToLower() const;                       // 0x0099ED10
    long FindCharPos(char c) const;                    // 0x0099E5A0
    const char* c_str() const { if (PStringData == 0) return ""; return PStringData->Text; }
    long Length() const { if (PStringData == 0) return 0; return PStringData->Length; }
    int CompareStart(const CCharString& prefix) const { return strncmp(c_str(), prefix.c_str(), prefix.Length()); }
};
struct __true_type {};
struct CULongVector {                                  // CArray<unsigned long> (STLport vector, 12 bytes)
    unsigned long* _M_start;
    unsigned long* _M_finish;
    unsigned long* _M_end_of_storage;
    CULongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CULongVector(const CULongVector& x) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        unsigned long n = x.size();
        _M_start = n != 0 ? (unsigned long*)malloc(n * sizeof(unsigned long)) : 0;
        _M_end_of_storage = _M_start + n;
        _M_finish = _M_start;
        _M_finish = CopyTrivial(x._M_start, x._M_finish, _M_start);
    }
    static unsigned long* CopyTrivial(const unsigned long* first, const unsigned long* last, unsigned long* result)
    {
        return last == first ? result : (unsigned long*)((char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first));
    }
    unsigned long size() const { return _M_finish - _M_start; }
    ~CULongVector() { if (_M_start) free(_M_start); }
    void _M_insert_overflow(unsigned long* pos, const unsigned long& x, const __true_type&, unsigned int fill, bool atend);   // 0x00464ACE
    void push_back(const unsigned long& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else
        {
            __true_type pod;
            _M_insert_overflow(_M_finish, x, pod, 1, true);
        }
    }
};
struct CFilenameNode {
    long Color; CFilenameNode* Parent; CFilenameNode* Left; CFilenameNode* Right;
    CCharString Key; unsigned long Value;
};
struct CFilenameMap {
    CFilenameNode* Head; unsigned long Count; unsigned char Compare;
    CFilenameNode* UpperBound(const CCharString&); //009D2B50
    CFilenameNode* LowerBound(const CCharString&); //0042AD34
};
inline CFilenameNode* NextFilenameNode(CFilenameNode* p) {
    if (p->Right) { p=p->Right; while(p->Left) p=p->Left; }
    else { CFilenameNode* parent=p->Parent; while(p==parent->Right) {p=parent;parent=parent->Parent;} if(p->Right!=parent) p=parent; }
    return p;
}
struct CRuntimeData { unsigned long DataOffset,DataSize; unsigned char DataType; bool Valid; unsigned char _pad[2]; };
template<class T> struct CArray {
    T* First; T* Last; T* Limit;
    bool IsEmpty() const { return First == Last; }
};
struct CPackedUIntArray {
    unsigned long  m_bitsPerValue;
    unsigned long* m_pData;
    unsigned long  m_count;
    unsigned long  m_capacity;
    unsigned long operator[](unsigned long index) const;   // 0x00A629F0
    bool IsPacked() const { return m_pData != 0; }
};
struct CBankFile_Retail {
    void* VTable; unsigned long Size;
    CArray<CCharString> Symbols; CArray<unsigned long> Checksums;
    CArray<CRuntimeData> RuntimeData; CArray<void*> UpdateData;
    CPackedUIntArray PackedDataOffset, PackedDataSize, PackedDataType, PackedValid;
    unsigned char _otherBankState[0xc4 - 0x78];
    CFilenameMap FilenameIndices;                      // +0xc4
    CCharString ConvertPathToFilename(const CCharString&) const; //009CBE10
    unsigned long GetEntryDataType(unsigned long index) const
    {
        if (PackedDataType.IsPacked())
            return PackedDataType[index - 1];
        return RuntimeData.First[index].DataType;
    }
    CCharString GetEntrySymbol(unsigned long index) const   // 0x009CC1D0 (landed), inlined
    {
        if (Symbols.IsEmpty())
            return CCharString("(Unknown)", -1);
        return CCharString(*(Symbols.First + index));
    }
    CULongVector FindIndicesByFilename(const CCharString& path, long type, bool excludeUnassigned) const;
};

CULongVector CBankFile_Retail::FindIndicesByFilename(const CCharString& path, long type, bool excludeUnassigned) const
{
    CULongVector indices;
    CCharString name = ConvertPathToFilename(path).ToLower();
    CFilenameNode* end = const_cast<CFilenameMap&>(FilenameIndices).UpperBound(name);
    CFilenameNode* p = const_cast<CFilenameMap&>(FilenameIndices).LowerBound(name);
    for (; p != end; p = NextFilenameNode(p)) {
        unsigned long index = p->Value;
        if (type == -1 || GetEntryDataType(index) == (unsigned long)type) {
            CCharString symbol = GetEntrySymbol(index);
            bool skip = excludeUnassigned && symbol.FindCharPos('[') == -1 && symbol.CompareStart("UNASSIGNED") != 0;
            if (!skip)
                indices.push_back(index);
        }
    }
    return indices;
}
