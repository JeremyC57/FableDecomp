// CCollection_00c0eb70::GetAt(unsigned long) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, null when the index is not below the count of the polymorphic member at +0x50 (virtual slot 15), else the element fetched out of line (@ 0x00C0EA70); the result survives the lock's destructor in esi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0EB70, oracle length 150 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov eax,[esi+50h] ; lea ecx,[esi+50h] ; mov dword ptr [esp+14h],0 ; call [eax+3Ch] ; mov ecx,[index] ; cmp ecx,eax ; jb fetch ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; xor eax,eax ; pop esi ; ... ret 4 ; fetch: push ecx ; mov ecx,esi ; call Fetch ; lea ecx,[esp+4] ; mov esi,eax ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; mov ecx,[esp+0Ch] ; mov eax,esi ; pop esi ; ...
// The manifest label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CItems_00c0eb70 {                   // polymorphic member at +0x50
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual unsigned long Count();         // slot 15
};

struct CCollection_00c0eb70 {
    unsigned char _pad[0x50];
    CItems_00c0eb70 m_items;               // +0x50
    void* Fetch(unsigned long index);      // 0x00C0EA70
    void* GetAt(unsigned long index);
};

void* CCollection_00c0eb70::GetAt(unsigned long index)
{
    CScopedLock lock(&g_lock_0143bb14);
    if (index >= m_items.Count())
        return 0;
    return Fetch(index);
}
