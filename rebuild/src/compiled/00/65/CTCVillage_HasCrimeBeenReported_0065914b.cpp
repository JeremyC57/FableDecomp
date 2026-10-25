// CTCVillage::HasCrimeBeenReported(long crime) -- true when the crime is already in the village's PlayersCrimes set
// Leaf: ?HasCrimeBeenReported@CTCVillage@@QAE_NJ@Z
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065914B, 25 bytes, thiscall `ret 4`: `lea esi,[ecx+64h] ; push &crime ;
//   call _M_find ; cmp eax,[esi] ; setne al` -- `PlayersCrimes.find(crime) != PlayersCrimes.end()`.
// Renamed from CTCVillage::IsCriminalKnown / CBankFileAsyncData::StartRead (descriptive / propagated; FINDINGS chunk 381).
// Name: Ego_r.pdb / FableWin.pdb ?HasCrimeBeenReported@CTCVillage@@QAE_NJ@Z (non-const, bool, one long) -- the only
//   CTCVillage `bool (long)` member besides PlayerCanApologiseForCurrentCrimes, which is landed at 0x0065A686.
// Identity: every caller gets `this` from CTCVillageMember::GetPVillage 0x007EE540 (e.g. CTCGuard::WitnessedCrime
//   0x007B2FD0, CTCShopKeeper::WitnessedCrime, CTCCreatureOpinionOfHero::ReportAllCrimes) and passes the deed handle;
//   the callee 0x00614A10 is the landed set<long>::_M_find, and CTCVillage::ReportCrime 0x0065CB6B tests the same set
//   with `find(crime) == end()` before reporting.
// Layout: struct_layouts_egor CTCVillage::PlayersCrimes (set<long>) at PDB +0xDC after fifteen 12-byte lists from +0x28;
//   with STLport's 4-byte lists that is retail +0x28 + 15*4 = +0x64. engine/CTCVillage.h is the PDB layout -> local model.
// Callee: std::_Rb_tree<long, long, _Identity<long>, less<long>, allocator<long> >::_M_find(const long&) const 0x00614A10
//   (STLport `iterator find(k) { return iterator(_M_find(k)); }`; end() is the header word at +0).
// Shape: the found iterator is held in a local so end() is read after the call (`cmp eax,[esi]`); a single
//   `find(crime) != end()` expression lets VC7.1 load end() first (29 v 25 bytes); the `if (..) return true;
//   return false;` form gives the bare `setne al` (returning the comparison goes through `xor ecx,ecx ; setne cl`).
namespace std {
template <class T> struct less;
template <class T> struct _Identity;
template <class T> struct allocator;
struct _Rb_tree_node_base;
template <class _Value> struct _Rb_tree_node;

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    _Rb_tree_node<_Value>* _M_header;                        // +0x00 (the header node: end())
    unsigned int           _M_node_count;                    // +0x04
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const;   // 0x00614A10 for set<long>
};

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    typedef _Rb_tree_node<_Key>* iterator;                   // the STLport iterator wraps the node pointer
    _Rep_type _M_t;
    iterator end() { return _M_t._M_header; }
    iterator find(const _Key& __x) { return _M_t._M_find(__x); }
};
}

class CTCVillage {
public:
    unsigned char   _pad_0x00[0x64];
    std::set<long>  PlayersCrimes;                           // +0x64 (PDB +0xDC)
    bool HasCrimeBeenReported(long crime);
};

bool CTCVillage::HasCrimeBeenReported(long crime)
{
    std::set<long>::iterator it = PlayersCrimes.find(crime);
    if (it != PlayersCrimes.end())
        return true;
    return false;
}
