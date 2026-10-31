// CTCVillage::UpdateHornTasks() -- during curfew add one horn task for every village horn that has none (once, flagged by HornTasksAdded); outside curfew take every horn out of all tasks and clear the flag
// Grade: DIFFER(336v336) -- register tie: retail keeps this in ebx and the def temporary / find_if result / new'd task in edi; ours the reverse; every other byte matches
// Leaf: ?UpdateHornTasks@CTCVillage@@AAEXXZ
// Tried (a-h): PDB accessors PeekWorld / CWorld::PeekGameTimeManager / CGameTimeManager::IsTimeBetween const (fixed the [eax+0x1c] load), CIsHornTaskWithThisHorn, STLport list names, private UpdateHornTasks; engine/CDefPointer.h (and any Release that returns the count) gets retail's registers but adds `cmp [obj+4],0` after each dec (344 bytes); the IsCurfewTime 0x00658DB3 def model gives the exact release bytes with this swap.
#pragma optimize("y", off)
#pragma optimize("s", on)
// Local def model: the same as the landed sibling IsCurfewTime 0x00658DB3 (whose curfew test this function expands).
// engine/CDefPointer.h's Release (`if (object->Release() == 0) object->Delete()`) re-tests the count after the decrement
// (`dec ; cmp [esi+4],0`); retail here is `dec [esi+4] ; jne` like that sibling.
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;
    mutable unsigned long IVRefCount;
    __forceinline void AddRef() const { ++IVRefCount; }
    __forceinline long Release() const { if (--IVRefCount == 0) { Delete(); return 0; } return IVRefCount; }
};
class CResourceList;
class CResource : public CIVCountedPointeeBase { public:
    CResourceList* ResourceList;
    CResource* PrevResource;
    CResource* NextResource;
    unsigned long ResourceSize;
    unsigned long LastUsedFrame;
};
class CDefPointeeBase : public CResource { };
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase { public:
    CDefinitionManager* PDefManager;
    unsigned long GlobalIndex;
    unsigned char Setup : 1, Template : 1, DefaultValsApplied : 1;
};
#pragma pack(pop)
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    __forceinline CDefPointer(const CDefPointer& x) : Object(x.Object) { if (Object) Object->AddRef(); }
    __forceinline void Release() { if (Object) { Object->Release(); Object = 0; } }
    __forceinline ~CDefPointer() { Release(); }
    __forceinline T* operator->() const { return static_cast<T*>(Object); }
};
class CThing;
class CWorld;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* PItem;                                      // +0x04
    CThing* GetPItem() const;                           // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer& other);   // 0x004A9730
};
class CVillageTaskHorn {
public:
    unsigned char _pad[0x18];
    CVillageTaskHorn(CThing* horn);                     // 0x0065EDBA
};
class CIsHornTaskWithThisHorn {                         // find_if predicate (Ego_r.pdb: PHorn +0)
public:
    CIntelligentPointer<CThing> PHorn;
    CIsHornTaskWithThisHorn(CIntelligentPointer<CThing> horn);   // 0x0065F0FA
};
template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;
    CCountedPointer(T* data);                           // 0x0065E29D
    ~CCountedPointer();                                 // 0x0065D5A0
};
namespace std {
template <class T> class allocator { };
template <class T> struct _List_node { _List_node* _M_next; _List_node* _M_prev; T _M_data; };
template <class T> struct _Nonconst_traits { };
template <class T, class Traits> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator() {}
    _List_iterator(_List_node<T>* node) : _M_node(node) {}
    _List_iterator(const _List_iterator& other) : _M_node(other._M_node) {}
    bool operator==(const _List_iterator& other) const { return _M_node == other._M_node; }
    bool operator!=(const _List_iterator& other) const { return _M_node != other._M_node; }
    T& operator*() const { return _M_node->_M_data; }
    T* operator->() const { return &_M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
};
template <class T, class A> class list {
public:
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    _List_node<T>* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    void push_back(const T& value);                     // 0x00660539
};
template <class It, class P> It __fastcall find_if(It first, It last, P pred);   // 0x0065FFCA
}
class CVillageDef : public CDefClassBase {
public:
    unsigned char _pad_0x25[0xdc - 0x25];
    long          CurfewStartTime;                      // +0xdc
    long          CurfewFinishTime;                     // +0xe0
};
class CGameTimeManager {
public:
    bool IsTimeBetween(long start, long end) const;     // 0x006BB390
};
namespace boost { template <class T> class scoped_ptr { T* ptr; public: T* operator->() const { return ptr; } }; }
class CWorld {
public:
    unsigned char _pad_0x00[0x1c];
    boost::scoped_ptr<CGameTimeManager> PGameTimeManager;   // +0x1c
    const CGameTimeManager& PeekGameTimeManager() const { return *PGameTimeManager.operator->(); }   // Ego_r.pdb ?PeekGameTimeManager@CWorld@@QBEABVCGameTimeManager@@XZ
};
class CTCVillage {
public:
    virtual ~CTCVillage();
    unsigned char _pad_0x04[0x30];
    std::list<CIntelligentPointer<CThing>, std::allocator<CIntelligentPointer<CThing> > > VillageHorns;   // +0x34
    unsigned char _pad_0x38[0x18];
    std::list<CCountedPointer<CVillageTaskHorn>, std::allocator<CCountedPointer<CVillageTaskHorn> > > HornTasks;   // +0x50
    unsigned char _pad_0x54[0x27];
    bool          HornTasksAdded;                       // +0x7b
    unsigned char _pad_0x7c[0x18];
    CDefPointer<const CVillageDef> PDef;                // +0x94
    __forceinline CDefPointer<const CVillageDef> GetPDef() const { return PDef; }
    void RemoveThingFromAllTasks(const CThing& thing);  // 0x00659EA9
protected:
    const CWorld& PeekWorld() const;                    // 0x00686A80 (CTCBase)
private:
    void UpdateHornTasks();
};

void CTCVillage::UpdateHornTasks()
{
    bool curfew = PeekWorld().PeekGameTimeManager().IsTimeBetween(GetPDef()->CurfewStartTime, GetPDef()->CurfewFinishTime);
    if (curfew) {
        if (!HornTasksAdded) {
            for (std::list<CIntelligentPointer<CThing>, std::allocator<CIntelligentPointer<CThing> > >::iterator it = VillageHorns.begin(); it != VillageHorns.end(); ++it) {
                std::list<CCountedPointer<CVillageTaskHorn>, std::allocator<CCountedPointer<CVillageTaskHorn> > >::iterator found;
                found = std::find_if(HornTasks.begin(), HornTasks.end(), CIsHornTaskWithThisHorn(*it));
                if (found == HornTasks.end())
                    HornTasks.push_back(CCountedPointer<CVillageTaskHorn>(new CVillageTaskHorn(it->GetPItem())));
            }
            HornTasksAdded = true;
        }
    } else if (HornTasksAdded) {
        for (std::list<CIntelligentPointer<CThing>, std::allocator<CIntelligentPointer<CThing> > >::iterator it = VillageHorns.begin(); it != VillageHorns.end(); ++it)
            RemoveThingFromAllTasks(*it->GetPItem());
        HornTasksAdded = false;
    }
}
