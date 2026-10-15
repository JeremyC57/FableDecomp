// CTCHeroOpinionDeedLog::GetDeedFromHandle(long) const -- looks a deed handle up in the opinion-deed map, null when absent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C58F0, 46 bytes (+2 int3 padding).
// Callees: map<long,COpinionDeed*>::find(const long&) const @ 0x006C7830 (STLport red-black tree; its non-const
//          twin is 0x006C77E0, an identical body that was not folded).
//
// Shape: `push ecx` = the one-word iterator returned through the hidden pointer (the iterator has a user
// constructor); the key is passed by reference to the argument's own slot (`lea eax,[esp+0Ch]`); `end()` is the
// header pointer at the map's +0 re-read after the call (`mov ecx,[esi]`), and the found path returns the
// node's mapped value (+0x14 = pair<const long, COpinionDeed*>::second, after the STLport node's
// color/parent/left/right words and the key).
// Layout: OpinionDeedMap +0x0C (PDB, unchanged: it is the first member after CTCBase); local model because the
// PDB map is 16 bytes and every later member sits lower in retail.

struct COpinionDeed;

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
    bool operator==(const COpinionDeedIterator& other) const { return m_pNode == other.m_pNode; }
    SOpinionDeedNode* operator->() const { return m_pNode; }
};

struct COpinionDeedMap {                      // retail map<long, COpinionDeed*>: header pointer, count, compare
    SOpinionDeedNode* m_pHeader;              // +0x00
    unsigned long     m_count;                // +0x04
    unsigned char     m_compare[4];           // +0x08
    COpinionDeedIterator find(const long& key) const;      // 0x006C7830
    COpinionDeedIterator end() const { return COpinionDeedIterator(m_pHeader); }
};

struct CTCHeroOpinionDeedLog_Retail {
    void*           __vftable;                // +0x00
    unsigned char   _pad_0x04[0x8];           // +0x04 CTCBase
    COpinionDeedMap OpinionDeedMap;           // +0x0C

    COpinionDeed* GetDeedFromHandle(long handle) const;
};

COpinionDeed* CTCHeroOpinionDeedLog_Retail::GetDeedFromHandle(long handle) const
{
    COpinionDeedIterator it = OpinionDeedMap.find(handle);
    if (it == OpinionDeedMap.end())
        return 0;
    return it->Value;
}
