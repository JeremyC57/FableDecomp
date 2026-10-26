// CVtable_012b4f84::Close() -- when the object is live, the base shutdown runs, then the +0x30 member is released if the master object's +0x1d4 interface exists
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3C720, length 45 bytes.
// Leaf: ?Close@CVtable_012b4f84
// Retail bytes: push esi ; mov esi,ecx ; mov eax,[esi+2Ch] ; test eax,eax ; je E ; call base Close ; mov eax,[g_pMaster] ; test eax,eax ; je E ;
//               mov ecx,[eax+1D4h] ; test ecx,ecx ; je E ; lea ecx,[esi+30h] ; pop esi ; jmp Release ; E: pop esi ; ret
// Callees: the base class's Close @ 0x00C3C450 (slot 1 of vtable 0x012B4F74: frees the +0x28 block when flag 2 of +0x2c is
// set, notifies the master's +0x1d4 sink, clears +0x2c), the +0x30 member's release @ 0x00C4D9B0 (tail jump). Global: the
// module's master object @ 0x0143BC2C (its +0x1d4 interface pointer, as in the landed CreateMasterObject_00c4d340).
// The manifest label "CScriptDefinitionManager::CleanupAudioAndRender" is propagated: this is slot 1 of vtable 0x012B4F84
// (no-RTTI 0x00C3xxxx module), the override of 0x00C3C450 in the derived class whose destructor 0x00C331A0 calls it before
// the base destructor 0x00C33120; the classes are named by their vtables.
// Shape: the explicit base-class call is direct (qualified), the release is a sibling-call `jmp` with ecx = &member.
struct CMaster_0143bc2c {
    unsigned char _pad[0x1d4];
    void*         m_pSink;                     // +0x1d4
};
extern CMaster_0143bc2c* g_pMaster_0143bc2c;   // 0x0143BC2C

struct CResourceLink_00c4d9b0 {
    void*         m_pHandle;
    void Release();                            // 0x00C4D9B0
};

struct CVtable_012b4f74 {                      // the base class (vtable 0x012B4F74)
    virtual void v00();                        // slot 0: scalar deleting destructor 0x00C33170
    virtual void Close();                   // slot 1: 0x00C3C450
    unsigned char _pad04[0x28 - 0x04];         // +0x04
    void*         m_pBlock;                    // +0x28 freed by the base Close when flag 2 is set
    unsigned long m_flags;                     // +0x2c non-zero while live
};

struct CVtable_012b4f84 : CVtable_012b4f74 {   // vtable 0x012B4F84
    CResourceLink_00c4d9b0 m_link;             // +0x30
    virtual void Close();                   // slot 1: 0x00C3C720
};

void CVtable_012b4f84::Close()
{
    if (m_flags) {
        CVtable_012b4f74::Close();
        if (g_pMaster_0143bc2c && g_pMaster_0143bc2c->m_pSink)
            m_link.Release();
    }
}