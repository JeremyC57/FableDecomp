// C3DMesh2_00c13470::~C3DMesh2() -- EH-framed destructor (vtable 0x012B2748; state 1 at entry): the module's profiling lock (0x0143BB14; @ 0x00C0FFC0 / 0x00C0FFE0, state 2) is held around the shutdown call (@ 0x00C123B0), then the four owned parts at +0x7c4 / +0x7bc / +0x7b8 / +0x7b4 are deleted through non-virtual destructors (@ 0x00C25C20, @ 0x00C1C230, and two whose implicit destructors only destroy the member at +0xc @ 0x00C14630 / 0x00C14560) and cleared, and the members at +0x7d8 (@ 0x00C25E90, state 0) and +0x2c (@ 0x00C22520) destroyed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C13470, oracle length 268 bytes. Built under /EHsc.
// Retail bytes:  ... mov [esp+0Ch],esi ; mov dword ptr [esi],12B2748h ; mov ebx,1 ; push offset g_section ; lea ecx,[esp+14h] ; mov [esp+24h],ebx ; call CProfileLock::CProfileLock ; mov ecx,esi ; mov byte ptr [esp+20h],2 ; call Shutdown ; lea ecx,[esp+10h] ; mov byte ptr [esp+20h],bl ; call ~CProfileLock ; mov edi,[esi+7C4h] ; xor ebx,ebx ; cmp edi,ebx ; je a ; mov ecx,edi ; call ~CPartA ; push edi ; call operator delete ; add esp,4 ; mov [esi+7C4h],ebx ; a: ... (+7BCh with ~CPartB) ... mov edi,[esi+7B8h] ; cmp edi,ebx ; je c ; lea ecx,[edi+0Ch] ; call ~CPartC::member ; push edi ; call operator delete ; add esp,4 ; mov [esi+7B8h],ebx ; c: ... (+7B4h) ... lea ecx,[esi+7D8h] ; mov byte ptr [esp+20h],bl ; call ~CMember7D8 ; lea ecx,[esi+2Ch] ; mov dword ptr [esp+20h],-1 ; call ~CMember2C ; ... ret
// The classes are named by address.
void __cdecl operator delete(void* block);
extern unsigned char g_section_0143bb14[];  // 0x0143BB14: the module's critical section

struct CProfileLock_00c13470 {             // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    CProfileLock_00c13470(void* section);  // 0x00C0FFC0
    ~CProfileLock_00c13470();              // 0x00C0FFE0
};

struct CPartA_00c13470 { unsigned char _body[4]; ~CPartA_00c13470(); };   // 0x00C25C20
struct CPartB_00c13470 { unsigned char _body[4]; ~CPartB_00c13470(); };   // 0x00C1C230
struct CInnerC_00c13470 { unsigned char _body[4]; ~CInnerC_00c13470(); };   // 0x00C14630
struct CInnerD_00c13470 { unsigned char _body[4]; ~CInnerD_00c13470(); };   // 0x00C14560
struct CPartC_00c13470 { unsigned char _pad[0xc]; CInnerC_00c13470 m_c; };   // implicit destructor: only the +0xc member
struct CPartD_00c13470 { unsigned char _pad[0xc]; CInnerD_00c13470 m_c; };

struct CMember2C_00c13470 { unsigned char _body[0x788]; ~CMember2C_00c13470(); };   // 0x00C22520
struct CMember7D8_00c13470 { unsigned char _body[4]; ~CMember7D8_00c13470(); };    // 0x00C25E90

struct C3DMesh2_00c13470 {
    virtual ~C3DMesh2_00c13470();
    unsigned char _pad[0x28];              // +4
    CMember2C_00c13470 m_2c;               // +0x2c
    CPartD_00c13470* m_pD;                 // +0x7b4
    CPartC_00c13470* m_pC;                 // +0x7b8
    CPartB_00c13470* m_pB;                 // +0x7bc
    long m_7c0;
    CPartA_00c13470* m_pA;                 // +0x7c4
    unsigned char _pad2[0x10];             // +0x7c8
    CMember7D8_00c13470 m_7d8;             // +0x7d8
    void Shutdown();                       // 0x00C123B0
};

C3DMesh2_00c13470::~C3DMesh2_00c13470()
{
    {
        CProfileLock_00c13470 lock(g_section_0143bb14);
        Shutdown();
    }
    if (m_pA) {                            // each clear sits inside its own test (retail places the store before the join)
        delete m_pA;
        m_pA = 0;
    }
    if (m_pB) {
        delete m_pB;
        m_pB = 0;
    }
    if (m_pC) {
        delete m_pC;
        m_pC = 0;
    }
    if (m_pD) {
        delete m_pD;
        m_pD = 0;
    }
}
