// CAnimFilterOf2Groups::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CAnimFilterOf2Groups inlined (Group2 then Group1, then CAnimFilterBase's inline destructor) and operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCAnimFilterOf2Groups@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073EA40, 47 bytes (+ int3 padding), thiscall, `ret 4`; reached only through vtable slot 2.
// Identity (proven): CAnimFilterOf2Groups::`scalar deleting destructor'; evidence: slot 2 of vtable 0x01267578 (RTTI
//   .?AVCAnimFilterOf2Groups@@ : CAnimFilterBase); CAnimFilterBase's vtable 0x0122FFCC has two pure slots (FableWin.pdb
//   ??RCAnimFilterNoGroup@@UBE_NABVCAnimationEntry@@@Z / ?Clone@...@@UBEPAVCAnimFilterBase@@XZ overrides) and its destructor
//   in slot 2, as CAnimFilterOfGroup / AnyGroup's ??_G rows in Ego_r.pdb; Ego_r.pdb CAnimFilterOf2Groups = vptr, CCharString
//   Group1 +0x04, Group2 +0x08, Group1CRC +0x0C, Group2CRC +0x10.
// Label: the manifest's "`scalar_deleting_destructor'" is propagated (family J109).
// Callees: CCharString::~CCharString() 0x0099EAE0 (twice; ??1CCharString@@QAE@XZ), operator delete 0x00BFE9BC.
// Shape: no vtable store for CAnimFilterOf2Groups (implicit destructor); Group2 and Group1 are destroyed in reverse order,
//   then CAnimFilterBase's user-written inline destructor stores 0x0122FFCC. MakeOne is the construction that emits the
//   vtable (a modelling device).
// Model: local (no engine headers for the filter classes).
class CCharStringData;
class CCharString {                                         // 4 bytes
public:
    CCharString();
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;                           // +0x00
};
class CAnimationEntry;
class CAnimFilterBase {                                     // vtable 0x0122FFCC
public:
    virtual bool operator()(const CAnimationEntry& entry) const = 0;   // slot 0
    virtual CAnimFilterBase* Clone() const = 0;             // slot 1
    virtual ~CAnimFilterBase() { }                          // slot 2
};
class CAnimFilterOf2Groups : public CAnimFilterBase {      // Ego_r.pdb 0x14, vtable 0x01267578
public:
    virtual bool operator()(const CAnimationEntry& entry) const;   // 0x0073EA00
    virtual CAnimFilterBase* Clone() const;                 // 0x0073EA70
    CCharString   Group1;                                   // +0x04
    CCharString   Group2;                                   // +0x08
    unsigned long Group1CRC;                                // +0x0C
    unsigned long Group2CRC;                                // +0x10
};
void* MakeOne()
{
    return new CAnimFilterOf2Groups;
}