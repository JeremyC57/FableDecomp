// GFPSizeof(C3DMeshPartitionElement* const& element) -- memory footprint of a partition-tree pointer: the pointer itself plus, when set, the element's Sizeof (both sub-element pointers, recursively)
// Leaf: ??$GFPSizeof@QAVC3DMeshPartitionElement@@@@YIKABQAVC3DMeshPartitionElement@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC3910, 40 bytes, __fastcall (the reference in ecx), plain `ret`.
// Identity: FableWin.pdb `??$GFPSizeof@QAVC3DMeshPartitionElement@@@@YIKABQAVC3DMeshPartitionElement@@@Z` (unsigned long
//   __fastcall GFPSizeof<C3DMeshPartitionElement* const>(C3DMeshPartitionElement* const&)), with GFPIsNull<X>(X*),
//   GFSizeof<C3DMeshPartitionElement>(X&) and the const C3DMeshPartitionElement::Sizeof() in the same name sets. Its only
//   calls are its own recursion on SubElements[1] (+0x24, 0x00AC391B) and SubElements[0] (+0x20, 0x00AC3925); it sits
//   after the C3DMeshPartitionElement deleting dtor 0x00AC37B0 and the CCountedPointer helpers of that block.
// Layout: Ego_r.pdb C3DMeshPartitionElement (56 bytes): BoundingBox +0, StartTriangle +0x18, TriangleCount +0x1C,
//   SubElements[2] +0x20, BoundingSphere +0x28.
// Shape: `if (!GFPIsNull(p)) return sizeof(T) + GFSizeof(*p); return sizeof(T);` with GFSizeof -> Sizeof() inline: two
//   returns, the non-null one falling through (`lea eax,[edi+eax+4]`), the null one (4) at the end. The ternary form
//   shares one exit (43v40), a size accumulator merges the 4 into both paths (38v40).
class C3DMeshPartitionElement;

template <class T> inline bool GFPIsNull(T* p) { return p == 0; }
template <class T> inline unsigned long GFSizeof(T& object) { return object.Sizeof(); }
template <class T> unsigned long __fastcall GFPSizeof(T& pointer)
{
    if (!GFPIsNull(pointer))
        return sizeof(T) + GFSizeof(*pointer);
    return sizeof(T);
}

class C3DVector { public: float X, Y, Z; };
class C3DBoundingBox { public: C3DVector Min, Max; };
class C3DBoundingSphere { public: C3DVector Centre; float Radius; };

class C3DMeshPartitionElement {
public:
    C3DBoundingBox            BoundingBox;              // +0x00
    long                      StartTriangle;            // +0x18
    long                      TriangleCount;            // +0x1C
    C3DMeshPartitionElement*  SubElements[2];           // +0x20
    C3DBoundingSphere         BoundingSphere;           // +0x28
    unsigned long Sizeof() const { return GFPSizeof(SubElements[0]) + GFPSizeof(SubElements[1]); }
};

template unsigned long __fastcall GFPSizeof<C3DMeshPartitionElement* const>(C3DMeshPartitionElement* const& pointer);