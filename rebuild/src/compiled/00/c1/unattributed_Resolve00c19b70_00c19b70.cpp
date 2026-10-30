// CClothingSuitMap_00c19b70::Resolve(void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, -1 when the key is not found (@ 0x00C196A0), else the entry resolved out of line (@ 0x00C19850); the result survives the lock's destructor in esi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C19B70, oracle length 150 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[key] ; push eax ; mov ecx,esi ; mov dword ptr [esp+18h],0 ; call Find ; test eax,eax ; jne hit ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; or eax,-1 ; pop esi ; ... ret 4 ; hit: push eax ; mov ecx,esi ; call ResolveEntry ; lea ecx,[esp+4] ; mov esi,eax ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; mov eax,esi ; pop esi ; ...
// The manifest label is propagated; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CEntry_00c19b70;

struct CClothingSuitMap_00c19b70 {
    CEntry_00c19b70* Find(void* key);      // 0x00C196A0
    long ResolveEntry(CEntry_00c19b70* entry);   // 0x00C19850
    long Resolve(void* key);
};

long CClothingSuitMap_00c19b70::Resolve(void* key)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    CEntry_00c19b70* entry = Find(key);
    if (!entry)
        return -1;
    return ResolveEntry(entry);
}