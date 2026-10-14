// CAnimationTable_00c199a0::GetMaxDamageInAnimation(key) -- EH-framed under the module's profiling lock (0x0143BB14; @ 0x00C0FFC0 / 0x00C0FFE0, state 0): the animation set is looked up by key (@ 0x00C196A0); -1 when absent; otherwise every animation of every group (slot 5 of the set, slot 5 count / slot 4 count-of-group / slot 6 animation-of-group on the list, all unsigned and re-evaluated each iteration) offers the damage of its +8 record (@ 0x00C1BA70, evaluated again when it wins) and the largest is returned, starting from zero (the running maximum lives in the dead key slot)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C199A0, oracle length 289 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,8 ; push esi ; push edi ; mov esi,ecx ; push offset g_section ; lea ecx,[esp+0Ch] ; call CProfileLock::CProfileLock ; mov eax,[esp+20h] ; xor edi,edi ; push eax ; mov ecx,esi ; mov [esp+1Ch],edi ; call CAnimationTable::Find ; mov esi,eax ; cmp esi,edi ; jne walk ; lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~CProfileLock ; fld dword ptr [-1.0f] ; ... ret 4 ; walk: mov edx,[esi] ; mov ecx,esi ; mov dword ptr [esp+20h],0 ; call [edx+14h] ; mov edx,[eax] ; mov ecx,eax ; call [edx+14h] ; test eax,eax ; jbe done ; push ebx ; push ebp ; outer: mov eax,[esi] ; mov ecx,esi ; xor ebx,ebx ; call [eax+14h] ; mov edx,[eax] ; push edi ; mov ecx,eax ; call [edx+10h] ; test eax,eax ; jbe next ; inner: mov eax,[esi] ; mov ecx,esi ; call [eax+14h] ; mov edx,[eax] ; push ebx ; push edi ; mov ecx,eax ; call [edx+18h] ; lea ebp,[eax+8] ; mov ecx,ebp ; call CDamageRecord::GetDamage ; fcomp dword ptr [esp+28h] ; fnstsw ax ; test ah,41h ; jne keep ; mov ecx,ebp ; call CDamageRecord::GetDamage ; fstp dword ptr [esp+28h] ; keep: ... inc ebx ; ... cmp ebx,eax ; jb inner ; next: ... inc edi ; ... cmp edi,eax ; jb outer ; pop ebp ; pop ebx ; done: lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~CProfileLock ; fld dword ptr [esp+20h] ; ... ret 4
// The manifest label ("Weapon_GetMaxDamageInAnimation") is propagated; the classes are named by address.
extern unsigned char g_section_0143bb14[];  // 0x0143BB14: the module's critical section

struct CProfileLock_00c199a0 {             // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    CProfileLock_00c199a0(void* section);  // 0x00C0FFC0
    ~CProfileLock_00c199a0();              // 0x00C0FFE0
};

struct CDamageRecord_00c199a0 {            // +8 of an animation
    float GetDamage();                     // 0x00C1BA70
};
struct CAnimation_00c199a0 {
    unsigned char _pad[8];
    CDamageRecord_00c199a0 m_damage;       // +8
};

struct IGroupList_00c199a0 {
    virtual void Slot0();
    virtual void Slot4();
    virtual void Slot8();
    virtual void SlotC();
    virtual unsigned long CountOfGroup(unsigned long group);   // slot 0x10
    virtual unsigned long Count();                              // slot 0x14
    virtual CAnimation_00c199a0* AnimationOfGroup(unsigned long group, unsigned long index);   // slot 0x18
};

struct IAnimationSet_00c199a0 {
    virtual void Slot0();
    virtual void Slot4();
    virtual void Slot8();
    virtual void SlotC();
    virtual void Slot10();
    virtual IGroupList_00c199a0* Groups();   // slot 0x14
};

struct CAnimationTable_00c199a0 {
    IAnimationSet_00c199a0* Find(void* key);   // 0x00C196A0
    float GetMaxDamageInAnimation(void* key);
};

float CAnimationTable_00c199a0::GetMaxDamageInAnimation(void* key)
{
    CProfileLock_00c199a0 lock(g_section_0143bb14);
    IAnimationSet_00c199a0* set = Find(key);
    if (!set)
        return -1.0f;
    float best = 0.0f;
    for (unsigned long group = 0; group < set->Groups()->Count(); ++group) {
        for (unsigned long index = 0; index < set->Groups()->CountOfGroup(group); ++index) {
            CAnimation_00c199a0* animation = set->Groups()->AnimationOfGroup(group, index);
            if (animation->m_damage.GetDamage() > best)
                best = animation->m_damage.GetDamage();
        }
    }
    return best;
}