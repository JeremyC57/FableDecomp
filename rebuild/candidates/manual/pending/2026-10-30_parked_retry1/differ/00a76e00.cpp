// CNavQuadTree::GetPreferabilityCostForNode(const CANavQuadTreeNode* node) const -- the topology weight of the map cell at the node square's corner (cell = floor of the map-relative corner + 1/8), or the default 0x40 for other layers, an empty weight map or an out-of-range cell
// Grade: DIFFER(298v298) -- landed FloorToLong (x2) and FloatToLong retail_asm helpers, the corner and the map-relative position as C2DVector arithmetic; residue: frame packing only (retail sub esp,28h with corner.Y at [ebp-24h] and rel.X at [ebp-18h], the lane 20h)
// Retail-asm: 0x00A76E66 fld dword ptr [ebp-0Ch] ; fsub dword ptr [ebp-8] ; fistp dword ptr [ebp-4] -- the landed FloorToLong (twice) and FloatToLong (MapWidth, 0x00A76EC0) helpers
// Leaf: GetPreferabilityCostForNode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A76E00, 298 bytes (+ int3 padding), `ret 4`; ebp frame.
// Globals: the per-level node size table 0x0129DCAC, the 0.5f / 1.0f / 0.125f literals.
// Layout: as 0x00A76CC0 (packed CNavQuadTree: MapPos +7, MapWidth +0x10, TopologyWeights +0x34).
// Shape: retail rounds with bare `fistp` (an inline-asm float->int helper in the original, which is also why the body has
//   an ebp frame): floor(v) = round(v - 0.5), bumped when v equals the result + 1. The 64-bit product range test
//   guards the index. Written here with the C++ conversion, so it differs by construction (asm_bake family).
class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
};
extern const float g_QuadTreeLevelSizes[];         // 0x0129DCAC
class CANavQuadTreeNode {
public:
    virtual void v00();
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
};
struct CByteVector {                               // STLport vector<unsigned char>
    unsigned char* _M_start;
    unsigned char* _M_finish;
    unsigned char* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
};
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
// the engine's inline-assembly floor helper (the landed CThingPhysical::ModifyHealth 0x006A68F0 FloorToLong, verbatim)
__forceinline long FloorToLong(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}
#pragma pack(push, 1)
class CNavQuadTree {
public:
    virtual void v00();
    bool          Initialised;                     // +0x04
    bool          Activated;                       // +0x05
    bool          Initialising;                    // +0x06
    C2DVector     MapPos;                          // +0x07
    unsigned char _pad0f;
    float         MapWidth;                        // +0x10
    float         MapHeight;                       // +0x14
    long          MapIndex;                        // +0x18
    unsigned char NavigationLayers[0x0c];          // +0x1c
    unsigned char TransitionPoints[0x0c];          // +0x28
    CByteVector   TopologyWeights;                 // +0x34
    unsigned char GetPreferabilityCostForNode(const CANavQuadTreeNode* node) const;
};
#pragma pack(pop)

unsigned char CNavQuadTree::GetPreferabilityCostForNode(const CANavQuadTreeNode* node) const
{
    if (node->NavLayer != 0 || TopologyWeights.empty())
        return 0x40;
    float half = g_QuadTreeLevelSizes[node->QuadTreeLevel] * 0.5f;
    C2DVector corner = node->Position - C2DVector(half, half);
    C2DVector rel = corner - MapPos;
    long cellY = FloorToLong(rel.Y + 0.125f);
    long cellX = FloorToLong(rel.X + 0.125f);
    __int64 index = (__int64)FloatToLong(MapWidth) * cellY + cellX;
    if (index < 0 || index > 0x7fffffff)
        return 0x40;
    return TopologyWeights._M_start[FloatToLong(MapWidth) * cellY + cellX];
}
