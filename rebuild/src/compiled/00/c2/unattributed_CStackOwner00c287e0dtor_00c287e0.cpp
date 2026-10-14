// CStackOwner_00c287e0::~CStackOwner_00c287e0() -- EH-framed destructor: own vtable (0x012B4098), an out-of-line cleanup on `this` (@ 0x00C27E60, state 2), the member at +0x450 destroyed out of line (@ 0x00C0BA40, state 1), the vector at +0x440 freed inline (global operator delete @ 0x00BFE9BC, then its three words zeroed), then the polymorphic member at +0x34 stores its vtable (0x012B37FC) from its inlined empty destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C287E0, oracle length 136 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov dword ptr [esp+10h],2 ; call Cleanup ; lea ecx,[esi+450h] ; mov byte ptr [esp+10h],1 ; call ~CMember ; mov eax,[esi+440h] ; test eax,eax ; je skip ; push eax ; call operator delete ; add esp,4 ; skip: mov ecx,[esp+8] ; mov dword ptr [esi+440h],0 ; mov dword ptr [esi+444h],0 ; mov dword ptr [esi+448h],0 ; mov dword ptr [esi+34h],offset base2vtable ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The object at +0x34 is a member, not a second base (a second base would have its vtable
// stored at entry too). The vector member's inline release calls a non-throwing operator
// delete, so no further EH state is stored. The manifest
// label (`~Stack<unsigned long,64>`) is propagated; the classes are named by address.
void __cdecl operator delete(void* block) throw();   // non-throwing: the vector release registers no EH state

struct CListener_00c287e0 {                // +0x34, vtable 0x012B37FC: a polymorphic member with an inline empty destructor
    virtual void v00();
    ~CListener_00c287e0() { }
};

struct CVector_00c287e0 {                  // +0x440: {begin, end, capacity}
    void* m_pBegin;
    void* m_pEnd;
    void* m_pCapacity;
    ~CVector_00c287e0()
    {
        if (m_pBegin)
            operator delete(m_pBegin);
        m_pBegin = 0;
        m_pEnd = 0;
        m_pCapacity = 0;
    }
};

struct CMember_00c287e0 {                  // +0x450
    unsigned char _body[4];
    ~CMember_00c287e0();                   // 0x00C0BA40
};

struct CStackOwner_00c287e0 {              // vtable 0x012B4098
    virtual void v00();
    unsigned char _pad[0x30];
    CListener_00c287e0 m_listener;         // +0x34
    unsigned char _pad2[0x408];
    CVector_00c287e0 m_vector;             // +0x440
    long m_44c;
    CMember_00c287e0 m_member;             // +0x450
    void Cleanup();                        // 0x00C27E60
    ~CStackOwner_00c287e0();
};

CStackOwner_00c287e0::~CStackOwner_00c287e0()
{
    Cleanup();
}