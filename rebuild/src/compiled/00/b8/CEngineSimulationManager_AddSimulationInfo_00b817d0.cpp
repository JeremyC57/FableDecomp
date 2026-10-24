// CEngineSimulationManager::AddSimulationInfo(const CIVCountedPointer<CEnginePrimitiveSimulationInfo>& info, bool paused, bool fullUpdateOnly) -- queue a primitive's simulation info on the paused, full-update-only or always-update list
// Leaf: ?AddSimulationInfo@CEngineSimulationManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B817D0, 79 bytes. __fastcall, `ret 0Ch`.
// Callees: malloc (CRT import thunk) @ 0x00BFEA0E (the STLport malloc allocator's node allocate).
// Callers: CEngineInternalPrimitiveStaticMesh::InitialiseSimulation 0x00B9DE90, CEngineInternalPrimitiveAnimatedMesh::
//   InitialiseSimulation 0x00BCFCB0 and CEngineInternalPrimitive2DAnimatedMesh::InitialiseSimulation 0x00BCFDE0 (all
//   `g_SimulationManager->AddSimulationInfo(Simulation, false, false)` on the global @ 0x01436E68).
// Identity: Ego_r.pdb / FableWin.pdb `?AddSimulationInfo@CEngineSimulationManager@@QAEXABV?$CIVCountedPointer@
//   VCEnginePrimitiveSimulationInfo@@@@_N1@Z`; the manifest label "CEngineInternalPrimitiveAnimatedMesh_LinkNodeToList"
//   names the caller's class. The boolean parameter names are descriptive.
// Layout: the retail STLport lists are 4 bytes (the node pointer), so FullUpdateOnlySimulationInfoList +0x10,
//   AlwaysUpdateSimulationInfoList +0x14, PausedSimulationInfoList +0x18 (Ego_r: 12-byte Dinkumware lists at +0x10 /
//   +0x1C / +0x28, which engine/CEngineSimulationManager.h carries, so the manager is modelled locally).
// Shape: `if (paused) Paused.push_back(info); else if (fullUpdateOnly) FullUpdateOnly.push_back(info); else
//   Always.push_back(info);` -- VC7.1 tail-merges the three inline push_backs, leaving only the end-node loads in the
//   branches (a list chosen by reference first instead gives `lea` in the branches and one load, 82v79). STLport
//   `push_back(x)` = `insert(end(), x)` with the landed 0x00567FC0 list model (iterator with user ctors, end() read
//   before the node is created, the `__n` / `__p` locals): _M_create_node (allocate 12 bytes, placement copy with its
//   null test: the intrusive CIVCountedPointer copy bumps IVRefCount +4), then the four link stores.
#include "engine/CIVCountedPointeeBase.h"

extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
inline void* operator new(unsigned int, void* where) { return where; }

struct CEnginePrimitiveSimulationInfo : public CIVCountedPointeeBase {
};

template <class T> class CIVCountedPointer {            // intrusive: the pointee carries IVRefCount
public:
    T* Data;                                            // +0x00
    CIVCountedPointer(const CIVCountedPointer& other) : Data(other.Data)
    {
        if (Data)
            ++Data->IVRefCount;
    }
};

namespace _STL {
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x08
};

template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) {}
};

template <class _Tp> class list {                       // STLport list: the end node pointer only (4 bytes)
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;                           // +0x00
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = allocator<_Node>().allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator end() { return iterator(_M_node); }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}

typedef CIVCountedPointer<CEnginePrimitiveSimulationInfo> CSimulationInfoPtr;

class CEngineSimulationManager {
public:
    void* __vftable;                                    // +0x00 (CEngineComponent vptr)
    unsigned char _pad_0x04[0x0C];                      // +0x04 CEngineComponent members
    _STL::list<CSimulationInfoPtr> FullUpdateOnlySimulationInfoList;   // +0x10
    _STL::list<CSimulationInfoPtr> AlwaysUpdateSimulationInfoList;     // +0x14
    _STL::list<CSimulationInfoPtr> PausedSimulationInfoList;           // +0x18
    void AddSimulationInfo(const CSimulationInfoPtr& info, bool paused, bool fullUpdateOnly);
};

void CEngineSimulationManager::AddSimulationInfo(const CSimulationInfoPtr& info, bool paused, bool fullUpdateOnly)
{
    if (paused)
        PausedSimulationInfoList.push_back(info);
    else if (fullUpdateOnly)
        FullUpdateOnlySimulationInfoList.push_back(info);
    else
        AlwaysUpdateSimulationInfoList.push_back(info);
}