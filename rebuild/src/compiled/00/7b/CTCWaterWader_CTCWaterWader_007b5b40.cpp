// CTCWaterWader::CTCWaterWader(CThing&) -- CTCBase(thing), own vtable, both particle pointers built null, not wading
// Leaf: ??0CTCWaterWader@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCWaterWader@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B5B40, length 81 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Vtables: CTCWaterWader 0x0126D95C, CIntelligentPointer<CThing> 0x01238C6C.
// Layout (Ego_r.pdb CTCWaterWader, 32 bytes): CTCBase 0xc, PParticle +0x0c, PWaterfallParticle +0x14
// (CIntelligentPointer<CThing>, 8 bytes each), OldWade +0x1c.
// Shape: the derived vtable is stored before the member constructors (VC installs the vptr first); each
// pointer is constructed from a null CThing* (base constructor, its vtable, SetPItem(0)); OldWade = false last.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
class CBaseObject;

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct CTCWaterWader : CTCBase {                      // vtable 0x0126D95C
    CIntelligentPointer<CThing> PParticle;                   // +0x0c
    CIntelligentPointer<CThing> PWaterfallParticle;          // +0x14
    bool                       OldWade;                     // +0x1c
    CTCWaterWader(CThing& thing);
};

CTCWaterWader::CTCWaterWader(CThing& thing)
    : CTCBase(thing)
    , PParticle(0)
    , PWaterfallParticle(0)
    , OldWade(false)
{
}