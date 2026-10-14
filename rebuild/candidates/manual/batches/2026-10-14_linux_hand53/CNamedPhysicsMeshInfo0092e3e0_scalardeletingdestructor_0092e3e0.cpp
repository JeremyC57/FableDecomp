// CNamedPhysicsMeshInfo_0092e3e0::`scalar deleting destructor'(unsigned int flags) -- destroy the two CCharStrings at +0xb4 / +0xb0 (0x0099EAE0) and the intelligent pointer at +0xa8 (0x00A01C10), then the CPhysicsMeshInfo base destructor (0x00693EF0) and free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092E3E0, length 65 bytes.
// Leaf: ??_GCNamedPhysicsMeshInfo_0092e3e0@@QAEPAXI@Z
// The chunk label is propagated; named by address. Shape: no vtable store (the derived vtable folds into the base's; the base destructor is out of line), `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CCharString {
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
struct CBaseIntelligentPointer {                   // 8 bytes
    virtual void v00();
    void* m_pItem;
    ~CBaseIntelligentPointer();                    // 0x00A01C10
};
struct CPhysicsMeshInfo {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CPhysicsMeshInfo();                           // 0x00693EF0
};
struct __declspec(novtable) CNamedPhysicsMeshInfo_0092e3e0 : CPhysicsMeshInfo {
    CBaseIntelligentPointer m_pointera8;           // +0xa8
    CCharString             m_b0;                  // +0xb0
    CCharString             m_b4;                  // +0xb4
    ~CNamedPhysicsMeshInfo_0092e3e0() { }
};

void Kill_0092e3e0(CNamedPhysicsMeshInfo_0092e3e0* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}
