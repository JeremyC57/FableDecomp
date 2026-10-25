// CBaseObject::CBaseObject() -- the protected default constructor of the intelligent-pointer target base: install the vtable (0x0129A7D8) and clear the IntelligentPointer back-link
// Leaf: ??0CBaseObject@@IAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A380, 16 bytes, thiscall, returns this.
// Identity: the only vtable stored is 0x0129A7D8, whose RTTI type descriptor is .?AVCBaseObject@@ (rtti-vtables.tsv
//   CBaseObject slot 0 = 0x0099A4D0); Ego_r.pdb CBaseObject is 8 bytes {vptr, CBaseObjectPointer* IntelligentPointer +4}
//   and both PDBs list the protected default constructor ??0CBaseObject@@IAE@XZ (and the protected virtual
//   destructor ??1CBaseObject@@MAE@XZ); its 941 call sites are the base-constructor calls of CBaseObject-derived classes
//   (CCreatureActionBase, CVillageTask*, C3DAnimationUtil, ...). Was catalogued CBaseIntelligentPointer::CBaseIntelligentPointer
//   (a generated tiny-body label; Ego_r CBaseIntelligentPointer is a different class).
// Shape: `mov eax,ecx ; mov [eax],vtable ; mov [eax+4],0 ; ret` -- the member initialiser after the vptr store.
class CBaseObjectPointer;

class CBaseObject {
protected:
    CBaseObject();
    virtual ~CBaseObject();
public:
    CBaseObjectPointer* IntelligentPointer;     // +0x04
};

CBaseObject::CBaseObject()
    : IntelligentPointer(0)
{
}
