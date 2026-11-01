// CNavNavigableLeafNode::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCNavNavigableLeafNode@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A80340, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c671): slot 0 of vtable 0x0129DDA4; Ego_r.pdb name set.
// Callees: CNavNavigableLeafNode::~CNavNavigableLeafNode 0x00A76A80 (??1CNavNavigableLeafNode@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CNavNavigableLeafNode_scalar_deleting_destructor
//   calling CNavNavigableLeafNode_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class C2DVector;

class CNavNavigableLeafNode {
public:
    CNavNavigableLeafNode(const C2DVector&, unsigned char, unsigned char, unsigned char);   // ??0CNavNavigableLeafNode@@QAE@ABVC2DVector@@EEE@Z
    virtual ~CNavNavigableLeafNode();                   // 0x00A76A80 (??1CNavNavigableLeafNode@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CNavNavigableLeafNode@@QAE@ABVC2DVector@@EEE@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CNavNavigableLeafNode::CNavNavigableLeafNode(const C2DVector&, unsigned char, unsigned char, unsigned char) { }
