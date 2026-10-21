// Grade: DIFFER(336v336) -- pure ebx/edi swap: retail keeps this in ebx and the def temporary / find_if result / new'd task in edi; ours the reverse. Every other byte matches. Tried found as temp/named/assigned/top-of-function, const_iterator, inline helpers (IsCurfewTime, AddHornTasks), same-TU find_if body, dtor and def-constness variants.
// CTCVillage::UpdateHornTasks() -- during curfew add one horn task for every village horn that has none (once, flagged by HornTasksAdded); outside curfew take every horn out of all tasks and clear the flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065BE6C, length 336 bytes (`leave`, plain `ret`): frame-pointer size TU,
//   `#pragma optimize("y", off)` + `#pragma optimize("s", on)` as the landed CTCVillage siblings.
// Callees: CTCBase::GetWorld() const 0x00686A80 (as the landed IsTimeBetween 0x00658620); the game-time manager's
//   time-of-day range test 0x006BB390 (manifest "Game_IsTimeOfDayInRange", thiscall, two HHMM longs); the village def's
//   slot 1 (destroy at refcount zero); CIntelligentPointer<const CThing> copy constructor 0x004A9730; the horn-task
//   predicate constructor 0x0065F0FA (takes the pointer by value, manifest "CTCActionUseBuyHouse::OnCreate" propagated);
//   std::find_if 0x0065FFCA (`__fastcall`: hidden result in ecx, iterators and predicate by value; manifest
//   "CGuiControlTree::SelectNextEntryByName" propagated); ~CBaseIntelligentPointer 0x00A01C10; operator new 0x00BFEA1A;
//   CVillageTaskHorn::CVillageTaskHorn(CThing*) 0x0065EDBA; CCountedPointer<CVillageTaskHorn>(T*) 0x0065E29D;
//   list::push_back 0x00660539; ~CCountedPointer 0x0065D5A0; CBaseIntelligentPointer::GetPItem 0x00A01B50;
//   CTCVillage::RemoveThingFromAllTasks(const CThing&) 0x00659EA9.
// Layout: retail CTCVillage (4-byte STLport lists, landed siblings): VillageHorns +0x34, HornTasks +0x50, HornTasksAdded
//   +0x7b, PDef +0x94; CVillageDef CurfewStartTime +0xdc / CurfewFinishTime +0xe0 (PDB +0xfc/+0x100 less the eight
//   4-byte-narrower vectors, as landed GetSleepTimes 0x00833390); CWorld PGameTimeManager +0x1c.
// Shape: the def comes through an inline by-value getter, so each range bound holds its own CDefPointer temporary
//   (released without a null test, last first); the horn-task predicate is a temporary destroyed before the compare.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CBaseObject { };
class CThing : public CBaseObject { };

struct CBaseIntelligentPointer {
    void*        _vfptr;
    CBaseObject* m_pItem;
    ~CBaseIntelligentPointer();                         // 0x00A01C10
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(const CIntelligentPointer& other);                     // 0x004A9730
    template <class U> CIntelligentPointer(const CIntelligentPointer<U>& other);   // 0x004A9730 (folded)
    T* GetPItem() const;                                // 0x00A01B50
};

struct CVillageTaskHorn {
    unsigned char _pad[0x18];
    CVillageTaskHorn(CThing* horn);                     // 0x0065EDBA
};
struct CHornTaskHasThing {                              // find_if predicate: the task's horn is the given thing
    CIntelligentPointer<const CThing> Thing;
    CHornTaskHasThing(CIntelligentPointer<const CThing> thing);   // 0x0065F0FA
};

template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    CCountedPointer(T* data);                           // 0x0065E29D
    ~CCountedPointer();                                 // 0x0065D5A0
};

template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct SListIterator {
    SListNode<T>* _M_node;
    SListIterator() {}
    SListIterator(SListNode<T>* node) : _M_node(node) {}
    SListIterator(const SListIterator& other) : _M_node(other._M_node) {}
    bool operator==(const SListIterator& other) const { return _M_node == other._M_node; }
    bool operator!=(const SListIterator& other) const { return _M_node != other._M_node; }
    T* operator->() const { return &_M_node->_M_data; }
    SListIterator& operator++() { _M_node = _M_node->_M_next; return *this; }
};
template <class T> struct SList {
    SListNode<T>* _M_node;
    SListIterator<T> begin() { return SListIterator<T>(_M_node->_M_next); }
    SListIterator<T> end() { return SListIterator<T>(_M_node); }
    void push_back(const T& value);                     // 0x00660539
};

template <class It, class P> It __fastcall find_if(It first, It last, P pred);   // 0x0065FFCA

struct CVillageDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long          m_refCount;                           // +0x04
    unsigned char _pad08[0xdc - 0x08];
    long          CurfewStartTime;                      // +0xdc
    long          CurfewFinishTime;                     // +0xe0
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (--Object->m_refCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};

struct CGameTimeManager {
    bool IsTimeOfDayInRange(long start, long end) const;   // 0x006BB390
};
struct CWorld {
    unsigned char     _pad_0x00[0x1c];
    CGameTimeManager* PGameTimeManager;                 // +0x1c
    __forceinline bool IsTimeOfDayInRange(const long& start, const long& end) const
    {
        const CGameTimeManager* manager = PGameTimeManager;
        return manager->IsTimeOfDayInRange(start, end);
    }
};

struct CTCVillage {
    void*                                           __vftable;          // +0x00
    unsigned char                                   _pad_0x04[0x30];
    SList<CIntelligentPointer<CThing> >             VillageHorns;       // +0x34
    unsigned char                                   _pad_0x38[0x18];
    SList<CCountedPointer<CVillageTaskHorn> >       HornTasks;          // +0x50
    unsigned char                                   _pad_0x54[0x27];
    bool                                            HornTasksAdded;     // +0x7b
    unsigned char                                   _pad_0x7c[0x18];
    CDefPointer<CVillageDef>                        PDef;               // +0x94 CDefPointer<const CVillageDef>
    const CWorld* GetWorld() const;                                     // 0x00686A80 (CTCBase)
    __forceinline CDefPointer<CVillageDef> GetPDef() const { return PDef; }
    void RemoveThingFromAllTasks(const CThing& thing);                  // 0x00659EA9
    void UpdateHornTasks();
};

void CTCVillage::UpdateHornTasks()
{
    bool curfew = GetWorld()->IsTimeOfDayInRange(GetPDef()->CurfewStartTime, GetPDef()->CurfewFinishTime);
    if (curfew) {
        if (!HornTasksAdded) {
            for (SListIterator<CIntelligentPointer<CThing> > it = VillageHorns.begin(); it != VillageHorns.end(); ++it) {
                SListIterator<CCountedPointer<CVillageTaskHorn> > found;
                found = find_if(HornTasks.begin(), HornTasks.end(), CHornTaskHasThing(*it.operator->()));
                if (found == HornTasks.end())
                    HornTasks.push_back(CCountedPointer<CVillageTaskHorn>(new CVillageTaskHorn(it->GetPItem())));
            }
            HornTasksAdded = true;
        }
    } else if (HornTasksAdded) {
        for (SListIterator<CIntelligentPointer<CThing> > it = VillageHorns.begin(); it != VillageHorns.end(); ++it)
            RemoveThingFromAllTasks(*it->GetPItem());
        HornTasksAdded = false;
    }
}
