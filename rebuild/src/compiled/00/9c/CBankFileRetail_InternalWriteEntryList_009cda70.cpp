// CBankFile::InternalWriteEntryList(CDataOutputStream& stream) -- write the entry list: count the written entries per data type (and the furthest data end), write the per-type counts, then one record per written entry (index, type, size, offset, state CRC, symbol, checksum, filenames, info bytes); record the information size and file size, return the entry count
// Leaf: InternalWriteEntryList
// Retail 0x009CDA70, 1248 bytes, `ret 4`.
// Callees: CDataOutputStream::WriteULong 0x00993EA0, WritePresizedString 0x00995A80, the stream's virtual slots 2 (+0x08,
//   GetPosition) and 4 (+0x10, Write(data, size)); CPackedUIntArray::operator[] 0x00A629F0; map<unsigned long, unsigned
//   long> insert_unique 0x00431363 / operator[] 0x00553E60 / _M_erase 0x0042FE07; malloc 0x00BFEA0E, free 0x00BFEA14;
//   CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0 ("(Unknown)" 0x0129A03C).
// Layout: the landed retail CBankFile model (0x009D1320 / 0x009CCCD0): Symbols +0x08, Checksums +0x14, RuntimeData +0x20,
//   UpdateData +0x2c, PackedDataOffset +0x38, PackedDataSize +0x48, PackedDataType +0x58; FileInformationSize +0xa4 and
//   FileSize +0xa8 (Ego_r.pdb +0xb4 / +0xb8 less the four 4-byte-short CArrays); CBankFileEntryUpdateData as 0x009D1320.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memcpy, memset)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
struct CDataOutputStream {
    virtual void v00();
    virtual void v01();
    virtual unsigned long GetPosition();               // slot 2 (+0x08)
    virtual void v03();
    virtual void Write(const void* data, long size);   // slot 4 (+0x10)
    void WriteULong(unsigned long value);              // 0x00993EA0
    void WritePresizedString(const CCharString& s);    // 0x00995A80
};
struct CByteVector {                                   // CArray<char> (STLport vector, 12 bytes)
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    __forceinline CByteVector(unsigned long n, char value) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n != 0 ? (char*)malloc(n) : 0;
        _M_end_of_storage = _M_start + n;
        char* first = _M_start;
        memset(first, value, n);
        _M_finish = first + n;
    }
    ~CByteVector() { if (_M_start) free(_M_start); }
    long GetCount() const { return _M_finish - _M_start; }
    char* GetData() { return _M_finish - _M_start != 0 ? _M_start : (char*)this; }
};
struct CBankFileEntryUpdateData {
    unsigned long StateBlockCRC;                       // +0x00
    unsigned long InfoSize;                            // +0x04
    char*         InfoBuffer;                          // +0x08
    CCharString*  Filenames_Start;                     // +0x0c CSmallVector<CCharString,8>
    signed char   Filenames_Size;                      // +0x10
    unsigned char _pad11[3];
    bool          RequiresUpdate;                      // +0x14
    bool          Exists;                              // +0x15
    void ReadInfo(unsigned long start, void* data, unsigned long size) const
    {
        __int64 end = (__int64)(start + size);
        if (end >= 0 && end <= 0x7fffffff)
            memcpy(data, InfoBuffer + start, size);
    }
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
template <class T> struct CArray {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T* begin() const { return m_pFirst; }
    T* end() const { return m_pLast; }
    unsigned long size() const { return end() - begin(); }
    bool empty() const { return m_pFirst == m_pLast; }
    T& operator[](unsigned long index) const { return *(begin() + index); }
};
struct SULongPair { unsigned long first; unsigned long second; };
struct SULongNode {
    unsigned char color;
    SULongNode*   parent;
    SULongNode*   left;
    SULongNode*   right;
    SULongPair    value;
};
struct SULongMapIterator { SULongNode* node; };
struct SInsertResult { SULongMapIterator it; bool inserted; };
struct CULongMap {                                     // STLport map<unsigned long, unsigned long>
    SULongNode*   _M_header;
    unsigned long _M_node_count;
    bool          _M_key_compare;
    CULongMap() : _M_header(0)
    {
        _M_header = (SULongNode*)malloc(sizeof(SULongNode));
        _M_node_count = 0;
        _M_header->color = 0;
        _M_header->parent = 0;
        _M_header->left = _M_header;
        _M_header->right = _M_header;
    }
    void _M_erase_out(SULongNode* x);                  // 0x0042FE07
    void _M_erase(SULongNode* x)
    {
        while (x != 0) {
            _M_erase_out(x->right);
            SULongNode* y = x->left;
            free(x);
            x = y;
        }
    }
    ~CULongMap()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->parent);
            _M_header->left = _M_header;
            _M_header->parent = 0;
            _M_header->right = _M_header;
            _M_node_count = 0;
        }
        if (_M_header)
            free(_M_header);
    }
    SInsertResult insert(const SULongPair& value);     // 0x00431363
    unsigned long& operator[](const unsigned long& key);   // 0x00553E60
    static SULongNode* _M_increment(SULongNode* x)
    {
        if (x->right != 0) {
            x = x->right;
            while (x->left != 0)
                x = x->left;
        } else {
            SULongNode* y = x->parent;
            while (x == y->right) {
                x = y;
                y = y->parent;
            }
            if (x->right != y)
                x = y;
        }
        return x;
    }
};
template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }
struct CBankFile_Retail {
    unsigned long             Size;                 // +0x04
    CArray<CCharString>       Symbols;              // +0x08
    CArray<unsigned long>     Checksums;            // +0x14
    CArray<CRuntimeData>      RuntimeData;          // +0x20
    CArray<CBankFileEntryUpdateData*> UpdateData;   // +0x2c
    CPackedUIntArray          PackedDataOffset;     // +0x38
    CPackedUIntArray          PackedDataSize;       // +0x48
    CPackedUIntArray          PackedDataType;       // +0x58
    CPackedUIntArray          PackedValid;          // +0x68
    unsigned char             _pad78[0xa4 - 0x78];
    unsigned long             FileInformationSize;  // +0xa4
    unsigned long             FileSize;             // +0xa8
    virtual void s00();
    unsigned long GetEntryDataOffset(unsigned long index) const
    {
        if (PackedDataOffset.IsPacked())
            return PackedDataOffset[index - 1];
        return RuntimeData[index].DataOffset;
    }
    unsigned long GetEntryDataSize(unsigned long index) const
    {
        if (PackedDataSize.IsPacked())
            return PackedDataSize[index - 1];
        return RuntimeData[index].DataSize;
    }
    unsigned long GetEntryDataType(unsigned long index) const
    {
        if (PackedDataType.IsPacked())
            return PackedDataType[index - 1];
        return RuntimeData[index].DataType;
    }
    CCharString GetEntrySymbol(unsigned long index) const
    {
        if (Symbols.empty())
            return CCharString("(Unknown)", -1);
        return Symbols[index];
    }
    unsigned long InternalWriteEntryList(CDataOutputStream& stream);
};

#pragma auto_inline(off)
unsigned long& CULongMap::operator[](const unsigned long& key)
{
    _M_header->value.first = key;
    return _M_header->value.second;
}
#pragma auto_inline(on)
unsigned long CBankFile_Retail::InternalWriteEntryList(CDataOutputStream& stream)
{
    unsigned long dataEnd = stream.GetPosition();
    unsigned long count = 0;
    unsigned long maxIndex = 0;
    CULongMap typeCounts;
    for (unsigned long i = 0; i < UpdateData.size(); ++i) {
        if (RuntimeData[i].Valid && RuntimeData[i].DataOffset != 0 && !UpdateData[i]->RequiresUpdate) {
            ++count;
            unsigned long type = GetEntryDataType(i);
            SULongPair value;
            value.first = type;
            value.second = 0;
            typeCounts.insert(value);
            ++typeCounts[type];
            unsigned long end = GetEntryDataOffset(i) + GetEntryDataSize(i);
            dataEnd = Max(dataEnd, end);
            if (maxIndex < i)
                maxIndex = i;
        }
    }
    stream.WriteULong(typeCounts._M_node_count);
    for (SULongNode* it = typeCounts._M_header->left; it != typeCounts._M_header; it = CULongMap::_M_increment(it)) {
        stream.WriteULong(it->value.first);
        stream.WriteULong(it->value.second);
    }
    for (unsigned long i = 0; i < UpdateData.size(); ++i) {
        if (RuntimeData[i].Valid && RuntimeData[i].DataOffset != 0) {
            CBankFileEntryUpdateData* update = UpdateData[i];
            if (!update->RequiresUpdate) {
                stream.WriteULong(42);
                stream.WriteULong(i);
                stream.WriteULong(GetEntryDataType(i));
                stream.WriteULong(GetEntryDataSize(i));
                stream.WriteULong(GetEntryDataOffset(i));
                stream.WriteULong(update->StateBlockCRC);
                stream.WritePresizedString(GetEntrySymbol(i));
                stream.WriteULong(Checksums[i]);
                stream.WriteULong(update->Filenames_Size);
                for (unsigned long j = 0; j < (unsigned long)update->Filenames_Size; ++j)
                    stream.WritePresizedString(update->Filenames_Start[j]);
                unsigned long infoSize = update->InfoSize;
                CByteVector info(infoSize, 0);
                update->ReadInfo(0, info.GetData(), infoSize);
                stream.WriteULong(infoSize);
                stream.Write(info.GetData(), infoSize);
            }
        }
    }
    FileInformationSize = stream.GetPosition() - dataEnd;
    FileSize = Max(FileSize, stream.GetPosition());
    return count;
}