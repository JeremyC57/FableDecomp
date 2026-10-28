// CEnginePrimitiveRenderer2D::Layer2DToSortDistance(EEngine2DLayer layer) -- the layer's sort distance (Layer2DSortDistances[layer])
// Leaf: ?Layer2DToSortDistance@CEnginePrimitiveRenderer2D@@QAEMW4EEngine2DLayer@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B4A6A0, 14 bytes.
// Identity (2026-10-28, chunk 530): Ego_r.pdb orders Layer2DToSortDistance (0x009EB0C0) before Layer2DToDistance (0x009EB0D0),
//   as retail does 0x00B4A6A0 / 0x00B4A6B0; Ego_r Layer2DSortDistances is the float[83] at +0x1D8 (retail +0x1D4) and
//   Layer2DDistances the one at +0x8C (retail +0x88). Earlier both rows were landed as Layer2DToDistance.
enum EEngine2DLayer { };
class CEnginePrimitiveRenderer2D { public: float Layer2DToSortDistance(EEngine2DLayer l); };
float CEnginePrimitiveRenderer2D::Layer2DToSortDistance(EEngine2DLayer l){ return *(float*)((char*)this + l*4 + 0x1d4); }
