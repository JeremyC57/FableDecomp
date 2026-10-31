// CBankFile::AddEntryMultipleFiles(symbol, type, filenames, stateBlock) -- add or refresh a bank entry built from several source
// Grade: DIFFER(741v741) -- class key CBankFile (the parked CBankFile_Retail name missed the PDB leaf); the first
//   branch's filename + InitEntry in its own block stops VC7.1 tail-merging the two InitEntry calls, so the length now
//   matches; residue: ebx/ebp swap (retail symbol in ebx, index in ebp) and the else-branch filename / resize zero
//   temporaries in swapped frame words
//   files: a known symbol must not already exist in this update pass (throws CBankCreatorException "Duplicate symbol: <symbol>");
//   its record is flagged for update when the type, filenames or state-block CRC changed; a new symbol takes the first free index
//   (growing RuntimeData / UpdateData / Symbols / Checksums when none is free) and a fresh update record; both paths finish
//   with InitEntry(index, symbol, first filename or "")
// Leaf: ?AddEntryMultipleFiles@CBankFile@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CE5E0, 741 bytes, ret 0x14; vtable 0x0129B54C slot 6; PDB
//   ?AddEntryMultipleFiles@CBankFile@@UAEXABVCCharString@@KABV?$CArray@VCCharString@@@@V?$CCountedPointer@VCBankStateBlock@@@@@Z.
// Callees: SymbolIndices.find 0x009D2DD0 (node pointer in eax), CCharString default/copy/(const char*) ctors 0x0099E4B0 /
//   0x0099EC30 / 0x0099EBF0, dtor 0x0099EAE0, operator= 0x0099EFB0, operator+(const char*, const CCharString&) 0x0099F690
//   (__fastcall), CBankCreatorException (8 bytes: EntrySymbol, ErrorText; copy 0x009D47E0, dtor 0x009D47C0, throw info
//   0x013693B4) via _CxxThrowException 0x00BFEB84, CBankFileEntryUpdateData::Equals 0x009CC750 / SetFilenames 0x009CC840,
//   CBankStateBlock slot 2 GetCRC, CArray resizes 0x009D4010 (CRuntimeData) / 0x009D3DF0 (update pointers) / 0x0049B760
//   (CCharString) / 0x00464931 (ulong), operator new 0x00BFEA1A, CBankFile::InitEntry 0x009CE050 (PDB AAEXKABVCCharString@@0@Z),
//   CPackedUIntArray::operator[] 0x00A629F0 (inline EntryExists), operator delete 0x00BFE9BC. "Duplicate symbol: " 0x0129B5AC.
// Layout: the landed CBankFile view (CBankFile::GetMaxEntryIndex 0x009CC350); SymbolIndices at retail +0xb8.
// Tried: iterator struct return (hidden pointer: worse), node-pointer find, index hoisted to function scope.
struct CCharString {
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const CCharString& other);             // 0x0099EC30
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
void operator delete(void* p);                         // 0x00BFE9BC
struct CBankCreatorException {                         // 8 bytes (Ego_r.pdb)
    CCharString EntrySymbol;
    CCharString ErrorText;
    CBankCreatorException(const CCharString& text) : ErrorText(text) {}
    CBankCreatorException(const CBankCreatorException& other);   // 0x009D47E0
    ~CBankCreatorException();                                    // 0x009D47C0
};
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
struct CBankStateBlock {
    virtual void v0(); virtual void v1();
    virtual unsigned long GetCRC();                    // slot 2 (+0x08)
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        CCPPointerInfo* info = Info;
        if (info && --info->RefCount == 0) {
            info->DeleteFunc(info->Data);
            delete info;
        }
    }
    T* operator->() const { return Data; }
};
template <class T> struct CArray {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    long GetCount() const { return m_pLast - m_pFirst; }
    bool IsEmpty() const { return m_pFirst == m_pLast; }
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
    void resize(unsigned long count, const T& value);
    void resize(unsigned long count);
};
struct CRuntimeData {
    unsigned long DataOffset;
    unsigned long DataSize;
    unsigned char DataType;
    bool          Valid;
    unsigned char _pad[2];
};
struct CPackedUIntArray {
    unsigned long  m_bitsPerValue;
    unsigned long* m_pData;
    unsigned long  m_count;
    unsigned long  m_capacity;
    unsigned long operator[](unsigned long index) const;   // 0x00A629F0
    bool IsPacked() const { return m_pData != 0; }
};
struct CSmallVectorWords { void* Start; signed char Size; unsigned char _pad[3]; CSmallVectorWords() : Start(0), Size(0) {} };
struct CBankFileEntryUpdateData {                      // 0x18 bytes
    unsigned long     StateBlockCRC;                   // +0x00
    unsigned long     InfoSize;                        // +0x04
    char*             InfoBuffer;                      // +0x08
    CSmallVectorWords Filenames;                       // +0x0c CSmallVector<CCharString,8>
    bool              RequiresUpdate;                  // +0x14
    bool              Exists;                          // +0x15
    CBankFileEntryUpdateData() : StateBlockCRC(0), InfoSize(0), InfoBuffer(0), RequiresUpdate(false), Exists(true) {}
    bool Equals(const CArray<CCharString>& filenames, CCountedPointer<CBankStateBlock> stateBlock) const;   // 0x009CC750
    void SetFilenames(const CArray<CCharString>& filenames);   // 0x009CC840
};
struct CSymbolIndexMap {                               // map<CCharString, unsigned long> (retail 12 bytes)
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; CCharString first; unsigned long second; };
    struct iterator { node* _M_node; };
    node*         head;
    unsigned long size;
    unsigned long compare;
    node* find(const CCharString& key);                // 0x009D2DD0 (the iterator, returned in eax)
};
struct CBankFile {
                                                    // +0x00 vptr
    unsigned long             Size;                 // +0x04
    CArray<CCharString>       Symbols;              // +0x08
    CArray<unsigned long>     Checksums;            // +0x14
    CArray<CRuntimeData>      RuntimeData;          // +0x20
    CArray<CBankFileEntryUpdateData*> UpdateData;   // +0x2c
    CPackedUIntArray          PackedDataOffset;     // +0x38
    CPackedUIntArray          PackedDataSize;       // +0x48
    CPackedUIntArray          PackedDataType;       // +0x58
    CPackedUIntArray          PackedValid;          // +0x68
    unsigned char             _pad78[0xb8 - 0x78];
    CSymbolIndexMap           SymbolIndices;        // +0xb8
    virtual void s0();
    bool EntryExists(unsigned long index) const      // 0x009CC190, inlined
    {
        if (index != 0 && index < Size) {
            if (PackedValid.IsPacked())
                return PackedValid[index - 1] != 0;
            return RuntimeData[index].Valid;
        }
        return false;
    }
    void InitEntry(unsigned long index, const CCharString& symbol, const CCharString& filename);   // 0x009CE050
    virtual void AddEntryMultipleFiles(const CCharString& symbol, unsigned long type, const CArray<CCharString>& filenames, CCountedPointer<CBankStateBlock> stateBlock);
};

void CBankFile::AddEntryMultipleFiles(const CCharString& symbol, unsigned long type, const CArray<CCharString>& filenames, CCountedPointer<CBankStateBlock> stateBlock)
{
    unsigned long index;
    CSymbolIndexMap::node* it = SymbolIndices.find(symbol);
    if (it != SymbolIndices.head) {
        index = it->second;
        CBankFileEntryUpdateData* update = UpdateData[index];
        if (update->Exists)
            throw CBankCreatorException("Duplicate symbol: " + symbol);
        update->Exists = true;
        CRuntimeData& data = RuntimeData[index];
        if (data.DataType != type || !update->Equals(filenames, stateBlock)) {
            data.DataType = (unsigned char)type;
            update->SetFilenames(filenames);
            update->StateBlockCRC = stateBlock->GetCRC();
            update->RequiresUpdate = true;
        }
        {
            CCharString filename(filenames.IsEmpty() ? CCharString("") : filenames[0]);
            InitEntry(index, symbol, filename);
        }
    } else {
        for (index = 1; index < Size; ++index)
            if (!EntryExists(index))
                break;
        if (index >= Size) {
            CRuntimeData blank;
            blank.DataOffset = 0;
            blank.DataSize = 0;
            blank.DataType = 0;
            blank.Valid = false;
            RuntimeData.resize(index + 1, blank);
            UpdateData.resize(index + 1, 0);
            Symbols.resize(index + 1);
            Checksums.resize(index + 1, 0);
            Size = RuntimeData.GetCount();
        }
        CRuntimeData& data = RuntimeData[index];
        CBankFileEntryUpdateData* update = new CBankFileEntryUpdateData;
        UpdateData[index] = update;
        Symbols[index] = symbol;
        data.Valid = true;
        data.DataType = (unsigned char)type;
        update->SetFilenames(filenames);
        update->StateBlockCRC = stateBlock->GetCRC();
        update->Exists = true;
        update->RequiresUpdate = true;
        CCharString filename(filenames.IsEmpty() ? CCharString("") : filenames[0]);
        InitEntry(index, symbol, filename);
    }
}
