// CCreatureAction_VillagerBackOff::CCreatureAction_VillagerBackOff(CThingCreatureBase& creature,CThingCreatureBase& enemy) -- initialise the fifty-frame combat action with its selected sound pair.
// Leaf: ??0CCreatureAction_VillagerBackOff@@QAE@AAVCThingCreatureBase@@0@Z
// Identity: proven; own RTTI 01299204 and PDB exact ctor; actual CCombatActionBase00857F30 and protected static fastcall GetSoundPair008561A0 exact PDB spelling; null CCharString constructor0099EBF0/destructor0099EAE0.
// Signature audit: original CCharString(const char*,long) uses PDB ??0CCharString@@QAE@PBDJ@Z; old int/H alias removed, actual test constructor corrected too.
// Layout: generated headers flatten CBaseClass/CBaseObject/action hierarchy and omit constructor/protected helper APIs. Local original chain preserves action168/combat276. CThing tags only supply reference conversion; no object fields/sizeof used. Actual CCharString4/CSoundPair8; shared headers absent.
#ifndef _MSC_VER
#define __fastcall
#endif
class CThing{};class CThingCreatureBase:public CThing{};class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString(const char* =0,long=-1);~CCharString();};class CSoundPair{public:long FirstSound,LastSound;CSoundPair():FirstSound(0),LastSound(0){}};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseObjectPointer;class CBaseObject:public CBaseClass{public:CBaseObjectPointer* IntelligentPointer;};class CCreatureActionBase:public CBaseObject{public:unsigned char _unmodeled08[0xa8-8];};class CCombatActionBase:public CCreatureActionBase{public:unsigned char _unmodeledA8[0x114-0xa8];CCombatActionBase(CThingCreatureBase&,CThing&,long,const CSoundPair&);protected:static CSoundPair __fastcall GetSoundPair(const CThingCreatureBase&,const CCharString&);};
class CCreatureAction_VillagerBackOff:public CCombatActionBase{public:CCreatureAction_VillagerBackOff(CThingCreatureBase&,CThingCreatureBase&);};
CCreatureAction_VillagerBackOff::CCreatureAction_VillagerBackOff(CThingCreatureBase& creature,CThingCreatureBase& enemy):CCombatActionBase(creature,enemy,50,GetSoundPair(creature,0)){}