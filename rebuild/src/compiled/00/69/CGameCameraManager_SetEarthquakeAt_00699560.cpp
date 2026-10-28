// CGameCameraManager::SetEarthquakeAt. Actual manager/World donor headers;
// native EarthQuakes EC vs donor F8. CQuake48 agrees exactly with PDB.
#define CGameCameraManager CGameCameraManagerDonor
#define CWorld CWorldDonor
#include "engine/CGameCameraManager.h"
#include "engine/CWorld.h"
#undef CWorld
#undef CGameCameraManager
struct CQuakeVector {float X,Y,Z;void Rotate(const CQuakeVector&,float);}; //A13C60
struct CQuakeRandom {unsigned Seed;};
float __fastcall FloatRandom(float,CQuakeRandom*); //4978A0 genuine free-fastcall float stack/seedECX
long __fastcall GFROR13(unsigned long value); //497890 ?GFROR13@@YIJK@Z (Ego_r.pdb)
int __fastcall GetQuakeFrame(); //49D870 global13B89BC
struct CQuakeWorld {CQuakeRandom& GetRandom();}; //49D810 returns[World8]+1613C
struct CQuake {CQuakeVector Position,Axis;int Frame;float Amplitude,Duration,Frequency,Phase;bool Global;};
void __fastcall ConstructQuake(CQuake*,const CQuake&); //69C860 allocator helper nativeECXdest,EDXvalue
struct CQuakePodTag {CQuakePodTag(){}};
struct CQuakeArray {CQuake* Begin;CQuake* End;CQuake* Capacity;void Insert(CQuake*,const CQuake&,const CQuakePodTag&,unsigned,bool);
 __forceinline void Push(const CQuake& quake){if(End!=Capacity){ConstructQuake(End,quake);++End;}else Insert(End,quake,CQuakePodTag(),1,true);}
}; //69D300
class CGameCameraManager { public:
 unsigned char Prefix[0x10];CQuakeVector CameraAxis,CameraUp;unsigned char CameraTail[0x4c];CQuakeWorld* World;
 unsigned char BeforeQuakes[0x74];CQuakeArray EarthQuakes;
 void SetEarthquakeAt(const CQuakeVector&,float,float,bool);
};
#ifdef _MSC_VER
typedef char Quake48[sizeof(CQuake)==48?1:-1];
typedef char World74[offsetof(CGameCameraManager,World)==0x74?1:-1];
typedef char QuakesEC[offsetof(CGameCameraManager,EarthQuakes)==0xec?1:-1];
#endif
void CGameCameraManager::SetEarthquakeAt(const CQuakeVector& position,float amplitude,float duration,bool global){
 CQuake quake;quake.Position=position;quake.Axis=CameraAxis;
 quake.Axis.Rotate(CameraUp,FloatRandom(1.0f,&World->GetRandom()));
 quake.Frame=GetQuakeFrame();CQuakeRandom& random=World->GetRandom();random.Seed=random.Seed*9377u+9439u;random.Seed=GFROR13(random.Seed);
 quake.Phase=(0.5f-float(random.Seed&1u))*2.0f;
 quake.Amplitude=amplitude;quake.Duration=duration;quake.Frequency=2.0f;quake.Global=global;
 EarthQuakes.Push(quake);
}