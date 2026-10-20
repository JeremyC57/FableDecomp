// CTCCameraPointScriptedSpline::RemoveAnimIndices(const vector<long>& indices) -- catalog label; the body replaces the vector<long> at +0xf0 with a copy of `indices` (clear, then push_back each element)
//
// Retail 0x00857DC0, 120 bytes (`ret 4`). Callees: memmove @ 0x00BFEAE6 (STLport __copy_trivial in
// the inline clear), STLport vector<long>::_M_insert_overflow(pos, const long&, const __true_type&,
// size_type, bool) @ 0x005E1420.
// Label: CTCCameraPointScriptedSpline is 0xdc bytes in the PDB, so a vector at +0xf0 cannot be its;
// the four callers are creature-action members (CCreatureAction_BlockRespond::Initialise 0x00855500,
// CCreatureAction_InterruptableMidAttackAutoTurn::SetupTurningAnimation 0x0087CFE0), so the owner is a
// creature action with an anim-index vector at +0xf0. Modelled as a local class under the catalog leaf.
// Shape: STLport `clear()` = erase(begin, end) whose __copy_trivial is `n ? memmove(..)+n : result`
// (n = finish - finish, hence `cmp eax,eax`); the loop hands each element BY VALUE to a
// `__forceinline` helper, so it is copied into the dead argument slot, and push_back's fresh
// `__true_type()` lands in that same dead slot. The in-place construct is placement new (null test).
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6

inline void* operator new(unsigned int, void* p) { return p; }

struct __true_type { __true_type() {} };

struct CLongVector {                                // STLport vector<long>
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;

    void _M_insert_overflow(long* pos, const long& x, const __true_type&, unsigned int fill_len, bool atend);   // 0x005E1420

    static long* __copy_trivial(const long* first, const long* last, long* result)
    {
        if (last == first)
            return result;
        unsigned int n = (const char*)last - (const char*)first;
        return (long*)((char*)memmove(result, first, n) + n);
    }
    long* erase(long* first, long* last)
    {
        long* i = __copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(_M_start, _M_finish); }
    void push_back(const long& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) long(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);
    }
};
void* FindRef_CLongVector__M_insert_overflow_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CLongVector::_M_insert_overflow(long* pos, const long& x, const __true_type& p2, unsigned int fill_len, bool atend)
{
    void* p = FindRef_CLongVector__M_insert_overflow_0();
    if (!p)
        return;
    if (*(void**)&x != p)
        return;
}
#pragma auto_inline(on)

struct CAnimIndexOwner_00857dc0 {
    unsigned char _pad00[0xf0];
    CLongVector   AnimIndices;                      // +0xf0 vector<long>

    __forceinline void AddAnimIndex(long index) { AnimIndices.push_back(index); }
    void RemoveAnimIndices(const CLongVector& indices);
};

void CAnimIndexOwner_00857dc0::RemoveAnimIndices(const CLongVector& indices)
{
    AnimIndices.clear();
    for (const long* it = indices._M_start; it != indices._M_finish; ++it)
        AddAnimIndex(*it);
}