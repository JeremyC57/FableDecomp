// CTCHitOnCollision::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCTCHitOnCollision@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C8540, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c671): slot 0 of vtable 0x0126E7A4 (rtti-vtables.tsv); Ego_r.pdb ??_GCTCHitOnCollision@@UAEPAXI@Z; calls 0x007C8560.
// Callees: CTCHitOnCollision::~CTCHitOnCollision 0x007C8560 (??1CTCHitOnCollision@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CTCHitOnCollision_scalar_deleting_destructor
//   calling CTCHitOnCollision_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CThing;

class CTCHitOnCollision {
private:
    CTCHitOnCollision(CThing&);   // ??0CTCHitOnCollision@@AAE@AAVCThing@@@Z
public:
    virtual ~CTCHitOnCollision();                       // 0x007C8560 (??1CTCHitOnCollision@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CTCHitOnCollision@@AAE@AAVCThing@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CTCHitOnCollision::CTCHitOnCollision(CThing&) { }
