// CCombatStats_00c15d50::CalculateHitRatio(ratio) -- EH-framed under the module's profiling lock (0x0143BB14; ctor @ 0x00C0FFC0, dtor @ 0x00C0FFE0, state 0): the statistics source at +8 fills three frame words (@ 0x00C2D420, bool result); 0 when it fails, 1 when the hit or total count is negative, otherwise the ratio hits / total is written as a float (`fild` / `fidiv`) and 2 returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C15D50, oracle length 219 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,14h ; push esi ; mov esi,ecx ; push offset g_section ; lea ecx,[esp+14h] ; call CProfileLock::CProfileLock ; lea eax,[esp+8] ; push eax ; lea ecx,[esp+10h] ; push ecx ; mov ecx,[esi+8] ; lea edx,[esp+0Ch] ; push edx ; mov dword ptr [esp+2Ch],0 ; call CStats::Query ; test al,al ; pop esi ; jne ok ; lea ecx,[esp+0Ch] ; mov dword ptr [esp+1Ch],-1 ; call ~CProfileLock ; xor eax,eax ; ... ret 4 ; ok: mov eax,[esp] ; test eax,eax ; jl neg ; mov eax,[esp+4] ; test eax,eax ; jl neg ; fild dword ptr [esp] ; mov eax,[esp+24h] ; lea ecx,[esp+0Ch] ; mov dword ptr [esp+1Ch],-1 ; fidiv dword ptr [esp+4] ; fstp dword ptr [eax] ; call ~CProfileLock ; mov eax,2 ; ... ret 4 ; neg: lea ecx,[esp+0Ch] ; ... call ~CProfileLock ; ... mov eax,1 ; ... ret 4
// The manifest label is propagated; the classes are named by address.
extern unsigned char g_section_0143bb14[];  // 0x0143BB14: the module's critical section

struct CProfileLock_00c15d50 {             // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    CProfileLock_00c15d50(void* section);  // 0x00C0FFC0
    ~CProfileLock_00c15d50();              // 0x00C0FFE0
};

struct CStats_00c15d50 { bool Query(long* hits, long* extra, long* total); };   // 0x00C2D420

struct CCombatStats_00c15d50 {
    unsigned char _pad[8];
    CStats_00c15d50* m_pStats;             // +8
    long CalculateHitRatio(float* ratio);
};

long CCombatStats_00c15d50::CalculateHitRatio(float* ratio)
{
    CProfileLock_00c15d50 lock(g_section_0143bb14);
    long hits;
    long extra;
    long total;
    if (!m_pStats->Query(&hits, &extra, &total))
        return 0;
    if (hits < 0 || total < 0)
        return 1;
    *ratio = (float)hits / total;
    return 2;
}