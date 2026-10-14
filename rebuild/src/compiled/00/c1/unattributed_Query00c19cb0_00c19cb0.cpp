// CClothingSuitPartMap_00c19cb0::Query(void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the node found out of line (@ 0x00C19680) answers through virtual slot 15 of its member at +0x10; the result survives the lock's destructor in esi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C19CB0, oracle length 109 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov eax,[key] ; push eax ; mov ecx,esi ; mov dword ptr [esp+18h],0 ; call Find ; mov edx,[eax+10h] ; lea ecx,[eax+10h] ; call [edx+3Ch] ; lea ecx,[esp+4] ; mov esi,eax ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; mov ecx,[esp+0Ch] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 4
// The sibling of 0x00C19D20 with the virtual call on the node's embedded member. The manifest
// label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CPartTarget_00c19cb0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual long Query();                  // slot 15
};

struct CNode_00c19cb0 {
    unsigned char _pad[0x10];
    CPartTarget_00c19cb0 m_target;         // +0x10
};

struct CClothingSuitPartMap_00c19cb0 {
    CNode_00c19cb0* Find(void* key);       // 0x00C19680
    long Query(void* key);
};

long CClothingSuitPartMap_00c19cb0::Query(void* key)
{
    CScopedLock lock(&g_lock_0143bb14);
    return Find(key)->m_target.Query();
}