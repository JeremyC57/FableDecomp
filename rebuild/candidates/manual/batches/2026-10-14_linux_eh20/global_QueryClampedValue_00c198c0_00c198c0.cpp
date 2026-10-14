// QueryClampedValue_00c198c0(CEntry*, unsigned long) -- EH-framed `__stdcall`: under a scoped lock over the global @ 0x0143BB14, -1 when the index is not below the count of the entry's table (virtual slot 5 of the entry yields the table, its virtual slot 5 the count), else the table's element (virtual slot 4 with the index); the result survives the lock's destructor in esi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C198C0, oracle length 166 bytes. Built under /EHsc.
// Retail bytes:  ... mov esi,[entry] ; mov eax,[esi] ; mov ecx,esi ; mov dword ptr [esp+18h],0 ; call [eax+14h] ; mov edx,[eax] ; mov ecx,eax ; call [edx+14h] ; mov edi,[index] ; cmp edi,eax ; jb hit ; ... or eax,-1 ; ... ret 8 ; hit: mov eax,[esi] ; mov ecx,esi ; call [eax+14h] ; mov edx,[eax] ; push edi ; mov ecx,eax ; call [edx+10h] ; lea ecx,[esp+8] ; mov esi,eax ; ...
// `this` is not used, so the function is modelled as a `__stdcall` free function (the callee of
// the two-argument resolve landed at 0x00C19C10). The manifest label is a description.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CTable_00c198c0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual long Get(unsigned long index); // slot 4
    virtual unsigned long Count();         // slot 5
};

struct CEntry_00c198c0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual CTable_00c198c0* GetTable();   // slot 5
};

long __stdcall QueryClampedValue_00c198c0(CEntry_00c198c0* entry, unsigned long index)
{
    CScopedLock lock(&g_lock_0143bb14);
    if (index >= entry->GetTable()->Count())
        return -1;
    return entry->GetTable()->Get(index);
}
