// NParticleEngine::CParticleEmitterList::Insert(std::list<CCountedPointer<NParticleEngine::CParticleEmitter> >& visible, const CCountedPointer<NParticleEngine::CParticleEmitter>& emitter, long maxCount) -- insert a visible emitter into the caller's list kept sorted by priority (high first) then distance to the camera (near first); append at the end only while the list is below maxCount
// Leaf: ?Insert@CParticleEmitterList@NParticleEngine@@AAE_NAAV?$list@V?$CCountedPointer@VCParticleEmitter@NParticleEngine@@@@V?$allocator@V?$CCountedPointer@VCParticleEmitter@NParticleEngine@@@@@std@@@std@@ABV?$CCountedPointer@VCParticleEmitter@NParticleEngine@@@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEB190, 318 bytes, thiscall (this unused), `ret 0Ch`, returns bool.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?Insert@CParticleEmitterList@NParticleEngine@@AAE_NAAV?$list@...@std@@ABV?$
//   CCountedPointer@VCParticleEmitter@NParticleEngine@@@@J@Z (private) is the only CParticleEmitterList member taking (list&,
//   const CCountedPointer&, long); its only caller, the landed CParticleEmitterList::FindVisible 0x00AEB2D0, passes exactly
//   (visible, *it, maxCount) and keeps the emitter only when it returns true. The landed binding
//   `?InsertSorted@...QAE_NAAUCEmitterList@2@ABUCEmitterPointer@2@J@Z` is a descriptive stand-in (NAME_FIXES.tsv).
// Callees: malloc 0x00BFEA0E (STLport list node allocate(1), 16 bytes).
// Shape: STLport 4.6 list: size() is distance(begin(), end()) (the counting loop), insert = _M_create_node (malloc + the
//   null-tested placement copy: Data, then Info with `inc [Info]`) and the four link stores; push_front / push_back are
//   insert(begin()) / insert(end()). The first element is tested before the loop; a later hit inserts before the previous
//   node (`--it`), exactly as retail links it before node->_M_prev. The distance key is spilled into the dead `visible` slot.
// Layout: local model of NParticleEngine::CParticleEmitter (retail offsets: the Ego_r Priority ushort:3 bitfield at +0x4C
//   sits at +0x40, DistanceToCam at +0x154) as in the landed FindVisible 0x00AEB2D0: engine/CParticleEmitter.h is the
//   global-namespace flattened layout (vptr as data, the +0x40 bitfield word opaque), so it cannot name Priority (header debt).
extern "C" void* __cdecl malloc(unsigned int size);                     // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CCPPointerInfo {                              // Ego_r.pdb: RefCount +0
public:
    unsigned long RefCount;
};

template <class T> class CCountedPointer {
public:
    T*              Data;                           // +0x00
    CCPPointerInfo* Info;                           // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    T* operator->() const { return Data; }
};

namespace NParticleEngine {
#pragma pack(push, 1)
class CParticleEmitter {
public:
    unsigned char  _pad00[0x40];
    unsigned short Priority : 3;                    // +0x40
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;
    unsigned short _bits9 : 7;
    unsigned char  _pad42[0x112];
    float          DistanceToCam;                   // +0x154
};
#pragma pack(pop)
}

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Dummy> class _List_global {
public:
    typedef _List_node_base _Node;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base(const _List_iterator_base& __x) : _M_node(__x._M_node) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _Nonconst_traits {
    typedef _Tp& reference;
};

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _Self& __x) : _List_iterator_base(__x._M_node) { }
    typename _Traits::reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
    _Self& operator--() { this->_M_decr(); return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
protected:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> {
    typedef _List_base<_Tp, _Alloc> _Base;
    typedef list<_Tp, _Alloc> _Self;
public:
    typedef unsigned int size_type;
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;

    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    size_type size() const
    {
        size_type __result = 0;
        for (_List_node_base* __p = this->_M_node._M_data->_M_next; __p != this->_M_node._M_data; __p = __p->_M_next)
            ++__result;
        return __result;
    }
protected:
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
public:
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
    void push_front(const _Tp& __x) { insert(begin(), __x); }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}

namespace NParticleEngine {
class CParticleEmitterList {
public:
    std::list<CCountedPointer<CParticleEmitter> > Active;   // +0x00
    long MaxActiveEmitters;                                 // +0x04
private:
    bool Insert(std::list<CCountedPointer<CParticleEmitter> >& visible, const CCountedPointer<CParticleEmitter>& emitter,
                long maxCount);
};

bool CParticleEmitterList::Insert(std::list<CCountedPointer<CParticleEmitter> >& visible,
                                  const CCountedPointer<CParticleEmitter>& emitter, long maxCount)
{
    std::list<CCountedPointer<CParticleEmitter> >::iterator it = visible.begin();
    long count = visible.size();
    float distance = emitter->DistanceToCam;
    int priority = emitter->Priority;
    if (count == 0 || (*it)->Priority < priority ||
        ((*it)->Priority == priority && (*it)->DistanceToCam > distance)) {
        visible.push_front(emitter);
        return true;
    }
    for (++it; it != visible.end(); ++it) {
        if ((*it)->Priority < priority || ((*it)->Priority == priority && (*it)->DistanceToCam > distance)) {
            --it;
            visible.insert(it, emitter);
            return true;
        }
    }
    if (count < maxCount) {
        visible.push_back(emitter);
        return true;
    }
    return false;
}
}