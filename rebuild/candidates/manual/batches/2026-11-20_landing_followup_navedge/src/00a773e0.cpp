// CNavSwitchableLeafNode::IsEveryUniqueIDOpen() const -- true when every entry of the node's unique-ID open map is marked open
// Leaf: ?IsEveryUniqueIDOpen@CNavSwitchableLeafNode@@ABE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A773E0, length 53 bytes.
// Callees: none.
// Identity: the manifest label CPlayerManager::PrepareForLoad is propagated; the callers are CNavSwitchableLeafNode::Unlock
//   0x00A77420 and SetAsBlocked 0x00A78800, and Ego_r.pdb has the private ?IsEveryUniqueIDOpen@CNavSwitchableLeafNode@@ABE_NXZ.
// Re-port (audit h3c2b, 2026-11-20): onto engine/CNavSwitchableLeafNode.h (FABLE_RETAIL_CNAVSWITCHABLELEAFNODE; UniqueIDsOpen is the
//   shared CVectorMap<unsigned __int64, bool> at +0x20; FABLE_RETAIL_CNAVSWITCHABLELEAFNODE_SETASBLOCKED declares the member) in place of
//   the local view structs, in Ego_d.pdb's spelling (lib_nav_quad_tree.obj lines 592..601: one block local `long c1`; for 593, if 595,
//   `return false` 597, `return true` 600). The same text is the same-TU strong copy in row 0x00A78800.
// Shape: index loop with the element pointer strength-reduced (`add eax,10h`, flag at +8) and the size re-derived per iteration.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CNAVSWITCHABLELEAFNODE
#define FABLE_RETAIL_CNAVSWITCHABLELEAFNODE_SETASBLOCKED
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/CNavSwitchableLeafNode.h"
bool CNavSwitchableLeafNode::IsEveryUniqueIDOpen() const
{
    for (long c1 = 0; c1 < UniqueIDsOpen.size(); c1++)
    {
        if (!UniqueIDsOpen[c1].second)
            return false;
    }
    return true;
}
