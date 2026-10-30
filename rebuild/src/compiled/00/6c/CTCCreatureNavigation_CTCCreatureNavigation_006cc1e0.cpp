// CTCCreatureNavigation::~CTCCreatureNavigation() -- creature navigation TC dtor: if a navigator is active, drop the steering agent list from the global agent lists, forget the navigator and delete (and null) every possible navigator; then the member dtors and ~CTCBase
// Leaf: ??1CTCCreatureNavigation@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CC1E0, 137 bytes, tail jump to the base destructor.
// Identity: the vtable store 0x0125EC24 carries RTTI `.?AVCTCCreatureNavigation@@` (CTCCreatureNavigation : CTCBase);
//   FableWin.pdb has `??1CTCCreatureNavigation@@UAE@XZ`. The catalog label is right.
// Callees: CNavigatorExternalInterface::RemoveAgentListFromLists 0x00A47D80 (FableWin.pdb
//   `?RemoveAgentListFromLists@CNavigatorExternalInterface@@SIXPAV?$vector@PBVCSteeringAgent@@V?$allocator@PBVCSteeringAgent@@@std@@@std@@@Z`,
//   static fastcall: its body finds the vector pointer in the static AgentLists list 0x013D2834 and erases that node; the
//   catalog labels it RemoveAgentListFromLists), free 0x00BFEA14 (the two vector blocks), CBaseIntelligentPointer::
//   ~CBaseIntelligentPointer 0x00A01C10 (PThingToNotAvoid's implicit dtor), CTCBase::~CTCBase 0x00686830 (tail jump); each
//   navigator's virtual scalar deleting destructor (slot 0, flag 1).
// Layout: Ego_r.pdb CTCCreatureNavigation (0x68) with the retail STLport vector (0xC): PNavigator +0x0C,
//   PossibleNavigators +0x10, NavigationTarget +0x1C (CNavigationInput, 0x18), Speed +0x34, PThingToNotAvoid +0x38,
//   Agents +0x40, NextUpdate +0x4C .. CurrentSpeed +0x5C. No engine header exists; engine/CTCBase.h flattens the vptr, so the
//   CTCBase chain is modelled here (header debt).
// Shape: user body `if (PNavigator) { RemoveAgentListFromLists(&Agents); PNavigator = 0; for (i < size) { delete
//   PossibleNavigators[i]; PossibleNavigators[i] = 0; } }` (size re-read each pass, `jb`/`jbe` = unsigned), then the implicit
//   member dtors in reverse order: Agents (free), PThingToNotAvoid, PossibleNavigators (free), and the base dtor.
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

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer { public:                     // vptr + PData, 8 bytes
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
};

class C3DVector { public:
    float X;
    float Y;
    float Z;
};
enum EMovementType { };                                     // values not needed here
class CNavigationInput { public:                            // 0x18 bytes, trivially destructible
    C3DVector     Destination;                              // +0x00
    float         Proximity;                                // +0x0C
    EMovementType MovementType;                             // +0x10
    float         Speed;                                    // +0x14
};

class CNavigatorBase { public:
    virtual ~CNavigatorBase();
};
class CSteeringAgent;
class CNavigatorExternalInterface { public:
    static void __fastcall RemoveAgentListFromLists(std::vector<const CSteeringAgent*>* agents);   // 0x00A47D80
};

class CTCCreatureNavigation : public CTCBase { public:
    CNavigatorBase*                         PNavigator;                   // +0x0C
    std::vector<CNavigatorBase*>            PossibleNavigators;           // +0x10
    CNavigationInput                        NavigationTarget;             // +0x1C
    float                                   Speed;                        // +0x34
    CIntelligentPointer<const CThing>       PThingToNotAvoid;             // +0x38
    std::vector<const CSteeringAgent*>      Agents;                       // +0x40
    long                                    NextUpdate;                   // +0x4C
    long                                    FrameLastNavigated;           // +0x50
    bool                                    CalculatingNewRoute;          // +0x54
    bool                                    CanMoveThroughSolidObjects;   // +0x55
    bool                                    GotNavigationTarget;          // +0x56
    float                                   ProximityForDirectApproach;   // +0x58
    float                                   CurrentSpeed;                 // +0x5C
    virtual ~CTCCreatureNavigation();
};

CTCCreatureNavigation::~CTCCreatureNavigation()
{
    if (PNavigator) {
        CNavigatorExternalInterface::RemoveAgentListFromLists(&Agents);
        PNavigator = 0;
        for (unsigned long i = 0; i < PossibleNavigators.size(); ++i) {
            delete PossibleNavigators[i];
            PossibleNavigators[i] = 0;
        }
    }
}