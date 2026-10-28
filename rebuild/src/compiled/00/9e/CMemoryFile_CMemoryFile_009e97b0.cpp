// CMemoryFile::CMemoryFile(const CWideString& pathname, FileAccessType access, unsigned long flags) -- construct the base, the empty byte buffer and the empty pathname, then open the named file into memory
// Leaf: ??0CMemoryFile@@QAE@ABVCWideString@@W4FileAccessType@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E97B0, 61 bytes (+ int3 padding), thiscall, `ret 0Ch`.
// Identity (proven): the ctor stores CMemoryFile's vtable 0x0122E0FC (RTTI .?AVCMemoryFile@@) and then calls slot 1 (Open,
//   0x009E94A0) directly with its three arguments; both PDBs carry ??0CMemoryFile@@QAE@ABVCWideString@@W4FileAccessType@@K@Z.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0, CWideString::CWideString 0x0099AED0, CMemoryFile::Open
//   0x009E94A0 (a virtual called from the ctor: bound statically).
// Shape: the base ctor out of line, then the vptr, the STLport vector's three zero stores (one xor'ed register; the pathname lea
//   scheduled before the last store) and the pathname ctor; Opened is left for Open. The 2026-10-14 hand57 park ("lea scheduled
//   after the zero stores") was graded on the QFE-4035 lane with an invented vector stand-in.
// Layout: local model -- engine/CMemoryFile.h is the flat Ego_r layout (16-byte CArray, vptr as a data member). Header debt.
// ---- CMemoryFile model (retail STLport layout; engine/CMemoryFile.h is the flat Ego_r layout with a 16-byte CArray) ----
// RTTI (ghidra_out/rtti_bases.tsv): CMemoryFile : CAFile : CBaseClassNonCopyable : CBaseClass, all single-inheritance at 0.
// CBaseClassNonCopyable's protected ctor 0x0099A2F0 (stores vtable 0x0129A7C4, PDB ??0CBaseClassNonCopyable@@IAE@XZ) and protected
// virtual dtor 0x0099A300 (PDB ??1CBaseClassNonCopyable@@MAE@XZ) are out of line; CAFile adds no vtable store of its own in any
// retail ctor/dtor (modelled __declspec(novtable), implicit inline dtor).
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
extern "C" void __cdecl free(void* block);                                          // 0x00BFEA14
class CWideStringData;
class CASuspendableProcess;
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };

class CWideString {
public:
    CWideStringData* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};

class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                            // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                   // 0x0099A300
};
class __declspec(novtable) CAFile : public CBaseClassNonCopyable {
public:
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags) = 0;   // slot 1
    virtual void Close() = 0;                                                                       // slot 2
    virtual void Read(void* data, long length, CASuspendableProcess* process) = 0;                  // slot 3
    virtual void Write(const void* data, long length, CASuspendableProcess* process) = 0;           // slot 4
};

class CDiskFileWin32 : public CAFile {                  // vtable 0x0122D06C, 0x1C bytes
public:
    FileAccessType AccessType;                          // +0x04
    long           Position;                            // +0x08
    long           Length;                              // +0x0C
    void*          WinFileHandle;                       // +0x10
    bool           Opened;                              // +0x14
    CWideString    Pathname;                            // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099A6A0
    virtual void Close();                                                                       // 0x0099A920
    virtual void Read(void* data, long length, CASuspendableProcess* process);                  // 0x0099A960
    virtual void Write(const void* data, long length, CASuspendableProcess* process);           // 0x0099AA90
};

namespace std {
struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };
struct input_iterator_tag { input_iterator_tag() {} };
struct output_iterator_tag { output_iterator_tag() {} };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() {} };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() {} };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() {} };
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return (_OutputIter)__copy_trivial(__first, __last, __result);
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return __copy_ptrs(__first, __last, __result, __true_type());
}
template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy_aux(__first, __last, __result, __true_type());
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) {}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __true_type());
}
template <class _Tp> class allocator {
public:
    allocator() {}
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) {}
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) {}
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = copy(__last, this->_M_finish, __first);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
}   // namespace std

template <class T> class CArray : public std::vector<T> {   // retail 0x0C bytes (STLport vector)
public:
};

class CMemoryFile : public CAFile {                     // vtable 0x0122E0FC, 0x24 bytes in retail
public:
    FileAccessType        AccessType;                   // +0x04
    long                  Position;                     // +0x08
    long                  Length;                       // +0x0C
    CArray<unsigned char> DataBuffer;                   // +0x10 (Ego_r CArray<unsigned char>; 12 bytes in retail)
    bool                  Opened;                       // +0x1C
    CWideString           Pathname;                     // +0x20
    CMemoryFile(const CWideString& pathname, FileAccessType access, unsigned long flags);
    virtual ~CMemoryFile();                                                                      // slot 0 (??_G 0x004120C0)
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // slot 1, 0x009E94A0
    virtual void Close();                                                                       // slot 2, 0x009E9390
    virtual void Read(void* data, long length, CASuspendableProcess* process);                  // slot 3, 0x009E9220
    virtual void Write(const void* data, long length, CASuspendableProcess* process);           // slot 4, 0x009E95F0
    void DumpToFile();
private:
    void Copy(const CMemoryFile& other);
};

CMemoryFile::CMemoryFile(const CWideString& pathname, FileAccessType access, unsigned long flags)
{
    Open(pathname, access, flags);
}