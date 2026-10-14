// CAInputProcess_00c48290::~CAInputProcess_00c48290() -- EH-framed destructor: the owned object at +0x38 deleted (out-of-line destructor @ 0x00C354A0, operator delete) and nulled, then the out-of-line base destructor (@ 0x00C48220)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C48290, oracle length 100 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; mov edi,[esi+38h] ; test edi,edi ; mov dword ptr [esp+14h],0 ; je skip ; mov ecx,edi ; call ~CObject ; push edi ; call operator delete ; add esp,4 ; skip: mov ecx,esi ; mov dword ptr [esi+38h],0 ; mov dword ptr [esp+14h],-1 ; call ~CBase ; mov ecx,[esp+0Ch] ; pop edi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// No vtable store, so the class is `novtable` (or the base is not polymorphic). EH state 0 covers
// the body. The manifest label is propagated; the class is named by address.
struct CInputObject_00c48290 {
    ~CInputObject_00c48290();              // 0x00C354A0
};

struct CInputBase_00c48290 {
    unsigned char _body[0x38];
    ~CInputBase_00c48290();                // 0x00C48220
};

struct CAInputProcess_00c48290 : CInputBase_00c48290 {
    CInputObject_00c48290* m_pObject;      // +0x38
    ~CAInputProcess_00c48290();
};

CAInputProcess_00c48290::~CAInputProcess_00c48290()
{
    delete m_pObject;
    m_pObject = 0;
}