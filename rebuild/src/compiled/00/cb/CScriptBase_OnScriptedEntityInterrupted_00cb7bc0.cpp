// CScriptBase::OnScriptedEntityInterrupted(unsigned __int64 entityID) -- flag the active entity script bound to the entity UID as interrupted (it restarts on its next update)
// Leaf: ?OnScriptedEntityInterrupted@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB7BC0, 64 bytes (`ret 8`, thiscall).
// Identity: Ego_r.pdb `?OnScriptedEntityInterrupted@CScriptBase@@...`; the only caller 0x004AFBE5 passes an entity UID.
//   The manifest label "NavQuadTree_MarkNodeActive" is propagated and wrong.
// Callees: CVectorMap<unsigned __int64, ...>::LowerBound 0x00CB9840 (EntityScripts, CScriptBase +0x08).
// Shape: CVectorMap::Find inline (LowerBound, then `it == end() || key < it->first` -> end()); the 64-bit key compare
//   is high dword first; CSpawnedFuncBase::SetFuncAsInterrupted(true) is the inline `mov byte [eax+29h],1`.
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
class CCharString {
public:
    void* m_pStringData;
};
struct CCPPointerInfo {                                    // CCountedPointer count block
    long  RefCount;                                        // +0x00
    void  (__fastcall* DeleteFunc)(void* data);            // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    T* operator->() const { return Data; }
};
class CASuspendableProcess {
public:
    virtual ~CASuspendableProcess();
    bool  Running;                                         // +0x04
    bool  StopRunning;                                     // +0x05
    bool  TerminateUsingExceptions;                        // +0x06
    float TimeProcessSliceStarted;                         // +0x08
    float TimesliceLength;                                 // +0x0c
    unsigned long _pad10;                                  // +0x10
};
class CSpawnedFuncBase : public CASuspendableProcess {
public:
    unsigned long ID;                                      // +0x14
    long          LastScriptFrame;                         // +0x18
    long          CurrentScriptFrame;                      // +0x1c
    CCharString   Name;                                    // +0x20
    CCharString   AssociatedRegionName;                    // +0x24
    bool          FuncPaused;                              // +0x28
    bool          Interrupted;                             // +0x29
    void SetFuncAsInterrupted(bool interrupted) { Interrupted = interrupted; }
};
class CActiveEntityScriptBase : public CSpawnedFuncBase {
};
namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                                    // +0
    _Tp* _M_finish;                                                   // +4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;           // +8
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
};
template <class _Tp> struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp, class _Alloc = allocator<_Tp> >
class list {                                               // STLport 4.6 list: one node pointer (4 bytes)
public:
    _List_node_base<_Tp>* _M_node;
};
}
template <class K, class V> class CKeyPairCompareLess {    // empty
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};
template <class V> class CCompareString {                  // empty
};
template <class K, class V, class C> class CVectorMap {
public:
    typedef _STL::pair<K, V> value_type;
    typedef value_type* iterator;
    _STL::vector<value_type> Vector;                       // +0x00
    C    Compare;                                          // +0x0c
    bool Dirty;                                            // +0x0d
    iterator LowerBound(const K& key);                     // out of line (EntityScripts: 0x00CB9840)
    iterator UpperBound(const K& key);                     // out of line (0x00CB98A0)
    iterator begin() { return Vector.begin(); }
    iterator end() { return Vector.end(); }
    iterator Find(const K& key)
    {
        iterator it = LowerBound(key);
        if (it == end() || key < it->first)
            return end();
        return it;
    }
};
class CEntityScriptBindingBase;
typedef CVectorMap<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>, CKeyPairCompareLess<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > > CEntityScriptMap;
class CScriptBase {
public:
    virtual ~CScriptBase();
    _STL::list<CCountedPointer<CSpawnedFuncBase> > Threads;                                  // +0x04
    CEntityScriptMap EntityScripts;                                                          // +0x08
    CVectorMap<CCharString, CCountedPointer<CEntityScriptBindingBase>, CCompareString<CCountedPointer<CEntityScriptBindingBase> > > EntityScriptBindings;   // +0x18
    unsigned long     LastWorldUpdateFrame;                                                  // +0x28
    CSpawnedFuncBase* PActiveThread;                                                         // +0x2c
    void OnScriptedEntityInterrupted(unsigned __int64 entityID);
};

void CScriptBase::OnScriptedEntityInterrupted(unsigned __int64 entityID)
{
    CEntityScriptMap::iterator it = EntityScripts.Find(entityID);
    if (it != EntityScripts.end())
        it->second->SetFuncAsInterrupted(true);
}