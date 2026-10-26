// NAnimationEvents::CManager::GetEmbeddedEventTime(ulong,const CCharString&)const -- normalized embedded event time, zero for tiny duration.
// Retail006F9130 length169. A242C0 animation lookup; A269C0 owning stats; A4EAE0 event-time; scalar deleteBFE9BC.
#define CManager CManagerDonor
#include "engine/CManager.h"
#undef CManager
#define CMeshDataBank CMeshDataBankDonor
#include "engine/CMeshDataBank.h"
#undef CMeshDataBank
#define C3DAnimationInfo C3DAnimationInfoDonor
#include "engine/C3DAnimationInfo.h"
#undef C3DAnimationInfo
#include "fable_string.h"
void operator delete(void*) throw(); //00BFE9BC
class CCPPointerInfo { public:unsigned long RefCount;void(FABLE_FASTCALL* DeleteFunc)(void*);void* Data;};
template<class T>class CCountedPointer { public:
 T* Data;CCPPointerInfo* Info;
 CCountedPointer():Data(0),Info(0){}
 CCountedPointer(const CCountedPointer& o):Data(o.Data),Info(o.Info){if(Info)++Info->RefCount;}
 ~CCountedPointer(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}}
};
class C3DAnimationStats { public:
 // No header exists. PDB stats Duration+0C; native6F914E independently reads+0C.
 unsigned char NativeStatsPrefix[12];float Duration;
 float GetEventTime(const CCharString&)const; //00A4EAE0
};
class C3DAnimationInfo { public:
 CCountedPointer<const C3DAnimationStats> GetAnimationStats()const; //00A269C0
};
class CMeshDataBank { public:
 // NativeA242C0 returns an indexed pointer from table+380; original inline getter spelling and full bank layout remain unestablished.
public:
    const C3DAnimationInfo* PeekAnimation(unsigned long) const;   // 0x00A242C0
};
extern float g_MinEmbeddedAnimationDuration; //0129BA3C exactfloat0.0001
namespace NAnimationEvents {class CManager { public:
 unsigned long NativeManagerPrefix;CMeshDataBank* MeshDataBank;
 float GetEmbeddedEventTime(unsigned long,const CCharString&)const;
};}
float NAnimationEvents::CManager::GetEmbeddedEventTime(unsigned long animation,const CCharString& event)const{
 CCountedPointer<const C3DAnimationStats> stats=MeshDataBank->PeekAnimation(animation)->GetAnimationStats();
 if(stats.Data->Duration>g_MinEmbeddedAnimationDuration){
  float duration=stats.Data->Duration;
  return stats.Data->GetEventTime(event)/duration;
 }
 return 0.0f;
}