#ifndef FABLE_CODA_PROCESSING_TYPES_H
#define FABLE_CODA_PROCESSING_TYPES_H
#include "engine/CCodaVector.h"
#define C_CAPS_3D_VOLPAN        (0x00400000)        /* 3D processing using volume panning           */
#define C_CAPS_3D_HRTF_DELAY    (0x00200000)        /* 3D processing using HRTF ITD only            */
#define C_CAPS_3D_HRTF_FULL     (0x00100000)        /* Full 3D HRTF processing                      */
typedef enum                                        /* Output configurations                        */
{
    C_OUTPUT_NONE           =   (0x00000000),       /* Not initialized / no output                  */
    C_OUTPUT_HEADPHONES     =   (0x00000001),       /* Analogue outputs                             */
    C_OUTPUT_MONO           =   (0x00000002),
    C_OUTPUT_STEREO         =   (0x00000004),
    C_OUTPUT_QUAD           =   (0x00000008),
    C_OUTPUT_DOLBY_SURROUND =   (0x00000010),
    C_OUTPUT_DOLBY_PLII     =   (0x00000020),
    C_OUTPUT_5POINT1        =   (0x00000040),
    C_OUTPUT_7POINT1        =   (0x00000080),
    C_OUTPUT_RESERVED0      =   (0x00000100),       /* Reserved for future use                      */
    C_OUTPUT_RESERVED1      =   (0x00000200),       /* Reserved for future use                      */
    C_OUTPUT_DOLBY_DIGITAL  =   (0x00010000),       /* Digital outputs                              */
    C_OUTPUT_DTS            =   (0x00020000),
    C_OUTPUT_UNKNOWN        =   (0x80000000)        /* Unknown config                               */
}
COUTPUTCONFIG;
typedef enum                                        /* 3D source processing modes                   */
{
    C_HRTF_DISABLED = 0,                            /* No 3D processing. Position between speakers  */
    C_HRTF_FULL     = C_CAPS_3D_HRTF_FULL,          /* Full HRTF processing                         */
    C_HRTF_DELAY    = C_CAPS_3D_HRTF_DELAY,         /* HRTF delay processing only                   */
    C_HRTF_VOLPAN   = C_CAPS_3D_VOLPAN              /* Volume/panning                               */
}
CHRTFMODE;

struct XtcState {
 int bufLeft[16];
 int bufRight[16];
 int aLeft;
 int aRight;
 unsigned int index;
};
class CSalXTC {
 XtcState m_xtc;
};
struct _C3DSOURCEENVFX {
 int iEnvironment;
 int iEnvironmentHF;
 int iObstruction;
 float fObstructionLFRatio;
 int iOcclusion;
 float fOcclusionLFRatio;
 int iExclusion;
 float fExclusionLFRatio;
 float fRolloffFactor;
 float fAirAbsorptionFactor;
};
struct CODA3DSOURCE {
 CCodaVector vPosition;
 CCodaVector vVelocity;
 CCodaVector vConeDirection;
 CCodaVector vDirection;
 float fDistance;
 float fDistanceGain;
 float fConeGain;
 float fDoppler;
 int iDirect;
 int iDirectHF;
 unsigned int uiDirectHFReference;
 int iEnvFx;
 int iEnvFxHF;
 unsigned int uiEnvFxHFReference;
 float fMacroFx;
};
#endif
