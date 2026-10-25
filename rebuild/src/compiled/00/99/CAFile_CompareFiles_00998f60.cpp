// CAFile::CompareFiles(CAFile& file1, CAFile& file2) -- true when two open files have the same length and the same bytes, compared 4 KB at a time from the start
// Leaf: ?CompareFiles@CAFile@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00998F60, 323 bytes (+ int3 padding). FableWin.pdb `?CompareFiles@CAFile@@SI_NAAV1@0@Z`:
//   a static `__fastcall` (file1 in ecx, file2 in edx), plain `ret`. The catalog's "IsInvalidCustomTattoo" is the
//   caller's name (CTattooDef::IsInvalidCustomTattoo 0x00581765 compares the custom tattoo file with its reference).
// Callees (virtual, CAFile vtable as in CDiskFileWin32 0x0122D06C): slot 3 Read(void*, long, CASuspendableProcess*),
//   slot 5 SetPosition(long), slot 9 GetLength() const; malloc 0x00BFEA0E / free 0x00BFEA14.
// Shape: the landed sibling CAFile::CopyFileW 0x00998E70 -- both files rewound, the lengths compared (early `return
//   false` before any buffer exists), then two STLport vector<unsigned char>(0x1000) buffers (malloc + `rep stosd`
//   zero fill, freed behind ~_Vector_base's null test, second buffer first), the block size a const-ref operand of
//   STLport `min`, and the intrinsic memcmp (`repe cmpsb`) on each block. With two buffers VC7.1 stops inlining the
//   vector(n) / _Vector_base(n) constructors, so both are `__forceinline` (retail inlines them).
#include <string.h>

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

namespace std {
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
    __forceinline _Vector_base(unsigned int n, const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
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
    __forceinline explicit vector(unsigned int n) : _Vector_base<T, A>(n, A())
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
    static bool __fastcall CompareFiles(CAFile& file1, CAFile& file2);
};


bool __fastcall CAFile::CompareFiles(CAFile& file1, CAFile& file2)
{
    file1.SetPosition(0);
    file2.SetPosition(0);
    if (file1.GetLength() != file2.GetLength())
        return false;
    long bufferSize = 0x1000;
    std::vector<unsigned char> buffer1(bufferSize);
    std::vector<unsigned char> buffer2(bufferSize);
    long remaining = file1.GetLength();
    while (remaining > 0) {
        long chunk = (std::min)(remaining, bufferSize);
        file1.Read(buffer1.begin(), chunk, 0);
        file2.Read(buffer2.begin(), chunk, 0);
        if (memcmp(buffer1.begin(), buffer2.begin(), chunk) != 0)
            return false;
        remaining -= chunk;
    }
    return true;
}