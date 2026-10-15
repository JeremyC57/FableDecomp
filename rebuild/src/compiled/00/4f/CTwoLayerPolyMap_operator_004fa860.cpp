// CTwoLayerPolyMap::operator=(const CTwoLayerPolyMap& other) -- copy the settings and both 256-row poly layers of a packed two-layer map, leaving the 5-byte header alone and resetting the trailing cache word
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004FA860, length 109 bytes.
// NOT CRigidBodyConfig::operator= (that class is 0xB8 bytes with a vector member, PDB): this body copies a
// #pragma pack(1) object of 0x10822 bytes, which its only caller (0x0051C3A9, inside the factory at 0x0051C2F0 that
// does `new` 0x10822 bytes, constructs two CLayer members at +0x16 and +0x841E with 0x0050EE40 and wraps the object
// in a counted pointer) uses to clone a source object. Owning class name unknown: descriptive model names.
// CLayer (0x8400 bytes) = 256 rows of {32 dwords, a dword count} (the 0x0050EE40 clear loop strides 0x84).
// Shape: a user operator= that assigns the members from +0x05 one by one (dword moves, the +0x15 byte, the two
// layers as implicit 0x2100-dword `rep movsd` copies) and stores 0 in the +0x1081E word; the first five bytes
// (a refcount/type header; the caller tests the type byte at +0x04 against 8) are not copied.
#pragma pack(push, 1)
struct CLayer {
    struct CRow {
        unsigned long Bits[32];
        unsigned long Count;
    };
    CRow Rows[256];
};

struct CTwoLayerPolyMap {
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

    CTwoLayerPolyMap& operator=(const CTwoLayerPolyMap& other);
};
#pragma pack(pop)

CTwoLayerPolyMap& CTwoLayerPolyMap::operator=(const CTwoLayerPolyMap& other)
{
    Setting05 = other.Setting05;
    Setting09 = other.Setting09;
    Setting0D = other.Setting0D;
    Setting11 = other.Setting11;
    Flag15 = other.Flag15;
    LayerA = other.LayerA;
    Setting8416 = other.Setting8416;
    Setting841A = other.Setting841A;
    LayerB = other.LayerB;
    Cache = 0;
    return *this;
}