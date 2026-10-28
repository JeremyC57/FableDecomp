// CCreatureAction_JackFall::CCreatureAction_JackFall(CThingCreatureBase& creature) -- select the combat sound pair and animation.
// Leaf: ??0CCreatureAction_JackFall@@QAE@AAVCThingCreatureBase@@@Z
// Identity: proven; own RTTI 012980A4 and exact PDB ctor; base00858030; protected GetSoundPair008561A0 and SetCombatAnimation008574D0 exact PDB names; own animation literal ST_FALL_TO_GROUND and SWORD_SWING sound name.
// Signature audit: original CCharString(const char*,long) uses PDB ??0CCharString@@QAE@PBDJ@Z; old int/H alias removed, actual test constructor corrected too.
// Layout: generated action headers flatten original CBaseClass/CBaseObject/action inheritance and omit constructor/helper APIs. Local genuine chain preserves action168/combat276; unused CThing tags supply reference conversions only. Four-byte CCharString/eight-byte CSoundPair have no shared headers.
#ifndef _MSC_VER
#define __fastcall
#endif
class CThing{};class CThingCreatureBase:public CThing{};class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString(const char* =0,long=-1);~CCharString();};class CSoundPair{public:long FirstSound,LastSound;CSoundPair():FirstSound(0),LastSound(0){}};
class CBaseClass{public:virtual ~CBaseClass(){} };class CBaseObjectPointer;class CBaseObject:public CBaseClass{public:CBaseObjectPointer* IntelligentPointer;};class CCreatureActionBase:public CBaseObject{public:unsigned char _unmodeled08[0xa8-8];};class CCombatActionBase:public CCreatureActionBase{public:unsigned char _unmodeledA8[0x114-0xa8];CCombatActionBase(CThingCreatureBase&,CThing&,long,const CSoundPair&);CCombatActionBase(CThingCreatureBase&,long,const CSoundPair&);void SetCombatAnimation(const CCharString&);protected:static CSoundPair __fastcall GetSoundPair(const CThingCreatureBase&,const CCharString&);};
class CCreatureAction_JackFall:public CCombatActionBase{public:CCreatureAction_JackFall(CThingCreatureBase& creature);};
CCreatureAction_JackFall::CCreatureAction_JackFall(CThingCreatureBase& creature):CCombatActionBase(creature,250,GetSoundPair(creature,"SWORD_SWING")){SetCombatAnimation("ST_FALL_TO_GROUND");}