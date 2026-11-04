// NCreatureMode::CModeJackOfBladesScream::~CModeJackOfBladesScream() -- the compiler-generated destructor: PEmitter, PSword, then the Filter (its Group string and the CAnimFilterBase vtable reset), then ~CModeStandBase
// Leaf: ??1CModeJackOfBladesScream@NCreatureMode@@UAE@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00740540, 45 bytes, thiscall, tail `jmp ~CModeStandBase`.
// Identity (proven): the sole caller is the slot-0 `??_G` 0x00740520 of vtable 0x01267D5C (RTTI
//   .?AVCModeJackOfBladesScream@NCreatureMode@@ : CModeStandBase); the destroyed members are Ego_r.pdb's PEmitter
//   (CIntelligentPointer<CThing>, +0x9C), PSword (+0x74) and Filter (CAnimFilterOfGroup, +0x68: Group CCharString +0x6C, then
//   the CAnimFilterBase vtable 0x0122FFCC); FableWin.pdb spells the destructor
//   ??1CModeJackOfBladesScream@NCreatureMode@@UAE@XZ (Ego_r lists only the ??_G); it stores no vtable of its own, so it is the
//   implicit one. The catalog's "_Dest_val<allocator<CMacro>,CMacro>" is propagated.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (twice), CCharString::~CCharString 0x0099EAE0,
//   CModeStandBase::~CModeStandBase 0x0073D860 (PDB ??1CModeStandBase@@UAE@XZ).
// Shape: members destroyed in reverse declaration order; the implicit ~CAnimFilterOfGroup and the inline
//   `virtual ~CAnimFilterBase() {}` expand here (in the deleting destructor of the sibling 0x008C37C0, one inline level
//   deeper, ~CAnimFilterOfGroup stays the out-of-line 0x00739DB0). DestroyOne (a non-virtual destruction by name) makes VC7.1
//   emit this out-of-line `??1`, nothing more.
// Layout: local -- engine/CModeStandBase.h flattens the vptr into a data member; the base is sized to Ego_r's 0x68.
class CThing;
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

#include "engine/C3DVector.h"

class CRightHandedSet {
public:
    C3DVector Up;
    C3DVector Forward;
};

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
class CModeJackOfBladesScream : public CModeStandBase {   // vtable 0x01267D5C
public:
    CModeJackOfBladesScream();
    CAnimFilterOfGroup         Filter;            // +0x68
    CIntelligentPointer<CThing> PSword;           // +0x74
    CRightHandedSet            InitialSwordRHS;   // +0x7C
    bool                       SwordFalling;      // +0x94
    long                       SwordFallTimer;    // +0x98
    CIntelligentPointer<CThing> PEmitter;         // +0x9C
    C3DVector                  SwordFallPos;      // +0xA4
    C3DVector                  InitialSwordPos;   // +0xB0
};
}

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(NCreatureMode::CModeJackOfBladesScream* mode)
{
    mode->NCreatureMode::CModeJackOfBladesScream::~CModeJackOfBladesScream();
}