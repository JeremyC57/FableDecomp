// CNavQuadTreeNode::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCNavQuadTreeNode@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7D900, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c669): slot 0 of vtable 0x0129DCD4 (rtti-vtables.tsv); Ego_r.pdb ??_GCNavQuadTreeNode@@UAEPAXI@Z; calls 0x00A75C20.
// Callees: CNavQuadTreeNode::~CNavQuadTreeNode 0x00A75C20 (??1CNavQuadTreeNode@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CNavQuadTreeNode_scalar_deleting_destructor
//   calling CNavQuadTreeNode_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class C2DVector;
class CAFile;

class CNavQuadTreeNode {
public:
    CNavQuadTreeNode(const C2DVector&, unsigned char, unsigned char, CAFile&);   // ??0CNavQuadTreeNode@@QAE@ABVC2DVector@@EEAAVCAFile@@@Z
    virtual ~CNavQuadTreeNode();                        // 0x00A75C20 (??1CNavQuadTreeNode@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CNavQuadTreeNode@@QAE@ABVC2DVector@@EEAAVCAFile@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CNavQuadTreeNode::CNavQuadTreeNode(const C2DVector&, unsigned char, unsigned char, CAFile&) { }
