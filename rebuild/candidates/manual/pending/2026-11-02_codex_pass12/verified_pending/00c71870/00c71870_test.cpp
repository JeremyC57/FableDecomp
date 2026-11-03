#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#ifndef _MSC_VER
#define __cdecl
#endif
// Fixture-only: private/protected data access is opened for controlled setup.
#ifndef FABLE_CODA_ENVIRONMENT_H
#define FABLE_CODA_ENVIRONMENT_H
// GameCODA2.10 public structures and original SDK COFF types.
typedef unsigned int CUINT32;
typedef struct _CVECTOR                             /* Cartesian vector structure for listener and  */
{                                                   /* 3D source parameters                         */
    float   x;                                      /* X, Y, Z co-ordinates                         */
    float   y;
    float   z;
}
CVECTOR;
typedef struct _CENVIRONMENT                        /* Environment parameters                       */
{                                       
    CVECTOR vReflectionsPan;                        /* Reflections position control vector          */
    CVECTOR vReverbPan;                             /* Reverb position control vector               */
    int     iLevel;                                 /* in mB                                        */
    int     iLevelHF;                               /* in mB                                        */
    int     iLevelLF;                               /* in mB                                        */
    float   fDecayTime;                             /* -60 dB decay time in seconds                 */
    float   fDecayHFRatio;                          /* Fraction of time for HF to decay             */
    float   fDecayLFRatio;                          /* Fraction of time for LF to decay             */
    int     iReflections;                           /* Early reflections level in mB                */
    float   fReflectionsDelay;                      /* Early reflections delay in seconds           */
    int     iReverb;                                /* Late reverberation level in mB               */
    float   fReverbDelay;                           /* Late reverberation delay in seconds          */
    float   fSize;                                  /* Room size in metres                          */                      
    float   fDiffusion;                             /* 0 - 1                                        */
    float   fDensity;                               /* 0 - 1                                        */
    float   fEchoTime;                              /* Time lag between echoes in seconds           */
    float   fEchoDepth;                             /* Echo effect strength                         */
    float   fModulationTime;                        /* Pitch modulation period in seconds           */         
    float   fModulationDepth;                       /* Pitch modulation strength                    */
    CUINT32 uiHFReference;                          /* Frequency for HF effects                     */
    CUINT32 uiLFReference;                          /* Frequency for LF effects                     */
    float   fRolloff;                               /* Rolloff with distance                        */
    int     iAirAbsorptionHF;                       /* Air absorption in mB/unit                    */
}
CENVIRONMENT;
class CCodaObjectBase {
public:
 virtual ~CCodaObjectBase();
};
class CCodaObject:public CCodaObjectBase {
public:
 virtual ~CCodaObject();
public:
 CCodaObject* m_pPrev;
 CCodaObject* m_pNext;
};
class CSalReverb;
class CCodaEnvironment:public CCodaObject {
public:
 virtual ~CCodaEnvironment();
 const CENVIRONMENT& GetParams()const{return m_Params;}
 unsigned int GetCommit()const{return m_ffCommit;}
public:
 CENVIRONMENT m_Params;
 unsigned int m_ffCommit;
 CSalReverb* m_pReverb;
};
#endif

#ifndef FABLE_EAX40_TYPES_H
#define FABLE_EAX40_TYPES_H
// Creative EAX 4.0 API, revision 1.6.3, 21 February 2003.
// Original public SDK tags and members; all fields naturally align to four bytes.
#define EAX_MAX_ACTIVE_FXSLOTS 2
extern "C" {
typedef struct _GUID
        {
            unsigned int Data1;
            unsigned short Data2;
            unsigned short Data3;
            unsigned char Data4[8];
        } GUID;

typedef struct _EAXCONTEXTPROPERTIES
{
    GUID          guidPrimaryFXSlotID;
    float         flDistanceFactor;
    float         flAirAbsorptionHF;
    float         flHFReference;
} EAXCONTEXTPROPERTIES, *LPEAXCONTEXTPROPERTIES;

typedef struct _EAXSOURCEPROPERTIES
{
    int          lDirect;                 // direct path level (at low and mid frequencies)
    int          lDirectHF;               // relative direct path level at high frequencies
    int          lRoom;                   // room effect level (at low and mid frequencies)
    int          lRoomHF;                 // relative room effect level at high frequencies
    int          lObstruction;            // main obstruction control (attenuation at high frequencies)
    float         flObstructionLFRatio;    // obstruction low-frequency level re. main control
    int          lOcclusion;              // main occlusion control (attenuation at high frequencies)
    float         flOcclusionLFRatio;      // occlusion low-frequency level re. main control
    float         flOcclusionRoomRatio;    // relative occlusion control for room effect
    float         flOcclusionDirectRatio;  // relative occlusion control for direct path
    int          lExclusion;              // main exlusion control (attenuation at high frequencies)
    float         flExclusionLFRatio;      // exclusion low-frequency level re. main control
    int          lOutsideVolumeHF;        // outside sound cone level at high frequencies
    float         flDopplerFactor;         // like DS3D flDopplerFactor but per source
    float         flRolloffFactor;         // like DS3D flRolloffFactor but per source
    float         flRoomRolloffFactor;     // like DS3D flRolloffFactor but for room effect
    float         flAirAbsorptionFactor;   // multiplies EAXREVERB_AIRABSORPTIONHF
    unsigned int ulFlags;                 // modifies the behavior of properties
} EAXSOURCEPROPERTIES, *LPEAXSOURCEPROPERTIES;

typedef struct _EAXSOURCEALLSENDPROPERTIES
{
	GUID          guidReceivingFXSlotID;
	int          lSend;                   // send level (at low and mid frequencies)
	int          lSendHF;                 // relative send level at high frequencies
	int          lOcclusion;
	float         flOcclusionLFRatio;
	float         flOcclusionRoomRatio;
	float         flOcclusionDirectRatio;
	int          lExclusion;
	float         flExclusionLFRatio;
} EAXSOURCEALLSENDPROPERTIES, *LPEAXSOURCEALLSENDPROPERTIES;

typedef struct _EAXACTIVEFXSLOTS
{
    GUID          guidActiveFXSlots[EAX_MAX_ACTIVE_FXSLOTS];
} EAXACTIVEFXSLOTS, *LPEAXACTIVEFXSLOTS;

typedef struct _EAXOBSTRUCTIONPROPERTIES
{
    int          lObstruction;
    float         flObstructionLFRatio;
} EAXOBSTRUCTIONPROPERTIES, *LPEAXOBSTRUCTIONPROPERTIES;

typedef struct _EAXOCCLUSIONPROPERTIES
{
    int          lOcclusion;
    float         flOcclusionLFRatio;
    float         flOcclusionRoomRatio;
    float         flOcclusionDirectRatio;
} EAXOCCLUSIONPROPERTIES, *LPEAXOCCLUSIONPROPERTIES;

typedef struct _EAXEXCLUSIONPROPERTIES
{
    int          lExclusion;
    float         flExclusionLFRatio;
} EAXEXCLUSIONPROPERTIES, *LPEAXEXCLUSIONPROPERTIES;

typedef struct _EAXSOURCESENDPROPERTIES
{
    GUID          guidReceivingFXSlotID;
    int          lSend;
    int          lSendHF;
} EAXSOURCESENDPROPERTIES, *LPEAXSOURCESENDPROPERTIES;

typedef struct _EAXSOURCEOCCLUSIONSENDPROPERTIES
{
	GUID          guidReceivingFXSlotID;
	int          lOcclusion;
	float         flOcclusionLFRatio;
	float         flOcclusionRoomRatio;
	float         flOcclusionDirectRatio;
} EAXSOURCEOCCLUSIONSENDPROPERTIES, *LPEAXSOURCEOCCLUSIONSENDPROPERTIES;

typedef struct _EAXSOURCEEXCLUSIONSENDPROPERTIES
{
	GUID          guidReceivingFXSlotID;
	int          lExclusion;
	float         flExclusionLFRatio;
} EAXSOURCEEXCLUSIONSENDPROPERTIES, *LPEAXSOURCEEXCLUSIONSENDPROPERTIES;

typedef struct _EAXFXSLOTPROPERTIES
{
	GUID          guidLoadEffect;
	int          lVolume;
	int          lLock;
	unsigned int ulFlags;
} EAXFXSLOTPROPERTIES, *LPEAXFXSLOTPROPERTIES;

typedef struct _EAXVECTOR {
    float x;
    float y;
    float z;
} EAXVECTOR;

typedef struct _EAXREVERBPROPERTIES
{
    unsigned int ulEnvironment;   // sets all reverb properties
    float flEnvironmentSize;       // environment size in meters
    float flEnvironmentDiffusion;  // environment diffusion
    int lRoom;                    // room effect level (at mid frequencies)
    int lRoomHF;                  // relative room effect level at high frequencies
    int lRoomLF;                  // relative room effect level at low frequencies
    float flDecayTime;             // reverberation decay time at mid frequencies
    float flDecayHFRatio;          // high-frequency to mid-frequency decay time ratio
    float flDecayLFRatio;          // low-frequency to mid-frequency decay time ratio
    int lReflections;             // early reflections level relative to room effect
    float flReflectionsDelay;      // initial reflection delay time
    EAXVECTOR vReflectionsPan;     // early reflections panning vector
    int lReverb;                  // late reverberation level relative to room effect
    float flReverbDelay;           // late reverberation delay time relative to initial reflection
    EAXVECTOR vReverbPan;          // late reverberation panning vector
    float flEchoTime;              // echo time
    float flEchoDepth;             // echo depth
    float flModulationTime;        // modulation time
    float flModulationDepth;       // modulation depth
    float flAirAbsorptionHF;       // change in level per meter at high frequencies
    float flHFReference;           // reference high frequency
    float flLFReference;           // reference low frequency
    float flRoomRolloffFactor;     // like DS3D flRolloffFactor but for room effect
    unsigned int ulFlags;         // modifies the behavior of properties
} EAXREVERBPROPERTIES, *LPEAXREVERBPROPERTIES;

typedef enum
{
    EAXSOURCE_NONE,
    EAXSOURCE_ALLPARAMETERS,
    EAXSOURCE_OBSTRUCTIONPARAMETERS,
    EAXSOURCE_OCCLUSIONPARAMETERS,
    EAXSOURCE_EXCLUSIONPARAMETERS,
    EAXSOURCE_DIRECT,
    EAXSOURCE_DIRECTHF,
    EAXSOURCE_ROOM,
    EAXSOURCE_ROOMHF,
    EAXSOURCE_OBSTRUCTION,
    EAXSOURCE_OBSTRUCTIONLFRATIO,
    EAXSOURCE_OCCLUSION,
    EAXSOURCE_OCCLUSIONLFRATIO,
    EAXSOURCE_OCCLUSIONROOMRATIO,
    EAXSOURCE_OCCLUSIONDIRECTRATIO,
    EAXSOURCE_EXCLUSION,
    EAXSOURCE_EXCLUSIONLFRATIO,
    EAXSOURCE_OUTSIDEVOLUMEHF,
    EAXSOURCE_DOPPLERFACTOR,
    EAXSOURCE_ROLLOFFFACTOR,
    EAXSOURCE_ROOMROLLOFFFACTOR,
    EAXSOURCE_AIRABSORPTIONFACTOR,
    EAXSOURCE_FLAGS,
    EAXSOURCE_SENDPARAMETERS,
    EAXSOURCE_ALLSENDPARAMETERS,
    EAXSOURCE_OCCLUSIONSENDPARAMETERS,
    EAXSOURCE_EXCLUSIONSENDPARAMETERS,
    EAXSOURCE_ACTIVEFXSLOTID
} EAXSOURCE_PROPERTY;

typedef enum
{
    EAXFXSLOT_PARAMETER = 0, // range 0-0x40 reserved for loaded effect parameters
    EAXFXSLOT_NONE = 0x10000,
	EAXFXSLOT_ALLPARAMETERS,
    EAXFXSLOT_LOADEFFECT,
    EAXFXSLOT_VOLUME,
	EAXFXSLOT_LOCK,
	EAXFXSLOT_FLAGS
} EAXFXSLOT_PROPERTY;

typedef enum
{
    EAXREVERB_NONE,
    EAXREVERB_ALLPARAMETERS,
    EAXREVERB_ENVIRONMENT,
    EAXREVERB_ENVIRONMENTSIZE,
    EAXREVERB_ENVIRONMENTDIFFUSION,
    EAXREVERB_ROOM,
    EAXREVERB_ROOMHF,
    EAXREVERB_ROOMLF,
    EAXREVERB_DECAYTIME,
    EAXREVERB_DECAYHFRATIO,
    EAXREVERB_DECAYLFRATIO,
    EAXREVERB_REFLECTIONS,
    EAXREVERB_REFLECTIONSDELAY,
    EAXREVERB_REFLECTIONSPAN,
    EAXREVERB_REVERB,
    EAXREVERB_REVERBDELAY,
    EAXREVERB_REVERBPAN,
    EAXREVERB_ECHOTIME,
    EAXREVERB_ECHODEPTH,
    EAXREVERB_MODULATIONTIME,
    EAXREVERB_MODULATIONDEPTH,
    EAXREVERB_AIRABSORPTIONHF,
    EAXREVERB_HFREFERENCE,
    EAXREVERB_LFREFERENCE,
    EAXREVERB_ROOMROLLOFFFACTOR,
    EAXREVERB_FLAGS
} EAXREVERB_PROPERTY;
extern const GUID EAX_REVERB_EFFECT;
extern const GUID EAXPROPERTYID_EAX40_FXSlot0;
extern const GUID EAXPROPERTYID_EAX40_FXSlot1;
extern const GUID EAXPROPERTYID_EAX40_FXSlot2;
extern const GUID EAXPROPERTYID_EAX40_FXSlot3;

extern const GUID EAX_PrimaryFXSlotID;
}
#endif

#ifndef FABLE_CSAL_OPENAL_REVERB_H
#define FABLE_CSAL_OPENAL_REVERB_H
class CSalPlatform;
// Layout: SDK class names and hierarchy, native vtable012b95c4 has eight slots.
// Retail omits the later SDK's CCodaObject list base on this mix-only hierarchy.
class CSalReverb:public CCodaObjectBase {
public:
 virtual ~CSalReverb();
 virtual int Create(CSalPlatform*);
 virtual void Release();
 virtual bool IsSoftware()const;
 virtual void AddSwInput(const int*);
 virtual void Update(const CCodaEnvironment*)=0;
 virtual void Process()=0;
public:bool m_bActive;
};
class CSalBaseReverb:public CSalReverb {
public:
 virtual ~CSalBaseReverb();
 virtual void GetFrame(short*);
public:
 // Native ctor00c6ad70 points +21c at the aligned mix buffer inside this storage.
 unsigned char Opaque008[0x214];
 int* m_pMixBuffer;
};
// Layout: native ctor00c71710 adds no data; factory allocates0x220 bytes.
class CSalOpenALReverb:public CSalBaseReverb {
public:
 virtual ~CSalOpenALReverb();
 virtual int Create(CSalPlatform*);
 virtual void Release();
 virtual void Update(const CCodaEnvironment*);
 virtual void Process();
 virtual void GetFrame(short*);
};
#endif

#ifndef FABLE_CSAL_OPENAL_H
#define FABLE_CSAL_OPENAL_H
extern const GUID SAL_EAX30_ListenerProperties;
class CSalHw:public CCodaObjectBase {
public:virtual ~CSalHw();
};
// Layout: SDK CSalOpenAL members; native resolver and global object prove +e8/+f0.
// This retail library version has one extra pointer before those SDK slots.
// Entire 0x1b0 native object is preserved; unmodelled API pointers remain opaque.
class CSalOpenAL:public CSalHw {
public:
 virtual ~CSalOpenAL();
 void* ALGetProcAddress(unsigned char*);
 int EAXSet(const GUID* guid,unsigned int property,unsigned int source,void* value,unsigned int size){
  if(!m_pEAXSet)m_pEAXSet=(int(__cdecl*)(const GUID*,unsigned int,unsigned int,void*,unsigned int))ALGetProcAddress((unsigned char*)"EAXSet");
  return m_pEAXSet(guid,property,source,value,size);
 }
 int EAXSetGlobal(const GUID* guid,unsigned int property,void* value,unsigned int size){return EAXSet(guid,property,0,value,size);}
 int EAX3SetGlobal(unsigned int property,void* value,unsigned int size){return EAXSetGlobal(&SAL_EAX30_ListenerProperties,property,value,size);}
public:
 void* m_hModule;
 unsigned char Opaque008[0xe0];
 void* (__cdecl* m_pGetProcAddress)(unsigned char*);
 unsigned char Opaque0ec[4];
 int (__cdecl* m_pEAXSet)(const GUID*,unsigned int,unsigned int,void*,unsigned int);
 unsigned char Opaque0f4[0xbc];
};
extern CSalOpenAL g_OpenAL;
#endif

CCodaObjectBase::~CCodaObjectBase(){}
CCodaObject::~CCodaObject(){}
CCodaEnvironment::~CCodaEnvironment(){}
CSalReverb::~CSalReverb(){}
int CSalReverb::Create(CSalPlatform*){return 0;}
void CSalReverb::Release(){}
bool CSalReverb::IsSoftware()const{return false;}
void CSalReverb::AddSwInput(const int*){}
CSalBaseReverb::~CSalBaseReverb(){}
void CSalBaseReverb::GetFrame(short*){}
CSalOpenALReverb::~CSalOpenALReverb(){}
int CSalOpenALReverb::Create(CSalPlatform*){return 0;}
void CSalOpenALReverb::Release(){}
void CSalOpenALReverb::Process(){}
void CSalOpenALReverb::GetFrame(short*){}
CSalHw::~CSalHw(){}
CSalOpenAL::~CSalOpenAL(){}
const GUID SAL_EAX30_ListenerProperties={0xa8fa6882,0xb476,0x11d3,{0xbd,0xb9,0,0xc0,0xf0,0x2d,0xdf,0x87}};
CSalOpenAL g_OpenAL;
static unsigned propertyLog[20],sizeLog[20],calls,resolves,resetEach;
static unsigned char valueLog[20][12];static const void* pointerLog[20];
static int __cdecl record(const GUID* guid,unsigned property,unsigned source,void* value,unsigned size){
 assert(guid==&SAL_EAX30_ListenerProperties&&source==0&&calls<20&&(size==4||size==12));
 propertyLog[calls]=property;sizeLog[calls]=size;pointerLog[calls]=value;memcpy(valueLog[calls],value,size);++calls;
 if(resetEach)g_OpenAL.m_pEAXSet=0;return -8;
}
void* CSalOpenAL::ALGetProcAddress(unsigned char* name){assert(this==&g_OpenAL&&strcmp((char*)name,"EAXSet")==0);++resolves;return (void*)record;}
// CSalOpenALReverb::Update(const CCodaEnvironment*) -- submit changed environment parameters to EAX3.
// Leaf: ?Update@CSalOpenALReverb@@UAEXPBVCCodaEnvironment@@@Z
void CSalOpenALReverb::Update(const CCodaEnvironment* environment)
{
 unsigned int changed=environment->GetCommit();
 const CENVIRONMENT& params=environment->GetParams();
 if(!changed)return;
 if(changed&0x1)g_OpenAL.EAX3SetGlobal(5,(void*)&params.iLevel,sizeof(params.iLevel));
 if(changed&0x2)g_OpenAL.EAX3SetGlobal(6,(void*)&params.iLevelHF,sizeof(params.iLevelHF));
 if(changed&0x4)g_OpenAL.EAX3SetGlobal(7,(void*)&params.iLevelLF,sizeof(params.iLevelLF));
 if(changed&0x8)g_OpenAL.EAX3SetGlobal(8,(void*)&params.fDecayTime,sizeof(params.fDecayTime));
 if(changed&0x10)g_OpenAL.EAX3SetGlobal(9,(void*)&params.fDecayHFRatio,sizeof(params.fDecayHFRatio));
 if(changed&0x20)g_OpenAL.EAX3SetGlobal(10,(void*)&params.fDecayLFRatio,sizeof(params.fDecayLFRatio));
 if(changed&0x40)g_OpenAL.EAX3SetGlobal(11,(void*)&params.iReflections,sizeof(params.iReflections));
 if(changed&0x80)g_OpenAL.EAX3SetGlobal(12,(void*)&params.fReflectionsDelay,sizeof(params.fReflectionsDelay));
 if(changed&0x100)g_OpenAL.EAX3SetGlobal(13,(void*)&params.vReflectionsPan,sizeof(params.vReflectionsPan));
 if(changed&0x200)g_OpenAL.EAX3SetGlobal(14,(void*)&params.iReverb,sizeof(params.iReverb));
 if(changed&0x400)g_OpenAL.EAX3SetGlobal(15,(void*)&params.fReverbDelay,sizeof(params.fReverbDelay));
 if(changed&0x800)g_OpenAL.EAX3SetGlobal(16,(void*)&params.vReverbPan,sizeof(params.vReverbPan));
 if(changed&0x1000)g_OpenAL.EAX3SetGlobal(3,(void*)&params.fSize,sizeof(params.fSize));
 if(changed&0x2000)g_OpenAL.EAX3SetGlobal(4,(void*)&params.fDiffusion,sizeof(params.fDiffusion));
 if(changed&0x8000)g_OpenAL.EAX3SetGlobal(17,(void*)&params.fEchoTime,sizeof(params.fEchoTime));
 if(changed&0x10000)g_OpenAL.EAX3SetGlobal(18,(void*)&params.fEchoDepth,sizeof(params.fEchoDepth));
 if(changed&0x20000)g_OpenAL.EAX3SetGlobal(19,(void*)&params.fModulationTime,sizeof(params.fModulationTime));
 if(changed&0x40000)g_OpenAL.EAX3SetGlobal(20,(void*)&params.fModulationDepth,sizeof(params.fModulationDepth));
 if(changed&0x80000)g_OpenAL.EAX3SetGlobal(22,(void*)&params.uiHFReference,sizeof(params.uiHFReference));
 if(changed&0x80000)g_OpenAL.EAX3SetGlobal(23,(void*)&params.uiLFReference,sizeof(params.uiLFReference));
}

int main(){
 CSalOpenALReverb reverb;CCodaEnvironment environment;
 unsigned bits[20]={1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,32768,65536,131072,262144,524288,524288};
 unsigned props[20]={5,6,7,8,9,10,11,12,13,14,15,16,3,4,17,18,19,20,22,23};
 unsigned offsets[20]={24,28,32,36,40,44,48,52,0,56,60,12,64,68,76,80,84,88,92,96};
 for(unsigned i=0;i<sizeof(environment.m_Params)/4;++i)((unsigned*)&environment.m_Params)[i]=0x3f000000u+i*0x10000u;
 unsigned masks[90];masks[0]=0;masks[1]=0xffffffffu;masks[2]=0x4000;masks[3]=0x100000;
 for(unsigned i=0;i<20;++i){masks[4+i]=1u<<i;masks[24+i]=(1u<<i)|1u;}
 unsigned state=1234;for(unsigned i=44;i<90;++i){state=state*1664525u+1013904223u;masks[i]=state;}
 for(unsigned sample=0;sample<90;++sample)for(unsigned mode=0;mode<3;++mode){
  environment.m_ffCommit=masks[sample];calls=resolves=0;resetEach=mode==2;g_OpenAL.m_pEAXSet=mode==0?record:0;
  unsigned char before[sizeof(environment.m_Params)];memcpy(before,&environment.m_Params,sizeof(before));
  reverb.Update(&environment);
  unsigned expectedCalls=0;
  for(unsigned i=0;i<20;++i)if(masks[sample]&bits[i]){
   unsigned size=i==8||i==11?12:4;assert(propertyLog[expectedCalls]==props[i]&&sizeLog[expectedCalls]==size);
   assert(pointerLog[expectedCalls]==(char*)&environment.m_Params+offsets[i]);assert(memcmp(valueLog[expectedCalls],before+offsets[i],size)==0);++expectedCalls;
  }
  assert(calls==expectedCalls&&resolves==(mode==0?0:mode==1?(calls?1u:0u):calls));
  assert(environment.m_ffCommit==masks[sample]&&memcmp(before,&environment.m_Params,sizeof(before))==0);
 }
 puts("00c71870_TEST PASS");return 0;
}
