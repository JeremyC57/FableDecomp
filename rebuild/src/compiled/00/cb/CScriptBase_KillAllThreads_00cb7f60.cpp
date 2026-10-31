// CScriptBase::KillAllThreads() -- terminate every spawned thread (clearing the list) and every active entity script (marking a live scripted entity to die on level unload first), with PActiveThread set to each one while it terminates, then empty the entity-script map
// Leaf: ?KillAllThreads@CScriptBase@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB7F60, 394 bytes, thiscall, `ret`.
// Identity (two sources): (1) Ego_r.pdb / FableWin.pdb ?KillAllThreads@CScriptBase@@QAEXXZ (public, void(void)); the body is
//   exactly that over the PDB members: Threads +0x04 (STLport list<CCountedPointer<CSpawnedFuncBase> >, each thread
//   CASuspendableProcess::TerminateProcess 0x00A4B200, then the list's clear 0x00CBB090), EntityScripts +0x08 (CVectorMap
//   of (unsigned __int64, CCountedPointer<CActiveEntityScriptBase>), each script terminated, then erase(begin, end)) and
//   PActiveThread +0x2C (saved, set per thread, restored); (2) `this` is a CScriptBase: the same members are read by the
//   landed CScriptBase rows (RunEntityThreads 0x00CB8170, KillSpawnedFunction 0x00CB7AA0, IsActiveThreadTerminating
//   0x00CB7940). The landed binding `?Shutdown@CScriptBase@@QAEXXZ` (caller 0x004B3A77) is not a PDB name: NAME_FIXES.tsv.
//   The manifest label "Quest_DeactivateAllProcesses" is descriptive.
// Callees: CASuspendableProcess::TerminateProcess 0x00A4B200 (bound), std::_List_base<..>::clear 0x00CBB090 (the landed
//   binding `?clear@CEventList@@QAEXXZ` is not a PDB class: NAME_FIXES.tsv), the entity copy's CScriptThing slots 75
//   IsAlive (+0x12C) and 70 SetToKillOnLevelUnload (+0x118), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC,
//   __copy 0x00CBB120 and __destroy_aux 0x00CBB1B0 (bound by RunEntityThreads 0x00CB8170).
// Shape: CActiveEntityScriptBase::GetEntity() (FableWin.pdb) returns the +0x48 CScriptThing by value, expanded inline
//   (copy: vtable 0x01238C8C, PImp addref; dtor: vtable, PImp release, ~CBaseClass); the inlined STLport erase is
//   RunEntityThreads' (same-TU stand-in __copy body, so the tags share the frame bytes [esp+13h] / [esp+17h]).
#include <stddef.h>
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CScriptBase.h"
#include "engine/CActiveEntityScriptBase.h"

// Retail expands the CScriptThing copy and destructor and the by-value GetEntity at this site.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing CActiveEntityScriptBase::GetEntity() const { return Entity; }

namespace std {
template <class T> class allocator {
public:
    allocator() { }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
// Same-TU stand-in body of the out-of-line counted copy 0x00CBB120 (it never reads the tag).
#pragma auto_inline(off)
template <class InputIter, class OutputIter, class Distance>
__declspec(noinline) OutputIter __fastcall __copy(InputIter first, InputIter last, OutputIter result, const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
#pragma auto_inline(on)
template <class T>
inline T* __copy_ptrs(const T* first, const T* last, T* result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}
template <class ForwardIterator>
void __fastcall __destroy_aux(ForwardIterator first, ForwardIterator last, const __false_type&);   // 0x00CBB1B0
template <class ForwardIterator>
inline void _Destroy(ForwardIterator first, ForwardIterator last)
{
    __destroy_aux(first, last, __false_type());
}
template <class T, class Alloc = allocator<T> >
class vector {                                             // STLport 4.6 vector (12 bytes)
public:
    typedef T* iterator;
    T* _M_start;                                           // +0
    T* _M_finish;                                          // +4
    T* _M_end_of_storage;                                  // +8
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    iterator erase(iterator first, iterator last)
    {
        T* i = __copy_ptrs(last, _M_finish, first, __false_type());
        _Destroy(i, _M_finish);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
template <class T> struct _List_node {
    _List_node* _M_next;                                   // +0
    _List_node* _M_prev;                                   // +4
    T           _M_data;                                   // +8
};
template <class T, class Alloc> class _List_base {        // STLport 4.6: one node pointer (4 bytes)
public:
    _List_node<T>* _M_node;
    void clear();                                          // 0x00CBB090
};
template <class T, class Alloc = allocator<T> >
class list : public _List_base<T, Alloc> {
public:
    typedef _List_node<T>* iterator;
    iterator begin() { return this->_M_node->_M_next; }
    iterator end() { return this->_M_node; }
};
}   // namespace std

template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap {   // 0x10: vector, empty Compare, Dirty
public:
    std::vector<std::pair<K, V> > Vector;                  // +0x00
    C    Compare;                                          // +0x0c
    bool Dirty;                                            // +0x0d
};
typedef std::list<CCountedPointer<CSpawnedFuncBase> > CThreadList;
typedef CVectorMap<FableNScriptUInt64, CCountedPointer<CActiveEntityScriptBase>,
                   CKeyPairCompareLess<FableNScriptUInt64, CCountedPointer<CActiveEntityScriptBase> > > CEntityScriptMap;

void CScriptBase::KillAllThreads()
{
    CThreadList& threads = *reinterpret_cast<CThreadList*>(Threads);
    CSpawnedFuncBase* activeThread = PActiveThread;
    for (CThreadList::iterator it = threads.begin(); it != threads.end(); it = it->_M_next) {
        CSpawnedFuncBase* thread = it->_M_data.Data;
        PActiveThread = thread;
        thread->TerminateProcess();
        PActiveThread = activeThread;
    }
    threads.clear();

    CEntityScriptMap& scripts = *reinterpret_cast<CEntityScriptMap*>(EntityScripts);
    CSpawnedFuncBase* activeScript = PActiveThread;
    for (std::pair<FableNScriptUInt64, CCountedPointer<CActiveEntityScriptBase> >* it = scripts.Vector.begin();
         it != scripts.Vector.end(); ++it) {
        CActiveEntityScriptBase* script = it->second.Data;
        PActiveThread = script;
        if (script->Flags & 1) {
            if (script->GetEntity().IsAlive())
                script->GetEntity().SetToKillOnLevelUnload(true);
        }
        script->TerminateProcess();
        PActiveThread = activeScript;
    }
    scripts.Vector.clear();
}