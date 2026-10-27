// CDataOutputStream::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor (the member, then the base's inline destructor and the next base's), then free this when bit 0 of flags is set
// Leaf: ??_GCDataOutputStream@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00996F00, 46 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): its only reference is vtable 0x0129A6E0 (RTTI .?AVCDataOutputStream@@, bases CDataStream : CBaseClassNonCopyable : CBaseClass) slot 0; Ego_r.pdb ??_GCDataOutputStream@@UAEPAXI@Z; member NewLine (CCharString) +4.
// Callees: CCharString::~CCharString 0x0099EAE0 (NewLine), the CDataStream vptr (0x0129A69C), CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300, operator delete 0x00BFE9BC.
// Shape (family K050, representative 0x00996F00): an implicit destructor stores no vptr of its own; the base's user-declared
//   inline destructor does (the vptr store sits between the member destructor and the base call). A `new` of the class
//   makes VC7.1 emit the vtable and the ??_G COMDAT.
class CBaseClass {
public:
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                             // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                    // 0x0099A300
};
class CCharString {
public:
    CCharString();                                       // 0x0099E4B0
    ~CCharString();                                      // 0x0099EAE0
    void* PStringData;
};
class CDataStream : public CBaseClassNonCopyable {       // RTTI: CBaseClassNonCopyable : CBaseClass; vtable 0x0129A69C
public:
    CDataStream() { }
    virtual ~CDataStream() { }                           // user-declared (Ego_r.pdb ??1CDataStream@@UAE@XZ): stores its vptr
};
class CDataOutputStream : public CDataStream {           // Ego_r.pdb 8 bytes; its destructor is implicit
public:
    CCharString NewLine;                                 // +0x04
};
CDataOutputStream* MakeStream_00996f00() { return new CDataOutputStream; }