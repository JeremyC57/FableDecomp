// CTwoLayerPolyMap_005102f0::CTwoLayerPolyMap_005102f0() -- default constructor of the packed two-layer poly map: clears both 256-row CLayer members (+0x16 and +0x841E), nothing else is initialised
// Leaf: CTwoLayerPolyMap_005102f0
// Retail: Fable.exe (TLC, VC7.1) @ 0x005102F0, 26 bytes, __fastcall, plain `ret`.
// Callees: CLayer::CLayer() 0x0050EE40 (the 256 x 0x84-byte row clear loop; manifest label "CLayer::SetAllPolys") twice.
// Evidence: the factory at 0x00506BB0 news 0x10822 bytes and constructs the same two members (+0x16, +0x841E) inline;
//   layout from the landed CTwoLayerPolyMap::operator= 0x004FA860 (#pragma pack(1): the object really is byte-packed,
//   the layers sit at +0x16 / +0x841E). The manifest label "QuestData" is propagated (that PDB struct is two CCharStrings).
//   Byte twin at 0x0050F090, so each copy is suffixed by its address (one retail address = one symbol).
// Shape: no vtable, no other member initialisation: only the two member constructors.
#pragma pack(push, 1)
struct CLayer {
    struct CRow {
        unsigned long Bits[32];
        unsigned long Count;
    };
    CRow Rows[256];
    CLayer();                        // 0x0050EE40
};

struct CTwoLayerPolyMap_005102f0 {
    long          RefCount;          // +0x00
    unsigned char Type;              // +0x04
    long          Setting05;         // +0x05
    long          Setting09;         // +0x09
    long          Setting0D;         // +0x0D
    long          Setting11;         // +0x11
    unsigned char Flag15;            // +0x15
    CLayer        LayerA;            // +0x16
    long          Setting8416;       // +0x8416
    long          Setting841A;       // +0x841A
    CLayer        LayerB;            // +0x841E
    void*         Cache;             // +0x1081E
    CTwoLayerPolyMap_005102f0();
};
#pragma pack(pop)

CTwoLayerPolyMap_005102f0::CTwoLayerPolyMap_005102f0()
{
}