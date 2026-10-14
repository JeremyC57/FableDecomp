// CTCHeroStats_00c3dde0::CreateScreenEffectAction() -- EH-framed action factory, sibling of the landed 0x00C3DCD0: with a positive budget at +0x78 an action is taken from the pool at +0x58 (@ 0x00C3C9A0; 0 when empty); with a budget of -1 a 0x80-byte screen-effect action is allocated through the module allocator (@ 0x00C3C360, null-checked; base @ 0x00C4CF20, vtable 0x012B5A84, +0x7c cleared; state 0 frees it if the base throws); any other budget records error -9 once in the global at 0x0143BB48 (reported through the hook at 0x0143BBD4) and yields 0; a non-zero initialisation result (@ 0x00C40750) is recorded and reported the same way before the action is discarded (@ 0x00C3D6A0); the pool is touched for a positive budget (@ 0x00C3CA30) and the action registered (@ 0x00C3C9B0) and returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3DDE0, oracle length 291 bytes. Built under /EHsc.
// Retail bytes:  ... mov edi,ecx ; mov eax,[edi+74h] ; test eax,eax ; jle other ; lea ecx,[edi+44h] ; call CPool::Take ; mov esi,eax ; test esi,esi ; jne init ; zero: ... xor eax,eax ; ... ret ; other: cmp eax,-1 ; jne err ; push 4Ch ; call Allocate ; mov esi,eax ; add esp,4 ; mov [esp+8],esi ; test esi,esi ; mov dword ptr [esp+14h],0 ; je null ; mov ecx,esi ; call CActionBase::CActionBase ; mov dword ptr [esi],12B5A80h ; mov dword ptr [esi+48h],80000000h ; mov dword ptr [esp+14h],-1 ; jmp test ; null: xor esi,esi ; mov dword ptr [esp+14h],-1 ; jmp test ; err: mov eax,[g_error] ; test eax,eax ; jne zero ; mov eax,[g_pfnReport] ; test eax,eax ; mov dword ptr [g_error],-9 ; je zero ; push -9 ; call eax ; ... xor eax,eax ; ... ret ; init: mov ecx,esi ; call CAction::Init ; test al,al ; jne a ; push esi ; mov ecx,edi ; call Discard ; xor esi,esi ; a: mov eax,[edi+74h] ; test eax,eax ; jle b ; lea ecx,[edi+44h] ; call CPool::Touch ; b: push esi ; lea ecx,[edi+44h] ; call CPool::Register ; ... mov eax,esi ; ... ret
// The sibling factories at 0x00C3DA60 / 0x00C3DDE0 share the shape. The classes are named by address.
typedef void (__stdcall* PFNREPORT_00c3dde0)(long error);
extern long g_lastError_0143bb48;          // 0x0143BB48
extern PFNREPORT_00c3dde0 g_pfnReport_0143bbd4;   // 0x0143BBD4

struct CAction_00c3dde0 {                  // the pooled action
    virtual void v00();
    unsigned char _pad[0x44];
    long m_48;
    long Init();                           // 0x00C40750: 0 on success, else the error
};

struct CActionBase_00c3dde0 {              // 0x48 bytes
    virtual void v00();
    unsigned char _pad[0x44];
    CActionBase_00c3dde0();                // 0x00C4CF20
};

struct CMorphAction_00c3dde0 : CActionBase_00c3dde0 {   // vtable 0x012B5A84, 0x80 bytes
    unsigned char _pad[0x34];
    long m_7c;
    CMorphAction_00c3dde0() : CActionBase_00c3dde0(), m_7c(0) { }
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
};

struct CPool_00c3dde0 {                    // +0x58
    unsigned char _body[0x20];
    CAction_00c3dde0* Take();              // 0x00C3C9A0
    void Touch();                          // 0x00C3CA30
    void Register(CAction_00c3dde0* action);   // 0x00C3C9B0
};

struct CTCHeroStats_00c3dde0 {
    unsigned char _pad[0x58];
    CPool_00c3dde0 m_pool;                 // +0x58
    long m_budget;                         // +0x78 (the pool is 0x20 bytes here)
    void Discard(CAction_00c3dde0* action);   // 0x00C3D6A0
    CAction_00c3dde0* CreateScreenEffectAction();
};

CAction_00c3dde0* CTCHeroStats_00c3dde0::CreateScreenEffectAction()
{
    CAction_00c3dde0* action = 0;
    if (m_budget > 0) {
        action = m_pool.Take();
    } else if (m_budget == -1) {
        action = (CAction_00c3dde0*)new CMorphAction_00c3dde0;
    } else if (g_lastError_0143bb48 == 0) {
        g_lastError_0143bb48 = -9;
        if (g_pfnReport_0143bbd4)
            g_pfnReport_0143bbd4(-9);
    }
    if (!action)
        return 0;
    long error = action->Init();
    if (error) {
        if (g_lastError_0143bb48 == 0) {
            g_lastError_0143bb48 = error;
            if (g_pfnReport_0143bbd4)
                g_pfnReport_0143bbd4(error);
        }
        Discard(action);
        action = 0;
    }
    if (m_budget > 0)
        m_pool.Touch();
    m_pool.Register(action);
    return action;
}
