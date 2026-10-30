// CCreatureAction_NymphShot::CCreatureAction_NymphShot(CThingCreatureBase& creature,CThingCreatureBase& enemy) -- select the combat sound pair and animation.
// Leaf: ??0CCreatureAction_NymphShot@@QAE@AAVCThingCreatureBase@@0@Z
// Identity: proven; own RTTI 012824B4 and exact PDB ctor; base00857F30; protected GetSoundPair008561A0 and SetCombatAnimation008574D0 exact PDB names; own animation literal STRIKE_MEDIUM_FRONT and null sound name.
// Signature audit: original CCharString(const char*,long) uses PDB ??0CCharString@@QAE@PBDJ@Z; old int/H alias removed, actual test constructor corrected too.
// Layout: generated action headers flatten original CBaseClass/CBaseObject/action inheritance and omit constructor/helper APIs. Local genuine chain preserves action168/combat276; unused CThing tags supply reference conversions only. Four-byte CCharString/eight-byte CSoundPair have no shared headers.
#ifndef _MSC_VER
#define __fastcall
#endif
class CThing{};class CThingCreatureBase:public CThing{};class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString(const char* =0,long=-1);~CCharString();};class CSoundPair{public:long FirstSound,LastSound;CSoundPair():FirstSound(0),LastSound(0){}};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseObjectPointer;class CBaseObject:public CBaseClass{public:CBaseObjectPointer* IntelligentPointer;};class CCreatureActionBase:public CBaseObject{public:unsigned char _unmodeled08[0xa8-8];};class CCombatActionBase:public CCreatureActionBase{public:unsigned char _unmodeledA8[0x114-0xa8];CCombatActionBase(CThingCreatureBase&,CThing&,long,const CSoundPair&);CCombatActionBase(CThingCreatureBase&,long,const CSoundPair&);void SetCombatAnimation(const CCharString&);protected:static CSoundPair __fastcall GetSoundPair(const CThingCreatureBase&,const CCharString&);};
class CCreatureAction_NymphShot:public CCombatActionBase{public:CCreatureAction_NymphShot(CThingCreatureBase& creature,CThingCreatureBase& enemy);};
CCreatureAction_NymphShot::CCreatureAction_NymphShot(CThingCreatureBase& creature,CThingCreatureBase& enemy):CCombatActionBase(creature,enemy,150,GetSoundPair(creature,0)){SetCombatAnimation("STRIKE_MEDIUM_FRONT");}