// CAppearanceOwner_00ca4990::Rebuild() -- rebuild the appearance at +0x50 when present, then this object's virtual slot 10 with 1; returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA4990, length 28 bytes.
// Callees: the appearance's rebuild @ 0x00CADDC0 (on the object at +0x50; the manifest label is a propagated constructor symbol), virtual slot 10 (+0x28) of `this`.
// Shape: `test ecx,ecx ; je` around the first call, the vtable read after it, `xor eax,eax` result. The chunk label "CEngineResourceManager::WorldFrameUpdate"
// is propagated (the 0x00CAxxxx module); the class is named by address.
struct CAppearance_00caddc0 {
    void Rebuild();                                // 0x00CADDC0
};

struct CAppearanceOwner_00ca4990 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Invalidate(long flags);           // slot 10 (+0x28)
    unsigned char          _pad[0x4c];             // +0x04 .. +0x50
    CAppearance_00caddc0*  m_pAppearance;          // +0x50
    long Rebuild();
};

long CAppearanceOwner_00ca4990::Rebuild()
{
    if (m_pAppearance)
        m_pAppearance->Rebuild();
    Invalidate(1);
    return 0;
}
