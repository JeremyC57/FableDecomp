// CQuestManager::IsQuestActive(const CCharString& quest_name) const -- walk the active-quest list and report whether one of the active quests carries the given quest name
// Leaf: ?IsQuestActive@CQuestManager@@QBE_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF610, 85 bytes (96 with padding), `ret 4` (__fastcall).
// Identity: Ego_r.pdb `?IsQuestActive@CQuestManager@@QBE_NABVCCharString@@@Z`; the landed callers CQuestManager::Update
//   0x004B4490 and DeactivateQuest 0x004B3A20 declare 0x004AF610 as IsQuestActive (labels chunk 322).
// Callees: Compare(const char*, const char*) 0x00411570 (__fastcall, the inlined CCharString equality; a same-TU
//   `auto_inline(off)` stand-in body precedes the function, see Shape).
// Layout (retail): CQuestManager ActiveQuests (STLport list<CCountedPointer<CActiveQuest> >, head-node pointer) +0x38
//   (as the landed Update 0x004B4490); list node {_M_next, _M_prev, _M_data}, the counted pointer's Data at node +8;
//   CActiveQuest QuestName +0x30 (Ego_r.pdb +0x34: ID, PScriptInfo, PScript (8), Regions (12 in STLport),
//   MarkerPosition, Persistent, LoadedResources, PScriptData (8)). CCharString data block {chars, length}.
// Shape: STLport list walk with the end iterator cached before the loop (the head node stays in ebx) and the searched
//   name's data pointer hoisted out of the loop (VC7.1 does that only when it can see Compare's body only reads memory:
//   the declared-only callee reloads it every pass); the quest name read through the counted pointer's `operator->` (`add eax,30h ; mov
//   eax,[eax]`); the inline CCharString::operator== (same pointer -> true; both non-null and equal length -> Compare
//   == 0) as the landed CThingSwitch::OnThingWithScriptNameApplied 0x0083CC10.
// Label correction: catalogued as `CQuest_Array_FindByName`; CQuestManager::IsQuestActive (as labels chunk 322).
struct CCharStringData {
    const char*   m_pData;                           // +0x00
    unsigned long m_length;                          // +0x04
};
#pragma auto_inline(off)
// Same-TU stand-in for the signed C-string compare 0x00411570 (the landed body): it only reads, so VC may keep the
// searched name's data pointer across the call, as retail's whole-program build did.
long __fastcall Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
class CCharString {
public:
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};

template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Count;
    T* operator->() const { return Data; }
};

template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct _List_const_iterator {
    _List_node<T>* _M_node;
    _List_const_iterator(_List_node<T>* __x) : _M_node(__x) { }
    const T& operator*() const { return _M_node->_M_data; }
    _List_const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_const_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class T> struct list {                     // STLport list: the head node pointer (4 bytes in retail)
    typedef _List_const_iterator<T> const_iterator;
    _List_node<T>* _M_node;
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
};

class CQuestManager {
public:
    class CActiveQuest {
    public:
        long          ID;                            // +0x00
        void*         PScriptInfo;                   // +0x04
        void*         PScript[2];                    // +0x08 CCountedPointer<CScriptBase>
        void*         Regions[3];                    // +0x10 vector<const CRegion*>
        long          MarkerPosition[2];             // +0x1C C2DCoordI
        bool          Persistent;                    // +0x24
        bool          LoadedResources;               // +0x25
        void*         PScriptData[2];                // +0x28 CCountedPointer<CScriptDataBase>
        CCharString   QuestName;                     // +0x30
    };

    void*         __vftable;                         // +0x00
    unsigned char _pad04[0x38 - 0x04];               // PGameScriptThingManager, RuntimeQuestPersistence, RegisteredQuestNames
    list<CCountedPointer<CActiveQuest> > ActiveQuests;   // +0x38

    bool IsQuestActive(const CCharString& quest_name) const;
};

bool CQuestManager::IsQuestActive(const CCharString& quest_name) const
{
    list<CCountedPointer<CActiveQuest> >::const_iterator end = ActiveQuests.end();
    for (list<CCountedPointer<CActiveQuest> >::const_iterator it = ActiveQuests.begin(); it != end; ++it)
        if ((*it)->QuestName == quest_name)
            return true;
    return false;
}