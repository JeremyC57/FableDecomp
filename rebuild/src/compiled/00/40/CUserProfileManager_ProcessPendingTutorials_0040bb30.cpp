// CUserProfileManager::ProcessPendingTutorials() -- when tutorials are on, pop the next pending tutorial category and give it unless the profile already had it
// Leaf: ?ProcessPendingTutorials@CUserProfileManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040BB30, 89 bytes (+7 int3 pad). Callees:
// deque<ETutorialCategory>::pop_front 0x00410ED0 (manifest Std_Deque_PopFront_ChunkBased),
// CUserProfileManager::IsTutorialCategoryGiven 0x00407330 (manifest label
// CHitParameters::IsDivineWrathStrike is propagated), CUserProfileManager::GiveTutorialForCategory
// 0x0040B790; the global tutorial switch byte 0x01375740.
//
// Retail bytes:  mov al,[g_tutorialsEnabled] ; test al,al ; push esi ; mov esi,ecx ; je X ;
//   mov al,[esi+0xc] ; test al,al ; je A ; mov al,[esi+0xd7] ; jmp B ; A: mov al,[esi+0x17] ;
//   B: test al,al ; je X ; mov eax,[esi+0xa8] ; mov edx,[esi+0x98] ; cmp eax,edx ;
//   lea ecx,[esi+0x98] ; je X ; push edi ; mov edi,[edx] ; call pop_front ; push edi ;
//   mov ecx,esi ; call IsTutorialCategoryGiven ; test al,al ; jne N ; push 0 ; push edi ;
//   mov ecx,esi ; call GiveTutorialForCategory ; N: pop edi ; X: pop esi ; ret
// Identity: FableWin.pdb `?ProcessPendingTutorials@CUserProfileManager@@QAEXXZ` (manifest name
// Tutorial_CheckDivineWrathStrike is descriptive). Members (Ego_r.pdb names, RETAIL offsets: the
// donor's Dinkumware containers are 16/24 bytes, retail STLport vectors 12 and deques 40):
// UseNewProfileValues +0x0C, TutorialSystemEnabled +0x17, PendingTutorialCategories (a
// std::queue over an STLport deque: _M_start {cur,first,last,node} +0x98, _M_finish +0xA8, map,
// map size) +0x98, TutorialSystemEnabledNewProfile +0xD7. IsTutorialSystemEnabled() is the inline
// "new-profile value or current value" byte choice (same accessor as the landed
// CGameScriptInterface::MsgIsTutorialClickedPast 0x008943C0). queue::empty/front/pop are inline;
// deque::pop_front is out of line.
enum ETutorialCategory { TUTORIAL_CATEGORY_NONE = 0 };

extern bool g_tutorialsEnabled;                   // 0x01375740

template <class T>
struct CRetailDequeIterator {
    T* _M_cur;
    T* _M_first;
    T* _M_last;
    T** _M_node;
};

template <class T>
struct CRetailDeque {                             // STLport deque<T>, 40 bytes
    CRetailDequeIterator<T> _M_start;             // +0x00
    CRetailDequeIterator<T> _M_finish;            // +0x10
    T** _M_map;                                   // +0x20
    unsigned long _M_map_size;                    // +0x24
    bool empty() const { return _M_finish._M_cur == _M_start._M_cur; }
    T& front() { return *_M_start._M_cur; }
    void pop_front();                             // 0x00410ED0
};

template <class T>
struct CRetailQueue {                             // std::queue<T, deque<T> >
    CRetailDeque<T> c;
    bool empty() const { return c.empty(); }
    T& front() { return c.front(); }
    void pop() { c.pop_front(); }
};

class CUserProfileManager {
public:
    virtual void v00();
    unsigned char _pad04[0x08];
    bool UseNewProfileValues;                     // +0x0C
    unsigned char _pad0d[0x0A];
    bool TutorialSystemEnabled;                   // +0x17
    unsigned char _pad18[0x80];
    CRetailQueue<ETutorialCategory> PendingTutorialCategories;   // +0x98
    unsigned char _padc0[0x17];
    bool TutorialSystemEnabledNewProfile;         // +0xD7

    bool IsTutorialSystemEnabled() const { return UseNewProfileValues ? TutorialSystemEnabledNewProfile : TutorialSystemEnabled; }
    bool IsTutorialCategoryGiven(ETutorialCategory category) const;          // 0x00407330
    bool GiveTutorialForCategory(ETutorialCategory category, bool force);   // 0x0040B790
    void ProcessPendingTutorials();
};

void CUserProfileManager::ProcessPendingTutorials()
{
    if (g_tutorialsEnabled && IsTutorialSystemEnabled() && !PendingTutorialCategories.empty())
    {
        ETutorialCategory category = PendingTutorialCategories.front();
        PendingTutorialCategories.pop();
        if (!IsTutorialCategoryGiven(category))
            GiveTutorialForCategory(category, false);
    }
}