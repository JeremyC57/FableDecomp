#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
typedef long long __int64;
#endif
// behaviour test: CShaderBankEntry ctor reads the bank entry: code size and dwords, constant names, then the constant-layout name


#include <cstring>
#include <cstdlib>
#include <new>

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
    unsigned int size() const { return _M_finish - _M_start; }
    T* GetData() { return size() > 0 ? _M_start : (T*)this; }
    void resize(unsigned int n, const T& v)
    {
        T* p = (T*)std::malloc(n * sizeof(T) + 1);
        for (unsigned int i = 0; i < n; ++i) p[i] = v;
        std::free(_M_start); _M_start = p; _M_finish = p + n; _M_end_of_storage._M_data = p + n;
    }
    template <class Tag> void _M_insert_overflow(T* position, const T& x, const Tag&, unsigned int fill_len, bool atend);   // 0x00433530
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

static inline char* FillN(char* first, unsigned int n)
{
    memset(first, 0, n);
    return first + n;
}

template <class T> template <class Tag>
__declspec(noinline) void CArray<T>::_M_insert_overflow(T* pos, const T& x, const Tag&, unsigned int n, bool atend)
{
    unsigned int oldSize = size();
    unsigned int len = oldSize + (oldSize < n ? n : oldSize);
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
    explicit CCharBuffer(unsigned int n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
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
    unsigned int size() const { return _M_finish - _M_start; }
    char* GetData() { return size() != 0 ? _M_start : (char*)this; }
};

struct CMemoryDataInputStream {
    void*                m_pVtable;                          // +0x00
    unsigned int        m_position;                         // +0x04
    long                 StreamSize;                         // +0x08
    const unsigned char* m_pData;                            // +0x0c
    long                 SourceChunkStreamPos;               // +0x10
    long                 m_remaining;                        // +0x14
    long                 _field18;                           // +0x18
    CMemoryDataInputStream(void* data, unsigned int size);  // 0x00994930
    ~CMemoryDataInputStream();                               // 0x00993B10
    CCharString ReadNullTerminatedString();                  // 0x00995D30
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline unsigned int ReadULong()
    {
        unsigned int value;
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
    unsigned int GetEntryDataSize(unsigned int index);                                         // 0x009CC240
    void ReadEntryData(unsigned int index, unsigned int offset, void* buffer, unsigned int size);   // 0x009CC2A0
};
struct CShaderDataBank : CBankFile {};

struct CBaseClassNonCopyable {
    CBaseClassNonCopyable();                                 // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};

struct CShaderBankEntry : CBaseClassNonCopyable {
    unsigned int          ShaderType;                       // +0x04
    CArray<unsigned int>  ShaderCode;                       // +0x08
    CArray<CCharString>    ShaderConstants;                  // +0x14
    CCharString            VSConstantLayout;                 // +0x20
    CShaderBankEntry(unsigned int type, unsigned int index, CShaderDataBank* bank);
    virtual ~CShaderBankEntry();
};

CShaderBankEntry::CShaderBankEntry(unsigned int type, unsigned int index, CShaderDataBank* bank)
    : ShaderType(type)
{
    unsigned int size = bank->GetEntryDataSize(index);
    CCharBuffer data(size);
    bank->ReadEntryData(index, 0, data.GetData(), size);
    CMemoryDataInputStream stream(data.GetData(), size);
    unsigned int codeCount = stream.ReadULong() / 4;
    ShaderCode.resize(codeCount, 0);
    unsigned int* code = ShaderCode.GetData();
    long bytes = codeCount * sizeof(unsigned int);
    if (bytes > 0)
        stream.Read(code, bytes);
    unsigned int count = stream.ReadULong();
    for (unsigned int i = 0; i < count; ++i) {
        CCharString name = stream.ReadNullTerminatedString();
        ShaderConstants.push_back(name);
    }
    VSConstantLayout = stream.ReadNullTerminatedString();
}

#include <cstring>
static char* Dup(const char* s) { if (!s) return 0; char* d = (char*)std::malloc(std::strlen(s) + 1); std::strcpy(d, s); return d; }
CCharString::CCharString() { PStringData = 0; }
CCharString::CCharString(const CCharString& o) { PStringData = Dup((const char*)o.PStringData); }
CCharString::~CCharString() { std::free(PStringData); }
CCharString& CCharString::operator=(const CCharString& o) { if (this != &o) { std::free(PStringData); PStringData = Dup((const char*)o.PStringData); } return *this; }
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CShaderBankEntry::~CShaderBankEntry() {}
static unsigned char g_entry[64]; static unsigned int g_entrySize = 0; static unsigned int g_askedIndex = 99, g_readIndex = 99;
unsigned int CBankFile::GetEntryDataSize(unsigned int index) { g_askedIndex = index; return g_entrySize; }
void CBankFile::ReadEntryData(unsigned int index, unsigned int offset, void* buffer, unsigned int size)
{ g_readIndex = index; if (offset == 0 && size <= g_entrySize) std::memcpy(buffer, g_entry, size); }
static int g_overflows = 0;
CMemoryDataInputStream::CMemoryDataInputStream(void* data, unsigned int size)
{ m_position = 0; StreamSize = (long)size; m_pData = (const unsigned char*)data; SourceChunkStreamPos = 0; m_remaining = 4; }
CMemoryDataInputStream::~CMemoryDataInputStream() {}
void CMemoryDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
{ ++g_overflows; std::memcpy(buffer, m_pData, size); m_pData += size; m_position += size; m_remaining = 4; }
CCharString CMemoryDataInputStream::ReadNullTerminatedString()
{ CCharString s; s.PStringData = Dup((const char*)m_pData); long n = (long)std::strlen((const char*)m_pData) + 1; m_pData += n; m_position += n; return s; }
static int fails = 0;
static void Check(bool ok, int line) { if (!ok) { std::printf("fail line %d\n", line); ++fails; } }
static void Put(unsigned int& at, unsigned int v) { std::memcpy(g_entry + at, &v, 4); at += 4; }
static void PutS(unsigned int& at, const char* s) { std::strcpy((char*)g_entry + at, s); at += (unsigned int)std::strlen(s) + 1; }
int main()
{
    unsigned int at = 0;
    Put(at, 8); Put(at, 0x11111111); Put(at, 0x22222222); Put(at, 2); PutS(at, "c0"); PutS(at, "light"); PutS(at, "Layout");
    g_entrySize = at;
    static CShaderBankEntry e(1, 7, (CShaderDataBank*)0);
    Check(g_askedIndex == 7 && g_readIndex == 7 && e.ShaderType == 1, __LINE__);
    Check(e.ShaderCode.size() == 2 && e.ShaderCode._M_start[0] == 0x11111111 && e.ShaderCode._M_start[1] == 0x22222222, __LINE__);
    Check(g_overflows == 1, __LINE__);   // the 8-byte code read goes past the 4-byte chunk
    Check(e.ShaderConstants.size() == 2, __LINE__);
    Check(std::strcmp((const char*)e.ShaderConstants._M_start[0].PStringData, "c0") == 0, __LINE__);
    Check(std::strcmp((const char*)e.ShaderConstants._M_start[1].PStringData, "light") == 0, __LINE__);
    Check(std::strcmp((const char*)e.VSConstantLayout.PStringData, "Layout") == 0, __LINE__);
    // no code, no constants
    at = 0; Put(at, 0); Put(at, 0); PutS(at, "L2"); g_entrySize = at; g_overflows = 0;
    static CShaderBankEntry f(0, 3, (CShaderDataBank*)0);
    Check(f.ShaderCode.size() == 0 && f.ShaderConstants.size() == 0 && g_overflows == 1, __LINE__);   // the count read crosses the chunk
    Check(std::strcmp((const char*)f.VSConstantLayout.PStringData, "L2") == 0, __LINE__);
    if (fails) { std::printf("00a62120_TEST FAIL\n"); return 1; }
    std::printf("00a62120_TEST PASS\n");
    return 0;
}