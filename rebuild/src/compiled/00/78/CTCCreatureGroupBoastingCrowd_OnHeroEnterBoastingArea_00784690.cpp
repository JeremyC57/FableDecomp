// CTCCreatureGroupBoastingCrowd::OnHeroEnterBoastingArea() -- re-enters the crowd's current encouragement state when the quest manager has boasts on offer
// Leaf: OnHeroEnterBoastingArea@CTCCreatureGroupBoastingCrowd
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pQuestManager (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe @ 0x00784690, 22 bytes; Ego_r.pdb ?OnHeroEnterBoastingArea@CTCCreatureGroupBoastingCrowd@@QAEXXZ and private
// ?EnterCrowdState@...@@AAEXW4EBoastingCrowdState@1@@Z (0x00784260). Reads CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (0x013B89FC, as landed 0x006E7460)
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
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
class CQuestManager {
public:
    unsigned char          _pad00[0x68];
    CList<CIntelligentThingPointer> Boasts;                       // +0x68 list<CQuestManager::CBoast> (only empty() is used)
};
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
    if (!CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().Boasts.empty())
        EnterCrowdState(CurrentEncouragementState);
}