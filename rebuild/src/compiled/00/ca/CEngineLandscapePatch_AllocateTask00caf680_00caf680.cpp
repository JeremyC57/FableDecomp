// CEngineLandscapePatch::AllocateTask(CTask**, void*, double, void* x8) -- EH-framed: under a one-word scoped lock over the critical section at +8 (state 0) the out pointer and a result word are zeroed, a 0x50-byte task allocated (`new`, result homed in the dead out-pointer slot, state 1) and built with `this`, 0, the address of the result word and the eleven forwarded arguments (the double passed through the x87 stack, @ 0x00CAF230); E_OUTOFMEMORY (0x8007000E) when the allocation failed, else the task is stored and the result word returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAF680, oracle length 208 bytes. Built under /EHsc.
// Retail bytes:  ... lea edi,[esi+8] ; push edi ; mov [esp+14h],edi ; call [__imp_EnterCriticalSection] ; mov ebx,[esp+24h] ; push 50h ; mov dword ptr [esp+20h],0 ; mov dword ptr [esp+10h],0 ; mov dword ptr [ebx],0 ; call operator new ; add esp,4 ; mov [esp+24h],eax ; test eax,eax ; mov byte ptr [esp+1Ch],1 ; je null ; (the eleven arguments pushed last to first, the double through `fld`/`fstp qword ptr [esp]`) ; lea edx,[esp+38h] ; push edx ; push 0 ; push esi ; mov ecx,eax ; call CTask::CTask ; null: test eax,eax ; jne ok ; mov esi,8007000Eh ; jmp done ; ok: mov esi,[esp+0Ch] ; mov [ebx],eax ; done: push edi ; call [__imp_LeaveCriticalSection] ; ... mov eax,esi ... ret 30h
// The constructor's result word is read back after the call. Named from the manifest (the
// task constructor's label, COM_InitializeDispatchContainer, is a description).
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);

struct CSectionLock_00caf680 {             // scoped critical-section holder
    void* m_pSection;
    CSectionLock_00caf680(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CSectionLock_00caf680() { LeaveCriticalSection(m_pSection); }
};

struct CEngineLandscapePatch_00caf680;
struct CTask_00caf680 {                    // 0x50 bytes
    unsigned char _body[0x50];
    CTask_00caf680(CEngineLandscapePatch_00caf680* owner, long zero, long* result, void* a, double scale,
                   void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i);   // 0x00CAF230
};

struct CEngineLandscapePatch_00caf680 {
    long m_0;
    long m_4;
    unsigned char m_section[0x18];         // +8
    long AllocateTask(CTask_00caf680** out, void* a, double scale, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i);
};

long CEngineLandscapePatch_00caf680::AllocateTask(CTask_00caf680** out, void* a, double scale, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i)
{
    CSectionLock_00caf680 lock(m_section);
    long result = 0;
    *out = 0;
    CTask_00caf680* task = new CTask_00caf680(this, 0, &result, a, scale, b, c, d, e, f, g, h, i);
    long hr;
    if (!task)
        hr = 0x8007000E;
    else {
        hr = result;
        *out = task;
    }
    return hr;
}