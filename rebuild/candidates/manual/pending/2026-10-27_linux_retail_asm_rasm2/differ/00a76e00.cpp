// CNavQuadTree::GetPreferabilityCostForNode(const CANavQuadTreeNode* node) const -- the topology weight of the map cell at the node square's corner (cell = floor of the map-relative corner + 1/8), or the default 0x40 for other layers, an empty weight map or an out-of-range cell
// Leaf: GetPreferabilityCostForNode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A76E00, 298 bytes (+ int3 padding), `ret 4`; ebp frame.
// Globals: the per-level node size table 0x0129DCAC, the 0.5f / 1.0f / 0.125f literals.
// Layout: as 0x00A76CC0 (packed CNavQuadTree: MapPos +7, MapWidth +0x10, TopologyWeights +0x34).
// Shape: the two cell coordinates go through the engine's inline-asm floor helper (frame locals, the second result homed in the
//   dead `node` slot); the map width is rounded twice by the inline-asm ROUND helper (a by-value copy, bare fistp). The 64-bit
//   product range test guards the index.
#pragma optimize("y", off)
// Retail-asm: 0x00A76E66..0x00A76E80 and 0x00A76E9D..0x00A76EB7 (`fld [ebp-0Ch] ; fsub [ebp-8] ; fistp dword ptr [ebp-4] ; fild [ebp-4] ; fadd [ebp+8] ; fstp [ebp-10h] ; mov eax,[ebp-0Ch] ; cmp eax,[ebp-10h] ; jne ; inc dword ptr [ebp-4]`), plus the bare `fld [ebp-14h] ; fistp dword ptr [ebp-10h]` ROUND helper at 0x00A76EC0 and 0x00A76EF6 -- the engine's inline-assembly floor helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; inc dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
struct C2DVector {
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
// the engine's inline-assembly float-to-long helper (round in the current FPU mode): expanded in place, its by-value
// parameter is homed in a frame word and reloaded for the bare dword fistp (the landed 0x006C3B70 / 0x0052F440 form)
__forceinline long ROUND(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
// the engine's inline-assembly FPU floor helper (0x0048D940 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory increment included: rasm1 family (b)); the result
// is read next by C++ after the block.
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
    __int64 index = (__int64)ROUND(MapWidth) * cellY + cellX;
    if (index < 0 || index > 0x7fffffff)
        return 0x40;
    return TopologyWeights._M_start[ROUND(MapWidth) * cellY + cellX];
}
