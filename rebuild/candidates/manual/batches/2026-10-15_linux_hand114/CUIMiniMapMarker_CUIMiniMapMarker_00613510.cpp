// CUIMiniMapMarker::CUIMiniMapMarker() -- default constructor: region 0, empty UI def name, position (0,0), def name length 0, graphic index 0, angle 0
// Retail: Fable.exe @ 0x00613510, 43 bytes. Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (length -1),
// literal "" @ 0x0122D70E.
// Layout from Ego_r.pdb (CUIMiniMapMarker, 0x1c bytes): Region +0, UIDefName +4, Position (C2DVector) +8,
// DefNameLength +0x10, GraphicIndex +0x14, Angle +0x18.
// Shape: non-polymorphic record (no vtable store); everything in the init list in declaration order. Region's
// zero store is scheduled before the string constructor call, the float zeros share the integer zero register (edi).
#include "fable_string.h"
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct CUIMiniMapMarker {
    long Region;              // +0x00
    CCharString UIDefName;    // +0x04
    C2DVector Position;       // +0x08
    long DefNameLength;       // +0x10
    long GraphicIndex;        // +0x14
    float Angle;              // +0x18
    CUIMiniMapMarker();
};
CUIMiniMapMarker::CUIMiniMapMarker()
    : Region(0), UIDefName("", -1), Position(0.0f, 0.0f), DefNameLength(0), GraphicIndex(0), Angle(0.0f)
{
}
