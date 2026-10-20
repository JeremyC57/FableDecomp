// CBankFile::FindIndexByPathname(const CCharString& pathname, bool unused) -- look an entry up by its full path: split off the file name + extension, get the candidate indices with that file name, and return the first whose stored pathname matches case-insensitively (0 when none)
// Grade: DIFFER(267v268) -- retail keeps the result vector's _M_start in ebp across the _stricmp calls (also reused for the final free), spills the count to a 4th frame word and keeps `this` in ebx; ours keeps the count in edi and re-reads _M_start every pass (`for (i < indices.size())` and a cached `first` pointer are both further off); the `operator const char*` null arm order (retail `jne` over the "" load) also differs
// Leaf: ?FindIndexByPathname@CBankFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CD140, length 268 bytes (`ret 8`).
// Callees: CCharString() @ 0x0099E4B0 (x2), SplitPath @ 0x009976D0 (__fastcall: path, drive 0, directory 0, &filename,
//          &extension), CCharString::operator+=(const CCharString&) @ 0x0099F0A0, CBankFile::FindIndicesByFilename(const
//          CCharString&, long, bool) @ 0x009CCEF0 (returns CArray<unsigned long> by value), _stricmp @ 0x00BFEBA8,
//          free @ 0x00BFEA14 (the vector's storage), ~CCharString @ 0x0099EAE0.
// Layout (local): the entry-pointer array at +0x2c, each entry's pathname (CCharString*) at +0xc.
#include <string.h>
extern "C" void __cdecl free(void* block);    // 0x00BFEA14

struct CCharStringData { char* PData; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                             // 0x0099E4B0
    ~CCharString();                            // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);   // 0x0099F0A0
    operator const char*() const { return PStringData == 0 ? "" : PStringData->PData; }
};
void __fastcall SplitPath(const CCharString& path, CCharString* drive, CCharString* directory, CCharString* filename, CCharString* extension);   // 0x009976D0

template <class T> struct allocator { allocator() { } };
template <class T> struct _STLP_alloc_proxy : allocator<T> { T* _M_data; };
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    ~_Vector_base() { if (_M_start) free(_M_start); }
};
template <class T> struct CArray : _Vector_base<T> {
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
    CArray<unsigned long> FindIndicesByFilename(const CCharString& filename, long maxCount, bool exact);   // 0x009CCEF0
    unsigned long FindIndexByPathname(const CCharString& pathname, bool unused);
};

unsigned long CBankFile::FindIndexByPathname(const CCharString& pathname, bool)
{
    CCharString filename;
    CCharString extension;
    SplitPath(pathname, 0, 0, &filename, &extension);
    filename += extension;
    CArray<unsigned long> indices = FindIndicesByFilename(filename, -1, false);
    unsigned long count = indices.size();
    for (unsigned long i = 0; i < count; ++i) {
        unsigned long index = indices[i];
        if (_stricmp(*Entries[index]->PPathname, pathname) == 0)
            return index;
    }
    return 0;
}
