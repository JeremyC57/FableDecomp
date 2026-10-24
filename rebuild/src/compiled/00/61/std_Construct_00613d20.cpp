// std::_Construct(CTCInventoryMap::CUIMiniMapMarker*, const CTCInventoryMap::CUIMiniMapMarker&) -- placement copy of an inventory-map marker: region, def-name string, position, name length, graphic, angle
// Retail: Fable.exe @ 0x00613D20, 59 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CCharString copy ctor @ 0x0099EC30.
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N00613d20 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct C2DVector {
    float X, Y;
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }           // user memberwise copy
};
struct CUIMiniMapMarker {                        // CTCInventoryMap::CUIMiniMapMarker (PDB, sizeof 0x1C)
    long        Region;                          // +0x00
    CCharString UIDefName;                       // +0x04
    C2DVector   Position;                        // +0x08
    long        DefNameLength;                   // +0x10
    long        GraphicIndex;                    // +0x14
    float       Angle;                           // +0x18
    __forceinline CUIMiniMapMarker(const CUIMiniMapMarker& o)   // the implicit memberwise copy, spelled out so VC7.1 inlines it
        : Region(o.Region), UIDefName(o.UIDefName), Position(o.Position), DefNameLength(o.DefNameLength),
          GraphicIndex(o.GraphicIndex), Angle(o.Angle) { }
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CUIMiniMapMarker* p, const CUIMiniMapMarker& value)
{
    if (p != 0)
        new (p) CUIMiniMapMarker(value);
}
}