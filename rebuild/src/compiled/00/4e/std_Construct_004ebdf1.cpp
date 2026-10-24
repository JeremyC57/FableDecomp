// std::_Construct(CCardPositionsDef*, const CCardPositionsDef&) -- placement copy of a card-positions def: the CParentDefClassBase copy, CCardPositionsDef's vtable, then the Offset vector copy
// Leaf: _Construct
// Retail: Fable.exe @ 0x004EBDF1, 37 bytes (an unaligned, densely packed TU: CCardPositionsDef::GetSizeofClass follows at 0x004EBE16);
//   __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CParentDefClassBase copy ctor 0x0046EE98, vector<C2DVector> copy ctor 0x004EBE1A; CCardPositionsDef vtable 0x012439DC
//   (rtti-vtables.tsv: slot 20 GetSizeofClass 0x004EBE16, slot 19 Copy 0x004EBEBC). The catalog label pair<CCharString const,
//   CArray<unsigned long>> is propagated.
// Layout: CParentDefClassBase is 0x3C bytes in retail (the landed CCardPositionsDef ctor 0x004F9D15 puts the first own member at
//   +0x3C); Ego_r.pdb CCardPositionsDef: vector<C2DVector> Offset after the base. engine/CParentDefClassBase.h flattens the vptr
//   into a packed 0x3D-byte body, so the polymorphic base is modelled locally.
// Shape: the implicit copy constructor -- base copy (ecx = slot, push source), vtable store, member copy at +0x3C.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N004ebdf1 {
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _body[0x38];                   // +0x04 def-class base body
    CParentDefClassBase(const CParentDefClassBase& other);   // 0x0046EE98
};
struct C2DVector { float X, Y; };
struct C2DVectorVector {                         // STLport vector<C2DVector>, 12 bytes
    C2DVector* _M_start;
    C2DVector* _M_finish;
    C2DVector* _M_end_of_storage;
    C2DVectorVector(const C2DVectorVector& other);   // 0x004EBE1A
};
struct CCardPositionsDef : CParentDefClassBase { // vtable 0x012439DC
    C2DVectorVector Offset;                      // +0x3C
    virtual void v00();
};
// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CCardPositionsDef* p, const CCardPositionsDef& value)
{
    if (p != 0)
        new (p) CCardPositionsDef(value);
}
}