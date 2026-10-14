// CShaderResourceList_00c3b670::CShaderResourceList_00c3b670(const CShaderResourceList_00c3b670&) --
// copy constructor: the 4-byte shader-resource base is copied out of line, the two members are
// reset to zero (the copy does not share the source's buffer).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3B670, length 25 bytes (+ int3 padding).
// Callee: CShaderResourceBase_00c3b710::CShaderResourceBase_00c3b710(const&) @ 0x00C0DAB0 (the same
// base copy the landed CShaderResourceArray copy constructor @ 0x00C3B710 calls first). Unlike that
// one this body carries no EH frame, so the base is modelled without a destructor here.
// The manifest label "CControllerDisconnect::CControllerDisconnect" is propagated and rejected
// (the NUISystem::CControllerDisconnect constructors are 0x0041CBE4-shaped). Class named by shape.
struct CShaderResourceBase_00c3b710 {          // 4 bytes
    unsigned char _body[4];
    CShaderResourceBase_00c3b710(const CShaderResourceBase_00c3b710& other);   // 0x00C0DAB0
};

struct CShaderResourceList_00c3b670 : CShaderResourceBase_00c3b710 {
    void*         m_pItems;                    // +0x04 (not shared by the copy)
    unsigned long m_count;                     // +0x08
    CShaderResourceList_00c3b670(const CShaderResourceList_00c3b670& other);
};

CShaderResourceList_00c3b670::CShaderResourceList_00c3b670(const CShaderResourceList_00c3b670& other)
    : CShaderResourceBase_00c3b710(other), m_pItems(0), m_count(0)
{
}