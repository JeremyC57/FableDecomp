// CSoundManagerLH::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCSoundManagerLH@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A41A50, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c670): slot 0 of vtable 0x0129D2C4 (RTTI CSoundManagerLH), calls ??1CSoundManagerLH 0x00A3EDE0.
// Callees: CSoundManagerLH::~CSoundManagerLH 0x00A3EDE0 (??1CSoundManagerLH@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CSoundManagerLH_scalar_deleting_destructor
//   calling CSoundManagerLH_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC
void* __cdecl operator new(unsigned int size);       // 0x00BFEA4A

class CSoundManagerLH {
public:
    CSoundManagerLH() { }
    virtual ~CSoundManagerLH();                         // 0x00A3EDE0 (??1CSoundManagerLH@@UAE@XZ)
};

void* MakeOne_00a41a50()                                 // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CSoundManagerLH;
}
