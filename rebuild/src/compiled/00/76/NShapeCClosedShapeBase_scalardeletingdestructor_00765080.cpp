// NShape::CClosedShapeBase::`scalar deleting destructor'(unsigned int) -- an empty derived destructor: its own vtable store (0x0126A224) is dead and dropped, so the inlined CShape destructor's vtable store (0x0126A208) and the two vector frees are all that remain, then the flag-tested operator delete
// Leaf: ??_GCClosedShapeBase@NShape@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00765080, length 63 bytes (+ int3 padding).
// Callees: free @ 0x00BFEA14 twice, operator delete @ 0x00BFE9BC. Evidence: slot 0 of NShape::CClosedShapeBase's vtable 0x0126A224 (rtti-vtables.tsv). The catalog label
// "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip" is propagated.
// Shape: `??_G` with the whole destructor chain inlined: `mov [esi],0x0126A208`, each vector `test ; je ; push ; call free ;
// add esp,4` (+0x14 then +0x08), `test byte ptr [esp+8],1 ; je ; push esi ; call operator delete`, `mov eax,esi ; ret 4`.
// The vtable (and so the deleting destructor) is emitted through the inline constructor used by a `new` factory; the
// destructors are `__forceinline` (a plain inline virtual destructor leaves `call ??1 ; delete` in the `??_G`).
// Layout: the landed NShape::CShape model (0x00762440 constructor): id +0x04, point vector +0x08, edge vector +0x14.
extern "C" void __cdecl free(void* block);          // 0x00BFEA14
void __cdecl operator delete(void* block);            // 0x00BFE9BC

struct CRetailVector_pod {                            // VC6 vector of PODs: the destructor frees the block
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CRetailVector_pod() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    __forceinline ~CRetailVector_pod() { if (m_pFirst) free(m_pFirst); }
};

namespace NShape {
struct CShape {                                       // vtable 0x0126A208
    long              m_id;                           // +0x04
    CRetailVector_pod m_points;                       // +0x08
    CRetailVector_pod m_edges;                        // +0x14
    CShape(long id) : m_id(id) { }
    __forceinline virtual ~CShape() { }                             // slot 0: the scalar deleting destructor
};

struct CClosedShapeBase : CShape {                    // vtable 0x0126A224
    CClosedShapeBase(long id) : CShape(id) { }
    __forceinline virtual ~CClosedShapeBase() { }
};
}

NShape::CClosedShapeBase* NewClosedShapeBase(long id)
{
    return new NShape::CClosedShapeBase(id);
}