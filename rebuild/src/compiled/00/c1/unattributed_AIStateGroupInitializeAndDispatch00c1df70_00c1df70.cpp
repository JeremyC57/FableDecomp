// CAIStateGroup_00c1df70::InitializeAndDispatch(a, b, c, d) -- EH-framed: a request is built on the frame (head @ 0x00C1DD50, its part at +0x10 from the request and flag 1 @ 0x00C1D9D0; destructor @ 0x00C1DCE0, state 0) with a 12-byte dispatch functor (vtable 0x012B32CC, the part and the group's target at +0, both stores scheduled among the pushes); the target's slot-1 virtual takes the four arguments, 0 and the functor; false when it fails (request destroyed), otherwise the flag of the part's 8-byte result record (@ 0x00C1DA30) is returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1DF70, oracle length 211 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,44h ; push esi ; mov esi,ecx ; lea ecx,[esp+18h] ; call CHead::CHead ; push 1 ; lea eax,[esp+1Ch] ; push eax ; lea ecx,[esp+30h] ; call CPart::CPart ; mov ecx,[esi] ; lea edx,[esp+28h] ; mov [esp+10h],edx ; lea edx,[esp+0Ch] ; push edx ; mov edx,[esp+68h] ; push 0 ; push edx ; ... mov dword ptr [esp+20h],12B32CCh ; mov [esp+28h],ecx ; mov eax,[ecx] ; push edx ; mov dword ptr [esp+68h],0 ; call [eax+4] ; test al,al ; pop esi ; jne ok ; lea ecx,[esp+14h] ; mov dword ptr [esp+4Ch],-1 ; call CRequest::~CRequest ; xor al,al ; ... ret 10h ; ok: push ebx ; lea eax,[esp+4] ; push eax ; lea ecx,[esp+2Ch] ; call CPart::GetResult ; mov bl,[esp+4] ; lea ecx,[esp+18h] ; mov dword ptr [esp+50h],-1 ; call CRequest::~CRequest ; ... mov al,bl ; pop ebx ; ... ret 10h
// The manifest label is propagated; the classes are named by address.
struct CHead_00c1df70 {                    // 0x10 bytes: vtable 0x012B332C, a flag byte and two words
    unsigned char _body[0x10];
    CHead_00c1df70();                      // 0x00C1DD50
};

struct SResult_00c1df70 {                  // 8 bytes: the flag and its detail word
    bool m_flag;
    long m_detail;
};

struct CPart_00c1df70 {                    // 0x20 bytes
    unsigned char _body[0x20];
    CPart_00c1df70(CHead_00c1df70* owner, long flag);   // 0x00C1D9D0
    void GetResult(SResult_00c1df70* result);   // 0x00C1DA30
};

struct CRequest_00c1df70 : CHead_00c1df70 {
    CPart_00c1df70 m_part;                 // +0x10
    CRequest_00c1df70() : CHead_00c1df70(), m_part(this, 1) { }
    ~CRequest_00c1df70();                  // 0x00C1DCE0
};

struct CDispatch_00c1df70 {                // 12-byte functor: vtable 0x012B32CC
    virtual void v00();
    CPart_00c1df70* m_pPart;               // +4
    void* m_pTarget;                       // +8
    CDispatch_00c1df70(CPart_00c1df70* part, void* target) : m_pPart(part), m_pTarget(target) { }
};

struct ITarget_00c1df70 {
    virtual void Slot0();
    virtual bool Dispatch(void* a, void* b, void* c, void* d, void* e, CDispatch_00c1df70* functor);   // slot 4
};

struct CAIStateGroup_00c1df70 {
    ITarget_00c1df70* m_pTarget;           // +0
    bool InitializeAndDispatch(void* a, void* b, void* c, void* d);
};

bool CAIStateGroup_00c1df70::InitializeAndDispatch(void* a, void* b, void* c, void* d)
{
    CRequest_00c1df70 request;
    CDispatch_00c1df70 dispatch(&request.m_part, m_pTarget);
    if (!m_pTarget->Dispatch(a, b, c, d, 0, &dispatch))
        return false;
    SResult_00c1df70 result;
    request.m_part.GetResult(&result);
    return result.m_flag;
}