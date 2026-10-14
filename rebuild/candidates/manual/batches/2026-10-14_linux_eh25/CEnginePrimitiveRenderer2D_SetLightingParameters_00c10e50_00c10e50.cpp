// CEnginePrimitiveRenderer2D::SetLightingParameters(const C3DVector&, const C3DVector&, const C3DVector&) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, unless the three vectors at +8/+0x14/+0x20 already equal the arguments (`__fastcall` compare @ 0x00C256F0, short-circuited) they are assigned (POD dword copies through a fresh address register each) and the owner at +0x7bc updated with the three members (@ 0x00C1C280); the flags at +0x7c8 gain bit 2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C10E50, oracle length 230 bytes. Built under /EHsc.
// Retail bytes:  ... mov edi,[esp+28h] ; lea ebx,[esi+8] ; mov edx,edi ; mov ecx,ebx ; mov dword ptr [esp+20h],0 ; call C3DVector::Equals ; test al,al ; mov ebp,[esp+30h] ; je assign ; mov edx,[esp+2Ch] ; lea ecx,[esi+14h] ; call Equals ; test al,al ; je assign ; lea ecx,[esi+20h] ; mov edx,ebp ; call Equals ; test al,al ; jne done ; assign: mov ecx,[edi] ; mov eax,ebx ; mov [eax],ecx ; ... (three inline operator= copies, their results pushed) ; mov ecx,[esi+7BCh] ; ... call COwner::Update ; or dword ptr [esi+7C8h],4 ; done: lea ecx,[esp+10h] ; mov dword ptr [esp+20h],-1 ; call ~CScopedLock ; ...
// The copies keep their address registers for the update's pushes (a user operator= folds
// the addresses away). Named from the manifest (the caller at 0x00C10FE0 landed).
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct C3DVector_00c10e50 {
    float x, y, z;
    bool __fastcall Equals(const C3DVector_00c10e50& other) const;   // 0x00C256F0
};

struct COwner_00c10e50 {
    void Update(const C3DVector_00c10e50& a, const C3DVector_00c10e50& b, const C3DVector_00c10e50& c);   // 0x00C1C280
};

struct CEnginePrimitiveRenderer2D_00c10e50 {
    unsigned char _pad[8];
    C3DVector_00c10e50 m_a;                    // +8
    C3DVector_00c10e50 m_b;                    // +0x14
    C3DVector_00c10e50 m_c;                    // +0x20
    unsigned char _pad2[0x790];
    COwner_00c10e50* m_pOwner;                 // +0x7bc
    unsigned char _pad3[8];
    unsigned long m_flags;                 // +0x7c8
    void SetLightingParameters(const C3DVector_00c10e50& a, const C3DVector_00c10e50& b, const C3DVector_00c10e50& c);
};

void CEnginePrimitiveRenderer2D_00c10e50::SetLightingParameters(const C3DVector_00c10e50& a, const C3DVector_00c10e50& b, const C3DVector_00c10e50& c)
{
    CScopedLock lock(&g_lock_0143bb14);
    if (!(m_a.Equals(a) && m_b.Equals(b) && m_c.Equals(c))) {
        m_a = a;
        m_b = b;
        m_c = c;
        m_pOwner->Update(m_a, m_b, m_c);
        m_flags |= 4;
    }
}
