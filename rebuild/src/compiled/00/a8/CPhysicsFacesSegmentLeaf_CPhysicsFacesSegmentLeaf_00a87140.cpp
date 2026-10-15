// CPhysicsFacesSegmentLeaf::~CPhysicsFacesSegmentLeaf() -- free the triangle index array, fall back to the base vtable
// Leaf: ??1CPhysicsFacesSegmentLeaf
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A87140, length 33 bytes.
// Callee: free @ 0x00BFEA14 (the CArray<unsigned short> buffer).
// Identity: the manifest label "CSystemRegistry::~CSystemRegistry" is propagated over a byte-identical
// shape; the body stores vtable 0x0129DEA4 (CPhysicsFacesSegmentLeaf, rtti-vtables.tsv) and 0x0129DE70
// (CIPhysicsFacesSegmentBase), and its only caller is the leaf's deleting destructor 0x00A87120 (slot 3).
//
// Shape: own vtable store, the member CArray's inline destructor (`if (first) free(first)`, the PDB
// member TriangleListIndex at +0x04 -- the landed GetNoFaces 0x00A860F0 reads its first/last at +4/+8),
// then the base's empty inline destructor, which only re-stores the base vtable. The retail CArray is
// three pointers (0x0c bytes). No engine header exists for either class.
// Header note: engine/CPhysicsFacesSegmentLeaf.h is the flattened PDB layout (vptr as data, the 16-byte PDB
// CArray) and cannot carry a virtual destructor, so the class is modelled locally as
// CPhysicsFacesSegmentLeaf_Retail (retail CArray = 12 bytes).
extern "C" void __cdecl free(void* block);       // 0x00BFEA14

template<class T>
struct CArray {
    T* m_first;
    T* m_last;
    T* m_end;
    ~CArray() { if (m_first) free(m_first); }
};

class CIPhysicsFacesSegmentBase {
public:
    virtual bool HasChildren() const = 0;        // slot 0 (leaf: 0x00A87110)
    virtual long GetNoFaces() const = 0;         // slot 1
    virtual long GetNoChildren() const = 0;      // slot 2
    virtual ~CIPhysicsFacesSegmentBase() {}      // slot 3
};

class CPhysicsFacesSegmentLeaf_Retail : public CIPhysicsFacesSegmentBase {
public:
    virtual bool HasChildren() const;
    virtual long GetNoFaces() const;
    virtual long GetNoChildren() const;
    virtual ~CPhysicsFacesSegmentLeaf_Retail();
    CArray<unsigned short> TriangleListIndex;    // +0x04
};

CPhysicsFacesSegmentLeaf_Retail::~CPhysicsFacesSegmentLeaf_Retail()
{
}