#ifndef FABLE_ENGINE_ACTION_EVENTS_H
#define FABLE_ENGINE_ACTION_EVENTS_H
// Original Ego_r class records and native RTTI vtables 0125c0d8 through 0125c1a0.
// Layout: the base is 20 bytes; native allocation sizes agree with every derived field.
#include "engine/CBaseClass.h"
#include "fable_string.h"
class CCreatureActionBase;class CThingCreatureBase;class CGameCameraManager;
enum EQuakeStrength {QUAKE_STRENGTH_WEAK=0, QUAKE_STRENGTH_MEDIUM=1, QUAKE_STRENGTH_STRONG=2, QUAKE_STRENGTH_MADNESS=3};
enum EQuakeLength {QUAKE_LENGTH_SHORT=0, QUAKE_LENGTH_MEDIUM=1, QUAKE_LENGTH_LONG=2, QUAKE_LENGTH_CONTINUOUS=3};
namespace NCreatureAction {
class CActionEventBase:public CBaseClass {
public:
 virtual void Dispatch(CCreatureActionBase&)=0;
 virtual CCharString GetHook()const=0;
 virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&)const=0;
 virtual bool SetDataFromString(const CCharString&)=0;
protected:
 CActionEventBase():Finished(false){}
private:
 bool Finished;long ActivationFrame;CThingCreatureBase*PThing;CGameCameraManager*PCameraManager;
};
class CActionEventTriggerParticle:public CActionEventBase {
public:
 CActionEventTriggerParticle(){}
 virtual void Dispatch(CCreatureActionBase&);
 virtual CCharString GetHook()const;
 virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&)const;
protected:
 virtual bool SetDataFromString(const CCharString&);
private:
 long AttachmentFlags; // +20
 CCharString AttachmentObject; // +24
 CCharString EffectName; // +28
 bool Weapon; // +32
 bool DropRatherThanAttach; // +33
};
class CActionEventCameraShake:public CActionEventBase {
public:
 CActionEventCameraShake(){}
 virtual void Dispatch(CCreatureActionBase&);
 virtual CCharString GetHook()const;
 virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&)const;
protected:
 virtual bool SetDataFromString(const CCharString&);
private:
 EQuakeStrength Strength; // +20
 EQuakeLength Length; // +24
 bool Global; // +28
};
class CActionEventSound:public CActionEventBase {
public:
 CActionEventSound(){}
 virtual void Dispatch(CCreatureActionBase&);
 virtual CCharString GetHook()const;
 virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&)const;
protected:
 virtual bool SetDataFromString(const CCharString&);
private:
};
class CActionEventShot:public CActionEventBase {
public:
 CActionEventShot(){}
 virtual void Dispatch(CCreatureActionBase&);
 virtual CCharString GetHook()const;
 virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&)const;
protected:
 virtual bool SetDataFromString(const CCharString&);
private:
};
class CActionEventExplosion:public CActionEventBase {
public:
 CActionEventExplosion(){}
 virtual void Dispatch(CCreatureActionBase&);
 virtual CCharString GetHook()const;
 virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&)const;
protected:
 virtual bool SetDataFromString(const CCharString&);
private:
 CCharString AttachmentObject; // +20
 long ExplosionDefIndex; // +24
 bool Dummy; // +28
 bool UseActionHelper; // +29
};
class CActionEventAddObject:public CActionEventBase {
public:
 CActionEventAddObject(){}
 virtual void Dispatch(CCreatureActionBase&);
 virtual CCharString GetHook()const;
 virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&)const;
protected:
 virtual bool SetDataFromString(const CCharString&);
private:
 CCharString Object; // +20
 CCharString Hand; // +24
 bool DestroyOnExit; // +28
};
class CActionEventRemoveObject:public CActionEventBase {
public:
 CActionEventRemoveObject(){}
 virtual void Dispatch(CCreatureActionBase&);
 virtual CCharString GetHook()const;
 virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&)const;
protected:
 virtual bool SetDataFromString(const CCharString&);
private:
 CCharString Hand; // +20
};
}
#endif
