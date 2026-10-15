#include <cstdio>
// Behaviour test for COpinionReactionMaskDef::Transfer @ 0x0045DE67: persist both members, then drop every entry keyed below 18
struct SReactionNode { long m_color; SReactionNode* m_pParent; SReactionNode* m_pLeft; SReactionNode* m_pRight; long first; bool second; bool erased; };
struct SRbGlobal { static SReactionNode* Increment(SReactionNode* node) { return node->m_pRight; } };   // a right-leaning chain
class CReactionIterator {
public:
    SReactionNode* m_pNode;
    CReactionIterator(SReactionNode* node) : m_pNode(node) {}
    CReactionIterator(const CReactionIterator& other) : m_pNode(other.m_pNode) {}
    SReactionNode* operator->() const { return m_pNode; }
    CReactionIterator& operator++() { m_pNode = SRbGlobal::Increment(m_pNode); return *this; }
    bool operator!=(const CReactionIterator& other) const { return m_pNode != other.m_pNode; }
};
static int g_erased = 0;
class CReactionEnabledMap {
public:
    SReactionNode* m_pHeader;
    CReactionIterator begin() const { return CReactionIterator(m_pHeader->m_pLeft); }
    CReactionIterator end() const { return CReactionIterator(m_pHeader); }
    void erase(CReactionIterator position)
    {
        SReactionNode* prev = m_pHeader;
        while (prev->m_pRight != position.m_pNode) prev = prev->m_pRight;
        prev->m_pRight = position.m_pNode->m_pRight;
        if (m_pHeader->m_pLeft == position.m_pNode) m_pHeader->m_pLeft = position.m_pNode->m_pRight;
        position.m_pNode->erased = true; position.m_pNode->m_pRight = 0; ++g_erased;
    }
};
static int g_boolTransfers = 0, g_mapTransfers = 0;
class CPersistContext {
public:
    void Transfer(bool& value) { ++g_boolTransfers; value = true; }
    void Transfer(CReactionEnabledMap&) { ++g_mapTransfers; }
};
class COpinionReactionMaskDef {
public:
    virtual ~COpinionReactionMaskDef() {}
    bool ReactionEnabledDefault;
    CReactionEnabledMap ReactionEnabled;
    virtual void Transfer(CPersistContext& context)
    {
        context.Transfer(ReactionEnabledDefault);
        context.Transfer(ReactionEnabled);
        CReactionIterator end = ReactionEnabled.end();
        CReactionIterator it = ReactionEnabled.begin();
        CReactionIterator next = it;
        while (it != end) {
            ++next;
            if (it->first < 18)
                ReactionEnabled.erase(it);
            it = next;
        }
    }
};
int main()
{
    static const long keys[5] = { 3, 17, 18, 25, 40 };
    SReactionNode header; SReactionNode nodes[5];
    header.m_pLeft = &nodes[0]; header.m_pRight = &nodes[0];
    for (int i = 0; i < 5; ++i) { nodes[i].first = keys[i]; nodes[i].second = true; nodes[i].erased = false; nodes[i].m_pLeft = 0; nodes[i].m_pRight = (i < 4) ? &nodes[i + 1] : &header; }
    COpinionReactionMaskDef def; def.ReactionEnabledDefault = false; def.ReactionEnabled.m_pHeader = &header;
    CPersistContext context;
    def.Transfer(context);
    if (g_boolTransfers != 1 || g_mapTransfers != 1 || !def.ReactionEnabledDefault || g_erased != 2 || !nodes[0].erased || !nodes[1].erased ||
        nodes[2].erased || nodes[3].erased || nodes[4].erased || header.m_pLeft != &nodes[2]) { std::printf("0045de67_TEST FAIL\n"); return 1; }
    std::printf("0045de67_TEST PASS\n");
    return 0;
}