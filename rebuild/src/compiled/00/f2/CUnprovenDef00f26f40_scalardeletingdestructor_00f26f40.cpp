// CUnprovenDef_00f26f40::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor (the member, then the base's inline destructor and the next base's), then free this when bit 0 of flags is set
// Leaf: ??_GCUnprovenDef_00f26f40@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F26F40, 46 bytes, thiscall, `ret 4`, returns this.
// Identity (descriptive): no reference reaches it (an out-of-line COMDAT); it destroys a 12-byte container at +0x2C through 0x00F26F70 (landed as a CRetailVector stand-in over 0x38-byte polymorphic elements) and runs CDefPointeeBase's inline destructor (vptr 0x01230BA0) and ~CResource 0x009FC550: a def class of the def-system TU around 0x00F26F40, unnamed by the PDB name sets.
// Callees: 0x00F26F70 (the member's destructor), the CDefPointeeBase vptr (0x01230BA0), CResource::~CResource 0x009FC550, operator delete 0x00BFE9BC.
// Shape (family K050, representative 0x00996F00): an implicit destructor stores no vptr of its own; the base's user-declared
//   inline destructor does (the vptr store sits between the member destructor and the base call). A `new` of the class
//   makes VC7.1 emit the vtable and the ??_G COMDAT.
class CResource {                                        // RTTI base of CDefPointeeBase
public:
    CResource();
    virtual ~CResource();                                // 0x009FC550
    unsigned char _resource[8];                          // +0x04 (not touched here)
};
class CDefPointeeBase : public CResource {               // vtable 0x01230BA0
public:
    CDefPointeeBase() { }
    virtual ~CDefPointeeBase() { }                       // user-declared (Ego_r.pdb ??1CDefPointeeBase@@UAE@XZ): stores its vptr
};
class CUnprovenMember_00f26f40 {                            // descriptive: the 12-byte container destroyed by 0x00F26F70
public:
    CUnprovenMember_00f26f40();
    ~CUnprovenMember_00f26f40();                            // 0x00F26F70 (destroys its 0x38-byte polymorphic elements)
    unsigned char _body[12];
};
class CUnprovenDef_00f26f40 : public CDefPointeeBase {      // descriptive: the def class owning the member at +0x2C
public:
    unsigned char _pad_0x0c[0x2c - 0x0c];
    CUnprovenMember_00f26f40 Member;                        // +0x2C
};
CUnprovenDef_00f26f40* MakeDef_00f26f40() { return new CUnprovenDef_00f26f40; }