// CHTE_Executioner_00c2c970::Query() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, virtual slot 15 of the polymorphic member at +0x8, its result returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2C970, oracle length 95 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov eax,[esi+0x8] ; lea ecx,[esi+0x8] ; mov dword ptr [esp+14h],0 ; call [eax+3Ch] ; lea ecx,[esp+4] ; mov esi,eax ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; mov ecx,[esp+0Ch] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// The scoped-lock shape of `2026-10-14_linux_i1117` with a virtual call on an embedded member
// whose result survives the lock's destructor in esi. The manifest label (`OnPredicateFail`) is
// propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CExecutionTarget_00c2c970 {             // polymorphic member at +0x8
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual long Query();                  // slot 15
};

struct CHTE_Executioner_00c2c970 {
    unsigned char _pad[0x8];
    CExecutionTarget_00c2c970 m_target;         // +0x8
    long Query();
};

long CHTE_Executioner_00c2c970::Query()
{
    CScopedLock lock(&g_lock_0143bb14);
    return m_target.Query();
}