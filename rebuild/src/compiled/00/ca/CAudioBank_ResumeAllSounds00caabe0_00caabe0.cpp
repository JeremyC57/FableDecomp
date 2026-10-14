// CAudioBank_00caabe0::ResumeAllSounds(CBankState*) -- EH-framed __stdcall walk: holds the bank's critical section (+0x2c of the state part) through a scoped lock (state 0); when the bank state at +8 is 0 every sound of the owning list (base at -0xc: count through slot 0x18, element through slot 0x1c) with a live handle at +0x18 is resumed through slot 0x14, a negative result returned at once; otherwise the state is set to 1 and 0 returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAABE0, oracle length 192 bytes. Built under /EHsc.
// Retail bytes:  ... mov ebx,[esp+18h] ; mov eax,[ebx+2Ch] ; ... push eax ; mov [esp+14h],eax ; call [__imp_EnterCriticalSection] ; mov eax,[ebx+8] ; xor edi,edi ; cmp eax,edi ; mov [esp+1Ch],edi ; jne done ; mov eax,[ebx-0Ch] ; lea esi,[ebx-0Ch] ; mov ecx,esi ; call [eax+18h] ; mov ebp,eax ; test ebp,ebp ; jle done ; loop: mov edx,[esi] ; push edi ; mov ecx,esi ; call [edx+1Ch] ; mov ecx,[eax+18h] ; test ecx,ecx ; je next ; mov edx,[eax] ; mov ecx,eax ; call [edx+14h] ; mov ebx,eax ; test ebx,ebx ; jl fail ; mov ebx,[esp+24h] ; next: inc edi ; cmp edi,ebp ; jl loop ; done: ... mov dword ptr [ebx+8],1 ; call [__imp_LeaveCriticalSection] ; ... xor eax,eax ; ... ret 4 ; fail: ... mov eax,ebx ; ... ret 4
// The list part is the first base of the bank, the state part the second (+0xc); the walk
// reaches the list through a reference cast (no null test). The manifest label is propagated;
// the classes are named by address.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);

struct CSectionLock_00caabe0 {             // scoped critical-section holder
    void* m_pSection;
    CSectionLock_00caabe0(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CSectionLock_00caabe0() { LeaveCriticalSection(m_pSection); }
};

struct CSound_00caabe0 {                   // one bank sound
    virtual void Slot0();
    virtual void Slot4();
    virtual void Slot8();
    virtual void SlotC();
    virtual void Slot10();
    virtual long Resume();                 // slot 0x14
    unsigned char _pad[0x14];
    void* m_pHandle;                       // +0x18
};

struct CSoundList_00caabe0 {               // first base of the bank: the sound list
    virtual void Slot0();
    virtual void Slot4();
    virtual void Slot8();
    virtual void SlotC();
    virtual void Slot10();
    virtual void Slot14();
    virtual long GetCount();               // slot 0x18
    virtual CSound_00caabe0* GetAt(long index);   // slot 0x1c
    long m_a;                              // +4
    long m_b;                              // +8
};

struct CBankState_00caabe0 {               // second base of the bank (+0xc)
    unsigned char _pad[8];
    long m_state;                          // +8
    unsigned char _pad2[0x20];
    void* m_pSection;                      // +0x2c
};

struct CAudioBank_00caabe0 : CSoundList_00caabe0, CBankState_00caabe0 {
    static long __stdcall ResumeAllSounds(CBankState_00caabe0* state);
};

long __stdcall CAudioBank_00caabe0::ResumeAllSounds(CBankState_00caabe0* state)
{
    CSectionLock_00caabe0 lock(state->m_pSection);
    if (state->m_state == 0) {
        CSoundList_00caabe0& list = static_cast<CAudioBank_00caabe0&>(*state);
        long count = list.GetCount();
        for (long i = 0; i < count; ++i) {
            CSound_00caabe0* sound = list.GetAt(i);
            if (sound->m_pHandle) {
                long result = sound->Resume();
                if (result < 0)
                    return result;
            }
        }
    }
    state->m_state = 1;
    return 0;
}