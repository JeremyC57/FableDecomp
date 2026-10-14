// CCombatEventSource_00c2c7e0::CreateParryBlockEvent(target, key, data) -- EH-framed under the module's profiling lock (0x0143BB14; @ 0x00C0FFC0 / 0x00C0FFE0, state 0): 0 without a target or when the target's +0x74 map does not know the key (@ 0x00C0E950, `this` = the target); otherwise a 0x4c-byte event is allocated (null-checked `new`, state 1 frees it if the constructor @ 0x00C38570 throws; the pointer homed in the dead target slot) from the source, the resolved key and the data, registered with the world's +8 and +0xc dispatchers (@ 0x00C2A7F0 / 0x00C2BCF0, both reached through the +4 world pointer) and returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2C7E0, oracle length 259 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,8 ; push edi ; mov edi,ecx ; push offset g_section ; lea ecx,[esp+8] ; call CProfileLock::CProfileLock ; mov ecx,[esp+1Ch] ; test ecx,ecx ; mov dword ptr [esp+14h],0 ; jne go ; ... xor eax,eax ; ... ret 0Ch ; go: mov eax,[esp+20h] ; push esi ; push eax ; call CTarget::Lookup ; mov esi,eax ; test esi,esi ; jne make ; ... xor eax,eax ; ... ret 0Ch ; make: push 4Ch ; call operator new ; add esp,4 ; mov [esp+20h],eax ; test eax,eax ; mov byte ptr [esp+18h],1 ; je null ; mov ecx,[esp+28h] ; push ecx ; push esi ; push edi ; mov ecx,eax ; call CEvent::CEvent ; mov esi,eax ; jmp got ; null: xor esi,esi ; got: mov edx,[edi+4] ; mov ecx,[edx+8] ; push esi ; mov byte ptr [esp+1Ch],0 ; call CDispatcherA::Register ; mov eax,[edi+4] ; mov ecx,[eax+0Ch] ; push esi ; call CDispatcherB::Register ; lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~CProfileLock ; ... mov eax,esi ; ... ret 0Ch
// The manifest label is propagated; the classes are named by address.
void* __cdecl operator new(unsigned int size);
extern unsigned char g_section_0143bb14[];  // 0x0143BB14: the module's critical section

struct CProfileLock_00c2c7e0 {             // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    CProfileLock_00c2c7e0(void* section);  // 0x00C0FFC0
    ~CProfileLock_00c2c7e0();              // 0x00C0FFE0
};

struct CCombatEventSource_00c2c7e0;
struct CResolvedKey_00c2c7e0;
struct CTarget_00c2c7e0 { CResolvedKey_00c2c7e0* Lookup(void* key); };   // 0x00C0E950: a map lookup on the target's +0x74 map

struct CEvent_00c2c7e0 {                   // 0x4c bytes
    unsigned char _body[0x4c];
    CEvent_00c2c7e0(CCombatEventSource_00c2c7e0* source, CResolvedKey_00c2c7e0* key, void* data);   // 0x00C38570
};

struct CDispatcherA_00c2c7e0 { void Register(CEvent_00c2c7e0* event); };   // 0x00C2A7F0
struct CDispatcherB_00c2c7e0 { void Register(CEvent_00c2c7e0* event); };   // 0x00C2BCF0

struct CWorld_00c2c7e0 {
    unsigned char _pad[8];
    CDispatcherA_00c2c7e0* m_pDispatcherA; // +8
    CDispatcherB_00c2c7e0* m_pDispatcherB; // +0xc
};

struct CCombatEventSource_00c2c7e0 {
    long m_0;
    CWorld_00c2c7e0* m_pWorld;             // +4
    CEvent_00c2c7e0* CreateParryBlockEvent(CTarget_00c2c7e0* target, void* key, void* data);
};

CEvent_00c2c7e0* CCombatEventSource_00c2c7e0::CreateParryBlockEvent(CTarget_00c2c7e0* target, void* key, void* data)
{
    CProfileLock_00c2c7e0 lock(g_section_0143bb14);
    if (!target)
        return 0;
    CResolvedKey_00c2c7e0* resolved = target->Lookup(key);
    if (!resolved)
        return 0;
    CEvent_00c2c7e0* event = new CEvent_00c2c7e0(this, resolved, data);
    m_pWorld->m_pDispatcherA->Register(event);
    m_pWorld->m_pDispatcherB->Register(event);
    return event;
}
