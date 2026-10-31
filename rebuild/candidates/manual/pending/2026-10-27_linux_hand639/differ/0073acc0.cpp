// CTCCreatureModeManager::AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase> mode) -- refuse a mode whose type is already active; otherwise remember the current (front) mode's type when it is removed on deactivation, deactivate it, append the new mode, activate the front mode, re-pick the look component's turning anims, schedule the next idle mode in 30 seconds and finally remove the replaced type
// Grade: DIFFER(331v331) -- scheduling only: retail materialises the early `return false` (xor al,al) at the top of its join block, before the pops; ours puts it between pop ebp and pop ebx (the true path's mov al,1 slot)
// Leaf: ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@Z
// Tried (a-g): IsModeActive inline / __forceinline (0x0073A8A0 expanded), shared CCountedPointer.h (dtor re-reads Info: 353 bytes), local CCountedPointer with/without a cached block, same-TU push_back body, if/else single-exit form (335).
// Local CCountedPointer model: engine/CCountedPointer.h's destructor re-reads Info after the decrement (its out-of-line
// COMDAT shape) and zeroes Data/Info; this expansion is `dec [Info] ; jne` on the cached block pointer with no zero stores
// (the shape of the landed by-value caller AddMode(long) 0x0073AE10). Members and spellings follow the header.
class CCPPointerInfo { public:
    unsigned long RefCount;                             // +0x0
    void (__fastcall* DeleteFunc)(void*);               // +0x4
    void* Data;                                         // +0x8
};
template <class T> class CCountedPointer { public:
    T* Data;                                            // +0x0
    CCPPointerInfo* Info;                               // +0x4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    T* operator->() const { return Data; }
};
extern long g_framesPerSecond;                          // 0x01375550
class CTCBase;
enum ETCInterfaceType { };
void* operator new(unsigned int size);                  // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T> class allocator { };
template <class T> struct _List_node { _List_node* _M_next; _List_node* _M_prev; T _M_data; };
template <class T, class A> class list;
template <class T> class list<CCountedPointer<T>, allocator<CCountedPointer<T> > > {   // retail STLport list: the sentinel node pointer
public:
    _List_node<CCountedPointer<T> >* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    CCountedPointer<T>& front() { return _M_node->_M_next->_M_data; }
    __declspec(noinline) void push_back(const CCountedPointer<T>& x)   // 0x007421C0 (same-TU body, called out of line)
    {
        _List_node<CCountedPointer<T> >* pos = _M_node;
        _List_node<CCountedPointer<T> >* tmp = (_List_node<CCountedPointer<T> >*)operator new(sizeof(_List_node<CCountedPointer<T> >));
        new (&tmp->_M_data) CCountedPointer<T>(x);
        tmp->_M_next = pos;
        tmp->_M_prev = pos->_M_prev;
        pos->_M_prev->_M_next = tmp;
        pos->_M_prev = tmp;
    }
};
}
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return it->second;
    }
};
class CWorld { public:
    static long __fastcall GetFrame();                  // 0x0049D870
};
class CTCLook { public:
    void UpdateTurningAnims();                          // 0x00754050
};
class CAnimateThing;
class CAnimFilterBase;
class CTCCreatureModeManager { public:
    class CModeBase { public:                           // vtable 0x012673CC
        virtual ~CModeBase();                           // slot 0
        virtual unsigned long GetType() const = 0;      // slot 1 (_purecall)
        virtual void AnimUpdate(CAnimateThing& thing);  // slot 2
        virtual void FrameUpdate();                     // slot 3
        virtual const CAnimFilterBase& GetAnimFilter() const;   // slot 4
        virtual void OnActivate();                      // slot 5 (+0x14)
        virtual void OnDeactivate();                    // slot 6 (+0x18)
        virtual void v07();                             // slot 7 GetMovementSpeed (signature not modelled)
        virtual bool IsIdleMode() const;                // slot 8
        virtual bool IsAllowingPlayerSpecialMovementMoves() const;   // slot 9
        virtual bool IsHoldWeaponWithBothHands() const; // slot 10
        virtual bool IsRemoveOnDeactivate() const;      // slot 11 (+0x2c)
    };
    virtual ~CTCCreatureModeManager();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    std::list<CCountedPointer<CModeBase>, std::allocator<CCountedPointer<CModeBase> > > PModes;   // +0x0c
    long TimeLeftForCurrentMode;                        // +0x10
    long TimeForNextIdleMode;                           // +0x14
    __forceinline bool IsModeActive(unsigned long type) const   // 0x0073A8A0 (expanded here)
    {
        std::_List_node<CCountedPointer<CModeBase> >* end = PModes._M_node;
        std::_List_node<CCountedPointer<CModeBase> >* node = end->_M_next;
        for (; node != end; node = node->_M_next) {
            if (node->_M_data->GetType() == type)
                break;
        }
        return node != PModes._M_node;
    }
    void RemoveMode(unsigned long modeType);            // 0x0073ABA0
    bool AddMode(CCountedPointer<CModeBase> mode);
};

bool CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase> mode)
{
    if (IsModeActive(mode->GetType()))
        return false;
    unsigned long replacedType = 0;
    if (!PModes.empty()) {
        if (PModes.front()->IsRemoveOnDeactivate())
            replacedType = PModes.front()->GetType();
        PModes.front()->OnDeactivate();
        PModes.push_back(mode);
        PModes.front()->OnActivate();
    } else {
        PModes.push_back(mode);
        PModes.front()->OnActivate();
    }
    if (ParentThing->HasTC(0x43))
        ((CTCLook*)ParentThing->GetTC(0x43))->UpdateTurningAnims();
    long idleDelay = (long)(g_framesPerSecond * 30.0f);
    TimeForNextIdleMode = CWorld::GetFrame() + idleDelay;
    if (replacedType)
        RemoveMode(replacedType);
    return true;
}
