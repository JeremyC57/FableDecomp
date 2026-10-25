// CTCGuard::OnSerialise(CPersistContext& context, bool) -- persist the guard's criminal bookkeeping: the pending-crime deque through a vector (copied out when saving, copied back in when loading), the frame stamps, the bribe pool and the last crime severity
// Leaf: ?OnSerialise@CTCGuard@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B3760, length 367 bytes (thiscall, `ret 8`; 0x16F bytes + int3).
// Identity: CTCGuard vtable 0x0126D80C slot 1 (+0x04 = 0x0126D810), the CTCBase OnSerialise slot (as the landed
//   CTCObjectAugmentations::OnSerialise 0x00767200); Ego_r.pdb ?OnSerialise@CTCGuard@@UAEXAAVCPersistContext@@_N@Z.
//   The catalog label "Save_TransferCriminalRecord" is descriptive. Keys: "PendingCrimes", "FramePendingCrimesAdded",
//   "FrameLastBribeAdded", "FrameLastCrimeSeen", "FrameLastReceivedApology", "BribePool", "LastCrimeSeenSeverity".
// Callees: std::copy(deque iterator, deque iterator, back_insert_iterator<vector>) 0x007B52B0 (out of line, the
//   inserter returned through the hidden pointer), CPersistContext::TransferVector<CCrimeWaitingForBribe> 0x007B5470,
//   STLport __copy(const T*, const T*, back_insert_iterator<deque>, const random_access_iterator_tag&, int*) 0x007B5400,
//   free 0x00BFEA14 (the vector's block), CPersistContext::Transfer(const char*, long&, const long&) 0x00410BE0 (five
//   times), CPersistContext::Transfer<ECrimeSeverity> 0x007B45A0.
// Layout: retail CTCGuard (Ego_r.pdb order, STLport 40-byte deque): PDef +0x0C, BribePool +0x10, PendingCrimes +0x14
//   (start iterator +0x14, finish +0x24), FramePendingCrimesAdded +0x3C, FrameLastBribeAdded +0x40,
//   LastCrimeSeenSeverity +0x44, FrameLastCrimeSeen +0x48, FrameLastReceivedApology +0x4C; CPersistContext Mode +0x18
//   (1 text save, 3 binary save).
// Shape: `if (context.IsSaving()) { vector<CCrimeWaitingForBribe> crimes; copy(PendingCrimes.begin(),
//   PendingCrimes.end(), back_inserter(crimes)); context.TransferVector("PendingCrimes", crimes); } else { vector crimes;
//   context.TransferVector("PendingCrimes", crimes); copy(crimes.begin(), crimes.end(), back_inserter(PendingCrimes)); }`
//   (the two vector teardowns cross-jumped into one free), then the six member transfers with 0 defaults. The
//   pointer-range copy inlines down to __copy, whose same-TU `__declspec(noinline)` body never reads its tag, so VC7.1
//   homes the tag, the discarded inserter and every `0L` default in the dead `context` slot (context lives in edi).
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

class CCrimeWaitingForBribe {                               // 8 bytes
public:
    long DeedHandle;                                        // +0x00
    long Type;                                              // +0x04 EOpinionDeedType
};

enum ECrimeSeverity { CRIME_SEVERITY_NONE };

namespace std {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct output_iterator_tag { output_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Tp> struct _Deque_iterator_base {
    _Tp*  _M_cur;                                           // +0x00
    _Tp*  _M_first;                                         // +0x04
    _Tp*  _M_last;                                          // +0x08
    _Tp** _M_node;                                          // +0x0C
};

template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    typedef _Deque_iterator<_Tp, _Traits> _Self;
    _Deque_iterator(const _Self& __x) : _Deque_iterator_base<_Tp>(__x) { }
    static int _S_buffer_size() { return sizeof(_Tp) < 128 ? int(128 / sizeof(_Tp)) : 1; }   // STLport _MAX_BYTES = 128 (0x80-byte nodes)
    typename _Traits::reference operator*() const { return *this->_M_cur; }
    int operator-(const _Self& __x) const
    {
        return _S_buffer_size() * (this->_M_node - __x._M_node - 1) + (this->_M_cur - this->_M_first) + (__x._M_last - __x._M_cur);
    }
    _Self& operator++()
    {
        ++this->_M_cur;
        if (this->_M_cur == this->_M_last) {
            ++this->_M_node;
            this->_M_first = *this->_M_node;
            this->_M_last = this->_M_first + _S_buffer_size();
            this->_M_cur = this->_M_first;
        }
        return *this;
    }
};

template <class _Tp, class _Traits>
inline random_access_iterator_tag __ITERATOR_CATEGORY(const _Deque_iterator<_Tp, _Traits>&)
{
    return random_access_iterator_tag();
}
template <class _Tp, class _Traits> inline int* __DISTANCE_TYPE(const _Deque_iterator<_Tp, _Traits>&) { return (int*)0; }
template <class _Tp> inline random_access_iterator_tag __ITERATOR_CATEGORY(const _Tp*) { return random_access_iterator_tag(); }
template <class _Tp> inline int* __DISTANCE_TYPE(const _Tp*) { return (int*)0; }

template <class _Tp, class _Alloc> class _Deque_base {
public:
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator _M_start;                                      // +0x00
    iterator _M_finish;                                     // +0x10
    _STLP_alloc_proxy<_Tp**, _Tp*, allocator<_Tp*> > _M_map;                  // +0x20
    _STLP_alloc_proxy<unsigned int, _Tp, allocator<_Tp> > _M_map_size;        // +0x24
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class deque : protected _Deque_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef typename _Deque_base<_Tp, _Alloc>::iterator iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void push_back(const value_type& __t);
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef const _Tp* const_iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
    void push_back(const _Tp& __x);
};

template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) { }
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __value)
    {
        container->push_back(__value);
        return *this;
    }
    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
};

template <class _Container> inline back_insert_iterator<_Container> back_inserter(_Container& __x)
{
    return back_insert_iterator<_Container>(__x);
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{                                                           // 0x007B5400 (the deque inserter instance)
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __fastcall __copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, __ITERATOR_CATEGORY(__first), __DISTANCE_TYPE(__first));
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __fastcall copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{                                                           // 0x007B52B0 (the deque-iterator instance stays out of line)
    return __copy_aux(__first, __last, __result, __false_type());
}

}

class CPersistContext {
public:
    unsigned char _pad00[0x18];
    long          Mode;                                     // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, long& value, const long& defaultValue);                       // 0x00410BE0
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);         // 0x007B45A0 (ECrimeSeverity)
    template <class T> void TransferVector(const char* name, std::vector<T>& value);             // 0x007B5470
};

class CThing;

class CTCBase {
public:
    virtual ~CTCBase();
    virtual void OnSerialise(CPersistContext& context, bool);   // slot 1
    CThing* PThing;                                         // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCGuard : public CTCBase {
public:
    void*                                PDef;                              // +0x0C CDefPointer<const CGuardDef>
    long                                 BribePool;                         // +0x10
    std::deque<CCrimeWaitingForBribe>    PendingCrimes;                     // +0x14
    long                                 FramePendingCrimesAdded;           // +0x3C
    long                                 FrameLastBribeAdded;               // +0x40
    ECrimeSeverity                       LastCrimeSeenSeverity;             // +0x44
    long                                 FrameLastCrimeSeen;                // +0x48
    long                                 FrameLastReceivedApology;          // +0x4C
    bool                                 WaitingForRegionKickConfirmation;  // +0x50
    virtual void OnSerialise(CPersistContext& context, bool);
};

void CTCGuard::OnSerialise(CPersistContext& context, bool)
{
    if (context.IsSaving()) {
        std::vector<CCrimeWaitingForBribe> crimes;
        std::copy(PendingCrimes.begin(), PendingCrimes.end(), std::back_inserter(crimes));
        context.TransferVector("PendingCrimes", crimes);
    }
    else {
        std::vector<CCrimeWaitingForBribe> crimes;
        context.TransferVector("PendingCrimes", crimes);
        std::copy(crimes.begin(), crimes.end(), std::back_inserter(PendingCrimes));
    }
    context.Transfer("FramePendingCrimesAdded", FramePendingCrimesAdded, 0L);
    context.Transfer("FrameLastBribeAdded", FrameLastBribeAdded, 0L);
    context.Transfer("FrameLastCrimeSeen", FrameLastCrimeSeen, 0L);
    context.Transfer("FrameLastReceivedApology", FrameLastReceivedApology, 0L);
    context.Transfer("BribePool", BribePool, 0L);
    context.Transfer("LastCrimeSeenSeverity", LastCrimeSeenSeverity, CRIME_SEVERITY_NONE);
}