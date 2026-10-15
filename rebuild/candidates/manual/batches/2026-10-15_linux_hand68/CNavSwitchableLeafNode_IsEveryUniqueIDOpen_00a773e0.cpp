// CNavSwitchableLeafNode::IsEveryUniqueIDOpen() const -- true when every entry of the node's unique-ID open map is marked open
// Leaf: IsEveryUniqueIDOpen
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A773E0, length 53 bytes.
// Callees: none.
// Identity: the manifest label CPlayerManager::PrepareForLoad is propagated; the callers are
// CNavSwitchableLeafNode::Unlock 0x00A77420 and SetAsBlocked 0x00A78800, and Ego_r.pdb has the private
// ?IsEveryUniqueIDOpen@CNavSwitchableLeafNode@@ABE_NXZ over UniqueIDsOpen (CVectorMap<unsigned __int64, bool>,
// 16-byte entries) at +0x20.
// Model: engine/CNavSwitchableLeafNode.h (UniqueIDsOpen at +0x20 as opaque PDB bytes) viewed through the retail
// 12-byte vector of 16-byte entries.
// Shape: index loop with the element pointer strength-reduced (`add eax,10h`, flag at +8) and the size
// re-derived per iteration; `mov al,1` / `xor al,al` returns.
#include "engine/CNavSwitchableLeafNode.h"                 // UniqueIDsOpen at +0x20 (opaque PDB bytes; the retail map is one 12-byte vector)

struct SUniqueIDOpen {                                      // CVectorMap<unsigned __int64, bool> entry, 16 bytes
    unsigned __int64 ID;
    bool             Open;
};

struct CUniqueIDOpenMap {                                   // retail CVectorMap = one std::vector (12 bytes)
    SUniqueIDOpen* m_pFirst;
    SUniqueIDOpen* m_pLast;
    SUniqueIDOpen* m_pEnd;
    unsigned int size() const { return m_pLast - m_pFirst; }
    const SUniqueIDOpen& operator[](unsigned int i) const { return m_pFirst[i]; }
};

struct CNavSwitchableLeafNode_Methods : CNavSwitchableLeafNode {
    const CUniqueIDOpenMap& GetUniqueIDsOpen() const { return *reinterpret_cast<const CUniqueIDOpenMap*>(UniqueIDsOpen); }
    bool IsEveryUniqueIDOpen() const;
};

bool CNavSwitchableLeafNode_Methods::IsEveryUniqueIDOpen() const
{
    for (unsigned int i = 0; i < GetUniqueIDsOpen().size(); ++i) {
        if (!GetUniqueIDsOpen()[i].Open)
            return false;
    }
    return true;
}
