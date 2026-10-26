// CLandscapeBackgroundTreeNode::BuildDefaultSection(unsigned short mapX, unsigned short mapY, unsigned short width, unsigned short height) -- make this node a default section: store its map rectangle, mark LOD records 1..7 loaded, create its background patch (allocated from the landscape memory manager) and let it build its default section, then reset the node's bounding box to the rectangle in world map units
// Leaf: BuildDefaultSection
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF6F80, length 373 bytes, `ret 10h`.
// Callees: CLandscapeBackgroundPatch::operator new(unsigned int, CLandscapeBackgroundTreeNode*) @ 0x00BE5BB0
//   (`__fastcall`: size in ecx, owner in edx), CLandscapeBackgroundPatch::CLandscapeBackgroundPatch(CLandscapeBackgroundTreeNode*,
//   float, float, long, long, long, long, long, long, long, CSubPatchTreeLODLockElement*, CSubPatchTreeLODLockElement*, long,
//   long, long) @ 0x00BE5E70 (0x11C bytes), CLandscapeBackgroundPatch::BuildDefaultSection @ 0x00BE6880.
// Layout: the retail node layout of the landed constructor 0x00BF6E20 (LODLoadInfo +0x0C, SubNode +0xA0, Map +0xA4,
//   BoundingBox +0xA8, MapX..Height +0xD8..+0xDE); the map's origin (unsigned shorts) at +0x60/+0x62 of the map info
//   (+0x14 of the CEngineLandscapeMap).
// Shape (retry 10b): the patch constructor's height bounds come through inline float getters on the box
//   (BoundingBox.GetMinZ()/GetMaxZ()): VC7.1 evaluates them before the other arguments and parks them in the dead
//   mapX/mapY slots, as retail; the corners are written back through an inline C3DBoundingBox::Set(min, max) (implicit
//   C3DVector assignment, user copy ctor), giving retail's ecx/eax Min/Max pointers.
struct CEngineLandscapeMapInfo {
    unsigned char  _pad[0x60];
    unsigned short OriginX;                        // +0x60
    unsigned short OriginY;                        // +0x62
};
class CEngineLandscapeMap { public:
    unsigned char            _pad[0x14];
    CEngineLandscapeMapInfo* Info;                 // +0x14
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class C3DBoundingBox { public:
    C3DVector Min;
    C3DVector Max;
    C3DBoundingBox() { }
    C3DBoundingBox(const C3DVector& mn, const C3DVector& mx) : Min(mn), Max(mx) { }
    void Set(const C3DVector& mn, const C3DVector& mx) { Min = mn; Max = mx; }
    float GetMinZ() const { return Min.Z; }
    float GetMaxZ() const { return Max.Z; }
};
struct CLODLoadInfo {
    long FileBlockPos;
    long FileBlockSize;
    long OffsetIntoFileBlock;
    bool Loaded;
};
class CSubPatchTreeLODLockElement;
class CLandscapeBackgroundTreeNode;
class CLandscapeBackgroundPatch { public:
    static void* __fastcall operator new(unsigned int size, CLandscapeBackgroundTreeNode* owner);   // 0x00BE5BB0
    CLandscapeBackgroundPatch(CLandscapeBackgroundTreeNode* parent, float minZ, float maxZ, long mapX, long mapY, long width,
                              long height, long lodBand, long a, long b, CSubPatchTreeLODLockElement* lockU,
                              CSubPatchTreeLODLockElement* lockV, long c, long d, long e);   // 0x00BE5E70
    void BuildDefaultSection();                    // 0x00BE6880
    unsigned char _body[0x11c];
};
class CLandscapeBackgroundTreeNode {
public:
    virtual ~CLandscapeBackgroundTreeNode();
    void*                         m_04;            // +0x04
    void*                         m_08;            // +0x08
    CLODLoadInfo                  LODLoadInfo[8];  // +0x0c
    long                          FileBlockPos;    // +0x8c
    long                          FileBlockSize;   // +0x90
    long                          OffsetIntoFileBlock;   // +0x94
    void*                         AsyncLoadTask;   // +0x98
    CLandscapeBackgroundTreeNode* Parent;          // +0x9c
    CLandscapeBackgroundPatch*    SubNode;         // +0xa0
    CEngineLandscapeMap*          Map;             // +0xa4
    C3DBoundingBox                BoundingBox;     // +0xa8
    unsigned char                 _padC0[0xd8 - 0xc0];
    unsigned short                MapX;            // +0xd8
    unsigned short                MapY;            // +0xda
    unsigned short                Width;           // +0xdc
    unsigned short                Height;          // +0xde
    void BuildDefaultSection(unsigned short mapX, unsigned short mapY, unsigned short width, unsigned short height);
};

void CLandscapeBackgroundTreeNode::BuildDefaultSection(unsigned short mapX, unsigned short mapY, unsigned short width, unsigned short height)
{
    MapX = mapX;
    MapY = mapY;
    Width = width;
    Height = height;
    for (int i = 1; i < 8; ++i)
        LODLoadInfo[i].Loaded = true;
    CLandscapeBackgroundPatch* patch = new (this) CLandscapeBackgroundPatch(this, BoundingBox.GetMinZ(), BoundingBox.GetMaxZ(), MapX, MapY,
                                                                            Width, Height, 1, 0, 0, 0, 0, 0, 0, 0);
    patch->BuildDefaultSection();
    SubNode = patch;
    CEngineLandscapeMapInfo* info = Map->Info;
    C3DVector maxCorner;
    maxCorner.X = (float)(Width + info->OriginX + MapX);
    C3DVector minCorner;
    minCorner.Z = 0.0f;
    maxCorner.Y = (float)(Height + info->OriginY + MapY);
    maxCorner.Z = 0.0f;
    minCorner.X = (float)(info->OriginX + MapX);
    minCorner.Y = (float)(info->OriginY + MapY);
    BoundingBox.Set(minCorner, maxCorner);
}