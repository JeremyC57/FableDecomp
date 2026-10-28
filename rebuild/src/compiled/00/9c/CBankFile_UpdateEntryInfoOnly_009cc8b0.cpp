// CBankFile::UpdateEntryInfoOnly(unsigned long index, CDataInputStream& stream, unsigned long infoSize) -- read infoSize bytes of entry info from the stream into a zeroed buffer, store them as the entry's pending info (clearing its RequiresUpdate flag) and notify the bank of the updated entry
// Leaf: ?UpdateEntryInfoOnly@CBankFile@@QAEXKAAVCDataInputStream@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CC8B0, 307 bytes, thiscall, `ret 0Ch`.
// Identity: Ego_r.pdb `?UpdateEntryInfoOnly@CBankFile@@QAEXKAAVCDataInputStream@@K@Z` (the only CBankFile member taking
//   (unsigned long, CDataInputStream&, unsigned long)); its single caller CEngineSurfaceCompositionManager::WriteTextureToBank
//   0x00B2BD80 (site 0x00B2BFDA, pending hand286 notes reached the same name) hands it the texture stream; the body calls
//   CBankFileEntryUpdateData::SetInfo (landed 0x009CC6F0) and the bank's virtual NotifyEntryUpdated (Ego_r.pdb
//   `?NotifyEntryUpdated@CBankFile@@MAEXKKABV?$CArray@D@@@Z`, slot 13). The manifest label "CBankFile_LoadEntryData" is
//   descriptive.
// Callees: malloc 0x00BFEA0E (the inlined STLport vector<char>(n): allocate, memset to zero), CDataInputStream::
//   ReadWithSrcChunkOverflow 0x00993CA0 (the inlined Read's overflow path), CBankFileEntryUpdateData::SetInfo 0x009CC6F0,
//   CPackedUIntArray::operator[] 0x00A629F0 (the inlined GetEntryDataType of the landed 0x009CC210), slot 13, free.
// Shape: the update object is fetched first; the info buffer is the retail CArray<char> (GetAsCArray: its own address
//   when empty); the inlined Read is the landed ReadPresizedString one (size > 0, __int64 position check, memcpy or
//   overflow); the notify call passes the data type first, then the index (`push index ; push eax`).
// Layout: retail CBankFile as the landed 0x009CC210 (RuntimeData +0x20, UpdateData +0x2c, PackedDataType +0x58);
//   Ego_r.pdb CBankFileEntryUpdateData (InfoSize +4, InfoBuffer +8, RequiresUpdate +0x14) and CDataInputStream (0x18; the
//   engine header has no member functions, so the class is local: header debt).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
extern "C" void* __cdecl memset(void* dest, int value, unsigned int count);
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int count);
#pragma intrinsic(memset, memcpy)

namespace std {
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _STLP_alloc_proxy(const _Alloc& __a, _Tp __p) : _Alloc(__a), _M_data(__p) { }
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base() { if (_M_start) free(_M_start); }
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;    // +0x08
};
inline char* uninitialized_fill_n(char* __first, unsigned int __n, const char& __val)
{
    memset(__first, __val, __n);
    return __first + __n;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(unsigned int __n) : _Vector_base<_Tp, _Alloc>(__n, _Alloc())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, _Tp());
    }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    _Tp& operator[](unsigned int __n) { return *(this->_M_start + __n); }
};
}

template <class T> class CArray : public std::vector<T> {  // retail CArray<T>: the STLport vector plus the engine helpers
public:
    explicit CArray(unsigned int size) : std::vector<T>(size) { }
    T* GetAsCArray() { return this->size() != 0 ? &(*this)[0] : GetZeroSizeArray(); }
private:
    T* GetZeroSizeArray() { return (T*)this; }
};
template <class T> class CPlainArray { public:              // the bank's pointer / record arrays (retail CArray, 12 bytes)
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};

class CDataInputStream {                                   // Ego_r.pdb, 0x18 bytes
public:
    void*         __vftable;                               // +0x00
    unsigned long StreamPos;                               // +0x04
    long          StreamSize;                              // +0x08
    void*         CurrentSourcePtr;                        // +0x0c
    unsigned long SourceChunkStreamPos;                    // +0x10
    long          SourceChunkBytesRemaining;               // +0x14
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        if (size <= 0)
            return;
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
};

class CBankFileEntryUpdateData {                           // Ego_r.pdb, 0x20 bytes
public:
    unsigned long StateBlockCRC;                           // +0x00
    unsigned long InfoSize;                                // +0x04
    char*         InfoBuffer;                              // +0x08
    unsigned char Filenames[8];                            // +0x0c CSmallVector<CCharString,8>
    bool          RequiresUpdate;                          // +0x14
    bool          Exists;                                  // +0x15
private:
    friend class CBankFile;
    void SetInfo(const CArray<char>& info);                // 0x009CC6F0
};

struct CRuntimeData {                                      // CBankFile::CRuntimeData, 12 bytes
    unsigned long DataOffset;
    unsigned long DataSize;
    unsigned char DataType;
    bool          Valid;
};
struct CPackedUIntArray {                                  // 16 bytes; m_pData == 0: not packed
    unsigned long  m_bitsPerValue;
    unsigned long* m_pData;
    unsigned long  m_count;
    unsigned long  m_capacity;
    unsigned long operator[](unsigned long index) const;   // 0x00A629F0
    bool IsPacked() const { return m_pData != 0; }
};

class CBankFile {                                          // the retail layout (landed 0x009CC210)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
protected:
    virtual void NotifyEntryUpdated(unsigned long dataType, unsigned long index, const CArray<char>& info);   // slot 13 (+0x34)
public:
    unsigned long             Size;                        // +0x04
    unsigned char             Symbols[0x0c];               // +0x08
    unsigned char             Checksums[0x0c];             // +0x14
    CPlainArray<CRuntimeData> RuntimeData;                 // +0x20
    CPlainArray<CBankFileEntryUpdateData*> UpdateData;     // +0x2c
    CPackedUIntArray          PackedDataOffset;            // +0x38
    CPackedUIntArray          PackedDataSize;              // +0x48
    CPackedUIntArray          PackedDataType;              // +0x58
    unsigned long GetEntryDataType(unsigned long index) const
    {
        if (PackedDataType.IsPacked())
            return PackedDataType[index - 1];
        return RuntimeData[index].DataType;
    }
    void UpdateEntryInfoOnly(unsigned long index, CDataInputStream& stream, unsigned long infoSize);
};

void CBankFile::UpdateEntryInfoOnly(unsigned long index, CDataInputStream& stream, unsigned long infoSize)
{
    CBankFileEntryUpdateData* updateData = UpdateData[index];
    CArray<char> info(infoSize);
    stream.Read(info.GetAsCArray(), infoSize);
    updateData->SetInfo(info);
    updateData->RequiresUpdate = false;
    NotifyEntryUpdated(GetEntryDataType(index), index, info);
}