// CCardPositionsDef::~CCardPositionsDef() -- EH-framed destructor: own vtable (0x012B263C), the imported member at +0xc destroyed, then the imported base destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C10940, oracle length 84 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; lea ecx,[esi+0Ch] ; mov dword ptr [esp+10h],0 ; call [__imp_CMember::~CMember] ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call [__imp_CBase::~CBase] ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The destructor of the class constructed at 0x00C108E0: both the member type and the base live in
// a DLL (`__declspec(dllimport)`, destructors called through import-address-table slots). The
// member at +0xc goes first (EH state 0 while the base is alive), then the base (state -1). The
// manifest label (a persist-context name) is propagated garbage for this body; the class is the
// one named by the vtable.
struct __declspec(dllimport) CImportedDefBase_00c10940 {
    virtual void v00();
    unsigned char _body[8];
    ~CImportedDefBase_00c10940();          // IAT slot 0x01440028
};

struct __declspec(dllimport) CImportedPositions_00c10940 {   // +0xc
    unsigned char _body[4];
    ~CImportedPositions_00c10940();        // IAT slot 0x0143FF9C
};

struct CCardPositionsDef_00c10940 : CImportedDefBase_00c10940 {   // vtable 0x012B263C
    CImportedPositions_00c10940 m_positions;   // +0xc
    ~CCardPositionsDef_00c10940();
};

CCardPositionsDef_00c10940::~CCardPositionsDef_00c10940()
{
}