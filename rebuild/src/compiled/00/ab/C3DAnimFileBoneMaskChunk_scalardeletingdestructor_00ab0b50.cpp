// C3DAnimFileBoneMaskChunk::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GC3DAnimFileBoneMaskChunk@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB0B50, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c671): slot 0 of vtable 0x0129E980; Ego_r.pdb name set; calls 0x00A9A7C0.
// Callees: C3DAnimFileBoneMaskChunk::~C3DAnimFileBoneMaskChunk 0x00A9A7C0 (??1C3DAnimFileBoneMaskChunk@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall C3DAnimFileBoneMaskChunk_scalar_deleting_destructor
//   calling C3DAnimFileBoneMaskChunk_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class C3DAnimFileBoneMaskChunk {
public:
    C3DAnimFileBoneMaskChunk();   // 0x00AB0700
    virtual ~C3DAnimFileBoneMaskChunk();                // 0x00A9A7C0 (??1C3DAnimFileBoneMaskChunk@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0C3DAnimFileBoneMaskChunk@@QAE@XZ 0x00AB0700) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
C3DAnimFileBoneMaskChunk::C3DAnimFileBoneMaskChunk() { }
