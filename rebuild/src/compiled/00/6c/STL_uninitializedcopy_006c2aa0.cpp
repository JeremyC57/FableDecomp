// std::__uninitialized_copy(CLevelLoader::CObject::CLevelFiles* first, CLevelLoader::CObject::CLevelFiles* last, CLevelLoader::CObject::CLevelFiles* result, const __false_type&) -- STLport uninitialized copy of the level-file triples: placement copy-construct each 28-byte record (level id + three counted file pointers, each bumping its shared reference count) and return the advanced destination
// Leaf: __uninitialized_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C2AA0, 99 bytes, `ret 8` (__fastcall: first in ecx, last in edx, result
// and the __false_type tag reference on the stack). No callees. A byte-identical twin sits at 0x00513500 (another TU).
// Types (Ego_r.pdb): CLevelLoader::CObject::CLevelFiles (28 bytes) = long LevelID +0, CCountedPointer<CAFile>
// PTopologyFile +4, PNavigationFile +0xc, PThingFile +0x14; CCountedPointer<T> (8 bytes) = T* Data +0, CCPPointerInfo*
// Info +4; CCPPointerInfo.RefCount at +0 (the copy constructor bumps it when Info is set).
// Shape: STLport 4.x `for (; first != last; ++first, ++cur) _Construct(&*cur, *first); return cur;` -- placement new
// (the null test on the destination), CLevelFiles' implicit memberwise copy constructor running the three
// CCountedPointer copy constructors inline; VC7.1 strength-reduces second induction pointers at +0x10 for both sides.
// Label: catalogued as `_Move<CLevelLoader::CObject::CLevelFiles*, ...>` (propagated Dinkumware name).
inline void* operator new(unsigned int, void* p) { return p; }

struct CAFile;

struct CCPPointerInfo {                          // 12 bytes
    unsigned long RefCount;                      // +0x00
    void (__cdecl* DeleteFunc)(void*);           // +0x04
    void*         Data;                          // +0x08
};

template <class T>
struct CCountedPointer {                         // 8 bytes
    T*              Data;                        // +0x00
    CCPPointerInfo* Info;                        // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

struct CLevelFiles {                             // CLevelLoader::CObject::CLevelFiles, 28 bytes
    long                    LevelID;             // +0x00
    CCountedPointer<CAFile> PTopologyFile;       // +0x04
    CCountedPointer<CAFile> PNavigationFile;     // +0x0c
    CCountedPointer<CAFile> PThingFile;          // +0x14
};

struct __false_type { __false_type() { } };

CLevelFiles* __fastcall __uninitialized_copy(CLevelFiles* first, CLevelFiles* last, CLevelFiles* result, const __false_type&)
{
    CLevelFiles* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) CLevelFiles(*first);
    return cur;
}