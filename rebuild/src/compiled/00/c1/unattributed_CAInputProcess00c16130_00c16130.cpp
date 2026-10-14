// CAInputProcess_00c16130::~CAInputProcess_00c16130() -- EH-framed destructor: own vtable (0x012B2A74), the owned object at +8 deleted (out-of-line destructor @ 0x00C2D3B0, then operator delete) and nulled, then the inline base destructor restores the base vtable (0x012B2A40)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C16130, oracle length 97 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; mov dword ptr [esi],offset vtable ; mov edi,[esi+8] ; test edi,edi ; mov dword ptr [esp+14h],0 ; je skip ; mov ecx,edi ; call ~CObject ; push edi ; call operator delete ; add esp,4 ; skip: mov ecx,[esp+0Ch] ; mov dword ptr [esi+8],0 ; mov dword ptr [esi],offset base vtable ; pop edi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// `delete` of an object with a non-virtual out-of-line destructor: null test, destructor, operator
// delete; the null store follows unconditionally. EH state 0 covers the body. The manifest label is
// propagated (the class differs from the one at 0x00C885C0); the class is named by address.
struct CInputObject_00c16130 {
    ~CInputObject_00c16130();              // 0x00C2D3B0
};

struct CAInputProcessBase_00c16130 {       // vtable 0x012B2A40
    virtual void v00();
    ~CAInputProcessBase_00c16130() { }
};

struct CAInputProcess_00c16130 : CAInputProcessBase_00c16130 {   // vtable 0x012B2A74
    unsigned char _pad[4];
    CInputObject_00c16130* m_pObject;      // +8
    ~CAInputProcess_00c16130();
};

CAInputProcess_00c16130::~CAInputProcess_00c16130()
{
    delete m_pObject;
    m_pObject = 0;
}