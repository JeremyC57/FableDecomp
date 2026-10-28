// CTCPerceiveThing::OnHit(const CThing& thing) -- Record a hit by the currently perceived thing and refresh awareness.
// Leaf: ?OnHit@CTCPerceiveThing@@QAEXABVCThing@@@Z
#ifndef _MSC_VER
#define __fastcall
#endif
class CThing;class CBaseObject;class CBaseObjectPointer;class CTCPerceiveThing;
class CBaseIntelligentPointer {public:virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;CBaseObject* GetPItem()const;};
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
public:void OnHit(const CThing& thing);
};
void CTCPerceiveThing::OnHit(const CThing& thing){
    if(GetPerceivedThing()==&thing){
        if(GetPerceivedThing()){
            DirectAwareness=true;
            FramesSinceDirectlyPerceivedThing=0;
            SetLastAwareInfo();
        }
        FrameHitByPerceivedThing=GetWorld().GetFrame();
    }
}