// CANavQuadTreeNode::GetAreaCovering() const -- the square this node covers: position +/- half the level's cell size
// Leaf: ?GetAreaCovering@CANavQuadTreeNode@@QBE?AVC2DBoxF@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A763B0, length 61 bytes (+ int3 padding).
// Globals: the per-level cell size table g_QuadTreeLevelSizes (const float[] @ 0x0129DCAC: 32, 16, 8, 4, 2, 1, 0.5; descriptive,
// no PDB data symbol; the spelling of the landed 0x00A76640 / 0x00A76CC0 / 0x00A78E20 rows, was g_quadTreeLevelCellSize, landing
// 2026-11-03g), indexed by QuadTreeLevel at +0xc (a `movzx` byte), the 0.5f constant in the float pool @ 0x0122F59C.
// Shape: `half = size[level] * 0.5f` lives in the `push ecx` slot (`fst` keeps it on the x87 stack
// for the first use); the four box edges are computed right to left (Max.Y, Max.X, Min.Y, Min.X)
// as constructor arguments and stored through the hidden result pointer (`ret 4`), so C2DBoxF
// needs a user constructor. Position is C2DVector X +0x4, Y +0x8 (engine header).
extern const float g_QuadTreeLevelSizes[];       // 0x0129DCAC

class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;                   // Ego_r.pdb member names
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) {}
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
    float half = g_QuadTreeLevelSizes[QuadTreeLevel] * 0.5f;
    return C2DBoxF(Position_X - half, Position_Y - half, Position_X + half, Position_Y + half);
}