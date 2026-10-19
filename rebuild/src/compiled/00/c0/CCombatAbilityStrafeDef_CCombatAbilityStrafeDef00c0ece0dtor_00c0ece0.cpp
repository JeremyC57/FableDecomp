// CCombatAbilityStrafeDef_00c0ece0::~CCombatAbilityStrafeDef() -- EH-framed destructor matching the landed constructor 0x00C0EDF0 (the manifest's "CEngineVSConstantLayoutLights" is propagated; vtable 0x012B25A8, state 6): four teardown calls on the definition (@ 0x00C0EB40 / 0x00C0EAF0 / 0x00C0EC60 / 0x00C0EA10), the owner's +4 registry drops it (slot 8), then the members go in reverse with the container destructors inlined (+0x74 node @ 0x00C19250, +0x60 / +0x50 / +0x38 nodes @ 0x00C17B10; each container pointer homed in the frame, states 7-10, root vtable 0x012B1AB0 restored), the buffer at +0x2c freed, the `CNavButton` at +0xc destroyed (@ 0x00C10320, state 0) and the base vtable 0x012B221C restored
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0ECE0, oracle length 271 bytes. Built under /EHsc.
// Retail bytes:  ... mov [esp+8],esi ; mov dword ptr [esi],12B25A8h ; mov dword ptr [esp+18h],6 ; call Stop1 ; mov ecx,esi ; call Stop2 ; mov ecx,esi ; call Stop3 ; mov ecx,esi ; call Stop4 ; mov eax,[esi+8] ; lea ecx,[eax+4] ; mov eax,[ecx] ; push esi ; call [eax+20h] ; lea edi,[esi+74h] ; mov [esp+0Ch],edi ; mov dword ptr [edi],12B23F0h ; lea ecx,[edi+8] ; mov byte ptr [esp+18h],7 ; call ~CNode ; mov dword ptr [edi],12B1AB0h ; ... (+60h / 8, +50h / 9, +38h / 0Ah with ~CSmallNode) ... mov eax,[esi+2Ch] ; test eax,eax ; je a ; push eax ; call free ; add esp,4 ; mov dword ptr [esi+2Ch],0 ; a: lea ecx,[esi+0Ch] ; mov byte ptr [esp+18h],0 ; call ~CNavButton ; ... mov dword ptr [esi],12B221Ch ; ... ret
// The classes are named by address, matching the constructor's model.
void __cdecl operator delete[](void* block) throw();   // MSVCR71 import thunk 0x00BFEB1C (the lane <new> lacks it; the CRT one does not throw)

struct CCombatAbilityStrafeDef_00c0ece0;
struct CRegistry_00c0ece0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CCombatAbilityStrafeDef_00c0ece0* def);   // slot 8
    virtual void Add(CCombatAbilityStrafeDef_00c0ece0* def);      // slot 9
};
struct COwner_00c0ece0 {
    unsigned char _pad[4];
    CRegistry_00c0ece0 m_registry;         // +4
};

struct CDefBase_00c0ece0 {                 // polymorphic base: vtable 0x012B221C
    virtual void v00();
    ~CDefBase_00c0ece0() { }
};

struct CNavButton_00c0ece0 {               // 0x20 bytes
    unsigned char _body[0x20];
    ~CNavButton_00c0ece0();                // 0x00C10320
};

struct CBuffer_00c0ece0 {                  // three words
    void* m_pData;
    long m_a;
    long m_b;
    ~CBuffer_00c0ece0() { if (m_pData) { operator delete[](m_pData); m_pData = 0; } }
};

struct CSmallNode_00c0ece0 { unsigned char _body[8]; ~CSmallNode_00c0ece0(); };   // 0x00C17B10
struct CNode_00c0ece0 { unsigned char _body[0x28]; ~CNode_00c0ece0(); };          // 0x00C19250

struct CRootA_00c0ece0 { virtual void v00(); CSmallNode_00c0ece0* m_pNode; ~CRootA_00c0ece0() { } };   // vtable 0x012B1AB0
struct CRootB_00c0ece0 { virtual void v00(); CSmallNode_00c0ece0* m_pNode; ~CRootB_00c0ece0() { } };
struct CRootC_00c0ece0 { virtual void v00(); CSmallNode_00c0ece0* m_pNode; ~CRootC_00c0ece0() { } };
struct CRootD_00c0ece0 { virtual void v00(); CNode_00c0ece0* m_pNode; ~CRootD_00c0ece0() { } };

struct CContainerA_00c0ece0 : CRootA_00c0ece0 { CSmallNode_00c0ece0 m_node; ~CContainerA_00c0ece0() { } };   // vtable 0x012B2560, +0x38
struct CContainerB_00c0ece0 : CRootB_00c0ece0 { CSmallNode_00c0ece0 m_node; ~CContainerB_00c0ece0() { } };   // vtable 0x012B2350, +0x50
struct CContainerC_00c0ece0 : CRootC_00c0ece0 { CSmallNode_00c0ece0 m_node; ~CContainerC_00c0ece0() { } };   // vtable 0x012B23A0, +0x60
struct CContainerD_00c0ece0 : CRootD_00c0ece0 { CNode_00c0ece0 m_node; ~CContainerD_00c0ece0() { } };        // vtable 0x012B23F0, +0x74

struct CCombatAbilityStrafeDef_00c0ece0 : CDefBase_00c0ece0 {
    long m_4;
    COwner_00c0ece0* m_pOwner;             // +8
    CNavButton_00c0ece0 m_button;          // +0xc
    CBuffer_00c0ece0 m_buffer;             // +0x2c
    CContainerA_00c0ece0 m_38;
    long m_48;
    long m_4c;
    CContainerB_00c0ece0 m_50;
    CContainerC_00c0ece0 m_60;
    long m_70;
    CContainerD_00c0ece0 m_74;
    void Stop1();                          // 0x00C0EB40
    void Stop2();                          // 0x00C0EAF0
    void Stop3();                          // 0x00C0EC60
    void Stop4();                          // 0x00C0EA10
    ~CCombatAbilityStrafeDef_00c0ece0();
};

CCombatAbilityStrafeDef_00c0ece0::~CCombatAbilityStrafeDef_00c0ece0()
{
    Stop1();
    Stop2();
    Stop3();
    Stop4();
    m_pOwner->m_registry.Remove(this);
}