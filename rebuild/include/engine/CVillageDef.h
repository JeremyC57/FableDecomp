#ifndef FABLE_ENGINE_CVILLAGEDEF_H
#define FABLE_ENGINE_CVILLAGEDEF_H
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CVillageTaskBase.h"
#include "stlport/fable_containers.h"
class CValidateDefinitionInitBase;class CPersistContext;
#ifdef _MSC_VER
enum EVillageStanceToHero;
#else
enum EVillageStanceToHero{};
#endif
// Layout: complete original Ego1b2cd/FW1bda3; native268 with eight smaller vectors.
class CVillageDef:public CSubDefClassBase{
public:
 virtual void Copy(const CDefClassBase * p0);
 virtual unsigned long GetSizeofClass();
 static CDefClassBase * FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL StatGetTypeName();
 virtual void Validate(const CValidateDefinitionInitBase & p0, const CCharString & p1) const;
 std::vector<enum EVillageTaskType,std::allocator<enum EVillageTaskType> > Tasks;
 CDefString PatrolTrackName;
 EVillageStanceToHero DefaultStanceToHero;
 long SecondsSecuritySweepLasts;
 long WarehouseCrateDef;
 std::vector<long,std::allocator<long> > StoryStartTimes;
 long StoryDuration;
 std::vector<long,std::allocator<long> > SchoolStartTimes;
 long SchoolDuration;
 std::vector<long,std::allocator<long> > WorkStartTimes;
 long WorkDuration;
 std::vector<long,std::allocator<long> > CookStartTimes;
 long CookDuration;
 std::vector<long,std::allocator<long> > EatStartTimes;
 long EatDuration;
 std::vector<long,std::allocator<long> > TavernStartTimes;
 long TavernDuration;
 std::vector<long,std::allocator<long> > GamingStartTimes;
 long GamingDuration;
 long SleepAdultStartTime;
 long SleepAdultEndTime;
 long SleepChildStartTime;
 long SleepChildEndTime;
 long SleepElderlyStartTime;
 long SleepElderlyEndTime;
 long HouseLightsOnTime;
 long HouseLightsOffTime;
 long SociableStartTime;
 long SociableFinishTime;
 long CurfewStartTime;
 long CurfewFinishTime;
 bool EnableAutoPlacement;
 bool AllowHouseBuying;
 bool HearTownCrier;
 CCharString CurfewStartSound;
 CCharString CurfewEndSound;
 CCharString WorkStartSound;
 CCharString WorkEndSound;
 long BribeCost;
 long FramesBetweenGuardSnoop;
 long FramesBetweenGuardSnoopNightShops;
 long FramesBeforeGuardSnoop;
 long FramesBeforeGuardSnoopNightShops;
 virtual void Transfer(CPersistContext & p0);
 CVillageDef(const CVillageDef & p0);
 CVillageDef();
 virtual ~CVillageDef();
 CVillageDef & operator=(const CVillageDef & p0);
};
FABLE_STATIC_ASSERT(sizeof(CVillageDef)==268);
#endif
