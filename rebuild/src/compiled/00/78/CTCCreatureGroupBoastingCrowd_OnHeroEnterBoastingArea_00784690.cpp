// CTCCreatureGroupBoastingCrowd::OnHeroEnterBoastingArea() -- re-enters the crowd's current encouragement state when the quest manager has boasts on offer
// Leaf: OnHeroEnterBoastingArea@CTCCreatureGroupBoastingCrowd
// Retail: Fable.exe @ 0x00784690, 22 bytes; Ego_r.pdb ?OnHeroEnterBoastingArea@CTCCreatureGroupBoastingCrowd@@QAEXXZ and private
// ?EnterCrowdState@...@@AAEXW4EBoastingCrowdState@1@@Z (0x00784260). Reads g_pQuestManager (0x013B89FC, as landed 0x006E7460)
// +0x68 and tests the STLport list for emptiness (`cmp [node],node`). Layout (retail STLport 4.6, engine/CQuestManager.h and
// engine/CTCCreatureGroupBoastingCrowd.h): +0x68 is CQuestManager::Boasts (list<CBoast>, as the landed GetBoast 0x004AF7A0),
// and +0x30 is CurrentEncouragementState (EnterCrowdState 0x00784260 compares CrowdState at +0x2c, PHero at +0x24).
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
    CList<CIntelligentThingPointer> Boasts;                       // +0x68 list<CQuestManager::CBoast> (only empty() is used)
};
extern CQuestManager* g_pQuestManager;                   // 0x013B89FC
enum EBoastingCrowdState { BOASTING_CROWD_STATE_NONE };
class CTCCreatureGroupBoastingCrowd {
public:
    unsigned char       _pad00[0x2c];
    EBoastingCrowdState CrowdState;                      // +0x2c
    EBoastingCrowdState CurrentEncouragementState;       // +0x30
    void OnHeroEnterBoastingArea();
private:
    void EnterCrowdState(EBoastingCrowdState state);     // 0x00784260
};
void CTCCreatureGroupBoastingCrowd::OnHeroEnterBoastingArea()
{
    if (!g_pQuestManager->Boasts.empty())
        EnterCrowdState(CurrentEncouragementState);
}