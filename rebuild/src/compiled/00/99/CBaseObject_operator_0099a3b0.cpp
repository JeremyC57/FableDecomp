// CBaseObject::operator=(const CBaseObject&) -- assignment keeps this object's own intelligent-pointer record: returns *this
// Leaf: ??4CBaseObject@@IAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A3B0: `mov eax,ecx ; ret 4`.
// Identity: Ego_r.pdb (0x00B02000) and FableWin.pdb name the protected ??4CBaseObject@@IAEAAV0@ABV0@@Z after the two
// constructors, as here (0x0099A380, 0x0099A390, 0x0099A3B0); 0x00431F10-style def copies call it as the base
// operator=. The row was a placeholder (`C0099a3b0::M`, Leaf ??0CFourierAnalysis@) under a propagated label.
class CBaseObjectPointer;

class CBaseObject {                                    // vtable 0x0129A7D8
protected:
    CBaseObject& operator=(const CBaseObject& other);  // 0x0099A3B0
    virtual ~CBaseObject();                            // 0x0099A430
public:
    CBaseObjectPointer* IntelligentPointer;            // +0x04
};

CBaseObject& CBaseObject::operator=(const CBaseObject& other)
{
    return *this;
}
