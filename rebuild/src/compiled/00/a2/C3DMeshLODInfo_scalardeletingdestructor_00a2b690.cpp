// C3DMeshLODInfo::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GC3DMeshLODInfo@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2B690, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c670): slot 0 of vtable 0x0129CDA4 (RTTI C3DMeshLODInfo), calls ??1C3DMeshLODInfo 0x00A24470; Ego_r ??_GC3DMeshLODInfo@@UAEPAXI@Z.
// Callees: C3DMeshLODInfo::~C3DMeshLODInfo 0x00A24470 (??1C3DMeshLODInfo@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall C3DMeshLODInfo_scalar_deleting_destructor
//   calling C3DMeshLODInfo_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class C3DMeshInfo;

class C3DMeshLODInfo {
private:
    C3DMeshLODInfo(const C3DMeshInfo*, unsigned long);   // ??0C3DMeshLODInfo@@AAE@PBVC3DMeshInfo@@K@Z
public:
    virtual ~C3DMeshLODInfo();                          // 0x00A24470 (??1C3DMeshLODInfo@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0C3DMeshLODInfo@@AAE@PBVC3DMeshInfo@@K@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
C3DMeshLODInfo::C3DMeshLODInfo(const C3DMeshInfo*, unsigned long) { }
