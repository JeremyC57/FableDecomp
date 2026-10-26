// CTCBuyableHouse::MovedInNotification(const C3DVector& position) -- catalog label; the body reports whether a position is in the object's 12-byte position list (+0x50/+0x54)
// Leaf: ContainsPosition
//
// Retail 0x00A77540, 37 bytes (`ret 4`). Callee: the SGI-style find over 12-byte C3DVector entries
// @ 0x00A7EB00 (`__fastcall`: first in ecx, last in edx, then the value reference and the
// iterator-category tag reference on the stack; element count via the 0x2AAAAAAB divide-by-12).
// Called from CNavigatorManager::IsActionPoint (0x00A150A0) on the object at +8 of a matching
// region, so the "CTCBuyableHouse::MovedInNotification" label is propagated over an unrelated body;
// named here by what it does (CPositionList_00a77540).
// Shape: `last` is a named local (esi); the caller's reference is passed straight through
// (`push edx` of the argument word); the empty tag temporary (user constructor) is homed in that
// argument's dead slot (`lea eax,[esp+8]`); `return find(...) != last;` keeps the full-register bool
// (`xor ecx,ecx ; cmp ; setne cl ; mov al,cl`).

class C3DVector { public: float X, Y, Z; };

struct SRandomAccessTag { SRandomAccessTag() {} };  // random_access_iterator_tag

C3DVector* __fastcall FindPosition(C3DVector* first, C3DVector* last,
                                   const C3DVector& value, const SRandomAccessTag& tag);   // 0x00A7EB00

struct CPositionList_00a77540
{
    unsigned char _pad_0x00[0x50];
    C3DVector*    m_first;                          // +0x50 vector<C3DVector> {first, last, end}
    C3DVector*    m_last;                           // +0x54
    C3DVector*    m_end;                            // +0x58

    bool ContainsPosition(const C3DVector& position) const;
};

bool CPositionList_00a77540::ContainsPosition(const C3DVector& position) const
{
    C3DVector* last = m_last;
    return FindPosition(m_first, last, position, SRandomAccessTag()) != last;
}