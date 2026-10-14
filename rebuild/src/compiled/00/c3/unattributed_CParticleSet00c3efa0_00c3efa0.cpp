// CParticleSet_00c3efa0::CParticleSet_00c3efa0() -- EH-framed constructor: out-of-line base constructor (@ 0x00C591F0), own vtable (0x012B5BE0), the array of six 0x17c-byte elements at +0xc34 built through `__ehvec_ctor` (element constructor @ 0x00C59570, destructor @ 0x00C592C0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3EFA0, oracle length 95 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; call CBase::CBase ; push offset ~CElement ; push offset CElement::CElement ; push 6 ; push 17Ch ; lea eax,[esi+0C34h] ; push eax ; mov dword ptr [esp+24h],0 ; mov dword ptr [esi],offset vtable ; call __ehvec_ctor ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The constructor counterpart of the `novtable` destructor at 0x00C3F030 (same element type and
// base); an array member of class type is built through the CRT's vector constructor iterator.
// The manifest label is propagated garbage; the classes are named by address.
struct CParticleSetBase_00c3efa0 {
    virtual void v00();
    unsigned char _body[0xc30];
    CParticleSetBase_00c3efa0();           // 0x00C591F0
    ~CParticleSetBase_00c3efa0();
};

struct CElement_00c3efa0 {                 // 0x17c bytes
    unsigned char _body[0x17c];
    CElement_00c3efa0();                   // 0x00C59570
    ~CElement_00c3efa0();                  // 0x00C592C0
};

struct CParticleSet_00c3efa0 : CParticleSetBase_00c3efa0 {   // vtable 0x012B5BE0
    CElement_00c3efa0 m_items[6];          // +0xc34
    CParticleSet_00c3efa0();
};

CParticleSet_00c3efa0::CParticleSet_00c3efa0()
{
}