// CBankFile::FindIndexByPathname(const CCharString& pathname, long unused) const -- look an entry up by its full path: split off the file name + extension, get the candidate indices with that file name, and return the first whose stored pathname matches case-insensitively (0 when none)
// Leaf: ?FindIndexByPathname@CBankFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CD140, length 268 bytes (`ret 8`).
// Callees: CCharString() @ 0x0099E4B0 (x2), SplitPath @ 0x009976D0 (__fastcall: path, drive 0, directory 0, &filename,
//          &extension), CCharString::operator+=(const CCharString&) @ 0x0099F0A0, CBankFile::FindIndicesByFilename(const
//          CCharString&, long, bool) @ 0x009CCEF0 (returns CArray<unsigned long> by value), _stricmp @ 0x00BFEBA8,
//          free @ 0x00BFEA14 (the vector's storage), ~CCharString @ 0x0099EAE0.
// Layout (local): the entry-pointer array at +0x2c, each entry's pathname (CCharString*) at +0xc.
// Shape: Ego_r.pdb `?FindIndexByPathname@CBankFile@@QBEKABVCCharString@@J@Z` (const, the unused second parameter is a
// long). FindIndicesByFilename is in retail's TU, so VC7.1 sees that the returned CArray does not escape: its _M_start
// stays in ebp across the _stricmp calls and is reused for the final free, `this` keeps ebx and the count goes to a
// 4th frame word. The same-TU stand-in below builds the result in place from scalars (plain noinline, NOT inside an
// auto_inline(off) region, which would outline the vector destructor it instantiates). The entry's pathname is bound
// to a const reference before the compare, so its loads come before the argument's conversion.
#include <string.h>
extern "C" void __cdecl free(void* block);    // 0x00BFEA14

class CCharStringData { public: char* PData; };
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                             // 0x0099E4B0
    ~CCharString();                            // 0x0099EAE0
    const CCharString& operator+=(const CCharString& other);   // 0x0099F0A0
    operator const char*() const { return PStringData == 0 ? "" : PStringData->PData; }
};
class CAFile {
public:
    static void __fastcall SplitPath(const CCharString&, CCharString*, CCharString*, CCharString*, CCharString*);   // 0x009976D0
};   // 0x009976D0

template <class T> struct allocator { allocator() { } };
template <class T> struct _STLP_alloc_proxy : allocator<T> { T* _M_data; };
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    ~_Vector_base() { if (_M_start) free(_M_start); }
};
template <class T> class CArray : public _Vector_base<T> { public:
    CArray(T* start, T* finish) { this->_M_start = start; this->_M_finish = finish; this->_M_end_of_storage._M_data = finish; }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    T& operator[](unsigned int n) { return *(this->_M_start + n); }
};

struct CBankFileEntry {
    unsigned char _pad[0xc];
    CCharString*  PPathname;                  // +0x0c
};

class CBankFile {
public:
    virtual void v00();
    unsigned char    _pad04[0x28];
    CBankFileEntry** Entries;                 // +0x2c
    CArray<unsigned long> FindIndicesByFilename(const CCharString& filename, long maxCount, bool exact) const;   // 0x009CCEF0
    unsigned long FindIndexByPathname(const CCharString& pathname, long unused) const;
};
unsigned long* FindIndices_009ccef0(const CBankFile* bank, const CCharString& filename, long maxCount, bool exact, unsigned long* count);
// Stand-in for CBankFile::FindIndicesByFilename @ 0x009CCEF0 (same TU in retail): the result is built in place from
// scalars, so the caller's result vector does not escape.
__declspec(noinline) CArray<unsigned long> CBankFile::FindIndicesByFilename(const CCharString& filename, long maxCount, bool exact) const
{
    unsigned long count;
    unsigned long* start = FindIndices_009ccef0(this, filename, maxCount, exact, &count);
    return CArray<unsigned long>(start, start + count);
}

unsigned long CBankFile::FindIndexByPathname(const CCharString& pathname, long) const
{
    CCharString filename;
    CCharString extension;
    CAFile::SplitPath(pathname, 0, 0, &filename, &extension);
    filename += extension;
    CArray<unsigned long> indices = FindIndicesByFilename(filename, -1, false);
    unsigned long count = indices.size();
    for (unsigned long i = 0; i < count; ++i) {
        unsigned long index = indices[i];
        const CCharString& entryPath = *Entries[index]->PPathname;
        if (_stricmp(entryPath, pathname) == 0)
            return index;
    }
    return 0;
}