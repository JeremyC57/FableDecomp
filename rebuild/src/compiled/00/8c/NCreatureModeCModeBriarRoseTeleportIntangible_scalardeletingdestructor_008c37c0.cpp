// NCreatureMode::CModeBriarRoseTeleportIntangible::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CModeBriarRoseTeleportIntangible inlined (destroying PTarget, Filter, then ~CModeStandBase), and operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCModeBriarRoseTeleportIntangible@NCreatureMode@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C37C0, 48 bytes, thiscall, `ret 4`; reached only through vtable slot 0.
// Identity: NCreatureMode::CModeBriarRoseTeleportIntangible -- proven; evidence: vtable 0x0128026C slot 0 (RTTI .?AVCModeBriarRoseTeleportIntangible@NCreatureMode@@ : CModeStandBase); the destroyed members are Ego_r.pdb PTarget (CIntelligentPointer<CThingCreatureBase>, +0x78) and Filter (CAnimFilterOfGroup, +0x68: 0x00739DB0 destroys its Group string and stores the CAnimFilterBase vtable 0x0122FFCC -- the implicit ~CAnimFilterOfGroup, kept out of line in retail, so it is declared here); 0x0073D860 is ~CModeStandBase (PDB name set ??1CModeStandBase@@UAE@XZ).
// Label: the manifest's "`vector_deleting_destructor'" is propagated across byte-identical bodies.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10, CAnimFilterOfGroup::~CAnimFilterOfGroup 0x00739DB0, CModeStandBase::~CModeStandBase 0x0073D860, operator delete 0x00BFE9BC.
// Shape: no vtable store -- VC7.1 resets the vptr only in user-written destructors, and every destructor between this class
//   and CModeStandBase is implicit (CModeStandBase's own one stores its vtable); member destructors run in reverse declaration order.
//   The deleting destructor is emitted with the vtable, i.e. in a TU that constructs the class: MakeOne is that
//   construction, nothing more.
// Model: local -- no engine header for this class; the bases are sized to the retail layout.
class CThingCreatureBase;
#pragma pack(push, 1)
class CBaseIntelligentPointer {                   // 8 bytes, 1-aligned in retail (Coin::Thing +0x25, ShowTrophy::PEmitter +0xB5)
public:
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();           // 0x00A01C10
    void* PItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
#pragma pack(pop)
class CAnimFilterBase {                           // vtable 0x0122FFCC
public:
    CAnimFilterBase();
    virtual ~CAnimFilterBase();
};
class CAnimFilterOfGroup : public CAnimFilterBase {   // Ego_r.pdb 12 bytes: Group (CCharString) +4, GroupCRC +8
public:
    CAnimFilterOfGroup();
    virtual ~CAnimFilterOfGroup();                // 0x00739DB0
    unsigned char _data[8];
};
class CModeStandBase {                            // RTTI .?AVCModeStandBase@@ : CTCCreatureModeManager::CModeBase; Ego_r.pdb 104 bytes
public:
    CModeStandBase();
    virtual ~CModeStandBase();                    // 0x0073D860
    unsigned char _base[0x64];
};
namespace NCreatureMode {
class CModeBriarRoseTeleportIntangible : public CModeStandBase {
public:
    CAnimFilterOfGroup Filter;                    // +0x68
    long Timer;                                   // +0x74
    CIntelligentPointer<CThingCreatureBase> PTarget;   // +0x78
};
}
void* MakeOne()
{
    return new NCreatureMode::CModeBriarRoseTeleportIntangible;
}