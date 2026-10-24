// CTCOracleMinigame::ProcessInput(EOracleMinigameInput input) -- while the game waits for input (stage 4) and fewer symbols are pending than the level allows, queue the input and push its symbol onto the quest-info HUD; true when taken
// Leaf: ?ProcessInput@CTCOracleMinigame@@QAE_NW4EOracleMinigameInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E5F70, 120 bytes (`ret 4` + int3 padding). FableWin.pdb
//   `?ProcessInput@CTCOracleMinigame@@QAE_NW4EOracleMinigameInput@@@Z` (public thiscall); the catalog label
//   `CInputProcessOracleMinigame_PushSymbolToQueue` was an autoname guess (the neighbours are CTCOracleMinigame members).
// Layout: CTCOracleMinigame (Ego_r.pdb): Level +0x10, Stage +0x14 (EOracleMinigameState), NumOutputsFired +0x24,
//   InputsPending +0xe0 (STLport list<EOracleMinigameInput>: one head-node word in retail; Ego_r's 12 bytes are the
//   PDB build's list).
// Callees: malloc 0x00BFEA0E (the STLport list node, 12 bytes), NPlayerGui::CDrawQuestInfo::OracleSymbolPushBack
//   (const CCharString&) @ 0x0064A3A1 on g_pPlayerGui (0x013B8790) +0x1c8; the symbol names are a static array of four
//   CCharStrings at 0x013BAE94 (its atexit destructor loop is 0x01229DF0).
// Shape: STLport's walking size() first, then `NumOutputsFired + size() < Level` (unsigned compare: `jae`);
//   push_back = insert(end(), x) with the placement-construct null test on the node's data.
extern "C" void* __cdecl malloc(unsigned int size);              // 0x00BFEA0E

class CCharString {
public:
    void* m_pData;
};

template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CListIterator {
    SListNode<T>* _M_node;
    CListIterator(SListNode<T>* n) : _M_node(n) { }
};
template <class T> struct CRetailList {                          // STLport list (4 bytes: the head node)
    SListNode<T>* _M_node;
    CListIterator<T> end() { return CListIterator<T>(_M_node); }
    unsigned int size() const
    {
        unsigned int n = 0;
        for (SListNode<T>* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    CListIterator<T> insert(CListIterator<T> position, const T& x)
    {
        SListNode<T>* tmp = (SListNode<T>*)malloc(sizeof(SListNode<T>));
        T* data = &tmp->_M_data;
        if (data != 0)
            *data = x;
        SListNode<T>* n = position._M_node;
        SListNode<T>* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return CListIterator<T>(tmp);
    }
    void push_back(const T& x) { insert(end(), x); }
};

namespace NPlayerGui {
class CDrawQuestInfo {
public:
    void OracleSymbolPushBack(const CCharString& symbol);       // 0x0064A3A1
};
}

template <class T> struct CCountedPointer {
    T*    Data;
    void* Count;
    T* operator->() const { return Data; }
};

class CPlayerGui {
public:
    unsigned char                               _pad000[0x1c8];
    CCountedPointer<NPlayerGui::CDrawQuestInfo> PDrawQuestInfo; // +0x1c8
};
extern CPlayerGui* g_pPlayerGui;                                 // 0x013B8790

enum EOracleMinigameInput { ORACLE_INPUT_0 = 0 };
enum EOracleMinigameState { ORACLE_STAGE_WAITING_FOR_INPUT = 4 };

extern CCharString g_OracleSymbolNames[4];                       // 0x013BAE94

class CTCOracleMinigame {
public:
    virtual void v00();
    void*                              ParentThing;              // +0x04
    unsigned char                      _pad08[4];
    unsigned long                      CurrentText;              // +0x0c
    long                               Level;                    // +0x10
    EOracleMinigameState               Stage;                    // +0x14
    EOracleMinigameState               PostWaitStage;            // +0x18
    long                               WaitTime;                 // +0x1c
    float                              TimeBetweenOutputs;       // +0x20
    long                               NumOutputsFired;          // +0x24
    unsigned char                      _pad28[0xe0 - 0x28];
    CRetailList<EOracleMinigameInput>  InputsPending;            // +0xe0
    bool ProcessInput(EOracleMinigameInput input);
};

bool CTCOracleMinigame::ProcessInput(EOracleMinigameInput input)
{
    if (NumOutputsFired + InputsPending.size() < (unsigned int)Level && Stage == ORACLE_STAGE_WAITING_FOR_INPUT) {
        InputsPending.push_back(input);
        g_pPlayerGui->PDrawQuestInfo->OracleSymbolPushBack(g_OracleSymbolNames[input]);
        return true;
    }
    return false;
}