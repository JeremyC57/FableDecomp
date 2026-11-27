#ifndef FABLE_ENGINE_CTCEXPLODINGOBJECT_H
#define FABLE_ENGINE_CTCEXPLODINGOBJECT_H
#ifdef FABLE_RETAIL_CTCEXPLODINGOBJECT_PASS22
// Original Ego_r layout48: CTCBase12, fields at0c..28; both PDBs name FrameUpdate/Explode.
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
class CExplodingObjectDef;
class CTCExplodingObject : public CTCBase {
public:
    virtual void FrameUpdate();
#ifdef FABLE_RETAIL_EXPLOSION_HITS_PASS29
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_EXPLODING_OBJECT;}
    void SetCountdownTimer(long);
#endif
private:
    void Explode();
    float MaxDamage;
    float Radius;
    long FireDamage;
    long CountdownTimer;
    bool TriggeredOnCreatureProximity;
    float TriggerRadius;
    CDefPointer<const CExplodingObjectDef> PDef;
    CIntelligentPointer<CThing> PProximityParticle;
};
FABLE_STATIC_ASSERT_AT(23, sizeof(CTCExplodingObject)==0x30);
#endif
#endif
