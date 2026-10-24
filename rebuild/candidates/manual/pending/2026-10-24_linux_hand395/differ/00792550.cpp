// _STL::__uninitialized_fill_n(CTCDivineWrath::PerEffectInfo* first, unsigned int n, const CTCDivineWrath::PerEffectInfo& x, const __false_type&) -- copy-construct n copies of a per-effect record (position + intelligent effect pointer) into raw storage and return the end
// Grade: DIFFER(86v86) -- register allocation only: retail keeps the cursor in ebx and the source element in edi, ours
//   swaps them (edi = cursor, ebx = x); every instruction otherwise matches (Position as three dwords stored off the
//   esi Effect induction pointer, the intelligent pointer's copy constructor, the null test, `dec ebp ; jne`).
// Tried: implicit C3DVector copy (one block, 90v86), memberwise C3DVector copy ctor (fixes the stores), no `cur` local
//   (loop on `first`), an explicit memberwise PerEffectInfo copy ctor -- all leave the ebx/edi swap.
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00792550, 86 bytes, `ret 8` (/Gr algorithm TU: first in ecx, n in edx,
//   the element reference and the empty tag on the stack). Byte twin of 0x007979B0 (the other spell's TU).
// Instantiation: the fill path of vector<CTCDivineWrath::PerEffectInfo> (Ego_r.pdb names the 20-byte record: C3DVector
//   Position +0, CIntelligentPointer<CThing> Effect +0xc; the landed PerEffectInfo constructor stores the same vtable);
//   its one caller 0x00793432 is the vector's insert-overflow path.
// Callees: CBaseIntelligentPointer() 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90 (the intelligent pointer's copy
//   constructor; CIntelligentPointer<CThing> vtable 0x01238C6C).
// Shape: STLport `for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;` -- `test edx,edx ; jbe` entry, `dec ebp ;
//   jne` loop with the element and its Effect member strength-reduced to two pointers (ebx, esi); the placement copy keeps
//   VC's null test; the implicit copy constructor copies Position as three dwords and Effect through its user copy
//   constructor.
// Label correction: catalogued as `Vector_CopyConstruct_CBaseIntelligentPointer`; the body is STLport `__uninitialized_fill_n`.
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                           // 0x00A01B10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

class CThing;
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CTCDivineWrath {
public:
    struct PerEffectInfo {                              // 20 bytes
        C3DVector                   Position;            // +0x00
        CIntelligentPointer<CThing> Effect;              // +0x0c
    };
};

struct __false_type { __false_type() { } };
inline void* operator new(unsigned int, void* where) { return where; }

template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }

CTCDivineWrath::PerEffectInfo* __fastcall __uninitialized_fill_n(CTCDivineWrath::PerEffectInfo* first, unsigned int n,
                                                         const CTCDivineWrath::PerEffectInfo& x, const __false_type&)
{
    CTCDivineWrath::PerEffectInfo* cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
