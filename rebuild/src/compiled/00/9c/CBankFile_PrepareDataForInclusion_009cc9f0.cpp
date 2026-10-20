// CBankFile::PrepareDataForInclusion(long, const CCountedPointer<CCharString>& source, long, CCountedPointer<CBankEntryInfo> entry, CDataWriter* writer, long) -- open the source file named by `source` read-only on a local CDiskFileWin32, read its whole length into a zeroed byte array, hand the bytes to the writer, close the file (the by-value entry pointer is released on exit)
// Leaf: ?PrepareDataForInclusion@CBankFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CC9F0, length 302 bytes (`ret 1Ch`: seven argument words).
// Callees: CBase ctor 0x0099A2F0 / virtual dtor 0x0099A300, CWideString() 0x0099AED0 / ~ 0x0099B510, CCharString::
//          ToWideString 0x0099E480 (result homed in the dead `source` argument slot), CDiskFileWin32::Open 0x0099A6A0 /
//          Read 0x0099A960 / Close 0x0099A920 (direct calls on the local), malloc 0x00BFEA0E / free 0x00BFEA14 (the byte
//          array), the writer's virtual slot 4 (+0x10, Write(data, size)), operator delete 0x00BFE9BC (count block).
// Model: the landed CBankFile::OpenFileReadOnly (0x009D1090) CDiskFileWin32 (vtable 0x0122D06C, Opened = false in the
//          ctor body) with an inline `virtual ~CDiskFileWin32() { Close(); }` over a POLYMORPHIC CBase root (a plain
//          CBase base puts the base subobject at +4); the unused arguments are unnamed (types unknown, descriptive).
// Shape: the byte array is the STLport vector(n): _Vector_base(n) allocates (null on 0) and sets the end of storage,
//          uninitialized_fill_n of unsigned char is the memset intrinsic (`rep stosd ; rep stosb`); the data pointer is
//          the CArray `size() ? begin : (T*)this` accessor, evaluated twice; the source is reached through a counted
//          pointer's inline operator-> (the plain struct member form picks other registers).
#include <string.h>
#pragma intrinsic(memset)
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
void __cdecl operator delete(void* block);             // 0x00BFE9BC

struct CWideString {
    void* PStringData;
    CWideString();                                     // 0x0099AED0
    ~CWideString();                                    // 0x0099B510
};
struct CCharString {
    void* PStringData;
    CWideString ToWideString() const;                  // 0x0099E480
};
struct CASuspendableProcess;
struct CBase {                                         // the polymorphic root (vptr at +0)
    CBase();                                           // 0x0099A2F0
    virtual ~CBase();                                  // 0x0099A300
};
struct CDiskFileWin32 : CBase {                        // vtable 0x0122D06C
    long        AccessType;                            // +0x04
    long        Position;                              // +0x08
    long        Length;                                // +0x0c
    void*       WinFileHandle;                         // +0x10
    bool        Opened;                                // +0x14
    CWideString Pathname;                              // +0x18
    CDiskFileWin32() { Opened = false; }
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Open(const CWideString& pathname, long access, unsigned long flags);   // 0x0099A6A0
    virtual void Read(void* buffer, long size, CASuspendableProcess* process);         // 0x0099A960
    virtual void Close();                                                              // 0x0099A920
};

template <class T> struct allocator { allocator() { } };
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    _Vector_base(unsigned int n, const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = n ? (T*)malloc(n * sizeof(T)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    ~_Vector_base() { if (_M_start) free(_M_start); }
};
struct CByteArray : _Vector_base<unsigned char> {      // CArray<unsigned char> (STLport vector)
    CByteArray(unsigned int n) : _Vector_base<unsigned char>(n, allocator<unsigned char>())
    {
        _M_finish = uninitialized_fill_n(_M_start, n, (unsigned char)0);
    }
    static unsigned char* uninitialized_fill_n(unsigned char* first, unsigned int n, const unsigned char& value)
    {
        memset(first, value, n);
        return first + n;
    }
    unsigned char* GetData() { return (_M_finish - _M_start) ? _M_start : (unsigned char*)this; }
};

struct SCountInfo { long RefCount; void (__fastcall* Destroy)(void*); void* Object; };
__forceinline bool DecRefToZero(long& count) { return --count == 0; }
template <class T> struct CCountedPointer {
    T*          Data;
    SCountInfo* Info;
    ~CCountedPointer()
    {
        if (Info && DecRefToZero(Info->RefCount)) {
            Info->Destroy(Info->Object);
            delete Info;
        }
    }
};
struct CDataWriter {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Write(const void* data, unsigned long size);   // slot 4 (+0x10)
};
template <class T> struct CSourcePointer {           // a counted pointer to the source name, passed by reference
    T*          Data;
    SCountInfo* Info;
    T* operator->() const { return Data; }
};
struct CBankEntryInfo;

class CBankFile {
public:
    void PrepareDataForInclusion(long index, const CSourcePointer<CCharString>& source, long flags, CCountedPointer<CBankEntryInfo> entry, CDataWriter* writer, long reserved);
};

void CBankFile::PrepareDataForInclusion(long, const CSourcePointer<CCharString>& source, long, CCountedPointer<CBankEntryInfo>, CDataWriter* writer, long)
{
    CDiskFileWin32 file;
    file.Open(source->ToWideString(), 1, 4);
    unsigned long size = file.Length;
    CByteArray buffer(size);
    file.Read(buffer.GetData(), size, 0);
    writer->Write(buffer.GetData(), size);
    file.Close();
}