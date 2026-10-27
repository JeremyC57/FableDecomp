// NShape::CTrack::GetBestCameraPosition(const C3DVector& pos, float) const -- the camera position a track proposes for a position: the position itself
// Leaf: ?GetBestCameraPosition@CTrack@NShape@@QBE?AVC3DVector@@ABV3@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00760000, 27 bytes, `ret 0Ch` (thiscall: hidden result pointer, the position
//   reference and the float on the stack; `this` unused).
// Identity: Ego_r.pdb `?GetBestCameraPosition@CTrack@NShape@@QBE?AVC3DVector@@ABV3@M@Z`. The Ego_r.pdb donor keeps the
//   NShape TU in retail order: IsVectorBetween2OthersInThePlane (donor 0x7707E0, retail landed 0x0075FF00), then
//   GetBestCameraPosition (donor 0x770910, a 0x20-byte body), then IsLineIntersectingWithOthersInList (donor 0x770930,
//   retail 0x00760020); retail 0x00760000 sits in that slot and its `ret 0Ch` + 12-byte copy fit the signature (hidden
//   C3DVector result, const C3DVector&, float). No direct callers (a virtual or function-pointer use). Catalog row was the
//   placeholder `_gapscan::sub_00760000`.
// Shape: C3DVector's memberwise user copy constructor (as the landed 0x0075FF00 model) copies X, Y, Z through edx/ecx into
//   the hidden result, which is returned in eax.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

namespace NShape {
class CTrack {
public:
    C3DVector GetBestCameraPosition(const C3DVector& pos, float) const;
};

C3DVector CTrack::GetBestCameraPosition(const C3DVector& pos, float) const
{
    return pos;
}
}