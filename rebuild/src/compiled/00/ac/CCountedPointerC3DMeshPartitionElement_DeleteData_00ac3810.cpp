// CCountedPointer<C3DMeshPartitionElement>::DeleteData(void* data) -- the counted pointer's per-type deleter: delete the partition element (its destructor deletes both sub-elements and clears the triangle range)
// Leaf: ?DeleteData@?$CCountedPointer@VC3DMeshPartitionElement@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC3810, length 60 bytes (+4 int3), static __fastcall (data in ecx),
//   plain `ret`.
// Callees: C3DMeshPartitionElement::`scalar deleting destructor' @ 0x00AC37B0 (landed; the two child deletes),
//   operator delete @ 0x00BFE9BC.
// Identity: Ego_r.pdb `?DeleteData@?$CCountedPointer@VC3DMeshPartitionElement@@@@SIXPAX@Z`; the only reference is the
//   landed CCountedPointer Reset 0x00AC38A0, which stores 0x00AC3810 as the count block's destroy function. The
//   manifest's "DeleteData" leaf is right.
// Layout: C3DMeshPartitionElement as the landed 0x00AC37B0 (56 bytes, no vtable): StartTriangle +0x18,
//   TriangleCount +0x1C, SubElements[2] +0x20.
// Shape: `delete (T*)data` -- null test, the inline destructor (each child `delete` a null test + `push 1 ; call ??_G`,
//   then the two zero stores) and operator delete; the deleting destructor itself is inlined here.
void operator delete(void* p);                     // 0x00BFE9BC

class C3DVector { public: float X, Y, Z; };
class C3DBoundingBox { public: C3DVector Min, Max; };
class C3DBoundingSphere { public: C3DVector Center; float Radius; };

class C3DMeshPartitionElement {
public:
    C3DBoundingBox           BoundingBox;          // +0x00
    long                     StartTriangle;        // +0x18
    long                     TriangleCount;        // +0x1C
    C3DMeshPartitionElement* SubElements[2];       // +0x20
    C3DBoundingSphere        BoundingSphere;       // +0x28
    ~C3DMeshPartitionElement()
    {
        delete SubElements[0];
        delete SubElements[1];
        StartTriangle = 0;
        TriangleCount = 0;
    }
};

template <class T> class CCountedPointer {
public:
    T*    Data;                                    // +0x00
    void* Info;                                    // +0x04 (CCPPointerInfo)
    static void __fastcall DeleteData(void* data)
    {
        delete (T*)data;
    }
};

template void __fastcall CCountedPointer<C3DMeshPartitionElement>::DeleteData(void* data);