// CWorld::IsPerformingRegionLoad() const -- whether a region change is in progress (load status neither idle nor finished)
// Leaf: IsPerformingRegionLoad@CWorld
// Retail: Fable.exe @ 0x0048DB13, 22 bytes. Catalog label CPersistContext::IsBinary is wrong: retail CPersistContext is the
// 44-byte PDB class (ctor 0x009BAA30 writes +0x18/+0x24/+0x28 only), while this reads +0x104. The only `mov [reg+0x104],9`
// in .text is in CWorld::UpdateRegionLoad (0x004A5568), and Ego_r.pdb CWorld::RegionLoadStatus is ERegionLoadStatus at
// +0x110 (retail -0xc: two STLport vectors and the CArray before it are smaller). Callers test al (bool): e.g.
// CSubtitleRenderer::IsAllowedToSetText 0x0049095A, CDrawMiniMap::Update 0x00644049, CDrawScreenFade::Draw 0x00649748.
// Ego_r.pdb has both ?IsPerformingRegionLoad@CWorld@@QBE_NXZ and ?IsPerformingRegionLoadReally@CWorld@@QBE_NXZ; which one
// this is cannot be told from the bytes.
// Layout: retail CWorld RegionLoadStatus +0x104 (engine/CWorld.h carries the PDB +0x110).
// Shape: size TU; the `&&` expression gives `xor eax,eax; inc eax` for true.
#pragma optimize("s", on)
enum ERegionLoadStatus { REGION_LOAD_NONE = 0, REGION_LOAD_FINISHED = 9 };
class CWorld {
public:
    unsigned char     _pad00[0x104];
    ERegionLoadStatus RegionLoadStatus;                  // +0x104
    bool IsPerformingRegionLoad() const;
};
bool CWorld::IsPerformingRegionLoad() const
{
    return RegionLoadStatus != REGION_LOAD_NONE && RegionLoadStatus != REGION_LOAD_FINISHED;
}