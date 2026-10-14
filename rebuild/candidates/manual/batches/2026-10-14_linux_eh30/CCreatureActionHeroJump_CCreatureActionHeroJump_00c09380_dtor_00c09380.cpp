// CCreatureActionHeroJump_00c09380::~CCreatureActionHeroJump() -- EH-framed destructor (the manifest's "Cleanup" is propagated): vtable 0x012B1E24; stops its work (@ 0x00C08A80 and 0x00C08AE0), deletes the +0x6c helper (destructor @ 0x00C16580), detaches (@ 0x00C08A00), sleeps 200 ms, releases (@ 0x00C088D0), deletes the +0x60 object through its slot-0 virtual destructor, frees the raw +0x68 block, the +0x38 mesh (the landed 0x00C13470) and the +0x34 part (@ 0x00C0A9A0), deletes the +0x5c object through its slot-1 virtual destructor and clears the module byte at 0x0143BAF8; the compiler then destroys the +0x3c member (the landed 0x00C10320; state 1), the +4 container (vtable 0x012B1C98, its node @ 0x00C19250 at +0xc, root vtable 0x012B1AB0; state 3 for the root unwind @ 0x00C09DB0) and the vptr-only base (vtable 0x012B1A64, out-of-line unwind @ 0x00C09770); states 0-2 unwind the base, the container and the member
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C09380, oracle length 287 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,8 ; push ebx ; push esi ; mov esi,ecx ; push edi ; mov [esp+0Ch],esi ; mov dword ptr [esi],12B1E24h ; mov dword ptr [esp+1Ch],2 ; call Stop ; mov ecx,esi ; call Finish ; mov edi,[esi+6Ch] ; xor ebx,ebx ; cmp edi,ebx ; je a ; mov ecx,edi ; call CHelper::~CHelper ; push edi ; call operator delete ; add esp,4 ; mov [esi+6Ch],ebx ; a: mov ecx,esi ; call Detach ; push 0C8h ; call [__imp_Sleep] ; mov ecx,esi ; call Release ; mov ecx,[esi+60h] ; cmp ecx,ebx ; je b ; mov eax,[ecx] ; push 1 ; call [eax] ; mov [esi+60h],ebx ; b: mov eax,[esi+68h] ; push eax ; call operator delete ; mov edi,[esi+38h] ; add esp,4 ; cmp edi,ebx ; mov [esi+68h],ebx ; je c ; mov ecx,edi ; call C3DMesh2::~C3DMesh2 ; push edi ; call operator delete ; add esp,4 ; c: mov edi,[esi+34h] ; cmp edi,ebx ; mov [esi+38h],ebx ; je d ; mov ecx,edi ; call CPart::~CPart ; push edi ; call operator delete ; add esp,4 ; d: mov ecx,[esi+5Ch] ; cmp ecx,ebx ; mov [esi+34h],ebx ; je e ; mov edx,[ecx] ; push 1 ; call [edx+4] ; e: mov [esi+5Ch],ebx ; lea ecx,[esi+3Ch] ; mov [g_byte],bl ; mov byte ptr [esp+1Ch],1 ; call CPart3c::~CPart3c ; lea edi,[esi+4] ; mov [esp+10h],edi ; mov dword ptr [edi],12B1C98h ; lea ecx,[edi+8] ; mov byte ptr [esp+1Ch],3 ; call CNode::~CNode ; mov ecx,[esp+14h] ; mov dword ptr [edi],12B1AB0h ; pop edi ; mov dword ptr [esi],12B1A64h ; ... ret
// The classes are named by address.
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);
void __cdecl operator delete(void* block);
extern unsigned char g_bJumping_0143baf8;  // 0x0143BAF8

struct CNode_00c09380 {                    // 0x28 bytes
    unsigned char _body[0x28];
    ~CNode_00c09380();                     // 0x00C19250
};
struct CRoot_00c09380 {                    // vtable 0x012B1AB0
    virtual void v00();
    CNode_00c09380* m_pNode;               // +4
    ~CRoot_00c09380() { }
};
struct CContainer_00c09380 : CRoot_00c09380 {   // vtable 0x012B1C98, +4
    CNode_00c09380 m_node;
    ~CContainer_00c09380() { }
};

struct CPart_00c09380 { unsigned char _body[4]; ~CPart_00c09380(); };       // 0x00C0A9A0
struct C3DMesh2_00c09380 { unsigned char _body[4]; ~C3DMesh2_00c09380(); };   // 0x00C13470
struct CPart3c_00c09380 { unsigned char _body[0x20]; ~CPart3c_00c09380(); };   // +0x3c, 0x00C10320
struct CHelper_00c09380 { unsigned char _body[4]; ~CHelper_00c09380(); };   // 0x00C16580
struct IObject60_00c09380 { virtual ~IObject60_00c09380(); };   // slot 0 destroys
struct IObject5c_00c09380 { virtual void v00(); virtual ~IObject5c_00c09380(); };   // slot 1 destroys

struct CActionBase_00c09380 {              // vtable 0x012B1A64: a vptr-only base
    virtual ~CActionBase_00c09380() { }
};

struct CCreatureActionHeroJump_00c09380 : CActionBase_00c09380 {
    CContainer_00c09380 m_container;       // +4
    CPart_00c09380* m_pPart;               // +0x34
    C3DMesh2_00c09380* m_pMesh;            // +0x38
    CPart3c_00c09380 m_3c;                 // +0x3c
    IObject5c_00c09380* m_p5c;             // +0x5c
    IObject60_00c09380* m_p60;             // +0x60
    long m_64;
    char* m_pBlock;                        // +0x68
    CHelper_00c09380* m_pHelper;           // +0x6c
    void Stop();                           // 0x00C08A80
    void Finish();                         // 0x00C08AE0
    void Detach();                         // 0x00C08A00
    void Release();                        // 0x00C088D0
    virtual ~CCreatureActionHeroJump_00c09380();
};

CCreatureActionHeroJump_00c09380::~CCreatureActionHeroJump_00c09380()
{
    Stop();
    Finish();
    if (m_pHelper) {
        delete m_pHelper;
        m_pHelper = 0;
    }
    Detach();
    Sleep(200);
    Release();
    if (m_p60) {
        delete m_p60;
        m_p60 = 0;
    }
    delete m_pBlock;
    m_pBlock = 0;
    if (m_pMesh)
        delete m_pMesh;
    m_pMesh = 0;
    if (m_pPart)
        delete m_pPart;
    m_pPart = 0;
    if (m_p5c)
        delete m_p5c;
    m_p5c = 0;
    g_bJumping_0143baf8 = 0;
}
