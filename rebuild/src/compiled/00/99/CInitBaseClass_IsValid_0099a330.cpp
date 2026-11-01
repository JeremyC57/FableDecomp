// CInitBaseClass::IsValid() const -- the initialised flag (Valid, +0x04)
// Leaf: ?IsValid@CInitBaseClass@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A330, 4 bytes: mov al,[ecx+4] ; ret.
// Identity (proven, chunk c671): called on GFGetSystemManager() (CSystemManager : CInitBaseClass, RTTI) in ~CStaticFontBank;
//   CInitBaseClass's only bool getter in Ego_r / FableWin (IsValid QBE_NXZ).
// Layout: Ego_r.pdb CInitBaseClass (vptr, bool Valid +0x04; 8 bytes).
// Naming wave 16: landed as the descriptive C0099a330::M() (unsigned char result; catalog CTCInventoryBase::
//   GetCanItemBeQuickAssigned); respelled on the PDB member (name-only: the bytes are unchanged).
class CInitBaseClass {
public:
    virtual ~CInitBaseClass();                          // 0x0099A410
    bool IsValid() const;
    bool Valid;                                         // +0x04
};

bool CInitBaseClass::IsValid() const
{
    return Valid;
}
