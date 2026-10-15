// CObject::OnAdd(CLevelLoader*) -- remember the level loader and flag every map this object occupies as changed in the editable world map
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C20B0, 109 bytes (+ int3 padding; `ret 4`).
// Callees: CEditWorldMap::SetMapAsChanged(long, bool) @ 0x004FCFE0 (the loader's edit world map at +0x1c).
// Layout (retail): loader +0xc, an STLport vector of 28-byte map entries at +0x10 (start) / +0x14 (finish), map index first.
// Shape: `size()` is the signed pointer difference divided by 28 (`imul 92492493h`) re-evaluated every iteration and compared
//   unsigned; `operator[]` is `*(begin() + n)` (the index strength-reduced to a byte offset in ebx); the loader and its map are
//   re-read for every call.
struct CEditWorldMap {
    void SetMapAsChanged(long mapIndex, bool changed);       // 0x004FCFE0
};
struct CLevelLoader {
    unsigned char  _pad00[0x1c];
    CEditWorldMap* m_pEditWorldMap;                          // +0x1c
};
struct SObjectMapEntry {                                     // 28 bytes
    long          MapIndex;                                  // +0x00
    unsigned char _data04[0x18];
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
struct CObject {
    unsigned char                  _pad00[0x0c];
    CLevelLoader*                  m_pLevelLoader;           // +0x0c
    std::vector<SObjectMapEntry>   m_maps;                   // +0x10
    void OnAdd(CLevelLoader* loader);
};

void CObject::OnAdd(CLevelLoader* loader)
{
    m_pLevelLoader = loader;
    for (unsigned long i = 0; i < m_maps.size(); ++i)
        m_pLevelLoader->m_pEditWorldMap->SetMapAsChanged(m_maps[i].MapIndex, true);
}