// CAIStateGroup_FlyingThingBase::GetRandomFlightPos(const C3DVector&,float,C3DVector*,bool) -- find a same-region wander position and raise it above local and ground heights.
// Retail00912700,309 bytes. Shared Base/Brain storage; FlyingThingBase absent from active headers. Final bool argument is unused by retail.
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
class C3DVector { public:float X,Y,Z;};
struct CNavigationMethods {unsigned long GetNavigatorType();void SetNavigatorToType(unsigned long);bool GetNearbyWanderPosition(const C3DVector&,float,C3DVector*,bool);}; //006CB6A0,006CBA80,006CB6E0
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:typedef std::pair<ETCInterfaceType, CTCBase*> Entry;Entry* first;Entry* last;Entry* LowerBound(const ETCInterfaceType& key);};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap; //0040F020
class CThing { public:
    const C3DVector& GetPos() const;
};
struct CThingRetail : public CThing {unsigned char beforeComponents[0x20];unsigned long UsingComponents[9];CInterfaceMap Components;
 float GetHeightZBelowAt(const C3DVector&,bool,bool); //00666D40
 __forceinline bool GetNavigation(CNavigationMethods** out){if(UsingComponents[0]&128){ETCInterfaceType key = (ETCInterfaceType)(7);CInterfaceMap* map=&Components;CInterfaceMap::Entry* it=map->LowerBound(key);if(it==map->last||7<it->first)it=map->last;*out=(CNavigationMethods*)((void*)it->second);return true;}return false;}
};
struct CWorldMapMethods {
virtual void s00();virtual void s01();virtual void s02();virtual void s03();virtual void s04();virtual void s05();virtual void s06();virtual void s07();virtual void s08();virtual void s09();virtual void s10();virtual void s11();virtual void s12();virtual void s13();virtual void s14();virtual void s15();virtual long GetMapNumberAt(const C3DVector&)const;float GetGroundSizeZAt(const C3DVector&);}; //slot16=004FC0C0,0063A390
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
 float ground=PeekWorldMap()->GetGroundSizeZAt(*out);if(ground>out->Z)out->Z=ground;
 return true;
}