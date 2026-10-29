// N3DPrimitive::CDeviceIndependent::~CDeviceIndependent() -- free the two tangent arrays, destroy and free the group and triangle-list pointer arrays, free the vertex array, then the CBase destructor
// Leaf: ??1CDeviceIndependent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A98470, length 126 bytes.
// Label correction: catalogued as CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip (propagated); the only caller is
// the scalar deleting destructor @ 0x00A98450, slot 0 of N3DPrimitive::CDeviceIndependent's vtable 0x0129DFA8.
// Callees: free @ 0x00BFEA14 (x5), the CCountedPointer<C3DGroup2> range destroy @ 0x00A969D0 and the
// CIVCountedPointer<CTriangleList> range destroy @ 0x00A96990 (both `__fastcall` first/last with the iterator tag by
// reference homed in the `push ecx` frame word, `lea [esp+7]`), CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300.
// Layout: Ego_r.pdb N3DPrimitive::CDeviceIndependent (PDB 100 bytes with 16-byte CArrays; retail CArray is 12 bytes):
// Vertices +0x04, TriangleLists +0x10, Groups +0x1c, BoundingSphere +0x28 (16 bytes), UTangents +0x38, VTangents +0x44.
// Shape: members destroyed in reverse order through the CArray destructor (POD element arrays only free their block),
// no vtable store of this class (`__declspec(novtable)`), then the base destructor as a plain call; `ret`.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CBaseClassNonCopyable {
protected:                                      // vtable 0x01231710
    virtual ~CBaseClassNonCopyable();                                      // 0x0099A300
};

struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };

class C3DVertex2;
class C3DVector { public: float x, y, z; };
class C3DBoundingSphere { public: C3DVector Centre; float Radius; };
namespace N3DPrimitive { class CTriangleList; }
class C3DGroup2;
template <class T> class CIVCountedPointer { public: T* Object; };
template <class T> class CCountedPointer { public: T* Data; void* Count; };

void __fastcall DestroyRange(CIVCountedPointer<N3DPrimitive::CTriangleList>* first,
                             CIVCountedPointer<N3DPrimitive::CTriangleList>* last, const SNonscalarPtrIteratorTag& tag);   // 0x00A96990
void __fastcall DestroyRange(CCountedPointer<C3DGroup2>* first, CCountedPointer<C3DGroup2>* last,
                             const SNonscalarPtrIteratorTag& tag);   // 0x00A969D0

template <class T>
struct CPodArray {                                 // retail CArray<T> of a trivially destructible T: first / last / end
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    ~CPodArray() { if (m_pFirst) free(m_pFirst); }
};
template <class T>
class CArray { public:                                    // retail CArray<T>: first / last / end
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    ~CArray()
    {
        DestroyRange(m_pFirst, m_pLast, SNonscalarPtrIteratorTag());
        if (m_pFirst)
            free(m_pFirst);
    }
};

namespace N3DPrimitive {
struct __declspec(novtable) CDeviceIndependent : CBaseClassNonCopyable {   // vtable 0x0129DFA8, retail 0x50 bytes
    CPodArray<C3DVertex2>                          Vertices;        // +0x04
    CArray<CIVCountedPointer<CTriangleList> >      TriangleLists;   // +0x10
    CArray<CCountedPointer<C3DGroup2> >            Groups;          // +0x1c
    C3DBoundingSphere                              BoundingSphere;  // +0x28
    CPodArray<C3DVector>                           UTangents;       // +0x38
    CPodArray<C3DVector>                           VTangents;       // +0x44
    ~CDeviceIndependent();
};

CDeviceIndependent::~CDeviceIndependent()
{
}
}