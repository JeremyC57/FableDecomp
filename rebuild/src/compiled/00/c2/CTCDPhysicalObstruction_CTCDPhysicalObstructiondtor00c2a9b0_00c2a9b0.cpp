// CTCDPhysicalObstruction::~CTCDPhysicalObstruction() -- EH-framed destructor: four inline container members destroyed in reverse order (+0x98 vtable 0x012B42F8, +0x68 0x012B42B0, +0x38 0x012B4268, +8 0x012B4460), each storing its own vtable, destroying its node at +8 out of line (@ 0x00C19250) and ending on the root vtable 0x012B1AB0; the class stores no vtable of its own
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A9B0, oracle length 191 bytes. Built under /EHsc.
// Retail bytes:  ... mov [esp+8],esi ; mov dword ptr [esp+18h],0 ; lea edi,[esi+98h] ; mov [esp+0Ch],edi ; mov dword ptr [edi],012B42F8h ; lea ecx,[edi+8] ; mov byte ptr [esp+18h],3 ; call ~CNode ; mov dword ptr [edi],012B1AB0h ; lea edi,[esi+68h] ; mov [esp+0Ch],edi ; mov dword ptr [edi],012B42B0h ; lea ecx,[edi+8] ; mov byte ptr [esp+18h],4 ; call ~CNode ; mov dword ptr [edi],012B1AB0h ; lea edi,[esi+38h] ; mov [esp+0Ch],edi ; mov dword ptr [edi],012B4268h ; lea ecx,[edi+8] ; mov byte ptr [esp+18h],5 ; call ~CNode ; add esi,8 ; mov dword ptr [edi],012B1AB0h ; mov [esp+0Ch],esi ; mov dword ptr [esi],012B4460h ; lea ecx,[esi+8] ; mov dword ptr [esp+18h],6 ; call ~CNode ; mov ecx,[esp+10h] ; mov dword ptr [esi],012B1AB0h ; ...
// The root/node container pattern of the 0x012B1AB0 family (the root owns the vptr and the node
// pointer, the derived container embeds the node). Named from the manifest (PDB class).
struct CNode_00c2a9b0 {
    unsigned char _body[0x28];
    ~CNode_00c2a9b0();                     // 0x00C19250
};

struct CRoot_00c2a9b0 {                    // vtable 0x012B1AB0
    virtual void v00();
    CNode_00c2a9b0* m_pNode;               // +4
    ~CRoot_00c2a9b0() { }
};

struct CContainerA_00c2a9b0 : CRoot_00c2a9b0 { CNode_00c2a9b0 m_node; ~CContainerA_00c2a9b0() { } };   // vtable 0x012B4460, +8
struct CContainerB_00c2a9b0 : CRoot_00c2a9b0 { CNode_00c2a9b0 m_node; ~CContainerB_00c2a9b0() { } };   // vtable 0x012B4268, +0x38
struct CContainerC_00c2a9b0 : CRoot_00c2a9b0 { CNode_00c2a9b0 m_node; ~CContainerC_00c2a9b0() { } };   // vtable 0x012B42B0, +0x68
struct CContainerD_00c2a9b0 : CRoot_00c2a9b0 { CNode_00c2a9b0 m_node; ~CContainerD_00c2a9b0() { } };   // vtable 0x012B42F8, +0x98

struct CTCDPhysicalObstruction_00c2a9b0 {
    long m_0;
    long m_4;
    CContainerA_00c2a9b0 m_8;              // +8
    CContainerB_00c2a9b0 m_38;             // +0x38
    CContainerC_00c2a9b0 m_68;             // +0x68
    CContainerD_00c2a9b0 m_98;             // +0x98
    ~CTCDPhysicalObstruction_00c2a9b0();
};

CTCDPhysicalObstruction_00c2a9b0::~CTCDPhysicalObstruction_00c2a9b0()
{
}