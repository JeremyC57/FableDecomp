// CHeroExperience_00c19760::QueryDamage(IStats*, unsigned long, unsigned long) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the table behind the object's virtual slot 5 is consulted (re-fetched for each use): 0 when the first index is at or beyond its count (slot 5) or the second at or beyond the row length (slot 4 with the first); else the entry (slot 6 with both)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C19760, oracle length 232 bytes. Built under /EHsc.
// Retail bytes:  ... mov esi,[esp+20h] ; mov eax,[esi] ; mov ecx,esi ; mov dword ptr [esp+18h],0 ; call [eax+14h] ; mov edx,[eax] ; mov ecx,eax ; call [edx+14h] ; mov edi,[esp+24h] ; cmp edi,eax ; jb row ; lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~CScopedLock ; ... xor eax,eax ... ret 0Ch ; row: mov eax,[esi] ; push ebx ; mov ecx,esi ; call [eax+14h] ; mov edx,[eax] ; push edi ; mov ecx,eax ; call [edx+10h] ; mov ebx,[esp+2Ch] ; cmp ebx,eax ; jb entry ; ... xor eax,eax ... ret 0Ch ; entry: mov eax,[esi] ; mov ecx,esi ; call [eax+14h] ; mov edx,[eax] ; push ebx ; push edi ; mov ecx,eax ; call [edx+18h] ; ... mov esi,eax ... ret 0Ch
// `this` is unused; each early return keeps its own epilogue. The manifest label is a
// description; the classes are named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct IDamageTable_00c19760 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual unsigned long GetRowLength(unsigned long row);   // slot 4
    virtual unsigned long GetRowCount();                     // slot 5
    virtual long GetEntry(unsigned long row, unsigned long column);   // slot 6
};

struct IStats_00c19760 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual IDamageTable_00c19760* GetDamageTable();   // slot 5
};

struct CHeroExperience_00c19760 {
    long QueryDamage(IStats_00c19760* stats, unsigned long row, unsigned long column);
};

long CHeroExperience_00c19760::QueryDamage(IStats_00c19760* stats, unsigned long row, unsigned long column)
{
    CScopedLock lock(&g_lock_0143bb14);
    if (row >= stats->GetDamageTable()->GetRowCount())
        return 0;
    if (column >= stats->GetDamageTable()->GetRowLength(row))
        return 0;
    return stats->GetDamageTable()->GetEntry(row, column);
}
