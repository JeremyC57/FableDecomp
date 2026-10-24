// CControlsDef::operator=(const CControlsDef&) -- the implicit copy assignment: the parent-def base, the control list, the six toggle flags
// Leaf: ??4CControlsDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00431E80, length 80 bytes, `ret 4`, returns this.
// Name: CControlsDef's vtable 0x01230E84 slot 19 is the stub 0x00431E70 (`jmp 0x00431E80`: a virtual copy that tail-calls this
// assignment); the members copied are exactly Ego_r.pdb CControlsDef's. The catalog's "CParentDefClassBase_CopyAssign_1" is a label.
// Callees: CParentDefClassBase::operator= @ 0x00431ED0 (landed, compiler-generated), vector<CActionInputControl>::operator=
// @ 0x00411B90 (STLport, 28-byte elements: `imul 92492493h ; sar edx,4`; catalog "std_vector_assign").
// Layout: Ego_r.pdb CControlsDef (88 bytes with a 16-byte vector) -> retail 0x50: CParentDefClassBase (pack 1, 0x39 bytes, the
// landed 0x00431ED0 model), Controls +0x3C (STLport 12 bytes; PDB +0x40), ToggleZTarget +0x48, ToggleSpells +0x49, ToggleSneak
// +0x4A, ToggleExpressionMenu +0x4B, ToggleExpressionShift +0x4C, FlourishNeedsAttackButtonHeld +0x4D. No engine header exists
// for CControlsDef; the base is local for the reason the landed 0x00431ED0 gives (the header flattens vptr and base into bytes).
// Shape: /O2; compiler-generated member-wise assignment -- base, then the vector, then one byte move per bool (cl/dl/al rotation).
// It is emitted out of line by an assignment in a helper (the landed 0x00431ED0 pattern; retail's caller is the slot-19 stub).
// Model: local -- engine/CParentDefClassBase.h is the donor 0x3D-byte layout (retail 0x39, see the landed 0x00431ED0) and data-
// only, so the base is local.
class CActionInputControl {
public:
    unsigned char _body[28];
};
namespace std {
template <class T> class allocator {};
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector& operator=(const vector& x);                  // 0x00411B90 for CActionInputControl
};
}
#pragma pack(push, 1)
class CParentDefClassBase {                              // retail 0x39 bytes (CDefClassBase 0x25 + name + sub-def map)
public:
    virtual ~CParentDefClassBase();
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
    unsigned char _body[0x39 - 4];
};
#pragma pack(pop)
class CControlsDef : public CParentDefClassBase {
public:
    std::vector<CActionInputControl> Controls;           // +0x3C
    bool ToggleZTarget;                                  // +0x48
    bool ToggleSpells;                                   // +0x49
    bool ToggleSneak;                                    // +0x4A
    bool ToggleExpressionMenu;                           // +0x4B
    bool ToggleExpressionShift;                          // +0x4C
    bool FlourishNeedsAttackButtonHeld;                  // +0x4D
};
void AssignCControlsDef(CControlsDef& to, const CControlsDef& from)   // emission helper, not a retail function
{
    to = from;
}