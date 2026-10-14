// CCardPositionsDef::CCardPositionsDef(void*) -- EH-framed constructor: imported base constructor, own vtable (0x012B263C), the imported member at +0xc built from the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C108E0, oracle length 83 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; call [__imp_CBase::CBase] ; mov eax,[argument] ; push eax ; lea ecx,[esi+0Ch] ; mov dword ptr [esp+14h],0 ; mov dword ptr [esi],offset vtable ; call [__imp_CMember::CMember] ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// Both the base and the member type live in a DLL (`__declspec(dllimport)`: the constructors are
// called through import-address-table slots). The base is constructed on `this` (EH state 0 once it
// exists), the derived vtable is stored and the member at +0xc is built from the argument. The
// class comes from the manifest label; the imported types are descriptive.
struct __declspec(dllimport) CImportedDefBase_00c108e0 {
    virtual void v00();
    unsigned char _body[8];
    CImportedDefBase_00c108e0();           // IAT slot 0x0143FFF8
    ~CImportedDefBase_00c108e0();
};

struct __declspec(dllimport) CImportedPositions_00c108e0 {   // +0xc
    unsigned char _body[4];
    CImportedPositions_00c108e0(void* source);   // IAT slot 0x0143FFA0
    ~CImportedPositions_00c108e0();
};

struct CCardPositionsDef : CImportedDefBase_00c108e0 {   // vtable 0x012B263C
    CImportedPositions_00c108e0 m_positions;   // +0xc
    CCardPositionsDef(void* source);
};

CCardPositionsDef::CCardPositionsDef(void* source)
    : m_positions(source)
{
}