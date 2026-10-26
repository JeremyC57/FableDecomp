// CBaseObject::Validate() const -- the release build's validation hook: nothing (`ret`)
// Leaf: ?Validate@CBaseObject@@UBEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A400, 1 byte (`ret`).
// Identity: slot 1 of the CBaseObject vtable 0x0129A7D8 (rtti-vtables.tsv: 0x0099A4D0, 0x0099A400, ...); Ego_r.pdb and
//   FableWin.pdb ?Validate@CBaseObject@@UBEXXZ (public virtual const; InternalValidate is the protected worker).
class CBaseObjectPointer;

class CBaseObject {                                    // vtable 0x0129A7D8
public:
    virtual void Validate() const;                     // 0x0099A400
protected:
    virtual ~CBaseObject();                            // 0x0099A430
public:
    CBaseObjectPointer* IntelligentPointer;            // +0x04
};

void CBaseObject::Validate() const
{
}
