// CTCPerceiveThing::OnHitByShot(const CThing& thing) -- Record a hit by the currently perceived thing and refresh awareness.
// Leaf: ?OnHitByShot@CTCPerceiveThing@@QAEXABVCThing@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#ifndef _MSC_VER
#define __fastcall
#endif
class CThing;class CBaseObject;class CBaseObjectPointer;class CTCPerceiveThing;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CWorld {public:static long __fastcall GetFrame();};
class CTCBase {friend class CTCPerceiveThing;protected:CWorld& GetWorld()const;};
#define CTCPerceiveThing CTCPerceiveThing_HeaderLayout
#include "engine/CTCPerceiveThing.h"
#undef CTCPerceiveThing
// Layout: this uses the supplied shared data layout. The opaque PerceivedThing+12 is its real8-byte intelligent pointer. CTCBase is used only as the external GetWorld receiver at offset0, as the original base chain specifies.
class CTCPerceiveThing : public CTCPerceiveThing_HeaderLayout {
    const CThing* GetPerceivedThing()const{return reinterpret_cast<const CThing*>(reinterpret_cast<const CBaseIntelligentPointer*>(&PerceivedThing)->GetPItem());}
    CWorld& GetWorld()const{return reinterpret_cast<const CTCBase*>(this)->GetWorld();}
    void SetLastAwareInfo();
public:void OnHitByShot(const CThing& thing);
};
void CTCPerceiveThing::OnHitByShot(const CThing& thing){
    if(GetPerceivedThing()==&thing){
        if(GetPerceivedThing()){
            DirectAwareness=true;
            FramesSinceDirectlyPerceivedThing=0;
            SetLastAwareInfo();
        }
        FrameHitByPerceivedThing=GetWorld().GetFrame();
    }
}