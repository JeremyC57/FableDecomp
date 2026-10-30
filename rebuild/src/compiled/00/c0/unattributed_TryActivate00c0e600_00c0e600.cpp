// CScriptHost_00c0e600::TryActivate(void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, false while the word at +0x48 is set; else the current script at +4 is released (virtual slot 3) and nulled, the factory at +0x5c of the owner at +8 asked for a new one (virtual slot 0 with the key), stored at +4; true when one was made
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0E600, oracle length 171 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esi+48h] ; test eax,eax ; mov dword ptr [esp+14h],0 ; je go ; mov dword ptr [esp+14h],-1 ; lea ecx,[esp+4] ; call ~PKCProtectionInstance ; xor al,al ; ... ret 4 ; go: mov ecx,[esi+4] ; test ecx,ecx ; je make ; mov eax,[ecx] ; call [eax+0Ch] ; mov dword ptr [esi+4],0 ; make: mov ecx,[esi+8] ; mov ecx,[ecx+5Ch] ; mov eax,[esp+1Ch] ; mov edx,[ecx] ; push eax ; call [edx] ; test eax,eax ; mov [esi+4],eax ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; je false ; call ~PKCProtectionInstance ; ... mov al,1 ; ... ret 4
// The two false exits share the lock destructor call; the true exit keeps its own. The manifest
// label is a description; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CScript_00c0e600 {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void Release();                // slot 3
};

struct CScriptFactory_00c0e600 {
    virtual CScript_00c0e600* Create(void* key);   // slot 0
};

struct CScriptOwner_00c0e600 {
    unsigned char _pad[0x5c];
    CScriptFactory_00c0e600* m_pFactory;   // +0x5c
};

struct CScriptHost_00c0e600 {
    long m_0;
    CScript_00c0e600* m_pScript;           // +4
    CScriptOwner_00c0e600* m_pOwner;       // +8
    unsigned char _pad[0x3c];
    long m_locked;                         // +0x48
    bool TryActivate(void* key);
};

bool CScriptHost_00c0e600::TryActivate(void* key)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    if (m_locked)
        return false;
    if (m_pScript) {
        m_pScript->Release();
        m_pScript = 0;
    }
    m_pScript = m_pOwner->m_pFactory->Create(key);
    if (!m_pScript)
        return false;
    return true;
}