// CSoundManagerLH::ExtractMetaData() -- extract the meta data of a bank source through the sound system
// Name (agentSND, 2026-11-10): vtable slot 51 (+0xCC); spelling from the Fable Anniversary 360 PDB publics (CSoundManagerUN / CASoundManager at the same vtable position) with the TLC slot evidence in SLOTS.tsv; was CNamedOwner_00a3e800::SetNamedValue.
// Leaf: ExtractMetaData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3E800, length 37 bytes.
// Callees: CCharString::operator const char* @ 0x0099AF10 (on the first argument), the object's virtual slot 1 (+4). Shape: the object's vtable is loaded into
// edi, the value pushed, then the string converted (its address re-read from the stack) and pushed; the object pointer is re-read from +0x40 for `this`.
// The chunk label "CEnginePrimitiveRenderer::ClearAllChildPrimitives" is propagated (the 0x00A3xxxx module is game code); the class is named by address.
namespace N_0099af10 { struct CCharString {
    void* m_pData;
    operator const char*() const;                  // 0x0099AF10
}; } using N_0099af10::CCharString;

struct CSoundSystemLH {
    virtual void v00();
    virtual void ExtractMetaData(const char* name, long value);   // slot 1 (+0x04)
};

struct CSoundManagerLH {
    unsigned char          _pad[0x40];
    CSoundSystemLH* m_pObject;              // +0x40
    void ExtractMetaData(const CCharString& name, long value);
};

void CSoundManagerLH::ExtractMetaData(const CCharString& name, long value)
{
    if (m_pObject)
        m_pObject->ExtractMetaData(name, value);
}