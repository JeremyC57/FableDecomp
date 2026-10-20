// Grade: DIFFER(249v261) -- callee-saved push placement: ebp/ebx are pushed inside the hero branch (shrink-wrapped, so the frame offsets shift by 8) where retail pushes them at entry; the back() load and the KnownDeeds index rotate eax/ecx/edx
// CTCCreatureOpinionOfHero::ClearPendingDeeds() -- drain the pending deed handles: while the hero exists each (last first) is looked up in the hero's deed log and, when still valid, recorded at the front of its deed type's known list; without a hero the pending list is simply emptied
// Leaf: ClearPendingDeeds
//
// Retail 0x00683B00, 272 bytes incl. padding (plain `ret`). Identity: the catalog label
// "CTCHeroOpinionDeedLog::ApplyPendingDeedLog" is wrong for the class -- `this` is the creature's CTCCreatureOpinionOfHero
// (the callers 0x009600FB/0x0096010A look up interface 0x4F and call this on it; PendingDeeds +0x54 and KnownDeeds[92] +0x1C8
// as the landed ClearAllKnownDeeds 0x00681A70), and the deed log is fetched from the hero; the public void PDB method of the
// class that empties the pending list is `?ClearPendingDeeds@CTCCreatureOpinionOfHero@@QAEXXZ` (name probable, not proven).
// Callees: CTCBase::GetPlayerManager @ 0x00686D40, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroThing
// @ 0x00487DC0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined unchecked GetInterface for the
// deed log, type 0x51 = bit 17 of +0x28: the miss path reads the never-written out slot), CTCHeroOpinionDeedLog::GetDeedFromHandle
// (long) @ 0x006C58F0, malloc @ 0x00BFEA0E / free @ 0x00BFEA14 (the STLport list nodes).
// Layout: retail CTCCreatureOpinionOfHero (PDB offsets minus the 12-byte vector<bool> masks and the 4-byte lists): PendingDeeds
// +0x54, LastFrameAppliedDeedEffect[92] +0x58, KnownDeeds[92] +0x1c8; COpinionDeed Type +0, Handle +4.
// Shape: STLport list members written out: `back()`, `push_front` (the position read from the head's next) (placement new with its null test, `prev` read before the
// links), `pop_back` (next/prev read, prev->next first) and `clear()`, each re-reading the header member.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CTCBase;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CPlayer {
    CThing* GetHeroThing();                                 // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                               // 0x00449970
};
struct COpinionDeed {
    long Type;                                              // +0x00 EOpinionDeedType
    long Handle;                                            // +0x04
};
struct CTCHeroOpinionDeedLog {
    const COpinionDeed* GetDeedFromHandle(long handle) const;   // 0x006C58F0
};

template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CRetailList {                     // STLport list (4 bytes: the head node)
    SListNode<T>* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    T& back() { return _M_node->_M_prev->_M_data; }
    void push_front(const T& value)
    {
        SListNode<T>* position = _M_node->_M_next;
        SListNode<T>* node = (SListNode<T>*)malloc(sizeof(SListNode<T>));
        new (&node->_M_data) T(value);
        SListNode<T>* prev = position->_M_prev;
        node->_M_next = position;
        node->_M_prev = prev;
        prev->_M_next = node;
        position->_M_prev = node;
    }
    void pop_back()
    {
        SListNode<T>* node = _M_node->_M_prev;
        SListNode<T>* next = node->_M_next;
        SListNode<T>* prev = node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(node);
    }
    void clear()
    {
        SListNode<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SListNode<T>* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};

class CTCCreatureOpinionOfHero {
public:
    virtual ~CTCCreatureOpinionOfHero();
    CThing*            ParentThing;                         // +0x04 (CTCBase)
    unsigned char      _pad08[0x54 - 0x08];
    CRetailList<long>  PendingDeeds;                        // +0x54
    long               LastFrameAppliedDeedEffect[92];      // +0x58
    CRetailList<long>  KnownDeeds[92];                      // +0x1c8
    CPlayerManager* GetPlayerManager();                     // 0x00686D40 (CTCBase)
    void ClearPendingDeeds();
};

void CTCCreatureOpinionOfHero::ClearPendingDeeds()
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
    if (hero) {
        CTCHeroOpinionDeedLog* deedLog;
        hero->GetInterface(&deedLog, 0x51);
        while (!PendingDeeds.empty()) {
            const COpinionDeed* deed = deedLog->GetDeedFromHandle(PendingDeeds.back());
            if (deed)
                KnownDeeds[deed->Type].push_front(deed->Handle);
            PendingDeeds.pop_back();
        }
    } else {
        PendingDeeds.clear();
    }
}
