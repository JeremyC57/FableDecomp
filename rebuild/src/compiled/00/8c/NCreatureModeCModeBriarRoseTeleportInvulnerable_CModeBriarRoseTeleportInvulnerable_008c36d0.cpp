// NCreatureMode::CModeBriarRoseTeleportInvulnerable::~CModeBriarRoseTeleportInvulnerable() -- the compiler-generated destructor: PEffect, PTarget, then the Filter (its Group string and the CAnimFilterBase vtable reset), then ~CModeStandBase
// Leaf: ??1CModeBriarRoseTeleportInvulnerable@NCreatureMode@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C36D0, 45 bytes, thiscall, tail `jmp ~CModeStandBase`.
// Identity (proven): the sole caller is the slot-0 `??_G` 0x008C36B0 of vtable 0x01280228 (RTTI
//   .?AVCModeBriarRoseTeleportInvulnerable@NCreatureMode@@ : CModeStandBase); the destroyed members are Ego_r.pdb's PEffect
//   (CIntelligentPointer<CThing>, +0x80), PTarget (CIntelligentPointer<CThingCreatureBase>, +0x78) and Filter
//   (CAnimFilterOfGroup, +0x68: Group CCharString +0x6C, then the CAnimFilterBase vtable 0x0122FFCC); FableWin.pdb spells the destructor
//   ??1CModeBriarRoseTeleportInvulnerable@NCreatureMode@@UAE@XZ (Ego_r lists only the ??_G); it stores no vtable of its own, so it is the implicit one. The catalog's
//   "_Dest_val<allocator<CMacro>,CMacro>" is propagated. Masked byte twin of 0x00740540 (CModeJackOfBladesScream), which
//   destroys +0x9C / +0x74 instead: two classes, two symbols.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (twice), CCharString::~CCharString 0x0099EAE0,
//   CModeStandBase::~CModeStandBase 0x0073D860 (PDB ??1CModeStandBase@@UAE@XZ).
// Shape: members destroyed in reverse declaration order; the implicit ~CAnimFilterOfGroup and the inline
//   `virtual ~CAnimFilterBase() {}` expand here (in the deleting destructor of the sibling CModeBriarRoseTeleportIntangible
//   0x008C37C0, one inline level deeper, ~CAnimFilterOfGroup stays the out-of-line 0x00739DB0). DestroyOne (a non-virtual
//   destruction by name) makes VC7.1 emit this out-of-line `??1`, nothing more.
// Layout: local -- engine/CModeStandBase.h flattens the vptr into a data member; the base is sized to Ego_r's 0x68.
class CThing;
class CThingCreatureBase;
class CCharStringData;

class CCharString {
public:
    CCharStringData* PStringData;
    ~CCharString();                               // 0x0099EAE0
};

#pragma pack(push, 1)
class CBaseIntelligentPointer {                   // 8 bytes, 1-aligned in retail (as in the landed sibling 0x008C37C0)
public:
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();           // 0x00A01C10
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
#pragma pack(pop)

class CAnimFilterBase {                           // vtable 0x0122FFCC
public:
    CAnimFilterBase();
    virtual ~CAnimFilterBase() { }
};

class CAnimFilterOfGroup : public CAnimFilterBase {
public:
    CAnimFilterOfGroup();
    CCharString   Group;                          // +0x04
    unsigned long GroupCRC;                       // +0x08
};

class CModeStandBase {                            // RTTI .?AVCModeStandBase@@ : CTCCreatureModeManager::CModeBase; Ego_r 0x68 bytes
public:
    CModeStandBase();
    virtual ~CModeStandBase();                    // 0x0073D860
    unsigned char _base[0x64];
};

namespace NCreatureMode {
class CModeBriarRoseTeleportInvulnerable : public CModeStandBase {   // vtable 0x01280228
public:
    CModeBriarRoseTeleportInvulnerable();
    CAnimFilterOfGroup                      Filter;    // +0x68
    long                                    Timer;     // +0x74
    CIntelligentPointer<CThingCreatureBase> PTarget;   // +0x78
    CIntelligentPointer<CThing>             PEffect;   // +0x80
};
}

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(NCreatureMode::CModeBriarRoseTeleportInvulnerable* mode)
{
    mode->NCreatureMode::CModeBriarRoseTeleportInvulnerable::~CModeBriarRoseTeleportInvulnerable();
}