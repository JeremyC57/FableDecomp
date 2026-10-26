// CCompositeSurfaceInfo::CCompositeSurfaceInfo() -- CBase(), this vtable, then the Cache counted pointer
// zeroed by its inline constructor.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BED440, length 26 bytes.
// Callees: CBase::CBase @ 0x0099A2D0. Vtable 0x012A8154 = CCompositeSurfaceInfo (rtti-vtables.tsv); the
// manifest label "CCarriedReadableDef" is propagated and rejected.
// PDB CCompositeSurfaceInfo (sizeof 0xC): +0x04 CCountedPointer<CCompositeSurfaceCache> Cache. The vtable
// store precedes the two zero stores because the pointer is an embedded member with an inline constructor.
namespace N_0099a2d0 { struct CBase {
    virtual void v00();
    CBase();                                     // 0x0099A2D0
}; } using N_0099a2d0::CBase;

struct CCountedPointer {
    void* m_pObject;
    void* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
};

class CCompositeSurfaceInfo : public CBase { public:
    virtual void v00();                          // own vtable (0x012A8154)
    CCountedPointer Cache;                       // +0x04
    CCompositeSurfaceInfo();
};

CCompositeSurfaceInfo::CCompositeSurfaceInfo()
    : CBase(), Cache()
{
}