// CTCVillage::OnSerialise(CPersistContext& context, bool fullSave) -- persists the village's population flag, the player's crime set (as a vector), the guard-sighting frame (relative to now), the fine paid, limbo state, enemy flag and hero-criminal flag
// Leaf: ?OnSerialise@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065CE9B, 372 bytes, unaligned in the /Os frame-pointer CTCVillage TU, thiscall
//   `ret 8`. Ego_r.pdb name set: ?OnSerialise@CTCVillage@@UAEXAAVCPersistContext@@_N@Z (vtable 0x0125A9AC slot 1).
//   Catalog label "Player_TransferPersistence" is propagated and wrong.
// Callees: CPersistContext::Transfer(name, bool&) 0x004107C0 (catalog "GetCellBoundingBox"), IsSaving 0x004AEF50,
//   vector<long>::reserve 0x00660684, std::copy(set iterators, back_insert_iterator) 0x00661287 (__fastcall, hidden
//   result in ecx), TransferVector(name, vector<long>*) 0x005815D6, GFGetCurrentFrame 0x0049D870,
//   Transfer(name, long&, const long&) 0x00410BE0, set<long>::insert 0x00514230 (pair by hidden pointer),
//   Transfer(name, long&) 0x004110B0, Transfer(name, bool&, const bool&) 0x004045C0, free 0x00BFEA14.
// Shape: /Os + no frame-pointer omission (ebp frame, 0x1C locals, ebx = 0 for every zero store). The two branch-local
//   vectors get separate slots ([ebp-10] saving, [ebp-1c] loading); the pair temp of set::insert overlaps the saving
//   vector at [ebp-c], which needs a pair with a user constructor (else it comes back in edx:eax). The loop caches
//   crimes.end() in [ebp-4]; the literal default 0 is homed in the dead `context` slot [ebp+8], the bool default false
//   in the dead `fullSave` byte [ebp+0f]. The vector dtor is the STLport `if (_M_start) free(_M_start)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                   // 0x0049D870

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void reserve(unsigned int __n);                    // 0x00660684
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class T> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
};
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; pair(const _T1& a, const _T2& b) : first(a), second(b) { } };
template <class _Tp> struct less { };
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree_iterator<_Key> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    iterator begin() const { return iterator(_M_header->_M_left); }
    iterator end() const { return iterator(_M_header); }
    unsigned int size() const { return _M_node_count; }
    pair<iterator, bool> insert(const _Key& __x);      // 0x00514230
};
template <class C> struct back_insert_iterator {
    C* container;
    explicit back_insert_iterator(C& x) : container(&x) { }
};
template <class C> inline back_insert_iterator<C> back_inserter(C& x) { return back_insert_iterator<C>(x); }
back_insert_iterator<vector<long> > __fastcall copy(_Rb_tree_iterator<long> first, _Rb_tree_iterator<long> last,
                                                    back_insert_iterator<vector<long> > result);   // 0x00661287
}

class CPersistContext {
public:
    bool IsSaving() const;                                                     // 0x004AEF50
    void Transfer(const char* name, bool& value);                              // 0x004107C0
    void Transfer(const char* name, long& value);                              // 0x004110B0
    void Transfer(const char* name, bool& value, const bool& defaultValue);    // 0x004045C0
    void Transfer(const char* name, long& value, const long& defaultValue);    // 0x00410BE0
    void TransferVector(const char* name, std::vector<long>* value);           // 0x005815D6
};

class CTCVillage {
public:
    unsigned char  _pad04[0x64 - 0x04];                // vptr at +0x00
    std::set<long> PlayersCrimes;                      // +0x64
    unsigned char  _pad70[0x7a - 0x70];
    bool           HasBeenInitiallyPopulated;          // +0x7a
    unsigned char  _pad7b;
    bool           IsEnemyBecauseOfCrime;              // +0x7c
    unsigned char  _pad7d[3];
    long           FramePlayerLastSeenByGuard;         // +0x80
    unsigned char  _pad84[0xa0 - 0x84];
    long           AmountOfFineAlreadyPaid;            // +0xa0
    unsigned char  _padA4[0xb2 - 0xa4];
    bool           Limbo;                              // +0xb2
    bool           LastLimbo;                          // +0xb3
    unsigned char  _padB4[0xe4 - 0xb4];
    bool           CurrentIsHeroCriminal;              // +0xe4
    virtual void OnSerialise(CPersistContext& context, bool fullSave);   // vtable 0x0125A9AC slot 1 (slot 0 modelled away)
};

void CTCVillage::OnSerialise(CPersistContext& context, bool fullSave)
{
    context.Transfer("HasBeenInitiallyPopulated", HasBeenInitiallyPopulated);
    if (context.IsSaving()) {
        std::vector<long> crimes;
        crimes.reserve(PlayersCrimes.size());
        std::copy(PlayersCrimes.begin(), PlayersCrimes.end(), std::back_inserter(crimes));
        context.TransferVector("PlayersCrimes", &crimes);
        long framesSinceSeen = FramePlayerLastSeenByGuard - CWorld::GetFrame();
        context.Transfer("FramePlayerLastSeenByGuard", framesSinceSeen, 0);
    } else {
        std::vector<long> crimes;
        context.TransferVector("PlayersCrimes", &crimes);
        std::vector<long>::iterator end = crimes.end();
        for (std::vector<long>::iterator it = crimes.begin(); it != end; ++it)
            PlayersCrimes.insert(*it);
        context.Transfer("FramePlayerLastSeenByGuard", FramePlayerLastSeenByGuard);
        FramePlayerLastSeenByGuard += CWorld::GetFrame();
    }
    if (fullSave)
        context.Transfer("AmountOfFineAlreadyPaid", AmountOfFineAlreadyPaid);
    context.Transfer("Limbo", Limbo);
    if (!context.IsSaving())
        LastLimbo = Limbo;
    context.Transfer("IsEnemyBecauseOfCrime", IsEnemyBecauseOfCrime);
    context.Transfer("CurrentIsHeroCriminal", CurrentIsHeroCriminal, false);
}