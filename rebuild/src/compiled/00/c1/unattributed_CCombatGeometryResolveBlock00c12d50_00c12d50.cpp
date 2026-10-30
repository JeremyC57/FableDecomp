// CCombatGeometry_00c12d50::ResolveBlock(attacker, context, target) -- EH-framed under the module's profiling lock (0x0143BB14; @ 0x00C0FFC0 / 0x00C0FFE0, state 0): -1 without an attacker; otherwise a 0x50-byte query record is built on the frame (two parts with plain constructors @ 0x00C203A0 / 0x00C20390, the scalars 0, 0, 1.0f, 1.0f, 0 and 0.0f, 0 (word), 0.0f, 0.0f between them), bound to the target (@ 0x00C27C10) and resolved (@ 0x00C12CD0 with the attacker, the context, the record and 0); the hit's +0xc value or -1 is returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C12D50, oracle length 256 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,58h ; push ebx ; push esi ; push edi ; mov edi,ecx ; push offset g_section ; lea ecx,[esp+10h] ; call CProfileLock::CProfileLock ; mov ebx,[esp+74h] ; xor esi,esi ; cmp ebx,esi ; mov [esp+6Ch],esi ; jne go ; lea ecx,[esp+0Ch] ; mov dword ptr [esp+6Ch],-1 ; call ~CProfileLock ; ... or eax,-1 ; ... ret 0Ch ; go: lea ecx,[esp+14h] ; call CPartA::CPartA ; lea ecx,[esp+44h] ; mov [esp+30h],esi ; mov [esp+34h],esi ; mov dword ptr [esp+38h],3F800000h ; mov dword ptr [esp+3Ch],3F800000h ; mov [esp+40h],esi ; call CPartB::CPartB ; mov eax,[esp+7Ch] ; push eax ; lea ecx,[esp+18h] ; mov dword ptr [esp+58h],0 ; mov [esp+5Ch],si ; mov dword ptr [esp+60h],0 ; mov dword ptr [esp+64h],0 ; call CQuery::Bind ; mov edx,[esp+78h] ; push esi ; lea ecx,[esp+18h] ; push ecx ; push edx ; push ebx ; mov ecx,edi ; call Resolve ; cmp eax,esi ; je miss ; mov esi,[eax+0Ch] ; jmp done ; miss: or esi,-1 ; done: lea ecx,[esp+0Ch] ; mov dword ptr [esp+6Ch],-1 ; call ~CProfileLock ; ... mov eax,esi ; ... ret 0Ch
// The manifest label is propagated; the classes are named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;  // 0x0143BB14: the module's critical section

class PKCProtectionInstance { public:             // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    PKCProtectionInstance(PKCCriticalSection* section);  // 0x00C0FFC0
    ~PKCProtectionInstance();              // 0x00C0FFE0
};

struct CPartA_00c12d50 { unsigned char _body[0x1c]; CPartA_00c12d50(); };   // 0x00C203A0
struct CPartB_00c12d50 { unsigned char _body[0x10]; CPartB_00c12d50(); };   // 0x00C20390

struct CQuery_00c12d50 {                   // 0x50-byte query record
    CPartA_00c12d50 m_0;
    long m_1c;
    long m_20;
    float m_24;
    float m_28;
    long m_2c;
    CPartB_00c12d50 m_30;
    float m_40;                            // the three floats take immediate zeros, the words the zero register
    short m_44;
    float m_48;
    float m_4c;
    CQuery_00c12d50() : m_0(), m_1c(0), m_20(0), m_24(1.0f), m_28(1.0f), m_2c(0), m_30(), m_40(0.0f), m_44(0), m_48(0.0f), m_4c(0.0f) { }
    void Bind(void* target);               // 0x00C27C10
};

struct CHit_00c12d50 {
    unsigned char _pad[0xc];
    long m_value;                          // +0xc
};

struct CCombatGeometry_00c12d50 {
    CHit_00c12d50* Resolve(void* attacker, void* context, CQuery_00c12d50* query, long flags);   // 0x00C12CD0
    long ResolveBlock(void* attacker, void* context, void* target);
};

long CCombatGeometry_00c12d50::ResolveBlock(void* attacker, void* context, void* target)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    if (!attacker)
        return -1;
    CQuery_00c12d50 query;
    query.Bind(target);
    CHit_00c12d50* hit = Resolve(attacker, context, &query, 0);
    long result;
    if (hit)
        result = hit->m_value;
    else
        result = -1;
    return result;
}