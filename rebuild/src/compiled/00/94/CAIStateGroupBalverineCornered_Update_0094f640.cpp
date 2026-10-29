// CAIStateGroup_BalverineCornered::Update() -- choose a lunge or sky jump for a live target, then terminate.
// Retail 0094F640, 236 bytes. State/brain use shared headers; retail CThing map is +44 (donor +48).
// Combat action base is 0x114 retail versus donor 0x12c; sky-jump EndPos is +114.
// The shared result is compared after both temporary actions are destroyed, then every path terminates.
// Address-of-temporary syntax follows VC7.1; g++ fixtures compile the identical body with -fpermissive.
#define CAIStateGroup_BalverineCornered CAIStateGroup_BalverineCorneredLayout
#include "engine/CAIStateGroup_BalverineCornered.h"
#undef CAIStateGroup_BalverineCornered

class CAIStateGroupBase
{
public:
    void Terminate();   // 0x008FCF40
};
class CAIStateGroup_BalverineCornered : public CAIStateGroupBase, public CAIStateGroup_BalverineCorneredLayout
{
};
#include "engine/CAIBrain.h"
class CThing;
class CThingCreatureBase;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> Entry;
    Entry *First,*Last;
    Entry* LowerBound(const ETCInterfaceType& key); // 0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CRetailThing {
    unsigned char _prefix[0x20];
    unsigned long UsingComponents[9];
    CInterfaceMap Components;
    unsigned char _beforeDead[0x91-0x4c];
    unsigned char Dead:1;
    template<class T> __forceinline bool GetInterface(T** out,long type) {
        if (UsingComponents[type>>5] & (1UL<<(type&31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map=&Components;
            CInterfaceMap::Entry* it=map->LowerBound(key);
            if(it==map->Last || type<it->first) it=map->Last;
            *out=(T*)((void*)it->second);
            return true;
        }
        return false;
    }
};
class CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable; CThing* Object;
    CBaseObject* GetPItem() const; // 00A01B50
};
inline bool IsAlive(const CBaseIntelligentPointer& p) {
    if(((CThing*)p.GetPItem())!=0) { if(!((CRetailThing*)p.GetPItem())->Dead) return true; }
    return false;
}
class CTCVolumeContainmentTracker { public:
    unsigned char _base[0xc];
    CBaseIntelligentPointer PThingInside;
    void Update(); // 00785620
};
class C3DVector { public: float X,Y,Z; };
class CCombatActionBase { public:
    virtual void v00();
    unsigned char _body[0x110];
    virtual ~CCombatActionBase(); // 0062E900
};
class CCreatureAction_BalverineLungeAttack : public CCombatActionBase { public:
    CCreatureAction_BalverineLungeAttack(CThingCreatureBase*,CThing*); // 008CCC70
};
class CCreatureAction_BalverineCorneredJumpIntoSky : public CCombatActionBase { public:
    C3DVector* EndPos;
    CCreatureAction_BalverineCorneredJumpIntoSky(CThingCreatureBase*,CThing*,C3DVector*); // 008CCB80
};
struct CCorneredMethods : CAIStateGroup_BalverineCornered {
    CAIBrain* Brain() const {return *(CAIBrain* const*)_pad_0x04;}
    const CBaseIntelligentPointer& Target() const {return *(const CBaseIntelligentPointer*)PTarget;}
    long PerformActionAndWait(const CCombatActionBase*,bool); // 0097DA40
    void Update();
};
void CCorneredMethods::Update() {
    if(IsAlive(Target())) {
        // The owning state requires interface0x75; retail leaves the miss out-parameter unwritten.
        CTCVolumeContainmentTracker* tracker;
        ((CRetailThing*)Brain()->Creature)->GetInterface(&tracker,0x75);
        tracker->Update();
        long result;
        if(((CThing*)tracker->PThingInside.GetPItem())) {
            CThingCreatureBase* creature=(CThingCreatureBase*)Brain()->Creature;
            result=PerformActionAndWait(&CCreatureAction_BalverineLungeAttack(creature,((CThing*)Target().GetPItem())),false);
        } else {
            CThingCreatureBase* creature=(CThingCreatureBase*)Brain()->Creature;
            result=PerformActionAndWait(&CCreatureAction_BalverineCorneredJumpIntoSky(creature,((CThing*)Target().GetPItem()),(C3DVector*)&LandPos_X),false);
        }
        if(result==2) { Terminate(); return; }
    }
    Terminate();
}