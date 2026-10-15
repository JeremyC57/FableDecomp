// CUIMiniMapMarker::CUIMiniMapMarker(const CUIMiniMapMarker&) -- copy constructor: region, the UI def name (CCharString copy), position, name length, graphic index and angle
// Leaf: ??0CUIMiniMapMarker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00612980, length 61 bytes (`ret 4`, returns this). Caller: 0x0061889F.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Layout: PDB CUIMiniMapMarker (28 bytes): Region +0, UIDefName (CCharString) +4, Position (C2DVector) +8,
// DefNameLength +0x10, GraphicIndex +0x14, Angle (float) +0x18.
// Shape: the region load precedes the string's address set-up and its store follows the push; the five
// trailing words are copied one register each with integer moves (floats included, and the C2DVector
// member copied by its implicit bitwise copy): an out-of-line memberwise copy constructor.
struct CCharString {
    void* m_pStringData;
    CCharString(const CCharString& other);         // 0x0099EC30
};

struct C2DVector {
    float X, Y;
};

struct CUIMiniMapMarker {
    long        Region;                            // +0x00
    CCharString UIDefName;                         // +0x04
    C2DVector   Position;                          // +0x08
    long        DefNameLength;                     // +0x10
    long        GraphicIndex;                      // +0x14
    float       Angle;                             // +0x18
    CUIMiniMapMarker(const CUIMiniMapMarker& other);
};

CUIMiniMapMarker::CUIMiniMapMarker(const CUIMiniMapMarker& other)
    : Region(other.Region)
    , UIDefName(other.UIDefName)
    , Position(other.Position)
    , DefNameLength(other.DefNameLength)
    , GraphicIndex(other.GraphicIndex)
    , Angle(other.Angle)
{
}