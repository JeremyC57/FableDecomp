// CAIStateGroup_FlyingThingBase::GetRandomFlightPos(const C3DVector&,float,C3DVector*,bool) -- find a same-region wander position and raise it above local and ground heights.
// Plan phase 5 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members first/last are the header CArray base's begin() / end() (STLport vector start +0 / finish +4).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail00912700,309 bytes. Shared Base/Brain storage; FlyingThingBase absent from active headers. Final bool argument is unused by retail.
#include "fable_compiler.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/C3DVector.h"
struct CNavigationMethods {unsigned long GetNavigatorType();void SetNavigatorToType(unsigned long);bool GetNearbyWanderPosition(const C3DVector&,float,C3DVector*,bool);}; //006CB6A0,006CBA80,006CB6E0
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap; //0040F020
class CThing { public:
    const C3DVector& GetPos() const;
};
struct CThingRetail : public CThing {unsigned char beforeComponents[0x20];unsigned long UsingComponents[9];CInterfaceMap Components;
 float GetHeightZBelowAt(const C3DVector&,bool,bool); //00666D40
 FABLE_FORCEINLINE bool GetNavigation(CNavigationMethods** out){if(UsingComponents[0]&128){ETCInterfaceType key = (ETCInterfaceType)(7);CInterfaceMap* map=&Components;CInterfaceMap::value_type* it=map->LowerBound(key);if(it==map->end()||7<it->first)it=map->end();*out=(CNavigationMethods*)((void*)it->second);return true;}return false;}
};
struct CWorldMapMethods {
virtual void s00();virtual void s01();virtual void s02();virtual void s03();virtual void s04();virtual void s05();virtual void s06();virtual void s07();virtual void s08();virtual void s09();virtual void s10();virtual void s11();virtual void s12();virtual void s13();virtual void s14();virtual void s15();virtual long GetMapNumberAt(const C3DVector&)const;float GetThemeSizeZAt(const C3DVector&) const;}; //slot16=004FC0C0, 0063A390 = Ego_r.pdb ?GetThemeSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z (naming wave 19; the view class stays)
extern unsigned long FlyingNavigatorType; //0129CB44
extern const C3DVector HeightProbeOffset; //0138C500
struct FlyingMethods:CAIStateGroupBase {float MinHeightToKeepAboveGround;float MinHeightToKeepAboveGroundInFlight;float MaxHeightAboveGround;float FlightRange;
 CWorldMapMethods* PeekWorldMap(); //008FCFF0
 bool GetRandomFlightPos(const C3DVector&,float,C3DVector*,bool);
};
bool FlyingMethods::GetRandomFlightPos(const C3DVector& from,float radius,C3DVector* out,bool allowGround){
 if(PeekWorldMap()->GetMapNumberAt(from)!=PeekWorldMap()->GetMapNumberAt(((CThingRetail*)PBrain->Creature)->GetPos()))return false;
 CNavigationMethods* navigation;
 ((CThingRetail*)PBrain->Creature)->GetNavigation(&navigation);
 unsigned long oldType=navigation->GetNavigatorType();navigation->SetNavigatorToType(FlyingNavigatorType);
 if(!navigation->GetNearbyWanderPosition(from,radius,out,true))return false;
 navigation->SetNavigatorToType(oldType);
 C3DVector probe;probe.X=HeightProbeOffset.X+out->X;probe.Y=HeightProbeOffset.Y+out->Y;probe.Z=HeightProbeOffset.Z+out->Z;
 CThingRetail* creature=(CThingRetail*)PBrain->Creature;
 out->Z=creature->GetHeightZBelowAt(probe,true,true)+MinHeightToKeepAboveGround;
 float ground=PeekWorldMap()->GetThemeSizeZAt(*out);if(ground>out->Z)out->Z=ground;
 return true;
}