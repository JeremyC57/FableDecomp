// NParticleEngine::CParticleEmitterList::Insert(CCountedPointer<NParticleEngine::CParticleEmitter>& emitter) -- add an emitter to the active list by priority: an empty list just takes it; otherwise dead emitters met on the way are erased and the new one goes in front of the first lower-priority emitter; at the end it is appended only while the list is under its maximum; returns whether it was added
// Leaf: ?Insert@CParticleEmitterList@NParticleEngine@@QAE
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEB060, 301 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ?Insert@CParticleEmitterList@NParticleEngine@@QAE_NAAV?$CCountedPointer@VCParticleEmitter@
//   NParticleEngine@@@@@Z (public, NON-const reference; the other Insert is the private (list&, const&, long) at 0x00AEB190, landed).
//   The bound ?Insert@CParticleEmitterList@@QAE_NAAUCEmitterPointer@@@Z is a descriptive stand-in (NAME_FIXES.tsv).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the STLport node allocator), operator delete 0x00BFE9BC (CCountedPointer's count
//   block), and the out-of-line COMDATs of std::list<CCountedPointer<NParticleEngine::CParticleEmitter> >: push_back 0x00AEC780,
//   push_front 0x00AEC740 and insert(iterator, const value_type&) 0x00AEC6F0 (iterator by value, result by hidden pointer; the
//   landed 0x00AEC6F0 names it list<CCountedPointer<CSpawnedFuncBase> >::insert -- its only caller is this row: NAME_FIXES.tsv).
// Layout: CParticleEmitterList as Ego_r.pdb (Active = STLport list, one word; MaxActiveEmitters +4); CCountedPointer from the
//   shared engine/CCountedPointer.h; CParticleEmitter is the local retail model of the landed siblings 0x00AEB190 / FindVisible
//   0x00AEB2D0 (Priority:3 / Dead in the ushort bit-field word at +0x40; engine/CParticleEmitter.h flattens that word -- header
//   debt), with the Ego_d.pdb inline IsDead() const.
// Shape (what matched): the STLport 4.6 list of the landed sibling 0x00AEB190 with two levers. (1) size() is STLport's
//   `distance(begin(), end())` whose input-iterator __distance copies the first iterator (`_InputIterator __it(__first)`): retail
//   compares the begin pointer before copying it into the loop register (`cmp esi,edi ; mov eax,esi`); a raw node loop copies
//   first. (2) the insert position is `--it; Active.insert(it, emitter);` (the walk pointer itself steps back, `mov esi,[esi+4]`,
//   then the by-value iterator is built in the argument slot); building iterator(it->_M_prev) loads into ecx. The first push_back
//   is expanded inline, the last one calls the COMDAT (VC7.1's own per-site choice); erase inlines CCountedPointer's destructor.
#include "engine/CCountedPointer.h"
extern "C" void* __cdecl malloc(unsigned int size);                     // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

namespace NParticleEngine {
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
    bool IsDead() const { return Dead; }
};
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
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

struct input_iterator_tag { };
template <class _InputIterator>
inline int __distance(const _InputIterator& __first, const _InputIterator& __last, const input_iterator_tag&)
{
    int __n = 0;
    _InputIterator __it(__first);
    while (__it != __last) {
        ++__it;
        ++__n;
    }
    return __n;
}
template <class _InputIterator>
inline int distance(const _InputIterator& __first, const _InputIterator& __last)
{
    return __distance(__first, __last, input_iterator_tag());
}

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base(const _List_iterator_base& __x) : _M_node(__x._M_node) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
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

    iterator begin() const { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() const { return iterator(this->_M_node._M_data); }
    size_type size() const
    {
        size_type __result = distance(begin(), end());
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
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator(__next_node);
    }
};
}

namespace NParticleEngine {
class CParticleEmitterList {
public:
    typedef std::list<CCountedPointer<CParticleEmitter> > TEmitterList;
    TEmitterList Active;                                    // +0x00
    long MaxActiveEmitters;                                 // +0x04
    bool Insert(CCountedPointer<CParticleEmitter>& emitter);
};

bool CParticleEmitterList::Insert(CCountedPointer<CParticleEmitter>& emitter)
{
    if (Active.size() == 0) {
        Active.push_back(emitter);
        return true;
    }
    for (TEmitterList::iterator it = Active.begin(); it != Active.end(); ) {
        if ((*it)->IsDead())
            it = Active.erase(it);
        else {
            if ((*it)->Priority < emitter->Priority) {
                if (it == Active.begin()) {
                    Active.push_front(emitter);
                    return true;
                }
                --it;
                Active.insert(it, emitter);
                return true;
            }
            ++it;
        }
    }
    if ((long)Active.size() < MaxActiveEmitters) {
        Active.push_back(emitter);
        return true;
    }
    return false;
}
}