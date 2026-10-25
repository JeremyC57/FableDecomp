// CWorldMap::DeleteAllRegions() -- empty the region table and leave the single default region 0
// Leaf: ?DeleteAllRegions@CWorldMap@@UAEXXZ
// Retail: Fable.exe @ 0x00507610, 54 bytes. PDB ?DeleteAllRegions@CWorldMap@@UAEXXZ (primary vtable 0x01244AEC slot 2).
// Callees: STLport vector<CRegion>::erase(iterator, iterator) 0x0051CC80 (manifest "Vector_Erase_Range"),
// CRegion::CRegion() 0x006BC410 (manifest label "CEngineInit" is a propagation), vector<CRegion>::resize(size_type,
// const CRegion&) 0x0051D350 (its body erases past n or inserts n - size() copies; the manifest's CVectorMap template
// arguments are a propagation), CRegion::~CRegion() 0x0051A110.
// Retail CWorldMap (engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport vectors / CArrays and
// the map member are 12 bytes, so a local model is used): +0x2C Regions (vector<CRegion>, PDB +0x34); retail CRegion is
// 0x58 bytes (PDB 0x64).
// Shape: STLport `clear()` = `erase(begin(), end())` (end read first, right-to-left), then the inline one-argument
// `resize(n)` builds the default CRegion in the 0x58-byte frame, calls `resize(n, x)` and destroys it (no EH frame).
class CRegion {
public:
    unsigned char _body[0x58];                  // retail sizeof 0x58
    CRegion();                                  // 0x006BC410
    ~CRegion();                                 // 0x0051A110
};

namespace _STL {
template <class T> class allocator { };
template <class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);                // 0x0051CC80 (vector<CRegion>)
    void resize(unsigned int n, const T& x);    // 0x0051D350 (vector<CRegion>)
    void resize(unsigned int n) { resize(n, T()); }
    void clear() { erase(begin(), end()); }
};
}

class CWorldMap {
public:
    virtual void v00(); virtual void v01();
    virtual void DeleteAllRegions();            // slot 2
    unsigned char          _pad04[0x28];
    _STL::vector<CRegion>  Regions;             // +0x2C (PDB +0x34)
};

void CWorldMap::DeleteAllRegions()
{
    Regions.clear();
    Regions.resize(1);
}