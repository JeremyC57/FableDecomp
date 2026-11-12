// NShape::CBasicShape::~CBasicShape() -- this class's vtable, the subdivision vector at +0x20 destroyed (its counted pointers through the range destroy, then the block freed), then the inlined CShape destructor (its vtable, the edge and point vectors freed)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00762480, length 83 bytes.
// Callees: the counted-pointer range destroy @ 0x00764EE0 (`__fastcall`: first, last, the allocator tag by reference, its temporary homed in a dead frame byte `lea eax,[esp+7]`), free @ 0x00BFEA14
// three times. Vtables 0x0126A31C = NShape::CBasicShape, 0x0126A208 = NShape::CShape (the landed CBasicShape constructor @ 0x00762440 has the layout). Shape: a
// one-word frame (`push ecx`) for the tag, the base's vtable store between the member destructors.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct SScalarTag { SScalarTag() { } };

struct CRetailVector_pod { void* m_pFirst; void* m_pLast; void* m_pEnd; ~CRetailVector_pod() { if (m_pFirst) free(m_pFirst); } };

namespace NShape { class CSubdivisionTriangle; }
#include "engine/CCountedPointer.h"
namespace std {
struct __false_type { __false_type() { } };
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00764EE0 (std::__destroy_aux<CCountedPointer<NShape::CSubdivisionTriangle>*>)
}

struct CRetailVector_counted {                     // VC6 vector of counted pointers: destroy the range, free the block
    CCountedPointer<NShape::CSubdivisionTriangle>* m_pFirst;
    CCountedPointer<NShape::CSubdivisionTriangle>* m_pLast;
    CCountedPointer<NShape::CSubdivisionTriangle>* m_pEnd;
    ~CRetailVector_counted()
    {
        std::__destroy_aux(m_pFirst, m_pLast, std::__false_type());
        if (m_pFirst)
            free(m_pFirst);
    }
};

namespace NShape {
class CShape { public:                                    // vtable 0x0126A208
    virtual void v00();
    long             m_id;                         // +0x04
    CRetailVector_pod m_points;                    // +0x08
    CRetailVector_pod m_edges;                     // +0x14
    ~CShape() { }
};

struct CBasicShape : CShape {                      // vtable 0x0126A31C
    CRetailVector_counted Subdivisions;            // +0x20
    virtual void v00();
    ~CBasicShape();
};
}

NShape::CBasicShape::~CBasicShape()
{
}