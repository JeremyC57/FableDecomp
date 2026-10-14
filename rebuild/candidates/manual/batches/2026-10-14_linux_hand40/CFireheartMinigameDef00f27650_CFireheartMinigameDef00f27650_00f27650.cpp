// CFireheartMinigameDef_00f27650::CFireheartMinigameDef_00f27650() -- the CPSCBase base constructor, this class's vtable, then the two CCharString members at +0x28 and +0x2c default-constructed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F27650, length 48 bytes.
// Callees: CPSCBase::CPSCBase @ 0x0042D9E0, CCharString::CCharString @ 0x0099E4B0 twice (+0x28, +0x2c). Vtable 0x012FB500 = (not in rtti-vtables.tsv; the chunk label "CFireheartMinigameDef" names the class; a second build of the def module). Shape: the first member address formed before the vtable store; `this` is returned.
struct CCharString {
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
};
struct CPSCBase {
    virtual void v00();
    unsigned char _pad[0x24];                      // +0x04 .. +0x28
    CPSCBase();                                    // 0x0042D9E0
};

struct CFireheartMinigameDef_00f27650 : CPSCBase {
    CCharString            m_name;                        // +0x28
    CCharString            m_text;                        // +0x2c
    virtual void v00();
    CFireheartMinigameDef_00f27650();
};

CFireheartMinigameDef_00f27650::CFireheartMinigameDef_00f27650()
    : CPSCBase()
{
}
