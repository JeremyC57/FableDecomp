// CCardPositionsDef_00c13a00::CCardPositionsDef_00c13a00(const CCardPositionsDef_00c13a00&) -- EH-framed copy constructor: the imported base copied, own vtable (0x012B263C), the imported member at +0xc copied from the other's
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C13A00, oracle length 89 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push edi ; mov edi,[other] ; mov esi,ecx ; push edi ; mov [esp+0Ch],esi ; call [__imp_CBase::CBase(const CBase&)] ; add edi,0Ch ; push edi ; lea ecx,[esi+0Ch] ; mov dword ptr [esp+18h],0 ; mov dword ptr [esi],offset vtable ; call [__imp_CMember::CMember(const CMember&)] ; mov ecx,[esp+0Ch] ; pop edi ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// The copy-constructor counterpart of 0x00C108E0 (same vtable): base and member types live in a
// DLL (`__declspec(dllimport)`, their copy constructors called through import-address-table
// slots). `other` is kept in edi and advanced to its member. The class is named by address.
struct __declspec(dllimport) CImportedDefBase_00c13a00 {
    virtual void v00();
    unsigned char _body[8];
    CImportedDefBase_00c13a00(const CImportedDefBase_00c13a00& other);   // IAT slot 0x0143FFFC
    ~CImportedDefBase_00c13a00();
};

struct __declspec(dllimport) CImportedPositions_00c13a00 {   // +0xc
    unsigned char _body[4];
    CImportedPositions_00c13a00(const CImportedPositions_00c13a00& other);   // IAT slot 0x0143FFA0
    ~CImportedPositions_00c13a00();
};

struct CCardPositionsDef_00c13a00 : CImportedDefBase_00c13a00 {   // vtable 0x012B263C
    CImportedPositions_00c13a00 m_positions;   // +0xc
    CCardPositionsDef_00c13a00(const CCardPositionsDef_00c13a00& other);
};

CCardPositionsDef_00c13a00::CCardPositionsDef_00c13a00(const CCardPositionsDef_00c13a00& other)
    : CImportedDefBase_00c13a00(other), m_positions(other.m_positions)
{
}