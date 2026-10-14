// CCombatAbilityStrafeDef_00c0edf0::CCombatAbilityStrafeDef(owner) -- EH-framed constructor (the manifest's "Clone" is propagated): vtable 0x012B25A8 over a base with an out-of-line destructor (@ 0x00C0E580, state 0), +4 cleared and the owner at +8, a CNavButton member at +0xc (ctor @ 0x00C0A6D0, state 1; its destructor is the landed 0x00C10320), a three-word buffer at +0x2c (inline zeroing, destructor @ 0x00C0EF40, state 2), four list containers whose roots are inline and whose root/container destructors are out of line (+0x38 / +0x50 / +0x60 with 8-byte nodes @ 0x00C185B0 mode 1, +0x74 with the 0x28-byte node @ 0x00C194F0 mode 0; states 3-10, every container pointer homed in the dead argument slot), +0x48 cleared, +0x4c set to 1, +0x70 to -1, then the owner's registry at +4 takes the definition (slot 9)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0EDF0, oracle length 248 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esp+1Ch] ; xor edi,edi ; lea ecx,[esi+0Ch] ; mov [esp+14h],edi ; mov dword ptr [esi],12B25A8h ; mov [esi+4],edi ; mov [esi+8],eax ; call CNavButton::CNavButton ; mov [esi+2Ch],edi ; mov [esi+30h],edi ; mov [esi+34h],edi ; lea eax,[esi+38h] ; lea ecx,[eax+8] ; mov [esp+1Ch],eax ; mov [eax+4],ecx ; push 1 ; mov byte ptr [esp+18h],3 ; mov dword ptr [eax],12B2560h ; call CSmallNode::CSmallNode ; lea eax,[esi+50h] ; lea ecx,[eax+8] ; mov [esi+48h],edi ; mov dword ptr [esi+4Ch],1 ; ... (12B2350h, 5) ... (12B23A0h, 7) ... mov dword ptr [esi+70h],-1 ; ... push edi ; mov byte ptr [esp+18h],9 ; mov dword ptr [eax],12B23F0h ; call CNode::CNode ; mov eax,[esi+8] ; mov edx,[eax+4] ; lea ecx,[eax+4] ; push esi ; mov byte ptr [esp+18h],0Ah ; call [edx+24h] ; ... mov eax,esi ; ... ret 4
// The classes are named by address.
struct CCombatAbilityStrafeDef_00c0edf0;
struct CRegistry_00c0edf0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Add(CCombatAbilityStrafeDef_00c0edf0* def);   // slot 9
};
struct COwner_00c0edf0 {
    unsigned char _pad[4];
    CRegistry_00c0edf0 m_registry;         // +4
};

struct CDefBase_00c0edf0 {                 // polymorphic base: its vtable store is dead and dropped
    virtual void v00();
    CDefBase_00c0edf0() { }
    ~CDefBase_00c0edf0();                  // 0x00C0E580
};

struct CNavButton_00c0edf0 {               // 0x20 bytes
    unsigned char _body[0x20];
    CNavButton_00c0edf0();                 // 0x00C0A6D0
    ~CNavButton_00c0edf0();                // 0x00C10320
};

struct CBuffer_00c0edf0 {                  // three words
    void* m_pData;
    long m_a;
    long m_b;
    CBuffer_00c0edf0() : m_pData(0), m_a(0), m_b(0) { }
    ~CBuffer_00c0edf0();                   // 0x00C0EF40
};

struct CSmallNode_00c0edf0 {               // 8-byte node
    unsigned char _body[8];
    CSmallNode_00c0edf0(long mode);        // 0x00C185B0
    ~CSmallNode_00c0edf0();
};

struct CNode_00c0edf0 {                    // 0x28-byte node
    unsigned char _body[0x28];
    CNode_00c0edf0(long mode);             // 0x00C194F0
    ~CNode_00c0edf0();                     // 0x00C19250
};

struct CRootA_00c0edf0 { virtual void v00(); CSmallNode_00c0edf0* m_pNode; CRootA_00c0edf0(CSmallNode_00c0edf0* node) : m_pNode(node) { } ~CRootA_00c0edf0(); };   // 0x00C0F4D0
struct CRootB_00c0edf0 { virtual void v00(); CSmallNode_00c0edf0* m_pNode; CRootB_00c0edf0(CSmallNode_00c0edf0* node) : m_pNode(node) { } ~CRootB_00c0edf0(); };   // 0x00C0F040
struct CRootC_00c0edf0 { virtual void v00(); CSmallNode_00c0edf0* m_pNode; CRootC_00c0edf0(CSmallNode_00c0edf0* node) : m_pNode(node) { } ~CRootC_00c0edf0(); };   // 0x00C0F050
struct CRootD_00c0edf0 { virtual void v00(); CNode_00c0edf0* m_pNode; CRootD_00c0edf0(CNode_00c0edf0* node) : m_pNode(node) { } ~CRootD_00c0edf0(); };            // 0x00C0F060

struct CContainerA_00c0edf0 : CRootA_00c0edf0 { CSmallNode_00c0edf0 m_node; CContainerA_00c0edf0() : CRootA_00c0edf0(&m_node), m_node(1) { } ~CContainerA_00c0edf0(); };   // vtable 0x012B2560, +0x38; dtor 0x00C0FE40
struct CContainerB_00c0edf0 : CRootB_00c0edf0 { CSmallNode_00c0edf0 m_node; CContainerB_00c0edf0() : CRootB_00c0edf0(&m_node), m_node(1) { } ~CContainerB_00c0edf0(); };   // vtable 0x012B2350, +0x50; dtor 0x00C0F580
struct CContainerC_00c0edf0 : CRootC_00c0edf0 { CSmallNode_00c0edf0 m_node; CContainerC_00c0edf0() : CRootC_00c0edf0(&m_node), m_node(1) { } ~CContainerC_00c0edf0(); };   // vtable 0x012B23A0, +0x60; dtor 0x00C0F780
struct CContainerD_00c0edf0 : CRootD_00c0edf0 { CNode_00c0edf0 m_node; CContainerD_00c0edf0() : CRootD_00c0edf0(&m_node), m_node(0) { } ~CContainerD_00c0edf0(); };                  // vtable 0x012B23F0, +0x74; dtor 0x00C0F980

struct CCombatAbilityStrafeDef_00c0edf0 : CDefBase_00c0edf0 {
    long m_4;
    COwner_00c0edf0* m_pOwner;             // +8
    CNavButton_00c0edf0 m_button;          // +0xc
    CBuffer_00c0edf0 m_buffer;             // +0x2c
    CContainerA_00c0edf0 m_38;
    long m_48;
    long m_4c;
    CContainerB_00c0edf0 m_50;
    CContainerC_00c0edf0 m_60;
    long m_70;
    CContainerD_00c0edf0 m_74;
    CCombatAbilityStrafeDef_00c0edf0(COwner_00c0edf0* owner);
};

CCombatAbilityStrafeDef_00c0edf0::CCombatAbilityStrafeDef_00c0edf0(COwner_00c0edf0* owner)
    : m_4(0), m_pOwner(owner), m_button(), m_buffer(), m_38(), m_48(0), m_4c(1), m_50(), m_60(), m_70(-1), m_74()
{
    m_pOwner->m_registry.Add(this);
}