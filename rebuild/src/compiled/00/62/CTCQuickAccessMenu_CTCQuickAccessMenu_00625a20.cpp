// CTCQuickAccessMenu::~CTCQuickAccessMenu() -- quick access menu TC dtor: leave the global TC list, delete the menu's UI components and empty the vector, reset the UI manager (observers, action params, previous-selection list, bastard child); then the vector dtor and ~CTCBase
// Leaf: ??1CTCQuickAccessMenu@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00625A20, 203 bytes, tail jump to the base destructor.
// Identity: the vtable store 0x01258B74 carries RTTI `.?AVCTCQuickAccessMenu@@` (CTCQuickAccessMenu : CTCBase); FableWin.pdb
//   names CTCQuickAccessMenu as a V-keyed class and Ego_r.pdb lays it out (Active, Components, Quitting, LastRenderTime,
//   FirstRenderFrame). The catalog label is right.
// Callees: CTCBase::RemoveFromGlobalTCs 0x00686DF0, memmove 0x00BFEAE6 (STLport vector::clear through __copy_trivial),
//   NUISystem::CManager::GetInstance 0x0041E5F2 and its virtuals ClearObservers (CObservable slot 6, +0x18),
//   ClearActionParams (slot 29, +0x74, body 0x0041D207) and ClearBastardChild (slot 37, +0x94, body 0x0041DED2), the inlined
//   STLport list clear of NUISystem::CList::GPreviousSelected 0x013B8A98 (nodes freed with free 0x00BFEA14), the vector free
//   (free 0x00BFEA14) and CTCBase::~CTCBase 0x00686830 (tail jump). Each component's virtual scalar deleting destructor
//   (slot 0, flag 1). All of these as in the landed CTCQuestCompletionUI::~CTCQuestCompletionUI 0x005E41E0, whose model this is.
// Layout: Ego_r.pdb CTCQuickAccessMenu (0x38) with the retail STLport vector (0xC): Active +0x0C, Components +0x10,
//   Quitting +0x1C, LastRenderTime +0x20, FirstRenderFrame +0x28. No engine header exists; engine/CTCBase.h flattens the vptr,
//   so the CTCBase chain is modelled here (header debt).
// Shape: user body `RemoveFromGlobalTCs(); for (i < Components.size()) delete Components[i]; Components.clear();
//   ClearObservers(); ClearActionParams(); CList::GPreviousSelected.clear(); ClearBastardChild();`, then the implicit vector
//   dtor (free of the block) and the base dtor.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
struct __true_type { __true_type() { } };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
template <class _Tp> inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __true_type());
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};
template <class _Tp, class _Alloc> class _List_base { public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, allocator<_Node> > _M_node;   // +0x0: the header node
    ~_List_base()
    {
        clear();
        _M_node.deallocate((_Node*)_M_node._M_data, 1);
    }
    void clear();
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { public:
};
} // namespace std

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();
};
class CThing;
class CTCBase : public CBaseClassNonCopyable { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase();                                     // 0x00686830
    void RemoveFromGlobalTCs();                             // 0x00686DF0
};

namespace NUISystem {
class CComponent { public: virtual ~CComponent(); };
enum EEvent { };                                            // values not needed here
class CObserver;
class CObservable { public:                                 // virtuals in Ego_r declaration order
    virtual void SetEvent(EEvent event);                    // slot 0
    virtual void SetOwnedEvent(CObserver* observer, EEvent event);   // slot 1
    virtual void AddObserver(CObserver* observer);          // slot 2
    virtual void AddExclusiveObserver(CObserver* observer); // slot 3
    virtual void RemoveExclusiveObserver(CObserver* observer);       // slot 4
    virtual void RemoveObserver(CObserver* observer);       // slot 5
    virtual void ClearObservers();                          // slot 6 (+0x18)
    virtual CObserver* GetExclusiveObserver();              // slot 7
    virtual bool IsExclusivelyObserved();                   // slot 8
    virtual void AddConcurrentExclusiveObserver(CObserver* observer);    // slot 9
    virtual void RemoveConcurrentExclusiveObserver(CObserver* observer); // slot 10
};
class CManager : public CObservable { public:               // vtable 0x01230134
    virtual void CUnprovenSlot11_01230134();
    virtual void CUnprovenSlot12_01230134();
    virtual void CUnprovenSlot13_01230134();
    virtual void CUnprovenSlot14_01230134();
    virtual void CUnprovenSlot15_01230134();
    virtual void CUnprovenSlot16_01230134();
    virtual void CUnprovenSlot17_01230134();
    virtual void CUnprovenSlot18_01230134();
    virtual void CUnprovenSlot19_01230134();
    virtual void CUnprovenSlot20_01230134();
    virtual void CUnprovenSlot21_01230134();
    virtual void CUnprovenSlot22_01230134();
    virtual void CUnprovenSlot23_01230134();
    virtual void CUnprovenSlot24_01230134();
    virtual void CUnprovenSlot25_01230134();
    virtual void CUnprovenSlot26_01230134();
    virtual void CUnprovenSlot27_01230134();
    virtual void CUnprovenSlot28_01230134();
    virtual void ClearActionParams();                       // slot 29 (+0x74)
    virtual void CUnprovenSlot30_01230134();
    virtual void CUnprovenSlot31_01230134();
    virtual void CUnprovenSlot32_01230134();
    virtual void CUnprovenSlot33_01230134();
    virtual void CUnprovenSlot34_01230134();
    virtual void CUnprovenSlot35_01230134();
    virtual void CUnprovenSlot36_01230134();
    virtual void ClearBastardChild();                       // slot 37 (+0x94)
    static CManager* __fastcall GetInstance();              // 0x0041E5F2
};
class CList { public:
    static std::list<CList*> GPreviousSelected;             // 0x013B8A98
};
}

// STLport 4.6 _List_base<T*>::clear, inlined in this TU for the trivially destructible pointer list
template <> inline void std::_List_base<NUISystem::CList*, std::allocator<NUISystem::CList*> >::clear()
{
    _Node* __cur = (_Node*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _Node* __tmp = __cur;
        __cur = (_Node*)__cur->_M_next;
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}

class CTCQuickAccessMenu : public CTCBase { public:
    bool                                    Active;                       // +0x0C
    std::vector<NUISystem::CComponent*>     Components;                   // +0x10
    bool                                    Quitting;                     // +0x1C
    double                                  LastRenderTime;               // +0x20
    bool                                    FirstRenderFrame;             // +0x28
    virtual ~CTCQuickAccessMenu();
};

CTCQuickAccessMenu::~CTCQuickAccessMenu()
{
    RemoveFromGlobalTCs();
    for (unsigned long i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
    NUISystem::CManager::GetInstance()->ClearObservers();
    NUISystem::CManager::GetInstance()->ClearActionParams();
    NUISystem::CList::GPreviousSelected.clear();
    NUISystem::CManager::GetInstance()->ClearBastardChild();
}