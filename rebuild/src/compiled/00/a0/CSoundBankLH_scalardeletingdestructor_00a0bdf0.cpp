// CSoundBankLH::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCSoundBankLH@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0BDF0, 30 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c670): slot 0 of vtable 0x0129C9DC (RTTI CSoundBankLH), calls ??1CSoundBankLH 0x00A0B9A0.
// Callees: CSoundBankLH::~CSoundBankLH 0x00A0B9A0 (??1CSoundBankLH@@UAE@XZ), operator delete 0x00BFE9BC.
// Naming wave 16: landed as the free __fastcall CSoundBankLH_scalar_deleting_destructor
//   calling CSoundBankLH_dtor; re-authored on the proven compiler-generated ??_G (name-only: bytes unchanged).
void __cdecl operator delete(void* p);                   // 0x00BFE9BC
void* __cdecl operator new(unsigned int size);       // 0x00BFEA4A

class CSoundBankLH {
public:
    CSoundBankLH() { }
    virtual ~CSoundBankLH();                            // 0x00A0B9A0 (??1CSoundBankLH@@UAE@XZ)
};

void* MakeOne_00a0bdf0()                                 // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CSoundBankLH;
}
