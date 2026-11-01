// CEnginePrimitiveManagerRepeatedZSprites::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCEnginePrimitiveManagerRepeatedZSprites@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BAC970, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c672): slot 0 of RTTI vtable 0x012A5314; Ego_r lists this symbol.
// Callees: CEnginePrimitiveManagerRepeatedZSprites::~CEnginePrimitiveManagerRepeatedZSprites 0x00BABE60 (??1CEnginePrimitiveManagerRepeatedZSprites@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CEnginePrimitiveManagerRepeatedZSprites_scalar_deleting_destructor
//   calling CEnginePrimitiveManagerRepeatedZSprites_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

template <class T> class CArray;
class CEngineScene;

class CEnginePrimitiveManagerRepeatedZSprites {
public:
    CEnginePrimitiveManagerRepeatedZSprites(CArray<CEngineScene*>&);   // ??0CEnginePrimitiveManagerRepeatedZSprites@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z
    virtual ~CEnginePrimitiveManagerRepeatedZSprites(); // 0x00BABE60 (??1CEnginePrimitiveManagerRepeatedZSprites@@UAE@XZ)
};

// TU emitter: the PDB constructor (??0CEnginePrimitiveManagerRepeatedZSprites@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z) defined with an empty body emits the vtable and with it this
//   ??_G (the constructor body itself is its own row; not part of this graded body).
CEnginePrimitiveManagerRepeatedZSprites::CEnginePrimitiveManagerRepeatedZSprites(CArray<CEngineScene*>&) { }
