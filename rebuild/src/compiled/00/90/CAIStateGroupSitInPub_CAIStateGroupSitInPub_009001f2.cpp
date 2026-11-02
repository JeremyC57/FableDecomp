// CAIStateGroup_SitInPub::CAIStateGroup_SitInPub() -- base CAIStateGroup_UsesSeat(), own vtable, then the PBuilding member (+0x34)
// Leaf: ??0CAIStateGroup_SitInPub@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009001F2, 36 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128BF04 (RTTI .?AVCAIStateGroup_SitInPub@@, base CAIStateGroup_UsesSeat : CAIStateGroupBase : CBaseClass); member vtable 0x01238C6C = RTTI .?AV?$CIntelligentPointer@VCThing@@@@; Ego_r.pdb / FableWin.pdb
//   ??0CAIStateGroup_SitInPub@@QAE@XZ.
// Callees: CAIStateGroup_UsesSeat::CAIStateGroup_UsesSeat 0x008FF1F8 (??0CAIStateGroup_UsesSeat@@IAE@XZ), CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 (??0CBaseIntelligentPointer@@QAE@XZ, Ego_r.pdb / FableWin.pdb).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass, 0x14); Ego_r.pdb CAIStateGroup_SitInPub::PBuilding +0x34; the members before it
//   are left unmodelled. CBaseIntelligentPointer / CIntelligentPointer<T>: the chunk 703 model (0x00900B41).
// Shape: user default ctor, empty; CIntelligentPointer's ctor inlines to the base call + vtable store after this class's own vtable store.
// History: generated in a chunk batch for a repeated body shape over the CBase_008ff1f8 / CMemberBase_00a01b10 stand-ins; re-authored on the
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

class CAIStateGroup_UsesSeat : public CAIStateGroupBase {                    // RTTI CAIStateGroup_UsesSeat : CAIStateGroupBase
protected:
    CAIStateGroup_UsesSeat();                         // ??0CAIStateGroup_UsesSeat@@IAE@XZ (Ego_r.pdb / FableWin.pdb)
public:
    unsigned char _unmodeled14[0x34 - 0x14];       // Ego_r.pdb: PSeat +0x14, GotSeatPos +0x1c, Sitting +0x1d, SeatPosIndex +0x20, SeatPos +0x24, SittingOnBench +0x30
};

class CAIStateGroup_SitInPub : public CAIStateGroup_UsesSeat {      // vtable 0x0128BF04
public:
    CIntelligentPointer<CThing> PBuilding;                     // +0x34
    CAIStateGroup_SitInPub();
};

CAIStateGroup_SitInPub::CAIStateGroup_SitInPub()
{
}
