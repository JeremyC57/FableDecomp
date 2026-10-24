// CAFile::CopyFileW(CAFile& source, CAFile& destination) -- copy one open file into another through a 4 KB buffer, pumping the progress display after every block
// Retail: Fable.exe (TLC, VC7.1) @ 0x00998E70, 226 bytes. FableWin.pdb ?CopyFileW@CAFile@@SIXAAV1@0@Z: a static
// `__fastcall` (source in ecx, destination in edx), plain `ret`.
// Callees (virtual, CAFile vtable as in CDiskFileWin32 0x0122D06C): slot 3 Read(void*, long, CASuspendableProcess*),
// slot 4 Write(const void*, long, CASuspendableProcess*), slot 5 SetPosition(long), slot 8 SetLength(long), slot 9
// GetLength() const; malloc @ 0x00BFEA0E / free @ 0x00BFEA14; CCharString(const char*, long) @ 0x0099EBF0,
// NProgressDisplay::DisplayProgress(const CCharString&, float, bool, bool) @ 0x009E9F40 (`__fastcall`),
// ~CCharString @ 0x0099EAE0. Literal "" @ 0x0122D70E.
// Shape: the block size is a const-ref operand of STLport `min`, so both it and the remaining count live in frame
// words; the intrinsic memset over the constant size is `rep stosd`; the progress text is the implicit conversion
// temporary of "" (built after `remaining -= chunk`). The buffer is an STLport vector<unsigned char> (12 bytes of
// frame: retail's 0x18-byte frame, with the dead finish/end-of-storage words) over the malloc allocator: allocated
// in _Vector_base, zero-filled, re-read through begin() for each call and freed behind ~_Vector_base's null test.
// Leaf: ?CopyFileW@CAFile
#include <string.h>

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

struct CCharStringData;
class CCharString
{
public:
    CCharStringData* PStringData;                       // +0x00
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}

namespace _STL {
template <class T> inline const T& (min)(const T& a, const T& b) { return b < a ? b : a; }

template <class T> class allocator {                    // STLport allocator over malloc/free
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { free(p); }
};
template <class T, class A> class _STLP_alloc_proxy : public A {
public:
    T* _M_data;
    _STLP_alloc_proxy(const A& a, T* p) : A(a), _M_data(p) { }
};
template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                        // +0x00
    T* _M_finish;                                       // +0x04
    _STLP_alloc_proxy<T, A> _M_end_of_storage;          // +0x08
    _Vector_base(unsigned int n, const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    explicit vector(unsigned int n) : _Vector_base<T, A>(n, A())
    {
        memset(this->_M_start, 0, n * sizeof(T));      // uninitialized_fill_n(_M_start, n, T()) for a byte type
        this->_M_finish = this->_M_start + n;
    }
    T* begin() { return this->_M_start; }
};
}

class CASuspendableProcess;
class CWideString;
class CAFile
{
public:
    virtual ~CAFile();                                                          // slot 0
    virtual void Open(const CWideString& name, int access, unsigned long flags); // slot 1
    virtual void Close();                                                       // slot 2
    virtual void Read(void* destination, long size, CASuspendableProcess* process);        // slot 3
    virtual void Write(const void* source, long size, CASuspendableProcess* process);      // slot 4
    virtual void SetPosition(long position);                                    // slot 5
    virtual void MovePosition(long offset);                                     // slot 6
    virtual long GetPosition() const;                                           // slot 7
    virtual void SetLength(long length);                                        // slot 8
    virtual long GetLength() const;                                             // slot 9
    static void __fastcall CopyFileW(CAFile& source, CAFile& destination);
};

void __fastcall CAFile::CopyFileW(CAFile& source, CAFile& destination)
{
    destination.SetLength(source.GetLength());
    destination.SetPosition(0);
    source.SetPosition(0);
    long bufferSize = 0x1000;
    _STL::vector<unsigned char> buffer(bufferSize);
    long remaining = source.GetLength();
    while (remaining > 0) {
        long chunk = (_STL::min)(remaining, bufferSize);
        source.Read(buffer.begin(), chunk, 0);
        destination.Write(buffer.begin(), chunk, 0);
        remaining -= chunk;
        NProgressDisplay::DisplayProgress("", -1.0f, false, false);
    }
}