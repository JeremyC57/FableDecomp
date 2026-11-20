// CMeshBankStateBlock::CLODStateBlock::operator=(const CMeshBankStateBlock::CLODStateBlock& other) -- memberwise assignment: the leading part through its out-of-line operator=, then the counted pointer (release the old block, adopt, add-ref)
// Leaf: ??4CLODStateBlock@CMeshBankStateBlock@@QAEAAV01@ABV01@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a2cf50, 80 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ??4CLODStateBlock@CMeshBankStateBlock@@QAEAAV01@ABV01@@Z; its caller CMeshBankStateBlock::Set 0x00A2D6C0 (landed) assigns the block at +0x14 (Ego_r.pdb CMeshBankStateBlock::LODState); Ego_r.pdb members BoneLODs (map<CCharString, long>, the out-of-line operator= 0x00432B38) and MinXYZRange (CCountedPointer<C3DBoundingBox>, retail +0x0C).
// Callees: map<CCharString, long>::operator= 0x00432B38, operator delete 0x00BFE9BC (the old count block); CCountedPointer's operator= inline.
// Shape (family L088): the compiler-generated copy assignment, written out (same body) so the translation unit emits it.
#include "engine/CCountedPointer.h"
class CCharString { public: void* PStringData; };
class C3DBoundingBox;
// NF108 (2026-11-19, q6236): the local std::map model (operator= declared on map) replaced by the shared STLport map
//   (stlport/fable_map.h): STLport's map::operator= is implicit and inline (`_M_t = x._M_t`), so 0x00432B38 is the tree's out-of-line
//   ??4?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@J@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@J@std@@@3@@std@@QAEAAV01@ABV01@@Z
//   (as the landed 0x004321F0 / 0x0046CE2D bind it).
#include "stlport/fable_map.h"
class CMeshBankStateBlock {
public:
    class CLODStateBlock {
    public:
        std::map<CCharString, long>     BoneLODs;          // +0x00
        CCountedPointer<C3DBoundingBox> MinXYZRange;       // +0x0C
        CLODStateBlock& operator=(const CLODStateBlock& other);
    };
};
CMeshBankStateBlock::CLODStateBlock& CMeshBankStateBlock::CLODStateBlock::operator=(const CLODStateBlock& other)
{
    BoneLODs = other.BoneLODs;
    MinXYZRange = other.MinXYZRange;
    return *this;
}