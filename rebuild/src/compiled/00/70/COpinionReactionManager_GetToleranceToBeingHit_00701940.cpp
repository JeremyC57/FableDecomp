// COpinionReactionManager::GetToleranceToBeingHit(const CThing* thing) const -- how much hitting the thing tolerates: the def's ToleranceToBeingHit entry for the thing's attitude to the player, 0 when the attitude has no entry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00701940, length 68 bytes (`ret 4`, float in st0). Callees:
// COpinionReactionManager::GetThingAttitudeToPlayer @ 0x00701840, the map's find @ 0x00701FC0 (manifest
// "BinaryTree_FindClosestLessEqual"; hidden iterator result, key by const reference), the 0.0f pool constant @ 0x0122DEDC.
// Layout: PDef (the def object word) at +0; COpinionReactionManagerDef::ToleranceToBeingHit (map<EOpinionAttitudeType,
// float>) at retail +0x6C (PDB +0x80), an STLport tree whose header pointer is the map's first word (end()) and whose
// node holds the value at +0x10 (key) / +0x14 (float). Shape: one `push ecx` frame word holds the returned iterator;
// the attitude (an address-taken named local) is homed in the dead `thing` argument slot (`mov [esp+14h],eax` right
// before the find call); the map reference is kept in esi for both find and end(); `it != end` with the found arm
// first, `return 0.0f` loads the pool constant (no `fldz`).
// Leaf: GetToleranceToBeingHit
// Naming (NF173, 2026-11-20): q4078: the local COpinionReactionManager model declares GetThingAttitudeToPlayer(const CThing&) const as Ego_r.pdb 0x00551920 / FableWin.pdb 0x01F15FA0 spell it (was const CThing*); bytes unchanged (a reference passes the address).
// Leaf: ?GetToleranceToBeingHit@COpinionReactionManager@@QBEMABVCThing@@@Z
// Naming (NF177, PRIORITY q12095): Ego_r.pdb 0x00551E10 + FableWin.pdb 0x01F16E60 take the thing by const reference (was the pointer form
//   ?GetToleranceToBeingHit@COpinionReactionManager@@QBEMPBVCThing@@@Z, in neither PDB); same pushed address. Its three landed callers
//   0x00681940 / 0x00681980 / 0x006819D0 move with it.
class CThing;
enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE = 0 };

struct SToleranceNode {                            // STLport red-black node: colour, parent, left, right, value
    long                 m_color;                  // +0x00
    SToleranceNode*      m_pParent;                // +0x04
    SToleranceNode*      m_pLeft;                  // +0x08
    SToleranceNode*      m_pRight;                 // +0x0C
    EOpinionAttitudeType first;                    // +0x10
    float                second;                   // +0x14
};

struct CToleranceIterator {
    SToleranceNode* m_pNode;
    CToleranceIterator() {}
    CToleranceIterator(SToleranceNode* node) : m_pNode(node) {}
    SToleranceNode* operator->() const { return m_pNode; }
    bool operator!=(const CToleranceIterator& other) const { return m_pNode != other.m_pNode; }
};

struct CToleranceMap {                             // map<EOpinionAttitudeType, float>
    SToleranceNode* m_pHeader;
    CToleranceIterator find(const EOpinionAttitudeType& key) const;   // 0x00701FC0
    CToleranceIterator end() const { return CToleranceIterator(m_pHeader); }
};

class COpinionReactionManagerDef { public:
    unsigned char _pad00[0x6C];
    CToleranceMap ToleranceToBeingHit;             // +0x6C (PDB +0x80)
};

class COpinionReactionManager {
public:
    const COpinionReactionManagerDef* PDef;        // +0x00
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing& thing) const;   // 0x00701840 (Ego_r.pdb + FableWin.pdb: by reference)
    float GetToleranceToBeingHit(const CThing& thing) const;
};

float COpinionReactionManager::GetToleranceToBeingHit(const CThing& thing) const
{
    EOpinionAttitudeType attitude = GetThingAttitudeToPlayer(thing);
    const CToleranceMap& tolerance = PDef->ToleranceToBeingHit;
    CToleranceIterator it = tolerance.find(attitude);
    if (it != tolerance.end())
        return it->second;
    return 0.0f;
}