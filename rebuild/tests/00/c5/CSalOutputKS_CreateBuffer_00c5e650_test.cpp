#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#ifdef _MSC_VER
typedef __int64 TestInt64;typedef unsigned __int64 TestUInt64;
#define FIXTURE_PACKED
#else
#define __cdecl
#define __stdcall
#define __fastcall
typedef long TestInt64;
typedef unsigned long TestUInt64;
#define FIXTURE_PACKED __attribute__((packed))
#endif
// Fixture substitutes fixed-width native words, exposes private state and mocks OS boundaries.
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

#ifndef FABLE_RETAIL_CIMAADPCM_CODEC_H
#define FABLE_RETAIL_CIMAADPCM_CODEC_H
// Original Microsoft Xbox IMA ADPCM SDK interface, 2001.
// Copyright (C) 2001 Microsoft Corporation. All Rights Reserved.
// Layout: SDK imaadpcm.h and native Convert/Initialize: format+0, mode+20,
// left index+24, right index+28, converter+32, size36 in the native ABI.
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int BOOL;
typedef int INT;
typedef unsigned int UINT;
typedef TestUInt64 ULONGLONG;
typedef BYTE* LPBYTE;
typedef INT* LPINT;
typedef const void* LPCVOID;
typedef void* LPVOID;
// Original mmreg.h type definitions and its standard SDK include boundary.
// Copyright (C) Microsoft Corporation, 1991 - 1999.
#ifdef _MSC_VER
#include <pshpack1.h>
#endif
typedef struct FIXTURE_PACKED tWAVEFORMATEX {
 WORD wFormatTag,nChannels;
 DWORD nSamplesPerSec,nAvgBytesPerSec;
 WORD nBlockAlign,wBitsPerSample,cbSize;
} WAVEFORMATEX;
typedef WAVEFORMATEX* LPWAVEFORMATEX;
typedef struct ima_adpcmwaveformat_tag {
 WAVEFORMATEX wfx;WORD wSamplesPerBlock;
} IMAADPCMWAVEFORMAT;
typedef IMAADPCMWAVEFORMAT* LPIMAADPCMWAVEFORMAT;
typedef const WAVEFORMATEX* LPCWAVEFORMATEX;
typedef const IMAADPCMWAVEFORMAT* LPCIMAADPCMWAVEFORMAT;
#ifdef _MSC_VER
#include <poppack.h>
#endif
typedef BOOL (__fastcall* LPFNIMAADPCMCONVERT)(LPBYTE,LPBYTE,UINT,UINT,UINT,LPINT,LPINT);
enum CODEC_MODE {
 CODEC_MODE_DECODE,CODEC_MODE_ENCODE_NORMAL,CODEC_MODE_ENCODE_OPTIMIZE_WHOLE_FILE,CODEC_MODE_ENCODE_OPTIMIZE_EACH_BLOCK
};
class CImaAdpcmCodec {
public:
 static const short m_asNextStep[16];
 static const short m_asStep[89];
 IMAADPCMWAVEFORMAT m_wfxEncode;
 CODEC_MODE m_cmCodecMode;
 int m_nStepIndexL,m_nStepIndexR;
 LPFNIMAADPCMCONVERT m_pfnConvert;
public:
 CImaAdpcmCodec();~CImaAdpcmCodec();
 BOOL Initialize(LPCIMAADPCMWAVEFORMAT,CODEC_MODE);
 WORD GetEncodeAlignment();WORD GetDecodeAlignment();WORD GetSourceAlignment();WORD GetDestinationAlignment();
 BOOL Convert(LPCVOID,LPVOID,UINT);void Reset();
 static void __fastcall CreatePcmFormat(WORD,DWORD,LPWAVEFORMATEX);
 static void __fastcall CreateImaAdpcmFormat(WORD,DWORD,WORD,LPIMAADPCMWAVEFORMAT);
 static BOOL __fastcall IsValidPcmFormat(LPCWAVEFORMATEX);
 static BOOL __fastcall IsValidImaAdpcmFormat(LPCIMAADPCMWAVEFORMAT);
public:
 static WORD __fastcall CalculateEncodeAlignment(WORD,WORD);
 static BOOL __fastcall EncodeM16(LPBYTE,LPBYTE,UINT,UINT,UINT,LPINT,LPINT);
 static BOOL __fastcall EncodeS16(LPBYTE,LPBYTE,UINT,UINT,UINT,LPINT,LPINT);
 static BOOL __fastcall DecodeM16(LPBYTE,LPBYTE,UINT,UINT,UINT,LPINT,LPINT);
 static BOOL __fastcall DecodeS16(LPBYTE,LPBYTE,UINT,UINT,UINT,LPINT,LPINT);
 static int __fastcall EncodeSample(int,int*,int);
 static int __fastcall DecodeSample(int,int,int);
 static int __fastcall NextStepIndex(int,int);static BOOL __fastcall ValidStepIndex(int);
 ULONGLONG CalcDifference(LPBYTE,LPBYTE,UINT,UINT,DWORD);
};
#endif

#ifndef FABLE_KERNEL_STREAMING_H
#define FABLE_KERNEL_STREAMING_H
// Original public Windows KS, mmreg and Win32 definitions, confirmed by SDK COFF.
typedef void* HANDLE;
struct HINSTANCE__;typedef HINSTANCE__* HMODULE;
struct HWND__;typedef HWND__* HWND;
typedef struct _SECURITY_ATTRIBUTES {DWORD nLength;void* lpSecurityDescriptor;BOOL bInheritHandle;} SECURITY_ATTRIBUTES;
typedef struct _OVERLAPPED {
 DWORD Internal,InternalHigh;
 DWORD Offset,OffsetHigh;
 HANDLE hEvent;
} OVERLAPPED;
struct _RTL_CRITICAL_SECTION_DEBUG;
typedef struct _RTL_CRITICAL_SECTION {
 _RTL_CRITICAL_SECTION_DEBUG* DebugInfo;
 int LockCount,RecursionCount;
 HANDLE OwningThread,LockSemaphore;
 unsigned int SpinCount;
} CRITICAL_SECTION;
typedef struct {GUID Set;DWORD Id;DWORD Flags;} KSIDENTIFIER;
typedef struct {DWORD PriorityClass,PrioritySubClass;} KSPRIORITY;
typedef struct {KSIDENTIFIER Interface,Medium;DWORD PinId;HANDLE PinToHandle;KSPRIORITY Priority;} KSPIN_CONNECT;
typedef struct {DWORD FormatSize,Flags,SampleSize,Reserved;GUID MajorFormat,SubFormat,Specifier;} KSDATAFORMAT;
typedef struct tWAVEFORMATEXTENSIBLE {
 WAVEFORMATEX Format;
 union {WORD wValidBitsPerSample,wSamplesPerBlock,wReserved;} Samples;
 DWORD dwChannelMask;
 GUID SubFormat;
} WAVEFORMATEXTENSIBLE;
typedef struct {TestInt64 Time;DWORD Numerator,Denominator;} KSTIME;
typedef struct {DWORD Size,TypeSpecificFlags;KSTIME PresentationTime;TestInt64 Duration;DWORD FrameExtent,DataUsed;void* Data;DWORD OptionsFlags;} KSSTREAM_HEADER;
enum KSSTATE {KSSTATE_STOP,KSSTATE_ACQUIRE,KSSTATE_PAUSE,KSSTATE_RUN};
typedef int (__stdcall* FARPROC)();
typedef DWORD (__stdcall* LPTHREAD_START_ROUTINE)(void*);
extern "C" {
  HMODULE __stdcall TestLoadLibraryA(const char*);
  FARPROC __stdcall TestGetProcAddress(HMODULE,const char*);
  BOOL __stdcall TestFreeLibrary(HMODULE);
  HANDLE __stdcall TestCreateEventA(SECURITY_ATTRIBUTES*,BOOL,BOOL,const char*);
  BOOL __stdcall TestSetThreadPriority(HANDLE,int);
}





#endif

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

#ifndef FABLE_CODA_MEMORY_H
#define FABLE_CODA_MEMORY_H
// Original SDK Memory.obj matches both native callbacks and default allocation bodies.
extern "C" void* __cdecl CodaMalloc(unsigned int);
extern "C" void __cdecl CodaFree(void*);
#endif

#ifndef FABLE_CSAL_OUTPUT_KS_H
#define FABLE_CSAL_OUTPUT_KS_H
// All class and member names and access levels from original GameCODA SDK COFF.
class CSalFile {
public:
 virtual ~CSalFile();
 void Attach(HANDLE);
 void Close();
 operator void*(){return m_hDevice;}
public:HANDLE m_hDevice;
};
class CSalKsPin:public CSalFile {
public:
 virtual ~CSalKsPin();
 bool SetConnectionState(KSSTATE);
};
class CSalEvent {
public:
 virtual ~CSalEvent();
 bool Create(SECURITY_ATTRIBUTES* attributes,bool manual,bool initial,const char* name){m_handle=TestCreateEventA(attributes,manual,initial,name);return m_handle!=0;}
 void Close();
 operator void*(){return m_handle;}
public:HANDLE m_handle;
};
class CSalThread {
public:
 virtual ~CSalThread();
 bool Create(LPTHREAD_START_ROUTINE,void*,SECURITY_ATTRIBUTES*,unsigned int,unsigned int);
 void SetPriority(int priority){TestSetThreadPriority(m_handle,priority);}
public:HANDLE m_handle;DWORD m_threadId;
};
class CSalMutex {
public:virtual ~CSalMutex();
public:CRITICAL_SECTION m_critSection;bool m_bActive;
};
class CSalOutput:public CCodaObjectBase {
public:
 virtual ~CSalOutput();
 virtual bool Create(const GUID*,HWND,bool)=0;
 virtual void Release()=0;
 virtual bool CreateBuffer(unsigned int,unsigned int)=0;
 virtual void ReleaseBuffer()=0;
 virtual bool FrameIsReady()=0;
 virtual void* FrameLock()=0;
 virtual void FrameUnlock(void*)=0;
 virtual unsigned int GetMinBufferSamples()const=0;
};
struct KsPacketHeader {KSSTREAM_HEADER header;OVERLAPPED signal;};
// Layout: native receiver offsets and SDK full0x60 class agree.
class CSalOutputKS:public CSalOutput {
public:
 virtual ~CSalOutputKS();
 virtual bool Create(const GUID*,HWND,bool);
 virtual void Release();
 virtual bool CreateBuffer(unsigned int,unsigned int);
 virtual void ReleaseBuffer();
 virtual bool FrameIsReady();
 virtual void* FrameLock();
 virtual void FrameUnlock(void*);
 virtual unsigned int GetMinBufferSamples()const;
public:
 static unsigned int __stdcall NotifyProc(void*);
 CSalFile m_filter;
 CSalKsPin m_pin;
 KsPacketHeader* m_pPacketHeaderTable;
 HANDLE* m_pEventTable;
 char* m_pBuffer;
 unsigned int m_nFrames,m_nFrameCount,m_nFrameIndex;
 CSalEvent m_eventTerminate;
 CSalThread m_threadNotify;
 CSalMutex m_mutexFrameCount;
};
#endif

const GUID fixture_KSINTERFACESETID_Standard={0x1a8766a0,0x62ce,0x11cf,{0xa5,0xd6,0x28,0xdb,0x04,0xc1,0x00,0x00}};

const GUID fixture_KSMEDIUMSETID_Standard={0x4747b320,0x62ce,0x11cf,{0xa5,0xd6,0x28,0xdb,0x04,0xc1,0x00,0x00}};

const GUID fixture_KSDATAFORMAT_TYPE_AUDIO={0x73647561,0x0000,0x0010,{0x80,0x00,0x00,0xaa,0x00,0x38,0x9b,0x71}};

const GUID fixture_KSDATAFORMAT_SUBTYPE_PCM={0x00000001,0x0000,0x0010,{0x80,0x00,0x00,0xaa,0x00,0x38,0x9b,0x71}};

const GUID fixture_KSDATAFORMAT_SPECIFIER_WAVEFORMATEX={0x05589f81,0xc356,0x11ce,{0xbf,0x01,0x00,0xaa,0x00,0x55,0x59,0x5a}};

static unsigned scenario,channelsExpected,samplesExpected,pinCalls,eventCalls,allocCalls,freeCalls,threadCalls,stateCalls,libraryFreeCalls,eventCloseCalls,pinCloseCalls;
static void* allocations[3];static bool released[3];static int handles[32];static CSalOutputKS* expectedOutput;
struct TestFormat {KSPIN_CONNECT connect;KSDATAFORMAT data;WAVEFORMATEXTENSIBLE wave;};
static bool sameGuid(const GUID& a,const GUID& b){return memcmp(&a,&b,sizeof(GUID))==0;}
static DWORD __stdcall testCreatePin(HANDLE filter,KSPIN_CONNECT* connect,DWORD access,HANDLE* result){
 ++pinCalls;assert(filter==&handles[1]&&access==0xc0000000u);TestFormat* f=(TestFormat*)connect;
 assert(sameGuid(f->connect.Interface.Set,fixture_KSINTERFACESETID_Standard));assert(f->connect.Interface.Id==0&&f->connect.Interface.Flags==0);
 assert(sameGuid(f->connect.Medium.Set,fixture_KSMEDIUMSETID_Standard));assert(f->connect.Medium.Id==0&&f->connect.Medium.Flags==0);
 assert(f->connect.PinId==0&&f->connect.PinToHandle==0);assert(f->connect.Priority.PriorityClass==0x40000000&&f->connect.Priority.PrioritySubClass==1);
 assert(f->data.FormatSize==sizeof(KSDATAFORMAT)+sizeof(WAVEFORMATEXTENSIBLE));assert(f->data.Flags==0&&f->data.Reserved==0&&f->data.SampleSize==0);
 assert(sameGuid(f->data.MajorFormat,fixture_KSDATAFORMAT_TYPE_AUDIO)&&sameGuid(f->data.SubFormat,fixture_KSDATAFORMAT_SUBTYPE_PCM)&&sameGuid(f->data.Specifier,fixture_KSDATAFORMAT_SPECIFIER_WAVEFORMATEX));
 assert(f->wave.Format.wFormatTag==(pinCalls==1?0xfffe:1));assert(f->wave.Format.cbSize==(pinCalls==1?22:0));
 assert(f->wave.Format.nChannels==channelsExpected&&f->wave.Format.nSamplesPerSec==44100&&f->wave.Format.wBitsPerSample==16);
 assert(f->wave.Format.nBlockAlign==channelsExpected*2&&f->wave.Format.nAvgBytesPerSec==channelsExpected*2*44100);
 assert(f->wave.Samples.wValidBitsPerSample==16&&sameGuid(f->wave.SubFormat,fixture_KSDATAFORMAT_SUBTYPE_PCM));
 unsigned expectedMask=channelsExpected==2?3:channelsExpected==4?0x33:channelsExpected==5?0x37:channelsExpected==6?0x3f:0x63f;assert(f->wave.dwChannelMask==expectedMask);
 if(scenario==3||(scenario==4&&pinCalls==1))return 5;
 *result=&handles[2];return 0;
}
extern "C" HMODULE __stdcall TestLoadLibraryA(const char* name){assert(strcmp(name,"ksuser.dll")==0);return scenario==1?0:(HMODULE)&handles[0];}
extern "C" FARPROC __stdcall TestGetProcAddress(HMODULE module,const char* name){assert(module==(HMODULE)&handles[0]&&strcmp(name,"KsCreatePin")==0);return scenario==2?0:(FARPROC)testCreatePin;}
extern "C" BOOL __stdcall TestFreeLibrary(HMODULE module){assert(module==(HMODULE)&handles[0]);++libraryFreeCalls;return 1;}
extern "C" HANDLE __stdcall TestCreateEventA(SECURITY_ATTRIBUTES* attributes,BOOL manual,BOOL initial,const char* name){assert(!attributes&&!manual&&!initial&&!name);++eventCalls;if((scenario==5&&eventCalls==1)||(scenario==12&&eventCalls==2))return 0;return &handles[3+eventCalls];}
extern "C" BOOL __stdcall TestSetThreadPriority(HANDLE handle,int priority){assert(handle==&handles[20]&&priority==2);return 1;}
extern "C" void* __cdecl CodaMalloc(unsigned int bytes){unsigned i=allocCalls++;assert(i<3);if(scenario==6+i)return 0;unsigned n=samplesExpected/128;assert(bytes==(i==0?n*channelsExpected*256:i==1?n*sizeof(KsPacketHeader):(n+1)*sizeof(HANDLE)));void* p=malloc(bytes?bytes:1);assert(p);memset(p,0x79,bytes?bytes:1);allocations[i]=p;return p;}
extern "C" void __cdecl CodaFree(void* p){unsigned i=0;while(i<3&&allocations[i]!=p)++i;assert(i<3&&!released[i]);released[i]=true;++freeCalls;free(p);}
CCodaObjectBase::~CCodaObjectBase(){} CSalOutput::~CSalOutput(){} CSalOutputKS::~CSalOutputKS(){}
CSalFile::~CSalFile(){} CSalKsPin::~CSalKsPin(){} CSalEvent::~CSalEvent(){} CSalThread::~CSalThread(){} CSalMutex::~CSalMutex(){}
void CSalFile::Attach(HANDLE p){assert(this==&expectedOutput->m_pin&&p==&handles[2]);m_hDevice=p;}
void CSalFile::Close(){assert(this==&expectedOutput->m_pin);++pinCloseCalls;m_hDevice=0;}
void CSalEvent::Close(){assert(this==&expectedOutput->m_eventTerminate);++eventCloseCalls;m_handle=0;}
bool CSalThread::Create(LPTHREAD_START_ROUTINE proc,void* arg,SECURITY_ATTRIBUTES* attributes,unsigned int size,unsigned int flags){assert(this==&expectedOutput->m_threadNotify&&proc==CSalOutputKS::NotifyProc&&arg==expectedOutput&&!attributes&&!size&&!flags);++threadCalls;m_handle=&handles[20];return scenario!=9;}
bool CSalKsPin::SetConnectionState(KSSTATE value){++stateCalls;assert(this==&expectedOutput->m_pin&&value==(stateCalls==1?KSSTATE_PAUSE:KSSTATE_RUN));return !(scenario==10&&stateCalls==1||scenario==11&&stateCalls==2);}
DWORD __stdcall CSalOutputKS::NotifyProc(void*){assert(false);return 0;}
bool CSalOutputKS::Create(const GUID*,HWND,bool){assert(false);return false;}void CSalOutputKS::Release(){assert(false);}void CSalOutputKS::ReleaseBuffer(){assert(false);}bool CSalOutputKS::FrameIsReady(){assert(false);return false;}void* CSalOutputKS::FrameLock(){assert(false);return 0;}void CSalOutputKS::FrameUnlock(void*){assert(false);}unsigned int CSalOutputKS::GetMinBufferSamples()const{assert(false);return 0;}
// bool CSalOutputKS::CreateBuffer(unsigned int,unsigned int) -- configure kernel-streaming PCM pin, packets and notification thread.
// Leaf: ?CreateBuffer@CSalOutputKS@@UAE_NII@Z
// Flags: /GS
#include <string.h>
bool CSalOutputKS::CreateBuffer(unsigned int channels,unsigned int samples)
{
 HMODULE library=TestLoadLibraryA("ksuser.dll");
 if(!library)return false;
 typedef DWORD (__stdcall* PKSCREATEPIN)(HANDLE,KSPIN_CONNECT*,DWORD,HANDLE*);
 PKSCREATEPIN createPin=(PKSCREATEPIN)TestGetProcAddress(library,"KsCreatePin");
 if(!createPin){TestFreeLibrary(library);return false;}
 struct {KSPIN_CONNECT connect;KSDATAFORMAT data;WAVEFORMATEXTENSIBLE wave;} format;
 memset(&format,0,sizeof(format));
 format.connect.Interface.Set=fixture_KSINTERFACESETID_Standard;
 format.connect.Interface.Id=0;format.connect.Interface.Flags=0;
 format.connect.Medium.Set=fixture_KSMEDIUMSETID_Standard;
 format.connect.Medium.Id=0;format.connect.Medium.Flags=0;
 format.connect.PinId=0;format.connect.PinToHandle=0;
 format.connect.Priority.PriorityClass=0x40000000;
 format.connect.Priority.PrioritySubClass=1;
 format.data.FormatSize=sizeof(KSDATAFORMAT)+sizeof(WAVEFORMATEXTENSIBLE);
 format.data.Flags=0;format.data.Reserved=0;
 format.data.MajorFormat=fixture_KSDATAFORMAT_TYPE_AUDIO;
 format.data.SubFormat=fixture_KSDATAFORMAT_SUBTYPE_PCM;
 format.data.Specifier=fixture_KSDATAFORMAT_SPECIFIER_WAVEFORMATEX;
 format.wave.Format.wFormatTag=0xfffe;
 format.wave.Format.cbSize=22;
 format.wave.Format.nChannels=(WORD)channels;
 format.wave.Format.nSamplesPerSec=44100;
 format.wave.Format.wBitsPerSample=16;
 format.wave.Format.nBlockAlign=(WORD)(channels*2);
 format.wave.Format.nAvgBytesPerSec=44100*format.wave.Format.nBlockAlign;
 format.wave.Samples.wValidBitsPerSample=16;
 switch(channels){
 case 2:format.wave.dwChannelMask=3;break;
 case 4:format.wave.dwChannelMask=0x33;break;
 case 5:format.wave.dwChannelMask=0x37;break;
 case 6:format.wave.dwChannelMask=0x3f;break;
 default:format.wave.dwChannelMask=0x63f;break;
 }
 format.wave.SubFormat=fixture_KSDATAFORMAT_SUBTYPE_PCM;
 HANDLE pin;
 DWORD result=createPin(m_filter,&format.connect,0xc0000000,&pin);
 if(result){format.wave.Format.wFormatTag=1;format.wave.Format.cbSize=0;result=createPin(m_filter,&format.connect,0xc0000000,&pin);}
 TestFreeLibrary(library);
 if(result)return false;
 m_pin.Attach(pin);
 if(!m_eventTerminate.Create(0,false,false,0))return false;
 m_nFrames=samples/128;
 m_nFrameCount=0;m_nFrameIndex=0;
 unsigned int frameBytes=channels*256;
 m_pBuffer=(char*)CodaMalloc(m_nFrames*frameBytes);
 if(!m_pBuffer){m_eventTerminate.Close();m_pin.Close();return false;}
 m_pPacketHeaderTable=(KsPacketHeader*)CodaMalloc(m_nFrames*sizeof(KsPacketHeader));
 if(!m_pPacketHeaderTable){CodaFree(m_pBuffer);m_eventTerminate.Close();m_pin.Close();return false;}
 memset(m_pPacketHeaderTable,0,m_nFrames*sizeof(KsPacketHeader));
 m_pEventTable=(HANDLE*)CodaMalloc((m_nFrames+1)*sizeof(HANDLE));
 if(!m_pEventTable){CodaFree(m_pPacketHeaderTable);CodaFree(m_pBuffer);m_eventTerminate.Close();m_pin.Close();return false;}
 m_pEventTable[m_nFrames]=m_eventTerminate;
 for(unsigned int i=0;i<m_nFrames;++i){
  m_pPacketHeaderTable[i].header.Size=sizeof(KSSTREAM_HEADER);
  m_pPacketHeaderTable[i].header.FrameExtent=frameBytes;
  m_pPacketHeaderTable[i].header.DataUsed=frameBytes;
  m_pPacketHeaderTable[i].header.Data=m_pBuffer+i*frameBytes;
  m_pEventTable[i]=TestCreateEventA(0,false,false,0);
  m_pPacketHeaderTable[i].signal.hEvent=m_pEventTable[i];
 }
 if(!m_threadNotify.Create(NotifyProc,this,0,0,0))return false;
 m_threadNotify.SetPriority(2);
 if(!m_pin.SetConnectionState(KSSTATE_PAUSE))return false;
 if(!m_pin.SetConnectionState(KSSTATE_RUN))return false;
 return true;
}

int main(){
#ifdef _MSC_VER
 assert(sizeof(CSalOutputKS)==0x60&&sizeof(KsPacketHeader)==72&&sizeof(KSSTREAM_HEADER)==48);
 assert(sizeof(KSPIN_CONNECT)==64&&sizeof(KSDATAFORMAT)==64&&sizeof(WAVEFORMATEXTENSIBLE)==40);
 assert(offsetof(CSalOutputKS,m_filter)==4&&offsetof(CSalOutputKS,m_pin)==12&&offsetof(CSalOutputKS,m_mutexFrameCount)==64);
#endif
 unsigned sizes[]={0,127,128,256,385};
 for(channelsExpected=1;channelsExpected<=8;++channelsExpected)for(unsigned j=0;j<5;++j)for(scenario=0;scenario<=12;++scenario){
  samplesExpected=sizes[j];pinCalls=eventCalls=allocCalls=freeCalls=threadCalls=stateCalls=libraryFreeCalls=eventCloseCalls=pinCloseCalls=0;
  for(unsigned k=0;k<3;++k){allocations[k]=0;released[k]=false;}
  CSalOutputKS output;expectedOutput=&output;output.m_filter.m_hDevice=&handles[1];output.m_pin.m_hDevice=0;output.m_eventTerminate.m_handle=0;
  output.m_pBuffer=0;output.m_pPacketHeaderTable=0;output.m_pEventTable=0;
  bool good=output.CreateBuffer(channelsExpected,samplesExpected);assert(good==(scenario==0||scenario==4||scenario==12));
  assert(libraryFreeCalls==(scenario==1?0u:1u));
  assert(pinCalls==(scenario==1||scenario==2?0u:scenario==3||scenario==4?2u:1u));
  assert(eventCloseCalls==((scenario>=6&&scenario<=8)?1u:0u)&&pinCloseCalls==eventCloseCalls);
  if(scenario==7)assert(freeCalls==1);else if(scenario==8)assert(freeCalls==2);else assert(freeCalls==0);
  if(scenario==0||scenario==4||scenario>=9){
   unsigned n=samplesExpected/128;assert(output.m_nFrames==n&&output.m_nFrameCount==0&&output.m_nFrameIndex==0);
   assert(output.m_pEventTable[n]==output.m_eventTerminate.m_handle&&eventCalls==n+1&&threadCalls==1);
   for(unsigned i=0;i<n;++i){
    const KsPacketHeader& p=output.m_pPacketHeaderTable[i];
    assert(p.header.Size==sizeof(KSSTREAM_HEADER)&&p.header.TypeSpecificFlags==0&&p.header.PresentationTime.Time==0&&p.header.Duration==0);
    assert(p.header.FrameExtent==channelsExpected*256&&p.header.DataUsed==channelsExpected*256&&p.header.OptionsFlags==0);
    assert(p.header.Data==output.m_pBuffer+i*channelsExpected*256);
    HANDLE expected=scenario==12&&i==0?0:&handles[5+i];assert(output.m_pEventTable[i]==expected&&p.signal.hEvent==expected);
    assert(p.signal.Internal==0&&p.signal.InternalHigh==0&&p.signal.Offset==0&&p.signal.OffsetHigh==0);
   }
   assert(stateCalls==(scenario==9?0u:scenario==10?1u:2u));
  }
  for(unsigned i=0;i<3;++i)if(allocations[i]&&!released[i])free(allocations[i]);
 }
 puts("00c5e650_TEST PASS");return 0;
}