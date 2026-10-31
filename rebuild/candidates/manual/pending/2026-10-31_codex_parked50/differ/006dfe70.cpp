// 006dfe70 CTCDParticleEmitter::Construct
// Codex parked-50 best attempt: DIFFER(178v178); residue: 178/178 bytes; original typed particle-parameter model still differs in scheduling across constructor/virtual calls.
// Leaf: ?Construct@CTCDParticleEmitter@@AAEXJABVC3DVector@@_N1@Z
// Flags:
#define CTCPhysicsBase CTCPhysicsBaseLayout
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase
#define CClass CClassLayout
#include "engine/CClass.h"
#undef CClass
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CBaseClassNonCopyable.h"
#pragma pack(push,1)
class CClass:public CBaseObject{public:unsigned char unobservedClass[sizeof(CClassLayout)-sizeof(CBaseObject)];};
#pragma pack(pop)
class CTCParamsBase : public CBaseClassNonCopyable { };
class CCharString;class CPersistContext;class CThing;
class CTCBase : public CBaseClassNonCopyable {
public:
protected:
    CThing& ParentThing;
private:
    bool ToBeRemoved,IsInGlobalUpdateTCs;
    unsigned char unobservedAlignment[2];
protected: virtual void OnSerialise(CPersistContext&,bool);
public:
    virtual void OnRemove();
    virtual void ConstructFromParams(const CTCParamsBase&);
};
class CTCDriverBase : public CTCBase { };
class CTCPhysicsBase : public CTCBase { };
#pragma pack(push,1)
class C3DVector {
public: float X,Y,Z;
    C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z) { }
};
class CRightHandedSet { protected:C3DVector Up,Forward;public:CRightHandedSet(const CRightHandedSet&r):Up(r.Up),Forward(r.Forward){} };
#pragma pack(pop)
class CTCParamsPhysicsBase : public CTCParamsBase {
public:
    CTCParamsPhysicsBase(const CTCParamsPhysicsBase&);
    C3DVector Position;
    CRightHandedSet Rhs;
    CTCParamsPhysicsBase(const C3DVector& p,const CRightHandedSet& r):Position(p),Rhs(r) { }
};
// Layout: original static data DEFAULT_RHSET24 follows CParticleUpdateParameters RTTI by64 bytes in donor and retail.
// This declaration supplies its symbol for relocation to the original data; it defines no replacement storage.
extern CRightHandedSet DEFAULT_RHSET;
class CThing:public CClass {
public:
    unsigned char unobservedPrefix[0x60-sizeof(CClass)];
    CTCPhysicsBase* PhysicsTC;
};
class CCharStringData;class CCharString{public:CCharStringData*PStringData;};
class CTCDParticleEmitter : public CTCDriverBase {
private:
    CCharString EmitterName;
    long ParticleTypeID;
    unsigned char unobservedMiddle[0x44-0x14];
    bool WaitingForParticleToLoad:1;
    bool ForceParticleToLoad:1;
    bool IndependantObject:1;
    bool ParticleCreationDisabled:1;
    bool ResetEmitterPosition:1;
    bool ResetEmitterOrientation:1;
    bool AttachedToCamera:1;
    bool Moving:1;
private:
    void Construct(long,const C3DVector&,bool,bool);
};
void CTCDParticleEmitter::Construct(long particleType,const C3DVector& position,bool independent,bool forceLoad)
{
    CTCPhysicsBase* physics=ParentThing.PhysicsTC;
    {
        const CTCParamsPhysicsBase&params=CTCParamsPhysicsBase(position,DEFAULT_RHSET);
        physics->ConstructFromParams(params);
    }
    ParticleTypeID=particleType;
    IndependantObject=independent;
    ForceParticleToLoad=forceLoad;
}
