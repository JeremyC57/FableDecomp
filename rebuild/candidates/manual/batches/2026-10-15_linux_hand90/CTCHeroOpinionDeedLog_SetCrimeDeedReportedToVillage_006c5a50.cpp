// CTCHeroOpinionDeedLog::SetCrimeDeedReportedToVillage(long) -- marks the deed with this handle as reported to the village, if it is still logged
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C5A50, 44 bytes (+4 int3 padding).
// Callees: map<long,COpinionDeed*>::find(const long&) @ 0x006C77E0 (STLport red-black tree, non-const; the const
//          twin 0x006C7830 is used by GetDeedFromHandle).
//
// Shape: GetDeedFromHandle's frame (one-word hidden-pointer iterator, key by reference to its own argument slot,
// `end()` = the header word re-read after the call) with the test inverted: `je` past a byte store of 1 into
// COpinionDeed::IsCrimeReportedToVillage (+0x22) through the node's mapped pointer (+0x14).
// Layout: OpinionDeedMap +0x0C as in the PDB; COpinionDeed as in the PDB (+0x22 IsCrimeReportedToVillage).

struct COpinionDeed {
    long          DeedType;                   // +0x00 EOpinionDeedType
    long          DeedHandle;                 // +0x04
    float         Position[3];                // +0x08 C3DVector
    long          FrameHappened;              // +0x14
    void*         PTarget[2];                 // +0x18 CIntelligentPointer<const CThing>
    bool          WasSearchingForWitnesses;   // +0x20
    bool          IsStillHappening;           // +0x21
    bool          IsCrimeReportedToVillage;   // +0x22
    bool          IsCrime;                    // +0x23
    long          ExtraData;                  // +0x24
};

struct SOpinionDeedNode {                     // STLport _Rb_tree_node<pair<const long, COpinionDeed*>>
    long              Color;                  // +0x00
    SOpinionDeedNode* Parent;                 // +0x04
    SOpinionDeedNode* Left;                   // +0x08
    SOpinionDeedNode* Right;                  // +0x0C
    long              Key;                    // +0x10
    COpinionDeed*     Value;                  // +0x14
};

struct COpinionDeedIterator {
    SOpinionDeedNode* m_pNode;
    COpinionDeedIterator(SOpinionDeedNode* node) : m_pNode(node) {}
    bool operator!=(const COpinionDeedIterator& other) const { return m_pNode != other.m_pNode; }
    SOpinionDeedNode* operator->() const { return m_pNode; }
};

struct COpinionDeedMap {                      // retail map<long, COpinionDeed*>: header pointer, count, compare
    SOpinionDeedNode* m_pHeader;              // +0x00
    unsigned long     m_count;                // +0x04
    unsigned char     m_compare[4];           // +0x08
    COpinionDeedIterator find(const long& key);            // 0x006C77E0
    COpinionDeedIterator end() { return COpinionDeedIterator(m_pHeader); }
};

struct CTCHeroOpinionDeedLog_Retail {
    void*           __vftable;                // +0x00
    unsigned char   _pad_0x04[0x8];           // +0x04 CTCBase
    COpinionDeedMap OpinionDeedMap;           // +0x0C

    void SetCrimeDeedReportedToVillage(long handle);
};

void CTCHeroOpinionDeedLog_Retail::SetCrimeDeedReportedToVillage(long handle)
{
    COpinionDeedIterator it = OpinionDeedMap.find(handle);
    if (it != OpinionDeedMap.end())
        it->Value->IsCrimeReportedToVillage = true;
}
