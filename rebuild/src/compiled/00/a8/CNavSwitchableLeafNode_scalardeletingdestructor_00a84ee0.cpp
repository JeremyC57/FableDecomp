// CNavSwitchableLeafNode::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCNavSwitchableLeafNode@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A84EE0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c671): slot 0 of vtable 0x0129DE04; Ego_r.pdb name set; calls 0x00A84F00.
// Callees: CNavSwitchableLeafNode::~CNavSwitchableLeafNode 0x00A84F00 (??1CNavSwitchableLeafNode@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CNavSwitchableLeafNode_scalar_deleting_destructor
//   calling CNavSwitchableLeafNode_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

namespace std { template <class _Tp> class allocator; template <class _Tp, class _Alloc> class vector; }
class C2DVector;

class CNavSwitchableLeafNode {
public:
    CNavSwitchableLeafNode(const C2DVector&, unsigned char, unsigned char, unsigned char, const std::vector<unsigned __int64, std::allocator<unsigned __int64> >&);   // ??0CNavSwitchableLeafNode@@QAE@ABVC2DVector@@EEEABV?$vector@_KV?$allocator@_K@std@@@std@@@Z
    virtual ~CNavSwitchableLeafNode();                  // 0x00A84F00 (??1CNavSwitchableLeafNode@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CNavSwitchableLeafNode@@QAE@ABVC2DVector@@EEEABV?$vector@_KV?$allocator@_K@std@@@std@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CNavSwitchableLeafNode::CNavSwitchableLeafNode(const C2DVector&, unsigned char, unsigned char, unsigned char, const std::vector<unsigned __int64, std::allocator<unsigned __int64> >&) { }
