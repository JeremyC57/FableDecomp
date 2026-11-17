// CVertexGroupBlock::IsProperSubsetOf(const CVertexGroupBlock& other) -- every group of this block is in `other` and `other` has strictly more groups
// Grade: RELOCATION_MATCH -- same levers as IsSubsetOf 0x00AD8C30: named iterator local + Groups[i] index form.
// Leaf: ?IsProperSubsetOf@CVertexGroupBlock@@QBE_NABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD8CA0, length 105 bytes (padded to 112), `ret 4`.
// Callees: map<unsigned long, long>::find(iterator* result, const unsigned long& key) @ 0x006699C0 (landed as
// CRetailTree<ulong key>::find; header at the map's +0, end() re-read after the call).
// Layout (retail; PDB 32 bytes with debug containers): Groups CArray<unsigned long> +0x00 (12-byte vector), GroupToIndexMap +0x0c.
// Shape: sizes compared unsigned (`sar 2` then `ja`/`jae`); the group key is a named local homed in the dead `other`
// argument slot and passed to find by address; the result is a named iterator (its storage is the hidden-result slot,
// re-read after the call: `mov eax,[ebp] ; cmp [esp+10h],eax`) compared with the header; the loop indexes Groups[i].
// NF50 (round 7): moved onto the shared engine/CVertexGroupBlock.h (FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS: Ego_r.pdb Groups
// CArray<unsigned long> +0, GroupToIndexMap map<unsigned long,long> +0xC; FableWin.pdb HasGroup / GetGroupCount inline) with the
// STLport const find kept out of line (FABLE_RETAIL_STLP_TREE_CONSTFIND_NOINLINE, the 0x00AD9F10 precedent), so the call @ 0x006699C0
// binds ?find@?$_Rb_tree@KU?$pair@$$CBKJ@std@@U?$_Select1st@U?$pair@$$CBKJ@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKJ@std@@@2@@std@@QBE?AU?$_Rb_tree_iterator@U?$pair@$$CBKJ@std@@U?$_Const_traits@U?$pair@$$CBKJ@std@@@2@@2@ABK@Z
// (nf44 entry 85; was the local CGroupToIndexMap::find). The local CGroupArray / CGroupToIndexMap / node models are gone.
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_NOINLINE
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#include "engine/CArray.h"
#include "stlport/fable_map.h"
#include "engine/CVertexGroupBlock.h"

bool CVertexGroupBlock::IsProperSubsetOf(const CVertexGroupBlock& other) const
{
    unsigned long count = GetGroupCount();
    if (count < other.GetGroupCount()) {
        for (unsigned long i = 0; i < count; ++i) {
            if (!other.HasGroup(GetGroup(i)))
                return false;
        }
        return true;
    }
    return false;
}
