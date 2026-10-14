// CScriptVar_00c37050::UpdateFloatVariable() -- EH-framed: fetches the current value (@ 0x00C37000, kept in a frame slot) and, when it differs from the cached value at +8, deletes the bound variable at +0xc (destructor @ 0x00C463A0 then the allocator), caches the value, and for a non-zero value builds a 0x3c-byte binding state on the frame (@ 0x00C467D0 from the value and the owner at +4; destructor @ 0x00C46860, state 0) and allocates a fresh 12-byte variable from it (null-checked `new`, state 1 frees it if the constructor @ 0x00C46400 throws; the pointer reuses the value's slot)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C37050, oracle length 215 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,40h ; push esi ; mov esi,ecx ; call GetValue ; fstp dword ptr [esp+4] ; fld dword ptr [esi+8] ; fld dword ptr [esp+4] ; fucompp ; fnstsw ax ; test ah,44h ; jnp done ; push edi ; mov edi,[esi+0Ch] ; test edi,edi ; je a ; mov ecx,edi ; call CVar::~CVar ; push edi ; call operator delete ; add esp,4 ; a: fld dword ptr [__real@0] ; mov eax,[esp+8] ; fld dword ptr [esp+8] ; mov [esi+8],eax ; fucompp ; mov dword ptr [esi+0Ch],0 ; pop edi ; fnstsw ax ; test ah,44h ; jnp done ; mov ecx,[esi+4] ; mov edx,[esp+4] ; push ecx ; push edx ; lea ecx,[esp+10h] ; call CVarState::CVarState ; push 0Ch ; mov dword ptr [esp+50h],0 ; call operator new ; add esp,4 ; mov [esp+4],eax ; test eax,eax ; mov byte ptr [esp+4Ch],1 ; je null ; lea ecx,[esp+8] ; push ecx ; mov ecx,eax ; call CVar::CVar ; jmp store ; null: xor eax,eax ; store: lea ecx,[esp+8] ; mov [esi+0Ch],eax ; mov dword ptr [esp+4Ch],-1 ; call CVarState::~CVarState ; done: ... ret
// The manifest label is propagated; the classes are named by address.
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* block);

struct CVarState_00c37050 {                // 0x3c-byte binding state
    unsigned char _body[0x3c];
    CVarState_00c37050(float value, void* owner);   // 0x00C467D0
    ~CVarState_00c37050();                 // 0x00C46860
};

struct CVar_00c37050 {                     // 12-byte bound variable
    unsigned char _body[0xc];
    CVar_00c37050(const CVarState_00c37050* state);   // 0x00C46400
    ~CVar_00c37050();                      // 0x00C463A0
};

struct CScriptVar_00c37050 {
    void* m_0;
    void* m_pOwner;                        // +4
    float m_value;                         // +8
    CVar_00c37050* m_pVar;                 // +0xc
    float GetValue();                      // 0x00C37000
    void UpdateFloatVariable();
};

void CScriptVar_00c37050::UpdateFloatVariable()
{
    float value = GetValue();
    if (value != m_value) {
        delete m_pVar;
        m_value = value;
        m_pVar = 0;
        if (value != 0.0f) {
            CVarState_00c37050 state(value, m_pOwner);
            m_pVar = new CVar_00c37050(&state);
        }
    }
}