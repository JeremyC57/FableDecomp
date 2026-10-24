// CAnimateThing::GetAllCombinationAnimsFromType(long type, vector<CAnimSet*>& out) -- append the address of every combination anim set of the given type to a caller vector
// Leaf: GetAllCombinationAnimsFromType
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070D0A0, length 94 bytes (+ int3 padding). __fastcall, `ret 8`.
//         Ego_r.pdb: `?GetAllCombinationAnimsFromType@CAnimateThing@@QAEXJAAV?$vector@PAVCAnimSet@@...@std@@@Z`.
// Callees: vector<CAnimSet*>::_M_insert_overflow(pos, x, __true_type, 1, true) @ 0x0066C3F0 (COMDAT-folded with the other
//          4-byte POD vectors; its manifest label "std::vector<uint>::_Ufill" is propagated).
// Identity: the manifest label "CActionState_AddAnimSetToList" is descriptive. Retail CAnimateThing starts with CurrentAnim
//   (CCompleteAnimation: MainAnimSet 0x54 bytes, TransitionAnimSets +0x54, CombinationAnims +0x58 -- 4-byte STLport
//   lists); a list node is {next, prev, CAnimSet}, the set's Type byte at +0x50 (node +0x58), the same layout the landed
//   GetCombinationAnimFromTypeAndIndex 0x0070C170 reads.
// Shape: a plain iterator loop over CombinationAnims (begin = head->next, end re-read from the member every pass) with the
//   STLport push_back inlined: `&*it` is an rvalue, so VC7.1 copies it into the dead `out` argument slot and the empty
//   __true_type temporary of the overflow call shares that slot; the room case is the placement store with its null
//   test and `add [esi+4],4`. The overflow callee is a same-TU stand-in body under auto_inline(off): with a declaration
//   only, VC7.1 homes the `&*it` temporary in the dead `type` slot ([esp+14h]) instead of sharing the `out` slot.
inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() {} };

class CAnimSet {
public:
    unsigned char _pad00[0x50];
    unsigned char Type;                                 // +0x50
    unsigned char Flags;                                // +0x51 Looping/Finished/Forwards/HadUpdate/OwnedByMainAnim
};

struct SAnimSetNode {                                   // STLport _List_node<CAnimSet> (0x5C bytes)
    SAnimSetNode* _M_next;
    SAnimSetNode* _M_prev;
    CAnimSet _M_data;
};
struct SAnimSetIterator {                               // STLport _List_iterator
    SAnimSetNode* _M_node;
    SAnimSetIterator(SAnimSetNode* x) : _M_node(x) {}
    SAnimSetIterator(const SAnimSetIterator& x) : _M_node(x._M_node) {}
    CAnimSet& operator*() const { return _M_node->_M_data; }
    CAnimSet* operator->() const { return &_M_node->_M_data; }
    SAnimSetIterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const SAnimSetIterator& x) const { return _M_node != x._M_node; }
};
struct CAnimSetList {                                   // list<CAnimSet> (retail 4 bytes)
    SAnimSetNode* _M_node;
    SAnimSetIterator begin() { return SAnimSetIterator(_M_node->_M_next); }
    SAnimSetIterator end() { return SAnimSetIterator(_M_node); }
};

template <class _Tp> struct vector {                   // STLport vector (12 bytes)
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);   // 0x0066C3F0
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
    }
};

extern "C" void* __cdecl malloc(unsigned int size);
// Same-TU stand-in body (the real one, 0x0066C3F0, is a COMDAT of another TU): VC7.1 uses what it learns from it.
#pragma auto_inline(off)
template <class _Tp>
__declspec(noinline) void vector<_Tp>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend)
{
    unsigned int __old_size = (unsigned int)(_M_finish - _M_start);
    unsigned int __len = __old_size + (__old_size < __fill_len ? __fill_len : __old_size);
    _Tp* __new_start = (_Tp*)malloc(__len * sizeof(_Tp));
    _Tp* __new_finish = __new_start;
    for (_Tp* __p = _M_start; __p != __position; ++__p, ++__new_finish)
        *__new_finish = *__p;
    for (; __fill_len > 0; --__fill_len, ++__new_finish)
        *__new_finish = __x;
    if (!__atend)
        for (_Tp* __q = __position; __q != _M_finish; ++__q, ++__new_finish)
            *__new_finish = *__q;
    _M_start = __new_start;
    _M_finish = __new_finish;
    _M_end_of_storage = __new_start + __len;
}
#pragma auto_inline(on)

class CAnimateThing {
public:
    unsigned char MainAnimSet[0x54];                    // +0x00 CurrentAnim.MainAnimSet (retail CAnimSet)
    CAnimSetList TransitionAnimSets;                    // +0x54 CurrentAnim.TransitionAnimSets
    CAnimSetList CombinationAnims;                      // +0x58 CurrentAnim.CombinationAnims
    void GetAllCombinationAnimsFromType(long type, vector<CAnimSet*>& out);
};

void CAnimateThing::GetAllCombinationAnimsFromType(long type, vector<CAnimSet*>& out)
{
    for (SAnimSetIterator it = CombinationAnims.begin(); it != CombinationAnims.end(); ++it)
        if (it->Type == type)
            out.push_back(&*it);
}