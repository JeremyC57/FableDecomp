// CSoundEventManager::~CSoundEventManager() -- EH-framed destructor (vtable 0x012B2748; state 1 at entry): the module's profiling lock (0x0143BB14; @ 0x00C0FFC0 / 0x00C0FFE0, state 2) is held around Shutdown (@ 0x00C123B0), then the four owned parts at +0x7c4 / +0x7bc / +0x7b8 / +0x7b4 are deleted through non-virtual destructors (@ 0x00C25C20, @ 0x00C1C230, and two whose implicit destructors only destroy their PKCWorkerQueue member at +0xc @ 0x00C14630 / 0x00C14560) and cleared, and the members at +0x7d8 (@ 0x00C25E90, state 0) and the event set at +0x2c (@ 0x00C22520) destroyed
// Leaf: ??1CSoundEventManager@@UAE@XZ
// Flags: /EHsc
// Naming (sweep1, 2026-10-25): libpk class set CSoundEventManager (vtable 0x012B2748) / CSoundEventSet (its +0x2C
//   member) / CSoundEvent (0xB4 bytes, vtable 0x012B3674): the describer 0x00C21040 called by TraceEvents/
//   TracePlaybacks prints "EventId(", "Priority=", "State='Waiting for data'" / "Playing using physical player";
//   chunk 431/435's CPanel/CPanelItem names came from propagated UI labels (renamed from 00/c1/C3DMesh2_C3DMesh200c13470dtor_00c13470.cpp).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C13470, oracle length 268 bytes. Built under /EHsc.
// Retail bytes:  ... mov [esp+0Ch],esi ; mov dword ptr [esi],12B2748h ; mov ebx,1 ; push offset g_section ; lea ecx,[esp+14h] ; mov [esp+24h],ebx ; call CProfileLock::CProfileLock ; mov ecx,esi ; mov byte ptr [esp+20h],2 ; call Shutdown ; lea ecx,[esp+10h] ; mov byte ptr [esp+20h],bl ; call ~CProfileLock ; mov edi,[esi+7C4h] ; xor ebx,ebx ; cmp edi,ebx ; je a ; mov ecx,edi ; call ~CPartA ; push edi ; call operator delete ; add esp,4 ; mov [esi+7C4h],ebx ; a: ... (+7BCh with ~CPartB) ... mov edi,[esi+7B8h] ; cmp edi,ebx ; je c ; lea ecx,[edi+0Ch] ; call ~CPartC::member ; push edi ; call operator delete ; add esp,4 ; mov [esi+7B8h],ebx ; c: ... (+7B4h) ... lea ecx,[esi+7D8h] ; mov byte ptr [esp+20h],bl ; call ~CMember7D8 ; lea ecx,[esi+2Ch] ; mov dword ptr [esp+20h],-1 ; call ~CMember2C ; ... ret
// Identity: renamed (sweep1, 2026-10-25) from C3DMesh2_00c13470::~C3DMesh2 (manifest C3DMesh2::~C3DMesh2 propagated): the
//   vptr store is the CSoundEventManager vtable 0x012B2748 and the lock-guarded call is its Shutdown 0x00C123B0 (agent433:
//   'destructor 0x00C13470 landed as "C3DMesh2", a propagated label'); the worker queues are the landed PKCWorkerQueue
//   destructors 0x00C14630 / 0x00C14560. The other parts keep their address names.
class PKCCriticalSection;
void __cdecl operator delete(void* block);
extern PKCCriticalSection g_lock_0143bb14;  // 0x0143BB14: the module's critical section

class PKCProtectionInstance { public:             // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    PKCProtectionInstance(PKCCriticalSection* section);  // 0x00C0FFC0
    ~PKCProtectionInstance();              // 0x00C0FFE0
};

struct CPartA_00c13470 { unsigned char _body[4]; ~CPartA_00c13470(); };   // 0x00C25C20
struct CPartB_00c13470 { unsigned char _body[4]; ~CPartB_00c13470(); };   // 0x00C1C230
struct PKCWorkerQueue_00c14630 { unsigned char _body[4]; ~PKCWorkerQueue_00c14630(); };   // 0x00C14630
struct PKCWorkerQueue_00c14560 { unsigned char _body[4]; ~PKCWorkerQueue_00c14560(); };   // 0x00C14560
struct CPartC_00c13470 { unsigned char _pad[0xc]; PKCWorkerQueue_00c14630 m_c; };   // implicit destructor: only the +0xc member
struct CPartD_00c13470 { unsigned char _pad[0xc]; PKCWorkerQueue_00c14560 m_c; };

struct CSoundEventSet { unsigned char _body[0x788]; ~CSoundEventSet(); };   // 0x00C22520
struct CMember7D8_00c13470 { unsigned char _body[4]; ~CMember7D8_00c13470(); };    // 0x00C25E90

struct CSoundEventManager {
    virtual ~CSoundEventManager();
    unsigned char _pad[0x28];              // +4
    CSoundEventSet m_eventSet;               // +0x2c
    CPartD_00c13470* m_pD;                 // +0x7b4
    CPartC_00c13470* m_pC;                 // +0x7b8
    CPartB_00c13470* m_pB;                 // +0x7bc
    long m_7c0;
    CPartA_00c13470* m_pA;                 // +0x7c4
    unsigned char _pad2[0x10];             // +0x7c8
    CMember7D8_00c13470 m_7d8;             // +0x7d8
    void Shutdown();                       // 0x00C123B0
};

CSoundEventManager::~CSoundEventManager()
{
    {
        PKCProtectionInstance lock(&g_lock_0143bb14);
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