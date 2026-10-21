// CTCCreatureGroupBoastingCrowd::OnHeroEnterBoastingArea() -- re-enters the crowd's current state when the quest manager has active quest cards
// Leaf: OnHeroEnterBoastingArea@CTCCreatureGroupBoastingCrowd
// Retail: Fable.exe @ 0x00784690, 22 bytes; Ego_r.pdb ?OnHeroEnterBoastingArea@CTCCreatureGroupBoastingCrowd@@QAEXXZ and private
// ?EnterCrowdState@...@@AAEXW4EBoastingCrowdState@1@@Z (0x00784260). Reads g_pQuestManager (0x013B89FC, as landed 0x006E7460)
// +0x68 and tests the STLport list for emptiness (`cmp [node],node`). Which Ego_r.pdb list this is is inferred: retail
// CQuestManager shrinks every list/vector, and ActiveQuestCards (PDB +0x88) is the one a boast precondition would read.
// Layout: CrowdState +0x30 as in Ego_r.pdb (PHero CIntelligentPointer +0x28 precedes it).
template <class T> class CList {                         // STLport list: one node pointer
public:
    struct CNode { CNode* _M_next; CNode* _M_prev; T _M_data; };
    CNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
class CThing;
struct CIntelligentThingPointer { void* _pad00; void* PData; };   // CIntelligentPointer<CThing> (8 bytes)
class CQuestManager {
public:
    unsigned char          _pad00[0x68];
    CList<CIntelligentThingPointer> ActiveQuestCards;             // +0x68
};
extern CQuestManager* g_pQuestManager;                   // 0x013B89FC
enum EBoastingCrowdState { BOASTING_CROWD_STATE_NONE };
class CTCCreatureGroupBoastingCrowd {
public:
    unsigned char       _pad00[0x30];
    EBoastingCrowdState CrowdState;                      // +0x30
    void OnHeroEnterBoastingArea();
private:
    void EnterCrowdState(EBoastingCrowdState state);     // 0x00784260
};
void CTCCreatureGroupBoastingCrowd::OnHeroEnterBoastingArea()
{
    if (!g_pQuestManager->ActiveQuestCards.empty())
        EnterCrowdState(CrowdState);
}