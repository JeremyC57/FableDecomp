// CPersistTraits<CFaction>::TransferBinaryIn(CMemoryDataInputStream& stream, CFaction& value) -- binary persist read of a faction saved as text: the text length, that many bytes into a zeroed NUL-terminated buffer, then the text TransferIn over a string parser and a text persist context
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCFaction@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BA8B0, 394 bytes (+ int3 padding), `ret 8` (traits `this` passed on).
// Ego_r.pdb: ?TransferBinaryIn@?$CPersistTraits@VCFaction@@@@QAEXAAVCMemoryDataInputStream@@AAVCFaction@@@Z.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the 4-byte length), malloc @ 0x00BFEA0E / free @ 0x00BFEA14
//   (STLport allocator of the byte buffer), the stream's virtual byte read (slot 4, +0x10), CCharString::CCharString(const char*,
//   long) @ 0x0099EBF0 (the buffer text, then the "" file name @ 0x0122D70E), CStringParser::CStringParser @ 0x00404720,
//   ~CCharString @ 0x0099EAE0, CPersistContext::CPersistContext(CStringParser*, callback, bool, CDefinitionManager*,
//   CSymbolMap*, CCharString*) @ 0x009BADD0, CPersistTraits<CFaction>::TransferIn @ 0x006BA480, and the inlined
//   ~CStringParser (~list @ 0x004046B0, two ~CCharString, CBaseClass::~CBaseClass @ 0x0099A2E0).
// Shape: the length through an inline by-pointer long read wrapping CMemoryDataInputStream::Read (64-bit position range check, 4-byte fast copy, else
//   ReadWithSrcChunkOverflow into the dead stream slot); a CArray<char> (STLport vector) of length + 1 zero bytes (allocate,
//   inline memset through STLport uninitialized_fill_n, whose begin copy is spilled over the rep stos and gives the end); its `size ? first : (char*)this` GetData feeds the virtual read and the
//   string; the CGameTimeManager::Load (0x006BBBA0) parser / context pair. Speed-optimised (default) TU, no frame pointer.
#include <stddef.h>
extern "C" void* __cdecl memcpy(void* dest, const void* src, size_t bytes);
extern "C" void* __cdecl memset(void* dest, int value, size_t bytes);
#pragma intrinsic(memcpy, memset)
extern "C" void* __cdecl malloc(size_t size);  // 0x00BFEA0E
extern "C" void __cdecl free(void* p);          // 0x00BFEA14

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
struct CBaseClass {                                // the CStringParser root (polymorphic)
    virtual void v00();
    ~CBaseClass();                                 // 0x0099A2E0
};
struct CStringParserCommentList {                  // retail std::list<CStringParserCommentBoundaries>: sentinel only
    void* m_pHead;
    ~CStringParserCommentList();                   // 0x004046B0
};
struct CStringParser : CBaseClass {                // retail 0x28 bytes
    char*                    String;               // +0x04
    unsigned long            BaseLineOffset;       // +0x08
    CCharString              Filename;             // +0x0c
    CCharString              TempCharString;       // +0x10
    CStringParserCommentList Comments;             // +0x14
    void (*ErrorCallback)(CCharString*);           // +0x18
    long                     CursorPos[3];         // +0x1c CCursorPos
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);            // 0x00404720
};
struct CDefinitionManager;
struct CSymbolMap;
struct CPersistContext {                           // retail 0x2c bytes, trivial destructor
    unsigned char _pad00[0x2c];
    CPersistContext(CStringParser* parser, void (*errorCallback)(CCharString*), bool reportErrors,
                    CDefinitionManager* definitions, CSymbolMap* symbols, CCharString* sourceName);   // 0x009BADD0
};
extern const char g_emptyString_0122d70e[];       // 0x0122D70E ("")

class CMemoryDataInputStream {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void ReadBytes(void* buffer, long size);                  // slot 4 (+0x10)
    unsigned long        position;                                    // +0x04
    long                 sourceLength;                                // +0x08
    const unsigned char* chunkBase;                                   // +0x0C
    long                 chunkPosition;                               // +0x10
    long                 chunkLength;                                 // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);  // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

// STLport's char fill_n (memset), returning first + n from its own copy of the start pointer.
template <class T> __forceinline T* uninitialized_fill_n(T* first, size_t n, const T& x)
{
    memset(first, x, n);
    return first + n;
}

template <class T> class CArray {                  // STLport vector<T> with malloc/free allocation
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    explicit CArray(size_t n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n != 0 ? (T*)malloc(n * sizeof(T)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        _M_finish = uninitialized_fill_n(_M_start, n, T());
    }
    ~CArray() { if (_M_start != 0) free(_M_start); }
    T* GetData() { return (_M_finish - _M_start) ? _M_start : (T*)this; }
    T& operator[](size_t i) { return _M_start[i]; }
};

// The inline CPersistTraits<long>::TransferBinaryIn: the read goes through its own local (homed in the dead stream slot on
// the slow path), then the caller's long is assigned, so the caller's copy can live in a register.
__forceinline void TransferBinaryInLong(CMemoryDataInputStream& stream, long* out)
{
    long value;
    stream.Read(&value, sizeof(value));
    *out = value;
}

class CFaction;
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CFaction> {
    void TransferIn(CPersistContext& context, CStringParser& parser, CFaction& value);   // 0x006BA480
    void TransferBinaryIn(CMemoryDataInputStream& stream, CFaction& value);
};

void CPersistTraits<CFaction>::TransferBinaryIn(CMemoryDataInputStream& stream, CFaction& value)
{
    long length;
    TransferBinaryInLong(stream, &length);
    CArray<char> buffer(length + 1);
    stream.ReadBytes(buffer.GetData(), length);
    buffer[length] = 0;
    CCharString text(buffer.GetData(), -1);
    CStringParser parser(text, 0, CCharString(g_emptyString_0122d70e, -1), 0);
    CPersistContext context(&parser, 0, true, 0, 0, 0);
    TransferIn(context, parser, value);
}