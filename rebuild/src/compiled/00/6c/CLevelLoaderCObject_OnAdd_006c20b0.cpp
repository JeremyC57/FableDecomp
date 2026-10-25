// CLevelLoader::CObject::OnAdd(const CLevelLoader& loader) -- remember the level loader and flag every map this object occupies as changed in the editable world map
// Leaf: ?OnAdd@CObject@CLevelLoader@@QAEXABV2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C20B0, 109 bytes (+ int3 padding; `ret 4`).
// Signature (FINDINGS chunk 386): Ego_r.pdb `?OnAdd@CObject@CLevelLoader@@QAEXABV2@@Z` -- the nested
//   CLevelLoader::CObject taking `const CLevelLoader&` (the landed model was a top-level CObject::OnAdd(CLevelLoader*));
//   bytes are the same (a reference is passed as its address).
// Callees: CEditWorldMap::SetMapAsChanged(long, bool) @ 0x004FCFE0 (Ego_r.pdb `?SetMapAsChanged@CEditWorldMap@@QAEXJ_N@Z`)
//   on the loader's world map at +0x1c.
// Layout (retail): Ego_r.pdb CLevelLoader::CObject = LoadableSections (set<CCharString>, 12 bytes in retail STLport) +0x0,
//   PLevelLoader +0xc (PDB +0x10), RegionLevelFiles (vector<CLevelFiles>, STLport start/finish/end) +0x10 (PDB +0x14),
//   CLevelFiles 28 bytes with LevelID first; CLevelLoader WorldMap at retail +0x1c (PDB +0x24: its ObjectList list is
//   4 bytes in retail, 12 in the PDB), called as the CEditWorldMap.
// Shape: `size()` is the signed pointer difference divided by 28 (`imul 92492493h`) re-evaluated every iteration and compared
//   unsigned; `operator[]` is `*(begin() + n)` (the index strength-reduced to a byte offset in ebx); the loader and its map are
//   re-read for every call.
class CEditWorldMap {
public:
    void SetMapAsChanged(long mapIndex, bool changed);       // 0x004FCFE0
};
namespace std {
template <class T> struct allocator { };
template <class T, class A = allocator<T> > struct vector {  // STLport vector: start, finish, end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() { return end() - begin(); }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
class CLevelLoader {
public:
    class CObject {
    public:
        class CLevelFiles {                                  // 28 bytes
        public:
            long          LevelID;                           // +0x00
            unsigned char _files04[0x18];                    // +0x04 three CCountedPointer<CAFile>
        };
        unsigned char                  LoadableSections[0x0c];   // +0x00 set<CCharString> (STLport, 12 bytes)
        CLevelLoader*                  PLevelLoader;             // +0x0c
        std::vector<CLevelFiles>       RegionLevelFiles;         // +0x10
        void OnAdd(const CLevelLoader& loader);
    };
    unsigned char  _pad00[0x1c];
    CEditWorldMap* WorldMap;                                     // +0x1c
};

void CLevelLoader::CObject::OnAdd(const CLevelLoader& loader)
{
    PLevelLoader = const_cast<CLevelLoader*>(&loader);
    for (unsigned long i = 0; i < RegionLevelFiles.size(); ++i)
        PLevelLoader->WorldMap->SetMapAsChanged(RegionLevelFiles[i].LevelID, true);
}
