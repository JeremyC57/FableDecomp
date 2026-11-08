// CAIStateGroup_BalverineCornered::Update() -- choose a lunge or sky jump for a live target, then terminate.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members First/Last are the header CArray base's begin() / end() (STLport vector start +0 / finish +4) re-padded by-value maps +8.
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail 0094F640, 236 bytes. State/brain use shared headers; retail CThing map is +44 (donor +48).
// Combat action base is 0x114 retail versus donor 0x12c; sky-jump EndPos is +114.
// The shared result is compared after both temporary actions are destroyed, then every path terminates.
// Address-of-temporary syntax follows VC7.1; g++ fixtures compile the identical body with -fpermissive.
#include "fable_compiler.h"
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
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CRetailThing {
    unsigned char _prefix[0x20];
    unsigned long UsingComponents[9];
    CInterfaceMap Components;
    unsigned char _beforeDead[0x3d];
    unsigned char Dead:1;
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out,long type) {
        if (UsingComponents[type>>5] & (1UL<<(type&31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map=&Components;
            CInterfaceMap::value_type* it=map->LowerBound(key);
            if(it==map->end() || type<it->first) it=map->end();
            *out=(T*)((void*)it->second);
            return true;
        }
        return false;
    }
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
inline bool IsAlive(const CBaseIntelligentPointer& p) {
    if(((CThing*)p.GetPItem())!=0) { if(!((CRetailThing*)p.GetPItem())->Dead) return true; }
    return false;
}
class CTCVolumeContainmentTracker { public:
    unsigned char _base[0xc];
    CBaseIntelligentPointer PThingInside;
    void Update(); // 00785620
};
#include "engine/C3DVector.h"
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