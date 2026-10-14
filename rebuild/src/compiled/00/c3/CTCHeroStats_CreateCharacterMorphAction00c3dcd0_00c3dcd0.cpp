// CTCHeroStats_00c3dcd0::CreateCharacterMorphAction() -- EH-framed action factory: with a positive budget at +0x74 an action is taken from the pool at +0x44 (@ 0x00C3C9A0; 0 when empty); with a budget of -1 a 0x4c-byte morph action is allocated through the module allocator (@ 0x00C3C360, null-checked; base @ 0x00C4CF20, vtable 0x012B5A80, +0x48 = 0x80000000; state 0 frees it if the base throws); any other budget records error -9 once in the global at 0x0143BB48 (reported through the hook at 0x0143BBD4) and yields 0; the action is initialised (@ 0x00C40410) or discarded (@ 0x00C3D660), the pool touched for a positive budget (@ 0x00C3CA30) and the action registered (@ 0x00C3C9B0) and returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3DCD0, oracle length 260 bytes. Built under /EHsc.
// Retail bytes:  ... mov edi,ecx ; mov eax,[edi+74h] ; test eax,eax ; jle other ; lea ecx,[edi+44h] ; call CPool::Take ; mov esi,eax ; test esi,esi ; jne init ; zero: ... xor eax,eax ; ... ret ; other: cmp eax,-1 ; jne err ; push 4Ch ; call Allocate ; mov esi,eax ; add esp,4 ; mov [esp+8],esi ; test esi,esi ; mov dword ptr [esp+14h],0 ; je null ; mov ecx,esi ; call CActionBase::CActionBase ; mov dword ptr [esi],12B5A80h ; mov dword ptr [esi+48h],80000000h ; mov dword ptr [esp+14h],-1 ; jmp test ; null: xor esi,esi ; mov dword ptr [esp+14h],-1 ; jmp test ; err: mov eax,[g_error] ; test eax,eax ; jne zero ; mov eax,[g_pfnReport] ; test eax,eax ; mov dword ptr [g_error],-9 ; je zero ; push -9 ; call eax ; ... xor eax,eax ; ... ret ; init: mov ecx,esi ; call CAction::Init ; test al,al ; jne a ; push esi ; mov ecx,edi ; call Discard ; xor esi,esi ; a: mov eax,[edi+74h] ; test eax,eax ; jle b ; lea ecx,[edi+44h] ; call CPool::Touch ; b: push esi ; lea ecx,[edi+44h] ; call CPool::Register ; ... mov eax,esi ; ... ret
// The sibling factories at 0x00C3DA60 / 0x00C3DDE0 share the shape. The classes are named by address.
typedef void (__stdcall* PFNREPORT_00c3dcd0)(long error);
extern long g_lastError_0143bb48;          // 0x0143BB48
extern PFNREPORT_00c3dcd0 g_pfnReport_0143bbd4;   // 0x0143BBD4

struct CAction_00c3dcd0 {                  // the pooled action
    virtual void v00();
    unsigned char _pad[0x44];
    long m_48;
    bool Init();                           // 0x00C40410
};

struct CActionBase_00c3dcd0 {              // 0x48 bytes
    virtual void v00();
    unsigned char _pad[0x44];
    CActionBase_00c3dcd0();                // 0x00C4CF20
};

struct CMorphAction_00c3dcd0 : CActionBase_00c3dcd0 {   // vtable 0x012B5A80, 0x4c bytes
    long m_48;
    CMorphAction_00c3dcd0() : CActionBase_00c3dcd0(), m_48(0x80000000) { }
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
};

struct CPool_00c3dcd0 {                    // +0x44
    unsigned char _body[0x30];
    CAction_00c3dcd0* Take();              // 0x00C3C9A0
    void Touch();                          // 0x00C3CA30
    void Register(CAction_00c3dcd0* action);   // 0x00C3C9B0
};

struct CTCHeroStats_00c3dcd0 {
    unsigned char _pad[0x44];
    CPool_00c3dcd0 m_pool;                 // +0x44
    long m_budget;                         // +0x74
    void Discard(CAction_00c3dcd0* action);   // 0x00C3D660
    CAction_00c3dcd0* CreateCharacterMorphAction();
};

CAction_00c3dcd0* CTCHeroStats_00c3dcd0::CreateCharacterMorphAction()
{
    CAction_00c3dcd0* action = 0;
    if (m_budget > 0) {
        action = m_pool.Take();
    } else if (m_budget == -1) {
        action = (CAction_00c3dcd0*)new CMorphAction_00c3dcd0;
    } else if (g_lastError_0143bb48 == 0) {
        g_lastError_0143bb48 = -9;
        if (g_pfnReport_0143bbd4)
            g_pfnReport_0143bbd4(-9);
    }
    if (!action)
        return 0;
    if (!action->Init()) {
        Discard(action);
        action = 0;
    }
    if (m_budget > 0)
        m_pool.Touch();
    m_pool.Register(action);
    return action;
}