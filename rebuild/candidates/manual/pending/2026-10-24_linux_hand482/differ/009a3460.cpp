// ReadTextFileIntoArray(const CWideString& filename, CArray<unsigned short>& text) -- read a whole text file into a zero-terminated wide array: UTF-16 little- or big-endian after a byte-order mark, otherwise one byte per character; returns the character count
// Grade: DIFFER(424v424) -- register choice only: retail takes eax/ecx for the resize temporary's address and count + 1 in the big-endian and one-byte branches (edx/eax in the little-endian one), ours takes edx/eax in all three; tried named count, per-loop index, unsigned-char buffer
// Leaf: ?ReadTextFileIntoArray@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A3460, 424 bytes. FableWin.pdb
//   `?ReadTextFileIntoArray@?A0xfe46a155@@YIKABVCWideString@@AAV?$CArray@G@@@Z`: an anonymous-namespace `__fastcall`
//   (filename in ecx, text in edx), plain `ret`. The catalog name is the PDB name.
// Callees: CBase() 0x0099A2F0 / ~CBase 0x0099A300, CWideString() 0x0099AED0 / ~CWideString 0x0099B510 (the local
//   CDiskFileWin32's inline constructor and destructor, vtable 0x0122D06C), CDiskFileWin32::Open 0x0099A6A0,
//   ::Read 0x0099A960, ::Close 0x0099A920 (direct calls on the local), vector<unsigned char>::_M_fill_insert 0x004110D0 (the
//   buffer's resize), vector<unsigned short>::resize(size_type, const unsigned short&) 0x00517900, free 0x00BFEA14.
// Layout: CDiskFileWin32 (0x1C) as the landed NAnimationEvents::CManager::LoadBinary 0x006F9F90: AccessType +4,
//   Position +8, Length +0xC, WinFileHandle +0x10, Opened +0x14, Pathname +0x18.
// Shape: open read-only (access 1, flags 2), an STLport byte vector resized to length + 1 (its char() temporary at
//   [esp+0x13]), read through the "data or the vector itself when empty" idiom of the landed LoadBinary, closed, then
//   the BOM test on the first two bytes; each branch resizes the output to count + 1 and converts with 16-bit zero
//   extensions; the terminator is stored at text[count] and count returned.
extern "C" void __cdecl free(void* block);                                         // 0x00BFEA14

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                 // +0x00
    _Tp* _M_finish;                                // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    ~vector() { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);         // 0x004110D0 (unsigned char)
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    iterator erase(iterator __first, iterator __last);
    void resize(size_type __new_size, const _Tp& __x);                          // 0x00517900 (unsigned short)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
template <> inline void vector<unsigned char, allocator<unsigned char> >::resize(size_type __new_size, const unsigned char& __x)
{
    if (__new_size < size())
        erase(begin() + __new_size, end());
    else
        insert(end(), __new_size - size(), __x);
}
} // namespace std

template <class T> class CArray : public std::vector<T> { };

class CWideString
{
public:
    void* PStringData;                                  // +0x00
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
};

class CBase
{
public:
    CBase();                                            // 0x0099A2F0
    virtual ~CBase();                                   // 0x0099A300
};

enum FileAccessType { FILE_ACCESS_READ = 1 };
class CASuspendableProcess;
class CDiskFileWin32 : public CBase                     // vtable 0x0122D06C, 0x1C bytes
{
public:
    long        AccessType;                             // +0x04
    long        Position;                               // +0x08
    long        Length;                                 // +0x0C
    void*       WinFileHandle;                          // +0x10
    bool        Opened;                                 // +0x14
    CWideString Pathname;                               // +0x18
    CDiskFileWin32() { Opened = false; }
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099A6A0
    virtual void Close();                               // 0x0099A920
    virtual void Read(void* data, long size, CASuspendableProcess* process);                      // 0x0099A960
    long GetLength() const { return Length; }
};

static inline unsigned char* GetData(std::vector<unsigned char>& v) { return v.size() ? &v[0] : (unsigned char*)&v; }

namespace {
unsigned long __fastcall ReadTextFileIntoArray(const CWideString& filename, CArray<unsigned short>& text)
{
    CDiskFileWin32 file;
    file.CDiskFileWin32::Open(filename, FILE_ACCESS_READ, 2);
    unsigned long length = file.GetLength();
    std::vector<unsigned char> buffer;
    buffer.resize(length + 1);
    file.CDiskFileWin32::Read(GetData(buffer), length, 0);
    file.CDiskFileWin32::Close();
    unsigned long count;
    if (buffer[0] == 0xFF && buffer[1] == 0xFE) {
        count = length / 2 - 1;
        text.resize(count + 1);
        for (unsigned long i = 0; i < count; ++i)
            text[i] = (unsigned short)(((unsigned short)buffer[i * 2 + 3] << 8) + (unsigned short)buffer[i * 2 + 2]);
    } else if (buffer[0] == 0xFE && buffer[1] == 0xFF) {
        count = length / 2 - 1;
        text.resize(count + 1);
        for (unsigned long i = 0; i < count; ++i)
            text[i] = (unsigned short)(((unsigned short)buffer[i * 2 + 2] << 8) + (unsigned short)buffer[i * 2 + 3]);
    } else {
        count = length;
        text.resize(length + 1);
        for (unsigned long i = 0; i < count; ++i)
            text[i] = (unsigned short)buffer[i];
    }
    text[count] = 0;
    return count;
}
}

unsigned long (__fastcall* g_keepReadTextFileIntoArray)(const CWideString&, CArray<unsigned short>&) = ReadTextFileIntoArray;
