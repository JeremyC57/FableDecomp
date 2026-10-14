// CConstraintRepeat_00c38470::~CConstraintRepeat_00c38470() -- EH-framed destructor: own vtable (0x012B53B0), unregisters from two registries of the owner's owner (virtual slot 8 of the member at +0x40 of the object at +4 of the object at +4, then slot 8 of the member at +8 of the object at +4), then the polymorphic member at +0x40 and the base restore their vtables (0x012B2828, 0x012B53A0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C38470, oracle length 100 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov eax,[esi+4] ; mov eax,[eax+4] ; mov edx,[eax+40h] ; lea ecx,[eax+40h] ; push esi ; mov dword ptr [esp+14h],1 ; call [edx+20h] ; mov eax,[esi+4] ; lea ecx,[eax+8] ; mov eax,[ecx] ; push esi ; call [eax+20h] ; mov ecx,[esp+8] ; mov dword ptr [esi+40h],offset member vtable ; mov dword ptr [esi],offset base vtable ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// EH state 1 covers the body (two tracked objects: the polymorphic member at +0x40 and the base,
// both with inline empty destructors that only restore their vtables). The manifest label is a
// propagated unwind-handler name; the classes are named by address, the registries descriptive.
struct CConstraintRepeat_00c38470;
struct CRegistry_00c38470 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CConstraintRepeat_00c38470* constraint);   // slot 8
};

struct CProgramOwner_00c38470 {
    unsigned char _pad[0x40];
    CRegistry_00c38470 m_constraints;      // +0x40
};

struct CProgram_00c38470 {
    unsigned char _pad[4];
    CProgramOwner_00c38470* m_pOwner;      // +4
    CRegistry_00c38470 m_updates;          // +8
};

struct CConstraintBase_00c38470 {          // vtable 0x012B53A0
    virtual void v00();
    ~CConstraintBase_00c38470() { }
};

struct CConstraintSlot_00c38470 {          // +0x40: vtable 0x012B2828
    virtual void v00();
    ~CConstraintSlot_00c38470() { }
};

struct CConstraintRepeat_00c38470 : CConstraintBase_00c38470 {   // vtable 0x012B53B0
    CProgram_00c38470* m_pProgram;         // +4
    unsigned char _pad[0x38];
    CConstraintSlot_00c38470 m_slot;       // +0x40
    ~CConstraintRepeat_00c38470();
};

CConstraintRepeat_00c38470::~CConstraintRepeat_00c38470()
{
    m_pProgram->m_pOwner->m_constraints.Remove(this);
    m_pProgram->m_updates.Remove(this);
}