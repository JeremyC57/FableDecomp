// CTCWaterWader::~CTCWaterWader() -- destructor: kills the wading particle and the waterfall particle if they are still alive and clears both pointers, then the members and the CTCBase base are destroyed
// Leaf: ??1CTCWaterWader
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B5BA0, 243 bytes (plain `ret`; vtable 0x0126D95C stored first).
// Callees: CBaseIntelligentPointer::GetPItem() 0x00A01B50; CThing::Kill(bool) 0x004C9B80; CBaseIntelligentPointer()
//          0x00A01B10 (then the CIntelligentPointer<CThing> vtable 0x01238C6C); SetPItem 0x00A01B90; ~CBaseIntelligentPointer
//          0x00A01C10 (the temporaries, then both members); CTCBase::~CTCBase 0x00686830.
// Layout (Ego_r.pdb, offsets hold): PParticle +0x0c, PWaterfallParticle +0x14, OldWade +0x1c.
// Shape: each particle is an IS_THING_ALIVE test (three GetPItem calls) guarding `Kill(true)` and the clear, which is
//        `cleared(0); CThing* item = cleared.GetPItem(); member.SetPItem(item);` (the landed CThingSoundEmitter dtor
//        idiom); the two `cleared` temporaries live in sibling blocks and share one frame slot.
#include "engine/CThing.h"
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

class CTCBase { public:
    virtual ~CTCBase();                         // 0x00686830
    CThing*       ParentThing;                  // +0x04
    bool          ToBeRemoved;                  // +0x08
    bool          IsInGlobalUpdateTCs;          // +0x09
    unsigned char _pad0a[2];
};
struct CTCWaterWader : CTCBase {                // vtable 0x0126D95C
    CIntelligentPointer<CThing> PParticle;         // +0x0c
    CIntelligentPointer<CThing> PWaterfallParticle;   // +0x14
    bool                     OldWade;           // +0x1c
    virtual ~CTCWaterWader();
};

CTCWaterWader::~CTCWaterWader()
{
    if (IS_THING_ALIVE(PParticle)) {
        PParticle.Get()->Kill(true);
        CIntelligentPointer<CThing> cleared(0);
        CThing* item = cleared.Get();
        PParticle.Set(item);
    }
    if (IS_THING_ALIVE(PWaterfallParticle)) {
        PWaterfallParticle.Get()->Kill(true);
        CIntelligentPointer<CThing> cleared(0);
        CThing* item = cleared.Get();
        PWaterfallParticle.Set(item);
    }
}