// CShaderBankEntry::CShaderBankEntry(unsigned long type, unsigned long index, CShaderDataBank* bank) -- read the bank entry's
//   data into a zeroed char buffer, wrap it in a memory stream and read the shader: the code size in bytes and the code
//   dwords, the constant-name count and each null-terminated name, then the vertex-shader constant layout name.
// Leaf: CShaderBankEntry
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A62120, 688 bytes (678 + int3 padding), `ret 0Ch`; vtable 0x0129D698.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0 (base), CCharString() 0x0099E4B0,
//   CBankFile::GetEntryDataSize 0x009CC240, malloc 0x00BFEA0E, CBankFile::ReadEntryData 0x009CC2A0,
//   CMemoryDataInputStream(void*, ulong) 0x00994930, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0,
//   CArray<unsigned long>::resize(n, const T&) 0x00464931, CMemoryDataInputStream::ReadNullTerminatedString 0x00995D30
//   (devirtualised on the local stream), CCharString(const CCharString&) 0x0099EC30, vector<CCharString>::
//   _M_insert_overflow 0x00433530, ~CCharString 0x0099EAE0, CCharString::operator= 0x0099EFB0,
//   ~CMemoryDataInputStream 0x00993B10, free 0x00BFEA14.
// Layout: Ego_r.pdb CShaderBankEntry (44 bytes, 16-byte CArrays) -> retail 0x24 bytes (12-byte STLport vectors, as the
//   landed CShaderDataBank::NotifyEntryAdded 0x00A623D0): ShaderType +0x4, ShaderCode +0x8, ShaderConstants +0x14,
//   VSConstantLayout +0x20. CMemoryDataInputStream as the landed CWoundMorphs::TransferBinaryIn 0x00757990 (0x1c bytes on
//   the frame here).
// Shape: the counts are read through a by-value inline reader (the value lives in the dead `index` slot only on the
//   overflow path); the char buffer is STLport vector(n) (finish = uninitialized_fill_n's `first + n`); the vectors'
//   end-of-storage is the allocator proxy with its own ctor (member-init store order); CArray::GetData is
//   `size() > 0 ? _M_start : this`; push_back is inline with a `__false_type()` temporary passed to a same-TU member-template
//   _M_insert_overflow body (the empty-tag rule: the tag shares the name's slot, as retail).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memcpy, memset)
extern "C" void* __cdecl malloc(unsigned int bytes);
extern "C" void __cdecl free(void* p);
inline void* operator new(unsigned int, void* where) { return where; }

struct __false_type { __false_type() {} };

struct CCharString {
    void* PStringData;
    CCharString();                                           // 0x0099E4B0
    CCharString(const CCharString& other);                   // 0x0099EC30
    ~CCharString();                                          // 0x0099EAE0
    CCharString& operator=(const CCharString& other);        // 0x0099EFB0
};

template <class T> struct CArray {                           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    struct CStorageEnd { T* _M_data; CStorageEnd(T* p) : _M_data(p) {} } _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    unsigned long size() const { return _M_finish - _M_start; }
    T* GetData() { return size() > 0 ? _M_start : (T*)this; }
    void resize(unsigned long count, const T& value);        // 0x00464931 (unsigned long)
    template <class Tag> void _M_insert_overflow(T* position, const T& x, const Tag&, unsigned long fill_len, bool atend);   // 0x00433530
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

static inline char* FillN(char* first, unsigned long n)
{
    memset(first, 0, n);
    return first + n;
}
// Same-TU body of the STLport vector<CCharString>::_M_insert_overflow instantiation (tag unused).
template <class T> template <class Tag>
__declspec(noinline) void CArray<T>::_M_insert_overflow(T* pos, const T& x, const Tag&, unsigned long n, bool atend)
{
    unsigned long oldSize = size();
    unsigned long len = oldSize + (oldSize < n ? n : oldSize);
    T* ns = len ? (T*)malloc(len * sizeof(T)) : 0;
    T* nf = ns;
    T* p;
    for (p = _M_start; p != pos; ++p, ++nf)
        new (nf) T(*p);
    for (; n != 0; --n, ++nf)
        new (nf) T(x);
    if (!atend)
        for (p = pos; p != _M_finish; ++p, ++nf)
            new (nf) T(*p);
    for (p = _M_start; p != _M_finish; ++p)
        p->~T();
    if (_M_start)
        free(_M_start);
    _M_start = ns;
    _M_finish = nf;
    _M_end_of_storage._M_data = ns + len;
}

struct CCharBuffer {                                         // CArray<char> built with a size
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    explicit CCharBuffer(unsigned long n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n != 0 ? (char*)malloc(n) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        _M_finish = FillN(_M_start, n);
    }
    ~CCharBuffer()
    {
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    char* GetData() { return size() != 0 ? _M_start : (char*)this; }
};

struct CMemoryDataInputStream {
    void*                m_pVtable;                          // +0x00
    unsigned long        m_position;                         // +0x04
    long                 StreamSize;                         // +0x08
    const unsigned char* m_pData;                            // +0x0c
    long                 SourceChunkStreamPos;               // +0x10
    long                 m_remaining;                        // +0x14
    long                 _field18;                           // +0x18
    CMemoryDataInputStream(void* data, unsigned long size);  // 0x00994930
    ~CMemoryDataInputStream();                               // 0x00993B10
    CCharString ReadNullTerminatedString();                  // 0x00995D30
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline unsigned long ReadULong()
    {
        unsigned long value;
        Read(&value, sizeof(value));
        return value;
    }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= m_remaining) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

struct CBankFile {
    unsigned long GetEntryDataSize(unsigned long index);                                         // 0x009CC240
    void ReadEntryData(unsigned long index, unsigned long offset, void* buffer, unsigned long size);   // 0x009CC2A0
};
struct CShaderDataBank : CBankFile {};

struct CBaseClassNonCopyable {
    CBaseClassNonCopyable();                                 // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};

struct CShaderBankEntry : CBaseClassNonCopyable {
    unsigned long          ShaderType;                       // +0x04
    CArray<unsigned long>  ShaderCode;                       // +0x08
    CArray<CCharString>    ShaderConstants;                  // +0x14
    CCharString            VSConstantLayout;                 // +0x20
    CShaderBankEntry(unsigned long type, unsigned long index, CShaderDataBank* bank);
    virtual ~CShaderBankEntry();
};

CShaderBankEntry::CShaderBankEntry(unsigned long type, unsigned long index, CShaderDataBank* bank)
    : ShaderType(type)
{
    unsigned long size = bank->GetEntryDataSize(index);
    CCharBuffer data(size);
    bank->ReadEntryData(index, 0, data.GetData(), size);
    CMemoryDataInputStream stream(data.GetData(), size);
    unsigned long codeCount = stream.ReadULong() / 4;
    ShaderCode.resize(codeCount, 0);
    unsigned long* code = ShaderCode.GetData();
    long bytes = codeCount * sizeof(unsigned long);
    if (bytes > 0)
        stream.Read(code, bytes);
    unsigned long count = stream.ReadULong();
    for (unsigned long i = 0; i < count; ++i) {
        CCharString name = stream.ReadNullTerminatedString();
        ShaderConstants.push_back(name);
    }
    VSConstantLayout = stream.ReadNullTerminatedString();
}