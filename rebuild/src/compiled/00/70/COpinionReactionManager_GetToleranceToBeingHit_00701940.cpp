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

struct COpinionReactionManagerDef {
    unsigned char _pad00[0x6C];
    CToleranceMap ToleranceToBeingHit;             // +0x6C (PDB +0x80)
};

class COpinionReactionManager {
public:
    const COpinionReactionManagerDef* PDef;        // +0x00
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing* thing) const;   // 0x00701840
    float GetToleranceToBeingHit(const CThing* thing) const;
};

float COpinionReactionManager::GetToleranceToBeingHit(const CThing* thing) const
{
    EOpinionAttitudeType attitude = GetThingAttitudeToPlayer(thing);
    const CToleranceMap& tolerance = PDef->ToleranceToBeingHit;
    CToleranceIterator it = tolerance.find(attitude);
    if (it != tolerance.end())
        return it->second;
    return 0.0f;
}