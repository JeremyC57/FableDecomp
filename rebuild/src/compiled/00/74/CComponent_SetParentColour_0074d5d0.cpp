// std::vector<CDefIndex>::clear() @ 0x0074D5D0 (37 B, thiscall, `ret`)
// Leaf: ?clear@?$vector@VCDefIndex@@V?$allocator@VCDefIndex@@@std@@@std@@QAEXXZ
// Identity (NF385): the sole retail caller is CPersistContext::TransferVector<CDefIndex> 0x0074DE50 (call #2 at 0x0074DE6F, after
// CheckCRC; that caller's binary-in callee is the landed GFSerialiseVectorBinaryIn<CDefIndex> 0x004E89F7, and both PDB name sets hold
// ??$TransferVector@VCDefIndex@@@CPersistContext@@...); the FableWin body of that TransferVector (tc_register.obj) makes the same calls
// in the same order: CheckCRC, then ?clear@?$vector@VCDefIndex@@V?$allocator@VCDefIndex@@@std@@@std@@QAEXXZ (FableWin.pdb 0x01BC9EF2),
// then GFSerialiseVectorBinaryOut / In<CDefIndex>. The body is STLport erase(begin(), end()) for a 4-byte element with trivial
// assignment and destructor: copy [end, end) to begin (a dead loop), then _M_finish = the copy's end. Was the propagated stand-in
// ?CComponent_SetParentColour@@YIXPAUCComponent@@@Z (shared with 0x00618170 / 0x00411980, other vector<T>::clear instances).
class CDefIndex;
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;            // +0x0
    T* _M_finish;           // +0x4
    T* _M_end_of_storage;   // +0x8
    void clear();
};
}

static void copy_range(int* first, int* last, int* dst, std::vector<CDefIndex>* self)
{
    int n = (int)(last - first);
    while (n > 0) {
        *dst = *first;
        ++first;
        ++dst;
        --n;
    }
    self->_M_finish = (CDefIndex*)dst;
}

template <> void std::vector<CDefIndex>::clear()
{
    copy_range((int*)_M_finish, (int*)_M_finish, (int*)_M_start, this);
}
