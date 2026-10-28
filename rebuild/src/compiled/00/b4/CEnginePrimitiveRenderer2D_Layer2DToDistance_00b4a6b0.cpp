// CEnginePrimitiveRenderer2D::Layer2DToDistance(EEngine2DLayer layer) -- the layer's view distance (Layer2DDistances[layer])
// Leaf: ?Layer2DToDistance@CEnginePrimitiveRenderer2D@@QAEMW4EEngine2DLayer@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B4A6B0, 14 bytes.
// Identity (2026-10-28, chunk 530): Ego_r.pdb Layer2DToDistance follows Layer2DToSortDistance (0x00B4A6A0); Layer2DDistances
//   is the Ego_r float[83] at +0x8C (retail +0x88).
enum EEngine2DLayer { };
class CEnginePrimitiveRenderer2D { public: float Layer2DToDistance(EEngine2DLayer l); };
float CEnginePrimitiveRenderer2D::Layer2DToDistance(EEngine2DLayer l){ return *(float*)((char*)this + l*4 + 0x88); }
