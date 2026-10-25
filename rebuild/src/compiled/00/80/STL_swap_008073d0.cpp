// _STL::swap(NPlayerTargeting::CTargetPoint& a, NPlayerTargeting::CTargetPoint& b) -- exchange two player-targeting candidates (thing pointer, index, targeting position, rejection frame) through a by-value temporary
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x008073D0, 195 bytes (+ int3 padding), /Gr `__fastcall` (a in ecx -> edi, b in edx
//   -> esi), plain `ret`; 0x1c-byte frame temporary.
// Identity: the player-targeting candidate sort TU; callers are the out-of-line iter_swap 0x00808110 (a bare `jmp`) and
//   the partition step 0x0080A21E. Ego_r.pdb NPlayerTargeting::CTargetPoint (0x1c): CIntelligentPointer<CThing> PThing
//   +0 (vtable 0x01238C6C), long Index +8, C3DVector TargetingPosition +0xc, long FrameRejected +0x18.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90, dtor 0x00A01C10.
// Shape: STLport `swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }` with CTargetPoint's implicit copy constructor and
//   assignment: the pointer first (base constructor, vtable store, `SetPItem(o.GetPItem())`), then Index, the vector
//   (memberwise user copy constructor, so assignment is a block copy through `lea ebx,[esi+0Ch] ; mov eax,ebx`) and
//   FrameRejected; the temporary's pointer dies through the base destructor.
class CBaseObject;
class CThing;

#pragma pack(push, 1)
struct C3DVector {                                      // 1-aligned in retail
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
#pragma pack(pop)

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* PItem;                                 // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    virtual void v01();
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

namespace NPlayerTargeting {
class CTargetPoint {                                    // Ego_r.pdb, 0x1c bytes
public:
    CIntelligentPointer<CThing> PThing;                 // +0x00
    long                        Index;                  // +0x08
    C3DVector                   TargetingPosition;      // +0x0c
    long                        FrameRejected;          // +0x18
};
}

namespace _STL {
template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template void __fastcall swap<NPlayerTargeting::CTargetPoint>(NPlayerTargeting::CTargetPoint&, NPlayerTargeting::CTargetPoint&);
}