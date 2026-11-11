// Retail opt-in (Codex pass20): SDK CodeView and native00c3d470; PROCESS branch only, default preserved.
#ifndef FABLE_CODA_ENVIRONMENT_H
#define FABLE_CODA_ENVIRONMENT_H
// GameCODA2.10 public structures and original SDK COFF types.
#include "CCodaObjectBase.h"
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
#ifdef FABLE_RETAIL_CCODACONTEXT_PROCESS
#include "engine/CCodaObject.h"
#else
class CCodaObject:public CCodaObjectBase {
public:
 virtual ~CCodaObject();
private:
 CCodaObject* m_pPrev;
 CCodaObject* m_pNext;
};
#endif
class CSalReverb;
class CCodaEnvironment:public CCodaObject {
public:
 virtual ~CCodaEnvironment();
#ifdef FABLE_RETAIL_CCODACONTEXT_PROCESS
 void Commit();
 void Process();
 static unsigned int GetSourceCommit(){return m_ffSourceCommit;}
 static void ClearSourceCommit(){m_ffSourceCommit=0;}
private:
 static unsigned int m_ffSourceCommit;
public:
#endif
 const CENVIRONMENT& GetParams()const{return m_Params;}
 unsigned int GetCommit()const{return m_ffCommit;}
private:
 CENVIRONMENT m_Params;
 unsigned int m_ffCommit;
 CSalReverb* m_pReverb;
};
#endif
