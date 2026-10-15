// CQuestManager::DeactivateEventQuests() -- fail (silently) every active quest that is running a script, working on a copy of the active-quest list
// Grade: DIFFER(156v149) -- retail keeps the local list's header node in esi across the range-insert, SetQuestAsFailed and clear calls (loop end test, free(esi)) and `this` in ebp; ours reloads the header from the list object after every call (this in ebx), and stores the header before the pushes. Tried: STLport-style list constructor with direct member stores, and with a node local. Next: the real bodies of 0x004B92D0 / 0x004B9DA0 above under auto_inline(off) (unlikely: both write memory), or a list type whose header pointer is a separate local.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B2AD0, length 149 bytes.
// Callees: malloc @ 0x00BFEA0E / free @ 0x00BFEA14 (the local list's header node), STLport
// list::_M_insert_dispatch(iterator pos, const_iterator first, const_iterator last, const __false_type&) @ 0x004B92D0
// (catalog "std::list_Insert_Range"), CWideString::CWideString(const wchar_t*) @ 0x0099B6B0 (L"" @ 0x0122D70C),
// CQuestManager::SetQuestAsFailed(const CCharString&, bool, const CWideString&, bool) @ 0x004B2280,
// CWideString::~CWideString @ 0x0099B510, list::clear @ 0x004B9DA0 (catalog label propagated).
// Layout: ActiveQuests (list<CCountedPointer<CActiveQuest>>, head pointer) at +0x38 (PDB +0x44), node {next, prev,
// Data +8, Info +0xc} (0x10 bytes); retail CActiveQuest PScript +0x08, QuestName +0x30.
// Shape: a local STLport list copy-constructed by range insert (its constructor allocates and self-links the
// header, kept in esi; `pos` is an iterator with a user copy constructor so it is built in the argument area, the
// source iterators are plain); the reason text is a named CWideString local in the loop body (built before the
// pushes); the list destructor clears and frees the cached header.
// Leaf: DeactivateEventQuests
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);      // 0x00BFEA0E
extern "C" void __cdecl free(void* p);             // 0x00BFEA14

struct __false_type { __false_type() {} };

struct CCharString { void* PStringData; };
struct CWideString {
    void* PStringData;
    CWideString(const wchar_t* text);              // 0x0099B6B0
    ~CWideString();                                // 0x0099B510
};

struct CCPPointerInfo { long m_count; };
struct CActiveQuest {
    long          ID;                              // +0x00
    void*         PScriptInfo;                     // +0x04
    void*         PScript;                         // +0x08
    unsigned char _pad0c[0x24];
    CCharString   QuestName;                       // +0x30
};

struct CQuestNode {                                // list node: next, prev, CCountedPointer<CActiveQuest>
    CQuestNode*     next;
    CQuestNode*     prev;
    CActiveQuest*   Data;                          // +0x08
    CCPPointerInfo* Info;                          // +0x0c
};

struct CQuestIterator {
    CQuestNode* _M_node;
    CQuestIterator(CQuestNode* n) : _M_node(n) {}
    CQuestIterator(const CQuestIterator& o) : _M_node(o._M_node) {}
};

struct CQuestConstIterator {
    CQuestNode* _M_node;
    CQuestConstIterator(CQuestNode* n) : _M_node(n) {}
};

struct CQuestList {                                // STLport list<CCountedPointer<CActiveQuest> >
    CQuestNode* _M_node;
    CQuestList()
    {
        CQuestNode* node = static_cast<CQuestNode*>(malloc(sizeof(CQuestNode)));
        node->next = node;
        node->prev = node;
        _M_node = node;
    }
    ~CQuestList()
    {
        clear();
        free(_M_node);
    }
    void clear();                                  // 0x004B9DA0
    void _M_insert_dispatch(CQuestIterator pos, CQuestConstIterator first, CQuestConstIterator last,
                            const __false_type&);  // 0x004B92D0
    void insert(CQuestIterator pos, CQuestConstIterator first, CQuestConstIterator last)
    {
        _M_insert_dispatch(pos, first, last, __false_type());
    }
};

struct CQuestManager {
    unsigned char _pad_0x00[0x38];
    CQuestNode*   ActiveQuests;                    // +0x38 (head node)
    bool SetQuestAsFailed(const CCharString& name, bool showScreen, const CWideString& reason, bool silent);   // 0x004B2280
    void DeactivateEventQuests();
};

void CQuestManager::DeactivateEventQuests()
{
    CQuestList quests;
    quests.insert(CQuestIterator(quests._M_node), CQuestConstIterator(ActiveQuests->next), CQuestConstIterator(ActiveQuests));
    for (CQuestNode* node = quests._M_node->next; node != quests._M_node; node = node->next) {
        CActiveQuest* quest = node->Data;
        if (quest->PScript) {
            CWideString reason(L"");
            SetQuestAsFailed(quest->QuestName, false, reason, true);
        }
    }
}
