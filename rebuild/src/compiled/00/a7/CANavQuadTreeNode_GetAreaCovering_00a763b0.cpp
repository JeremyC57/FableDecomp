// CANavQuadTreeNode::GetAreaCovering() const -- the square this node covers: position +/- half the level's cell size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A763B0, length 61 bytes (+ int3 padding).
// Globals: the per-level cell size table (float[] @ 0x0129DCAC, indexed by QuadTreeLevel at +0xc,
// a `movzx` byte), the 0.5f constant in the float pool @ 0x0122F59C.
// Shape: `half = size[level] * 0.5f` lives in the `push ecx` slot (`fst` keeps it on the x87 stack
// for the first use); the four box edges are computed right to left (Max.Y, Max.X, Min.Y, Min.X)
// as constructor arguments and stored through the hidden result pointer (`ret 4`), so C2DBoxF
// needs a user constructor. Position is C2DVector X +0x4, Y +0x8 (engine header).
extern float g_quadTreeLevelCellSize[];          // 0x0129DCAC

class C2DBoxF { public:
    float MinX, MinY, MaxX, MaxY;
    C2DBoxF(float minX, float minY, float maxX, float maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) {}
};

class CANavQuadTreeNode { public:
    void* __vftable;                            // +0x00
    float Position_X;                           // +0x04
    float Position_Y;                           // +0x08
    unsigned char QuadTreeLevel;                // +0x0c
    unsigned char NavLayer;                     // +0x0d
    bool LeafNode;                              // +0x0e
    bool Blocked;                               // +0x0f
    C2DBoxF GetAreaCovering() const;
};

C2DBoxF CANavQuadTreeNode::GetAreaCovering() const
{
    float half = g_quadTreeLevelCellSize[QuadTreeLevel] * 0.5f;
    return C2DBoxF(Position_X - half, Position_Y - half, Position_X + half, Position_Y + half);
}