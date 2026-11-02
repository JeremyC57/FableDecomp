// CAIStateGroup_VersusScorpionKing::CAIStateGroup_VersusScorpionKing() -- construct the state group: base CAIStateGroup_VersusEnemyBase, own vtable, the PArenaCentreThing tracked pointer
// Leaf: ??0CAIStateGroup_VersusScorpionKing@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090143E, 36 bytes, thiscall, `ret`; a size-optimised TU packed without padding (the
//   previous function, the VersusEnemyBase ??_G 0x00901422, ends with `ret 4` at 0x0090143B), so the unaligned start is real.
// Identity (proven): stores the CAIStateGroup_VersusScorpionKing vtable 0x0128CBE4 (RTTI .?AVCAIStateGroup_VersusScorpionKing@@;
//   slot 0 is its ??_G 0x00901466 right after this body); the member vtable 0x01238C6C is .?AV?$CIntelligentPointer@VCThing@@@@,
//   at +0x1D = Ego_r.pdb CAIStateGroup_VersusScorpionKing::PArenaCentreThing (offset 29, CIntelligentPointer<CThing>); the only
//   caller is the factory 0x00902BA2 (`new` of 0x28 bytes = the PDB size 40, then a tail jmp here). Ego_r.pdb name set
//   ??0CAIStateGroup_VersusScorpionKing@@QAE@XZ (donor 0x0091DBE4, right after ??0CAIStateGroup_VersusEnemyBase@@QAE@XZ 0x0091DBC1,
//   as retail places 0x009013FA / 0x0090143E). The catalog label "CAIStateGroupBehaviourBase::CAIStateGroupBehaviourBase" is wrong.
// Callees: CAIStateGroup_VersusEnemyBase::CAIStateGroup_VersusEnemyBase 0x009013FA (landed), CBaseIntelligentPointer::
//   CBaseIntelligentPointer 0x00A01B10.
// Layout: Ego_r.pdb CAIStateGroup_VersusEnemyBase (0x1C: CAIStateGroupBase 0x14 + CIntelligentPointer<CThingCreatureBase>
//   PTargetThing +0x14, vtable 0x0123E7D8) + bool GoClockwise +0x1C + CIntelligentPointer<CThing> PArenaCentreThing +0x1D
//   (0x28 bytes): the tracked pointer type is 1-aligned (`#pragma pack(push, 1)` on CBaseIntelligentPointer, as the landed
//   CThingSoundEmitter 0x008367B0), the class keeps 4-byte alignment (PDB size 40, not 37).
#include "engine/CBaseClass.h"                          // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CAIBrain;
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
};
enum EScriptAIPriority { SCRIPT_PRIORITY_DEFAULT };

class CAIStateGroupBase : public CBaseClass {
public:
    CAIBrain*         PBrain;                           // +0x04
    EScriptAIPriority ScriptAIPriority;                 // +0x08
    CCharString       Name;                             // +0x0C
    bool              Terminated;                       // +0x10
    unsigned char     Priority;                         // +0x11
    bool              Enabled;                          // +0x12
    CAIStateGroupBase();                                // 0x008FD255
};

class CBaseObjectPointer;
class CThing;
class CThingCreatureBase;
#pragma pack(push, 1)
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    CBaseObjectPointer* PData;                          // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    virtual ~CIntelligentPointer() { }
};
#pragma pack(pop)

class CAIStateGroup_VersusEnemyBase : public CAIStateGroupBase {     // vtable 0x0128CBA4
public:
    CIntelligentPointer<CThingCreatureBase> PTargetThing;              // +0x14
    CAIStateGroup_VersusEnemyBase();                                   // 0x009013FA
};

class CAIStateGroup_VersusScorpionKing : public CAIStateGroup_VersusEnemyBase {   // vtable 0x0128CBE4
public:
    bool                        GoClockwise;                           // +0x1C
    CIntelligentPointer<CThing> PArenaCentreThing;                     // +0x1D
    CAIStateGroup_VersusScorpionKing();
};

CAIStateGroup_VersusScorpionKing::CAIStateGroup_VersusScorpionKing()
{
}