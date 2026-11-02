// CAIStateGroup_JackOfBladesStage2::CAIStateGroup_JackOfBladesStage2() -- base CAIStateGroup_JackOfBladesBase(), own vtable, then the PCutsceneLookAtMarker member (+0x4c)
// Leaf: ??0CAIStateGroup_JackOfBladesStage2@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00903095, 36 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128D3C4 (RTTI .?AVCAIStateGroup_JackOfBladesStage2@@, base CAIStateGroup_JackOfBladesBase : CAIStateGroupBase : CBaseClass); member vtable 0x01238C6C = RTTI .?AV?$CIntelligentPointer@VCThing@@@@; Ego_r.pdb / FableWin.pdb
//   ??0CAIStateGroup_JackOfBladesStage2@@QAE@XZ.
// Callees: CAIStateGroup_JackOfBladesBase::CAIStateGroup_JackOfBladesBase 0x00902FE8 (??0CAIStateGroup_JackOfBladesBase@@QAE@XZ), CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 (??0CBaseIntelligentPointer@@QAE@XZ, Ego_r.pdb / FableWin.pdb).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass, 0x14); Ego_r.pdb CAIStateGroup_JackOfBladesStage2::PCutsceneLookAtMarker +0x54 (retail +0x4c: the base's two STLport vectors are 8 bytes smaller than Ego_r's Dinkumware ones); the members before it
//   are left unmodelled. CBaseIntelligentPointer / CIntelligentPointer<T>: the chunk 703 model (0x00900B41).
// Shape: user default ctor, empty; CIntelligentPointer's ctor inlines to the base call + vtable store after this class's own vtable store.
// History: generated in a chunk batch for a repeated body shape over the CBase_00902fe8 / CMemberBase_00a01b10 stand-ins; re-authored on the
//   PDB classes in naming wave 32 (family aisg).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CBaseObjectPointer;
#pragma pack(push, 1)
class CBaseIntelligentPointer {                         // Ego_r.pdb: vptr + PData (8 bytes), 1-aligned (chunk 703 0x00900B41)
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObjectPointer* PData;                          // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    virtual ~CIntelligentPointer() { }
};
#pragma pack(pop)
class CThing;

class CAIStateGroup_JackOfBladesBase : public CAIStateGroupBase {                    // RTTI CAIStateGroup_JackOfBladesBase : CAIStateGroupBase
public:
    CAIStateGroup_JackOfBladesBase();                         // ??0CAIStateGroup_JackOfBladesBase@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
    unsigned char _unmodeled14[0x4c - 0x14];       // Ego_r.pdb: PSwordEmitter +0x14, PArenaCentre +0x1c, PTargetThing +0x24, EnemyKnockDownFrame +0x2c, SpeechCounter +0x30, PillarMarkers +0x34, Pillars +0x44
};

class CAIStateGroup_JackOfBladesStage2 : public CAIStateGroup_JackOfBladesBase {      // vtable 0x0128D3C4
public:
    CIntelligentPointer<CThing> PCutsceneLookAtMarker;                     // +0x4c
    CAIStateGroup_JackOfBladesStage2();
};

CAIStateGroup_JackOfBladesStage2::CAIStateGroup_JackOfBladesStage2()
{
}
