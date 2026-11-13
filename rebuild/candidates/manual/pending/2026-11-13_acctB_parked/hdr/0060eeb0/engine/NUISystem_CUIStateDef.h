#ifndef FABLE_ENGINE_NUISYSTEM_CUISTATEDEF_H
#define FABLE_ENGINE_NUISYSTEM_CUISTATEDEF_H
// NUISystem::CUIStateDef -- one UI state of a CUIDef (CUIDef::States is a std::vector<CUIStateDef>).
// New shared header (chunk 777, row 0x006303C0 CUIDef::CUIDef(const CUIDef&)): the CUIDef retail branch needs the complete
// element type to instantiate vector<CUIStateDef>. Retail-only model (no default text to keep): it requires the retail def
// chain (FABLE_RETAIL_DEF_CLASS_BASE + FABLE_PDCB_TYPED_MEMBERS, as engine/NUISystem_CUIDef.h's FABLE_RETAIL_CUIDEF branch sets).
// Identity: RTTI .?AVCUIStateDef@NUISystem@@, vtable 0x0125871C; PDB name sets (Ego_r + FableWin) `??0CUIStateDef@NUISystem@@QAE@XZ`,
//   `??0CUIStateDef@NUISystem@@QAE@ABV01@@Z`, `??1CUIStateDef@NUISystem@@UAE@XZ`, `?Transfer@CUIStateDef@NUISystem@@UAEXAAVCPersistContext@@@Z`.
// Layout (retail bytes, Ego_r.pdb members minus 4 for the packed 0x39-byte CParentDefClassBase; landed ctor 0x00630350,
//   copy ctor 0x006250B0, dtor 0x00624DC0, Transfer 0x00625630): GraphicIndex +0x3c, StateChangeFlag +0x40,
//   StateChangeType +0x44, Position +0x48, Zoom +0x50, Colour +0x58, UpdateTime +0x68, ChildrenNotAffected +0x6c
//   (12-byte STLport vector), LinearChange +0x78; sizeof 0x7c (the vector<CUIStateDef> stride, 0x006315E0 `imul 84210843h; sar 6`).
#include "engine/CParentDefClassBase.h"
#include "engine/NUISystem_enums.h"
#include "engine/C2DVector.h"
#include "engine/CRGBFloatColour.h"
#include "stlport/fable_containers.h"
namespace NUISystem {
class CUIStateDef : public CParentDefClassBase {
public:
    CUIStateDef();                                                   // 0x00630350
    CUIStateDef(const CUIStateDef&);                                 // 0x006250B0
#ifndef FABLE_RETAIL_CUISTATEDEF_IMPLICIT_DTOR
    virtual ~CUIStateDef();                                          // 0x00624DC0
#else
    // Retail opt-in (chunk 882, row 0x0061E6D0): the destructor is the implicit one (0x00624DC0 is its out-of-line COMDAT). The row
    // destroys two stack CUIStateDefs inline (0x00621089..0x00621106: free +0x6C, free +0x29, CDefPointeeBase vtable 0x01230BA0 store,
    // call ~CResource 0x009FC550), the same body as 0x00624DC0. Needs FABLE_RETAIL_CPARENTDEFCLASSBASE and FABLE_PDCB_TYPED_MEMBERS.
    // The three CDefClassBase pure virtuals it overrides (Ego_r.pdb / FableWin.pdb ?Transfer@CUIStateDef@NUISystem@@UAEXAAVCPersistContext@@@Z
    // 0x00625630, ?Copy@CUIStateDef@NUISystem@@UAEXPBVCDefClassBase@@@Z, ?GetSizeofClass@CUIStateDef@NUISystem@@UAEKXZ 0x00625150) are
    // declared so the row can construct the class.
    virtual void Transfer(CPersistContext&);                         // 0x00625630
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();                          // 0x00625150
#endif
    unsigned char _pad_39[3];
    unsigned long GraphicIndex;                                      // +0x3c
    unsigned long StateChangeFlag;                                   // +0x40
    EStateChangeType StateChangeType;                                // +0x44
    C2DVector Position;                                              // +0x48
    C2DVector Zoom;                                                  // +0x50
    CRGBFloatColour Colour;                                          // +0x58
    float UpdateTime;                                                // +0x68
    std::vector<unsigned long,std::allocator<unsigned long> > ChildrenNotAffected;   // +0x6c
    bool LinearChange;                                               // +0x78
};
FABLE_STATIC_ASSERT(sizeof(CUIStateDef)==0x7c);
FABLE_STATIC_ASSERT(offsetof(CUIStateDef,GraphicIndex)==0x3c);
FABLE_STATIC_ASSERT(offsetof(CUIStateDef,ChildrenNotAffected)==0x6c);
}
#endif
