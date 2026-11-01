// CUserProfileManager::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCUserProfileManager@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00412B00, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c673): slot 0 of vtable 0x0122E23C (rtti-vtables.tsv CUserProfileManager); scalar form (test 1 -> delete); Ego_r.pdb ??_GCUserProfileManager@@UAEPAXI@Z.
// Callees: CUserProfileManager::~CUserProfileManager 0x00408990 (??1CUserProfileManager@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the CUserProfileManager::ScalarDeletingDestructor (member stand-in)
//   calling CUserProfileManager::Dtor(); re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CUserProfileManager {
public:
    CUserProfileManager();   // 0x0040CEC0
    virtual ~CUserProfileManager();                     // 0x00408990 (??1CUserProfileManager@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CUserProfileManager@@QAE@XZ 0x0040CEC0) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CUserProfileManager::CUserProfileManager() { }
