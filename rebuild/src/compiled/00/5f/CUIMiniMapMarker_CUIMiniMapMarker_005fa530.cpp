// CUIMiniMapMarker::CUIMiniMapMarker() -- default marker: region 0, an empty UI def name, position, name length, graphic index and angle all zero
// Leaf: ??0CUIMiniMapMarker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FA530, length 43 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 with the empty literal @ 0x0122D70E and -1.
// Layout: PDB CUIMiniMapMarker (28 bytes): Region +0, UIDefName (CCharString) +4, Position (C2DVector) +8,
// DefNameLength +0x10, GraphicIndex +0x14, Angle (float) +0x18.
// Shape: one zero register (edi) set before the string's argument pushes serves the region store and the
// five trailing words: the float members are zeroed by body assignments (the 0.0f bit pattern rides the
// integer zero register).
struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length);    // 0x0099EBF0
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
    CUIMiniMapMarker();
};

CUIMiniMapMarker::CUIMiniMapMarker()
    : Region(0)
    , UIDefName("", -1)
{
    Position.X = 0;
    Position.Y = 0;
    DefNameLength = 0;
    GraphicIndex = 0;
    Angle = 0;
}