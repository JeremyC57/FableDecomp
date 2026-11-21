// Direct behavior test of the selected EAX setter.
#include <assert.h>
#include <stdio.h>

#define FABLE_RETAIL_EAX30_LISTENER_ORIGINAL_TAG
#include "engine/EAXNativeTypes.h"

#include <string.h>
#ifdef _MSC_VER
#pragma intrinsic(memcmp, memcpy)
#endif

extern "C" const GUID DSPROPSETID_EAX30_ListenerProperties;   // 0x012B9418
extern "C" const GUID DSPROPSETID_EAX30_BufferProperties;     // 0x012B9428
extern "C" const GUID EAXPROPERTYID_EAX40_Context;            // 0x012B9448
extern "C" const GUID EAXPROPERTYID_EAX40_Source;             // 0x012B94A8

enum {
    DSPROPERTY_EAX30LISTENER_NONE, DSPROPERTY_EAX30LISTENER_ALLPARAMETERS, DSPROPERTY_EAX30LISTENER_ENVIRONMENT,
    DSPROPERTY_EAX30LISTENER_ENVIRONMENTSIZE, DSPROPERTY_EAX30LISTENER_ENVIRONMENTDIFFUSION, DSPROPERTY_EAX30LISTENER_ROOM,
    DSPROPERTY_EAX30LISTENER_ROOMHF, DSPROPERTY_EAX30LISTENER_ROOMLF, DSPROPERTY_EAX30LISTENER_DECAYTIME,
    DSPROPERTY_EAX30LISTENER_DECAYHFRATIO, DSPROPERTY_EAX30LISTENER_DECAYLFRATIO, DSPROPERTY_EAX30LISTENER_REFLECTIONS,
    DSPROPERTY_EAX30LISTENER_REFLECTIONSDELAY, DSPROPERTY_EAX30LISTENER_REFLECTIONSPAN, DSPROPERTY_EAX30LISTENER_REVERB,
    DSPROPERTY_EAX30LISTENER_REVERBDELAY, DSPROPERTY_EAX30LISTENER_REVERBPAN, DSPROPERTY_EAX30LISTENER_ECHOTIME,
    DSPROPERTY_EAX30LISTENER_ECHODEPTH, DSPROPERTY_EAX30LISTENER_MODULATIONTIME, DSPROPERTY_EAX30LISTENER_MODULATIONDEPTH,
    DSPROPERTY_EAX30LISTENER_AIRABSORPTIONHF, DSPROPERTY_EAX30LISTENER_HFREFERENCE, DSPROPERTY_EAX30LISTENER_LFREFERENCE,
    DSPROPERTY_EAX30LISTENER_ROOMROLLOFFFACTOR, DSPROPERTY_EAX30LISTENER_FLAGS
};
enum {
    DSPROPERTY_EAX30BUFFER_NONE, DSPROPERTY_EAX30BUFFER_ALLPARAMETERS, DSPROPERTY_EAX30BUFFER_OBSTRUCTIONPARAMETERS,
    DSPROPERTY_EAX30BUFFER_OCCLUSIONPARAMETERS, DSPROPERTY_EAX30BUFFER_EXCLUSIONPARAMETERS, DSPROPERTY_EAX30BUFFER_DIRECT,
    DSPROPERTY_EAX30BUFFER_DIRECTHF, DSPROPERTY_EAX30BUFFER_ROOM, DSPROPERTY_EAX30BUFFER_ROOMHF,
    DSPROPERTY_EAX30BUFFER_OBSTRUCTION, DSPROPERTY_EAX30BUFFER_OBSTRUCTIONLFRATIO, DSPROPERTY_EAX30BUFFER_OCCLUSION,
    DSPROPERTY_EAX30BUFFER_OCCLUSIONLFRATIO, DSPROPERTY_EAX30BUFFER_OCCLUSIONROOMRATIO,
    DSPROPERTY_EAX30BUFFER_OCCLUSIONDIRECTRATIO, DSPROPERTY_EAX30BUFFER_EXCLUSION, DSPROPERTY_EAX30BUFFER_EXCLUSIONLFRATIO,
    DSPROPERTY_EAX30BUFFER_OUTSIDEVOLUMEHF, DSPROPERTY_EAX30BUFFER_DOPPLERFACTOR, DSPROPERTY_EAX30BUFFER_ROLLOFFFACTOR,
    DSPROPERTY_EAX30BUFFER_ROOMROLLOFFFACTOR, DSPROPERTY_EAX30BUFFER_AIRABSORPTIONFACTOR, DSPROPERTY_EAX30BUFFER_FLAGS
};
#define DSPROPERTY_EAX30_DEFERRED 0x80000000UL

struct SListenerBlock {                                 // 0x6c bytes (27 words)
    EAXVECTOR m_vector0;                                // +0x00
    EAXVECTOR m_vector1;                                // +0x0c
    long      m_room;                                   // +0x18
    long      m_roomHF;                                 // +0x1c
    long      m_reserved20;                             // +0x20
    float     m_decayTime;                              // +0x24
    float     m_decayHFRatio;                           // +0x28
    float     m_reserved2c;                             // +0x2c
    long      m_reflections;                            // +0x30
    float     m_reflectionsDelay;                       // +0x34
    long      m_reverb;                                 // +0x38
    float     m_reverbDelay;                            // +0x3c
    float     m_environmentSize;                        // +0x40
    float     m_environmentDiffusion;                   // +0x44
    float     m_reserved48;                             // +0x48
    float     m_echoTime;                               // +0x4c
    float     m_echoDepth;                              // +0x50
    float     m_modulationTime;                         // +0x54
    float     m_modulationDepth;                        // +0x58
    long      m_hfReference;                            // +0x5c
    long      m_lfReference;                            // +0x60
    float     m_roomRolloffFactor;                      // +0x64
    long      m_airAbsorption;                          // +0x68 mB
};

class CListenerState;

struct CEAXVoice {
    unsigned char         _pad[0x1a8];
    EAX30BUFFERPROPERTIES m_eax3BufferProperties;       // +0x1a8 .. +0x1ef
};

struct CEAXManager {
    unsigned char           _pad0[4];
    CListenerState*         m_pListenerState;           // +0x04
    SListenerBlock          m_listenerBlock;            // +0x08
    EAX30LISTENERPROPERTIES m_listenerProperties;       // +0x74 .. +0xe3
    int  ApplyListenerProperties(const EAX30LISTENERPROPERTIES* properties);   // 0x00C4E3C0
    long ApplyVoiceBufferProperties(CEAXVoice* voice, unsigned long version, unsigned long id, void* data, unsigned long size);   // 0x00C4EA30
    long SetEAXProperty(const GUID* propertySet, unsigned long id, CEAXVoice* voice, void* data, unsigned long size);
};

long CEAXManager::SetEAXProperty(const GUID* propertySet, unsigned long id, CEAXVoice* voice, void* data, unsigned long size)
{
    SListenerBlock environments[26] = {
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -100,
         0,
         1.49f,
         0.83f,
         1.0f,
         -2602,
         0.007f,
         200,
         0.011f,
         7.5f,
         1.0f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 0
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -6000,
         0,
         0.17f,
         0.1f,
         1.0f,
         -1204,
         0.001f,
         207,
         0.002f,
         1.4f,
         1.0f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 1
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -454,
         0,
         0.4f,
         0.83f,
         1.0f,
         -1646,
         0.002f,
         53,
         0.003f,
         1.9f,
         1.0f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 2
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -1200,
         0,
         1.49f,
         0.54f,
         1.0f,
         -370,
         0.007f,
         1030,
         0.011f,
         1.4f,
         1.0f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 3
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -6000,
         0,
         0.5f,
         0.1f,
         1.0f,
         -1376,
         0.003f,
         -1104,
         0.004f,
         2.5f,
         1.0f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 4
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -300,
         0,
         2.31f,
         0.64f,
         1.0f,
         -711,
         0.012f,
         83,
         0.017f,
         11.6f,
         1.0f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 5
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -476,
         0,
         4.32f,
         0.37f,
         1.0f,
         -789,
         0.02f,
         -289,
         0.03f,
         21.6f,
         1.0f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 6
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -500,
         0,
         3.92f,
         0.4f,
         1.0f,
         -1230,
         0.02f,
         -2,
         0.029f,
         19.6f,
         1.0f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 7
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         0,
         0,
         2.91f,
         1.3f,
         1.0f,
         -602,
         0.015f,
         -302,
         0.022f,
         14.6f,
         1.0f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 8
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -698,
         0,
         7.24f,
         0.22f,
         1.0f,
         -1166,
         0.02f,
         16,
         0.03f,
         36.2f,
         1.0f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 9
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -1000,
         0,
         10.05f,
         0.16f,
         1.0f,
         -602,
         0.02f,
         198,
         0.03f,
         50.3f,
         1.0f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 10
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -4000,
         0,
         0.3f,
         0.1f,
         1.0f,
         -1831,
         0.002f,
         -1630,
         0.03f,
         1.9f,
         1.0f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 11
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -300,
         0,
         1.49f,
         0.59f,
         1.0f,
         -1219,
         0.007f,
         441,
         0.011f,
         1.8f,
         1.0f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 12
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -237,
         0,
         2.7f,
         0.59f,
         1.0f,
         -1214,
         0.013f,
         395,
         0.02f,
         13.5f,
         1.0f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 13
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -270,
         0,
         1.49f,
         0.86f,
         1.0f,
         -1204,
         0.007f,
         -4,
         0.011f,
         7.5f,
         0.3f,
         1.0f,
         0.125f,
         0.95f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 14
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -3300,
         0,
         1.49f,
         0.54f,
         1.0f,
         -2560,
         0.162f,
         -229,
         0.088f,
         38.0f,
         0.3f,
         1.0f,
         0.125f,
         1.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 15
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -800,
         0,
         1.49f,
         0.67f,
         1.0f,
         -2273,
         0.007f,
         -1691,
         0.011f,
         7.5f,
         0.5f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 16
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -2500,
         0,
         1.49f,
         0.21f,
         1.0f,
         -2780,
         0.3f,
         -1434,
         0.1f,
         100.0f,
         0.27f,
         1.0f,
         0.25f,
         1.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 17
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -1000,
         0,
         1.49f,
         0.83f,
         1.0f,
         -10000,
         0.061f,
         500,
         0.025f,
         17.5f,
         1.0f,
         1.0f,
         0.125f,
         0.7f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 18
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -2000,
         0,
         1.49f,
         0.5f,
         1.0f,
         -2466,
         0.179f,
         -1926,
         0.1f,
         42.5f,
         0.21f,
         1.0f,
         0.25f,
         1.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 19
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         0,
         0,
         1.65f,
         1.5f,
         1.0f,
         -1363,
         0.008f,
         -1153,
         0.012f,
         8.3f,
         1.0f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 20
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -1000,
         0,
         2.81f,
         0.14f,
         1.0f,
         429,
         0.014f,
         1023,
         0.021f,
         1.7f,
         0.8f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         0.0f,
         5000,
         250,
         0.0f,
         -5}, // 21
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -4000,
         0,
         1.49f,
         0.1f,
         1.0f,
         -449,
         0.007f,
         1700,
         0.011f,
         1.8f,
         1.0f,
         1.0f,
         0.25f,
         0.0f,
         1.18f,
         0.35f,
         5000,
         250,
         0.0f,
         -5}, // 22
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         0,
         0,
         8.39f,
         1.39f,
         1.0f,
         -115,
         0.002f,
         985,
         0.03f,
         1.9f,
         0.5f,
         1.0f,
         0.25f,
         0.0f,
         0.25f,
         1.0f,
         5000,
         250,
         0.0f,
         -5}, // 23
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -400,
         0,
         17.23f,
         0.56f,
         1.0f,
         -1713,
         0.02f,
         -613,
         0.03f,
         1.8f,
         0.6f,
         1.0f,
         0.25f,
         1.0f,
         0.81f,
         0.31f,
         5000,
         250,
         0.0f,
         -5}, // 24
        {{0.0f, 0.0f, 0.0f},
         {0.0f, 0.0f, 0.0f},
         -1000,
         -151,
         0,
         7.56f,
         0.91f,
         1.0f,
         -626,
         0.02f,
         774,
         0.03f,
         1.0f,
         0.5f,
         1.0f,
         0.25f,
         0.0f,
         4.0f,
         1.0f,
         5000,
         250,
         0.0f,
         -5}, // 25
    };
    unsigned long propertySetType;
    if (!memcmp(propertySet, &EAXPROPERTYID_EAX40_FXSlot0, sizeof(GUID)))
        propertySetType = 0x20000;
    else if (!memcmp(propertySet, &EAXPROPERTYID_EAX40_FXSlot1, sizeof(GUID)))
        propertySetType = 0x40000;
    else if (!memcmp(propertySet, &EAXPROPERTYID_EAX40_FXSlot2, sizeof(GUID)))
        propertySetType = 0x80000;
    else if (!memcmp(propertySet, &EAXPROPERTYID_EAX40_FXSlot3, sizeof(GUID)))
        propertySetType = 0x100000;
    else if (!memcmp(propertySet, &EAXPROPERTYID_EAX40_Context, sizeof(GUID)))
        propertySetType = 0x200000;
    else if (!memcmp(propertySet, &EAXPROPERTYID_EAX40_Source, sizeof(GUID)))
        propertySetType = 4;
    else if (!memcmp(propertySet, &DSPROPSETID_EAX30_ListenerProperties, sizeof(GUID)))
        propertySetType = 0x10000;
    else if (!memcmp(propertySet, &DSPROPSETID_EAX30_BufferProperties, sizeof(GUID)))
        propertySetType = 2;
    else
        propertySetType = 0;

    bool changed = false;
    if (propertySetType & 6) {
        if (propertySetType == 2) {
            switch (id & ~DSPROPERTY_EAX30_DEFERRED) {
            case DSPROPERTY_EAX30BUFFER_NONE:
                changed = true;
                break;
            case DSPROPERTY_EAX30BUFFER_ALLPARAMETERS:
                if (!memcmp(&voice->m_eax3BufferProperties, data, sizeof(EAX30BUFFERPROPERTIES)))
                    return 0;
                voice->m_eax3BufferProperties = *(EAX30BUFFERPROPERTIES*)data;
                changed = true;
                break;
            case DSPROPERTY_EAX30BUFFER_OBSTRUCTIONPARAMETERS: {
                EAXOBSTRUCTIONPROPERTIES* obstruction = (EAXOBSTRUCTIONPROPERTIES*)data;
                if (voice->m_eax3BufferProperties.lObstruction == obstruction->lObstruction &&
                    voice->m_eax3BufferProperties.flObstructionLFRatio == obstruction->flObstructionLFRatio)
                    return 0;
                voice->m_eax3BufferProperties.lObstruction = obstruction->lObstruction;
                voice->m_eax3BufferProperties.flObstructionLFRatio = obstruction->flObstructionLFRatio;
                changed = true;
                break;
            }
            case DSPROPERTY_EAX30BUFFER_OCCLUSIONPARAMETERS: {
                EAXOCCLUSIONPROPERTIES* occlusion = (EAXOCCLUSIONPROPERTIES*)data;
                if (voice->m_eax3BufferProperties.lOcclusion == occlusion->lOcclusion &&
                    voice->m_eax3BufferProperties.flOcclusionLFRatio == occlusion->flOcclusionLFRatio &&
                    voice->m_eax3BufferProperties.flOcclusionRoomRatio == occlusion->flOcclusionRoomRatio &&
                    voice->m_eax3BufferProperties.flOcclusionDirectRatio == occlusion->flOcclusionDirectRatio)
                    return 0;
                voice->m_eax3BufferProperties.lOcclusion = occlusion->lOcclusion;
                voice->m_eax3BufferProperties.flOcclusionLFRatio = occlusion->flOcclusionLFRatio;
                voice->m_eax3BufferProperties.flOcclusionRoomRatio = occlusion->flOcclusionRoomRatio;
                voice->m_eax3BufferProperties.flOcclusionDirectRatio = occlusion->flOcclusionDirectRatio;
                changed = true;
                break;
            }
            case DSPROPERTY_EAX30BUFFER_EXCLUSIONPARAMETERS: {
                EAXEXCLUSIONPROPERTIES* exclusion = (EAXEXCLUSIONPROPERTIES*)data;
                if (voice->m_eax3BufferProperties.lExclusion == exclusion->lExclusion &&
                    voice->m_eax3BufferProperties.flExclusionLFRatio == exclusion->flExclusionLFRatio)
                    return 0;
                voice->m_eax3BufferProperties.lExclusion = exclusion->lExclusion;
                voice->m_eax3BufferProperties.flExclusionLFRatio = exclusion->flExclusionLFRatio;
                changed = true;
                break;
            }
            case DSPROPERTY_EAX30BUFFER_DIRECT:
                changed = voice->m_eax3BufferProperties.lDirect != *(long*)data;
                voice->m_eax3BufferProperties.lDirect = *(long*)data;
                break;
            case DSPROPERTY_EAX30BUFFER_DIRECTHF:
                changed = voice->m_eax3BufferProperties.lDirectHF != *(long*)data;
                voice->m_eax3BufferProperties.lDirectHF = *(long*)data;
                break;
            case DSPROPERTY_EAX30BUFFER_ROOM:
                changed = voice->m_eax3BufferProperties.lRoom != *(long*)data;
                voice->m_eax3BufferProperties.lRoom = *(long*)data;
                break;
            case DSPROPERTY_EAX30BUFFER_ROOMHF:
                changed = voice->m_eax3BufferProperties.lRoomHF != *(long*)data;
                voice->m_eax3BufferProperties.lRoomHF = *(long*)data;
                break;
            case DSPROPERTY_EAX30BUFFER_OBSTRUCTION:
                changed = voice->m_eax3BufferProperties.lObstruction != *(long*)data;
                voice->m_eax3BufferProperties.lObstruction = *(long*)data;
                break;
            case DSPROPERTY_EAX30BUFFER_OBSTRUCTIONLFRATIO:
                changed = voice->m_eax3BufferProperties.flObstructionLFRatio != *(float*)data;
                voice->m_eax3BufferProperties.flObstructionLFRatio = *(float*)data;
                break;
            case DSPROPERTY_EAX30BUFFER_OCCLUSION:
                changed = voice->m_eax3BufferProperties.lOcclusion != *(long*)data;
                voice->m_eax3BufferProperties.lOcclusion = *(long*)data;
                break;
            case DSPROPERTY_EAX30BUFFER_OCCLUSIONLFRATIO:
                changed = voice->m_eax3BufferProperties.flOcclusionLFRatio != *(float*)data;
                voice->m_eax3BufferProperties.flOcclusionLFRatio = *(float*)data;
                break;
            case DSPROPERTY_EAX30BUFFER_OCCLUSIONROOMRATIO:
                changed = voice->m_eax3BufferProperties.flOcclusionRoomRatio != *(float*)data;
                voice->m_eax3BufferProperties.flOcclusionRoomRatio = *(float*)data;
                break;
            case DSPROPERTY_EAX30BUFFER_OCCLUSIONDIRECTRATIO:
                changed = voice->m_eax3BufferProperties.flOcclusionDirectRatio != *(float*)data;
                voice->m_eax3BufferProperties.flOcclusionDirectRatio = *(float*)data;
                break;
            case DSPROPERTY_EAX30BUFFER_EXCLUSION:
                changed = voice->m_eax3BufferProperties.lExclusion != *(long*)data;
                voice->m_eax3BufferProperties.lExclusion = *(long*)data;
                break;
            case DSPROPERTY_EAX30BUFFER_EXCLUSIONLFRATIO:
                changed = voice->m_eax3BufferProperties.flExclusionLFRatio != *(float*)data;
                voice->m_eax3BufferProperties.flExclusionLFRatio = *(float*)data;
                break;
            case DSPROPERTY_EAX30BUFFER_OUTSIDEVOLUMEHF:
                changed = voice->m_eax3BufferProperties.lOutsideVolumeHF != *(long*)data;
                voice->m_eax3BufferProperties.lOutsideVolumeHF = *(long*)data;
                break;
            case DSPROPERTY_EAX30BUFFER_DOPPLERFACTOR:
                changed = voice->m_eax3BufferProperties.flDopplerFactor != *(float*)data;
                voice->m_eax3BufferProperties.flDopplerFactor = *(float*)data;
                break;
            case DSPROPERTY_EAX30BUFFER_ROLLOFFFACTOR:
                changed = voice->m_eax3BufferProperties.flRolloffFactor != *(float*)data;
                voice->m_eax3BufferProperties.flRolloffFactor = *(float*)data;
                break;
            case DSPROPERTY_EAX30BUFFER_ROOMROLLOFFFACTOR:
                changed = voice->m_eax3BufferProperties.flRoomRolloffFactor != *(float*)data;
                voice->m_eax3BufferProperties.flRoomRolloffFactor = *(float*)data;
                break;
            case DSPROPERTY_EAX30BUFFER_AIRABSORPTIONFACTOR:
                changed = voice->m_eax3BufferProperties.flAirAbsorptionFactor != *(float*)data;
                voice->m_eax3BufferProperties.flAirAbsorptionFactor = *(float*)data;
                break;
            case DSPROPERTY_EAX30BUFFER_FLAGS:
                changed = voice->m_eax3BufferProperties.ulFlags != *(unsigned long*)data;
                voice->m_eax3BufferProperties.ulFlags = *(unsigned long*)data;
                break;
            default:
                return 0;
            }
            if (!changed)
                return 0;
            if (id & DSPROPERTY_EAX30_DEFERRED)
                goto apply_listener;
            ApplyVoiceBufferProperties(voice, 2, id, data, size);
        }
    } else if (propertySetType & 0x3f0000) {
        if (propertySetType == 0x10000) {
            switch (id & ~DSPROPERTY_EAX30_DEFERRED) {
            case DSPROPERTY_EAX30LISTENER_NONE:
                changed = true;
                break;
            case DSPROPERTY_EAX30LISTENER_ALLPARAMETERS:
                if (memcmp(&m_listenerProperties, data, sizeof(EAX30LISTENERPROPERTIES)))
                    m_listenerProperties = *(EAX30LISTENERPROPERTIES*)data;
                changed = true;
                break;
            case DSPROPERTY_EAX30LISTENER_ROOM:
                changed = m_listenerProperties.lRoom != *(long*)data;
                m_listenerProperties.lRoom = *(long*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_ROOMHF:
                changed = m_listenerProperties.lRoomHF != *(long*)data;
                m_listenerProperties.lRoomHF = *(long*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_ROOMLF:
                changed = m_listenerProperties.lRoomLF != *(long*)data;
                m_listenerProperties.lRoomLF = *(long*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_ROOMROLLOFFFACTOR:
                m_listenerProperties.flRoomRolloffFactor = *(float*)data;
                // falls through (retail): the decay-time test and store follow
            case DSPROPERTY_EAX30LISTENER_DECAYTIME:
                changed = m_listenerProperties.flDecayTime != *(float*)data;
                m_listenerProperties.flDecayTime = *(float*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_DECAYHFRATIO:
                changed = m_listenerProperties.flDecayHFRatio != *(float*)data;
                m_listenerProperties.flDecayHFRatio = *(float*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_DECAYLFRATIO:
                changed = m_listenerProperties.flDecayLFRatio != *(float*)data;
                m_listenerProperties.flDecayLFRatio = *(float*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_REFLECTIONS:
                changed = m_listenerProperties.lReflections != *(long*)data;
                m_listenerProperties.lReflections = *(long*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_REFLECTIONSDELAY:
                changed = m_listenerProperties.flReflectionsDelay != *(float*)data;
                m_listenerProperties.flReflectionsDelay = *(float*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_REFLECTIONSPAN: {
                EAXVECTOR* pan = (EAXVECTOR*)data;
                if (m_listenerProperties.vReflectionsPan.x == pan->x &&
                    m_listenerProperties.vReflectionsPan.y == pan->y &&
                    m_listenerProperties.vReflectionsPan.z == pan->z)
                    return 0;
                m_listenerProperties.vReflectionsPan = *pan;
                changed = true;
                break;
            }
            case DSPROPERTY_EAX30LISTENER_REVERB:
                changed = m_listenerProperties.lReverb != *(long*)data;
                m_listenerProperties.lReverb = *(long*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_REVERBDELAY:
                // retail tests the room rolloff factor here and stores the reverb delay
                changed = m_listenerProperties.flRoomRolloffFactor != *(float*)data;
                m_listenerProperties.flReverbDelay = *(float*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_REVERBPAN: {
                EAXVECTOR* pan = (EAXVECTOR*)data;
                if (m_listenerProperties.vReverbPan.x == pan->x &&
                    m_listenerProperties.vReverbPan.y == pan->y &&
                    m_listenerProperties.vReverbPan.z == pan->z)
                    return 0;
                m_listenerProperties.vReverbPan = *pan;
                changed = true;
                break;
            }
            case DSPROPERTY_EAX30LISTENER_ENVIRONMENT:
                m_listenerProperties.ulEnvironment = *(unsigned long*)data;
                m_listenerProperties.flAirAbsorptionHF =
                    (float)environments[m_listenerProperties.ulEnvironment].m_airAbsorption;
                m_listenerProperties.flDecayHFRatio =
                    environments[m_listenerProperties.ulEnvironment].m_decayHFRatio;
                m_listenerProperties.flDecayTime =
                    environments[m_listenerProperties.ulEnvironment].m_decayTime;
                m_listenerProperties.flEnvironmentDiffusion =
                    environments[m_listenerProperties.ulEnvironment].m_environmentDiffusion;
                m_listenerProperties.flEnvironmentSize =
                    environments[m_listenerProperties.ulEnvironment].m_environmentSize;
                m_listenerProperties.flReflectionsDelay =
                    environments[m_listenerProperties.ulEnvironment].m_reflectionsDelay;
                m_listenerProperties.flReverbDelay =
                    environments[m_listenerProperties.ulEnvironment].m_reverbDelay;
                m_listenerProperties.flRoomRolloffFactor =
                    environments[m_listenerProperties.ulEnvironment].m_roomRolloffFactor;
                m_listenerProperties.lReflections =
                    environments[m_listenerProperties.ulEnvironment].m_reflections;
                m_listenerProperties.lReverb = environments[m_listenerProperties.ulEnvironment].m_reverb;
                m_listenerProperties.lRoom = environments[m_listenerProperties.ulEnvironment].m_room;
                m_listenerProperties.lRoomHF = environments[m_listenerProperties.ulEnvironment].m_roomHF;
                changed = true;
                break;
            case DSPROPERTY_EAX30LISTENER_ENVIRONMENTSIZE:
                changed = m_listenerProperties.flEnvironmentSize != *(float*)data;
                m_listenerProperties.flEnvironmentSize = *(float*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_ENVIRONMENTDIFFUSION:
                changed = m_listenerProperties.flEnvironmentDiffusion != *(float*)data;
                m_listenerProperties.flEnvironmentDiffusion = *(float*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_ECHOTIME:
                changed = m_listenerProperties.flEchoTime != *(float*)data;
                m_listenerProperties.flEchoTime = *(float*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_ECHODEPTH:
                changed = m_listenerProperties.flEchoDepth != *(float*)data;
                m_listenerProperties.flEchoDepth = *(float*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_MODULATIONTIME:
                changed = m_listenerProperties.flModulationTime != *(float*)data;
                m_listenerProperties.flModulationTime = *(float*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_MODULATIONDEPTH:
                changed = m_listenerProperties.flModulationDepth != *(float*)data;
                m_listenerProperties.flModulationDepth = *(float*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_AIRABSORPTIONHF:
                changed = m_listenerProperties.flAirAbsorptionHF != *(float*)data;
                m_listenerProperties.flAirAbsorptionHF = *(float*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_FLAGS:
                changed = m_listenerProperties.ulFlags != *(unsigned long*)data;
                m_listenerProperties.ulFlags = *(unsigned long*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_HFREFERENCE:
                changed = m_listenerProperties.flHFReference != *(float*)data;
                m_listenerProperties.flHFReference = *(float*)data;
                break;
            case DSPROPERTY_EAX30LISTENER_LFREFERENCE:
                changed = m_listenerProperties.flLFReference != *(float*)data;
                m_listenerProperties.flLFReference = *(float*)data;
                break;
            default:
                return 0;
            }
        }
    }
apply_listener:
    if (changed && !(id & DSPROPERTY_EAX30_DEFERRED))
        ApplyListenerProperties(&m_listenerProperties);
    return 0;
}

extern "C" const GUID DSPROPSETID_EAX30_ListenerProperties = {1,0,0,{0}};
extern "C" const GUID DSPROPSETID_EAX30_BufferProperties = {2,0,0,{0}};
extern "C" const GUID EAXPROPERTYID_EAX40_Context = {3,0,0,{0}};
extern "C" const GUID EAXPROPERTYID_EAX40_Source = {4,0,0,{0}};
extern "C" const GUID EAXPROPERTYID_EAX40_FXSlot0 = {5,0,0,{0}};
extern "C" const GUID EAXPROPERTYID_EAX40_FXSlot1 = {6,0,0,{0}};
extern "C" const GUID EAXPROPERTYID_EAX40_FXSlot2 = {7,0,0,{0}};
extern "C" const GUID EAXPROPERTYID_EAX40_FXSlot3 = {8,0,0,{0}};

static unsigned int voice_calls, listener_calls;
static void* expected_data;
static unsigned long expected_size, expected_id;
static CEAXVoice* expected_voice;
int CEAXManager::ApplyListenerProperties(const EAX30LISTENERPROPERTIES* props) {
    assert(props == &m_listenerProperties);
    ++listener_calls;
    return -1;
}
long CEAXManager::ApplyVoiceBufferProperties(CEAXVoice* v, unsigned long version, unsigned long id, void* data, unsigned long size) {
    assert(v == expected_voice && version == 2 && data == expected_data);
    assert(size == expected_size && id == expected_id && listener_calls == 0);
    ++voice_calls;
    return -1;
}
static CEAXManager manager;
static CEAXVoice voice;
template<class T>
static void check(const GUID& set, unsigned long id, T& value, unsigned int want_voice, unsigned int want_listener) {
    expected_data = &value; expected_voice = &voice;
    expected_size = sizeof(T); expected_id = id;
    voice_calls = listener_calls = 0;
    assert(manager.SetEAXProperty(&set,id,&voice,&value,sizeof(T)) == 0);
    assert(voice_calls == want_voice && listener_calls == want_listener);
}
int main() {
    long integer = 0;
    const GUID unknown = {99,0,0,{0}};
    check(unknown,0,integer,0,0);
    check(EAXPROPERTYID_EAX40_Context,0,integer,0,0);
    check(EAXPROPERTYID_EAX40_Source,0,integer,0,0);
    check(EAXPROPERTYID_EAX40_FXSlot0,0,integer,0,0);
    check(EAXPROPERTYID_EAX40_FXSlot1,0,integer,0,0);
    check(EAXPROPERTYID_EAX40_FXSlot2,0,integer,0,0);
    check(EAXPROPERTYID_EAX40_FXSlot3,0,integer,0,0);
    check(DSPROPSETID_EAX30_BufferProperties, 99,integer,0,0);
    check(DSPROPSETID_EAX30_BufferProperties, DSPROPERTY_EAX30BUFFER_NONE,integer,1,1);
    check(DSPROPSETID_EAX30_BufferProperties, DSPROPERTY_EAX30BUFFER_NONE|DSPROPERTY_EAX30_DEFERRED,integer,0,0);
    {
        EAX30BUFFERPROPERTIES value;
        memset(&value,0,sizeof(value));

        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_ALLPARAMETERS,value,0,0);
        value.lRoom=4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_ALLPARAMETERS,value,1,1);
        assert(!memcmp(&value,&voice.m_eax3BufferProperties,sizeof(value)));
    }
    {
        long value = 2;
        voice.m_eax3BufferProperties.lDirect = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_DIRECT,value,1,1);
        assert(voice.m_eax3BufferProperties.lDirect == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_DIRECT,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_DIRECT|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.lDirect == value);
    }
    {
        long value = 2;
        voice.m_eax3BufferProperties.lDirectHF = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_DIRECTHF,value,1,1);
        assert(voice.m_eax3BufferProperties.lDirectHF == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_DIRECTHF,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_DIRECTHF|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.lDirectHF == value);
    }
    {
        long value = 2;
        voice.m_eax3BufferProperties.lRoom = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_ROOM,value,1,1);
        assert(voice.m_eax3BufferProperties.lRoom == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_ROOM,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_ROOM|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.lRoom == value);
    }
    {
        long value = 2;
        voice.m_eax3BufferProperties.lRoomHF = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_ROOMHF,value,1,1);
        assert(voice.m_eax3BufferProperties.lRoomHF == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_ROOMHF,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_ROOMHF|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.lRoomHF == value);
    }
    {
        long value = 2;
        voice.m_eax3BufferProperties.lObstruction = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OBSTRUCTION,value,1,1);
        assert(voice.m_eax3BufferProperties.lObstruction == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OBSTRUCTION,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OBSTRUCTION|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.lObstruction == value);
    }
    {
        float value = 2;
        voice.m_eax3BufferProperties.flObstructionLFRatio = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OBSTRUCTIONLFRATIO,value,1,1);
        assert(voice.m_eax3BufferProperties.flObstructionLFRatio == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OBSTRUCTIONLFRATIO,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OBSTRUCTIONLFRATIO|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.flObstructionLFRatio == value);
    }
    {
        long value = 2;
        voice.m_eax3BufferProperties.lOcclusion = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OCCLUSION,value,1,1);
        assert(voice.m_eax3BufferProperties.lOcclusion == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OCCLUSION,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OCCLUSION|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.lOcclusion == value);
    }
    {
        float value = 2;
        voice.m_eax3BufferProperties.flOcclusionLFRatio = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OCCLUSIONLFRATIO,value,1,1);
        assert(voice.m_eax3BufferProperties.flOcclusionLFRatio == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OCCLUSIONLFRATIO,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OCCLUSIONLFRATIO|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.flOcclusionLFRatio == value);
    }
    {
        float value = 2;
        voice.m_eax3BufferProperties.flOcclusionRoomRatio = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OCCLUSIONROOMRATIO,value,1,1);
        assert(voice.m_eax3BufferProperties.flOcclusionRoomRatio == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OCCLUSIONROOMRATIO,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OCCLUSIONROOMRATIO|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.flOcclusionRoomRatio == value);
    }
    {
        float value = 2;
        voice.m_eax3BufferProperties.flOcclusionDirectRatio = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OCCLUSIONDIRECTRATIO,value,1,1);
        assert(voice.m_eax3BufferProperties.flOcclusionDirectRatio == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OCCLUSIONDIRECTRATIO,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OCCLUSIONDIRECTRATIO|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.flOcclusionDirectRatio == value);
    }
    {
        long value = 2;
        voice.m_eax3BufferProperties.lExclusion = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_EXCLUSION,value,1,1);
        assert(voice.m_eax3BufferProperties.lExclusion == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_EXCLUSION,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_EXCLUSION|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.lExclusion == value);
    }
    {
        float value = 2;
        voice.m_eax3BufferProperties.flExclusionLFRatio = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_EXCLUSIONLFRATIO,value,1,1);
        assert(voice.m_eax3BufferProperties.flExclusionLFRatio == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_EXCLUSIONLFRATIO,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_EXCLUSIONLFRATIO|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.flExclusionLFRatio == value);
    }
    {
        long value = 2;
        voice.m_eax3BufferProperties.lOutsideVolumeHF = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OUTSIDEVOLUMEHF,value,1,1);
        assert(voice.m_eax3BufferProperties.lOutsideVolumeHF == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OUTSIDEVOLUMEHF,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OUTSIDEVOLUMEHF|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.lOutsideVolumeHF == value);
    }
    {
        float value = 2;
        voice.m_eax3BufferProperties.flDopplerFactor = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_DOPPLERFACTOR,value,1,1);
        assert(voice.m_eax3BufferProperties.flDopplerFactor == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_DOPPLERFACTOR,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_DOPPLERFACTOR|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.flDopplerFactor == value);
    }
    {
        float value = 2;
        voice.m_eax3BufferProperties.flRolloffFactor = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_ROLLOFFFACTOR,value,1,1);
        assert(voice.m_eax3BufferProperties.flRolloffFactor == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_ROLLOFFFACTOR,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_ROLLOFFFACTOR|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.flRolloffFactor == value);
    }
    {
        float value = 2;
        voice.m_eax3BufferProperties.flRoomRolloffFactor = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_ROOMROLLOFFFACTOR,value,1,1);
        assert(voice.m_eax3BufferProperties.flRoomRolloffFactor == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_ROOMROLLOFFFACTOR,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_ROOMROLLOFFFACTOR|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.flRoomRolloffFactor == value);
    }
    {
        float value = 2;
        voice.m_eax3BufferProperties.flAirAbsorptionFactor = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_AIRABSORPTIONFACTOR,value,1,1);
        assert(voice.m_eax3BufferProperties.flAirAbsorptionFactor == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_AIRABSORPTIONFACTOR,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_AIRABSORPTIONFACTOR|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.flAirAbsorptionFactor == value);
    }
    {
        unsigned long value = 2;
        voice.m_eax3BufferProperties.ulFlags = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_FLAGS,value,1,1);
        assert(voice.m_eax3BufferProperties.ulFlags == value);
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_FLAGS,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_FLAGS|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(voice.m_eax3BufferProperties.ulFlags == value);
    }
    {
        EAXOBSTRUCTIONPROPERTIES value; memset(&value,0,sizeof(value));
        voice.m_eax3BufferProperties.lObstruction = 0;
        voice.m_eax3BufferProperties.flObstructionLFRatio = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OBSTRUCTIONPARAMETERS,value,0,0);
        value.lObstruction = 2;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OBSTRUCTIONPARAMETERS,value,1,1);
        assert(voice.m_eax3BufferProperties.lObstruction == 2);
        value.flObstructionLFRatio = 2;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OBSTRUCTIONPARAMETERS,value,1,1);
        assert(voice.m_eax3BufferProperties.flObstructionLFRatio == 2);
    }
    {
        EAXOCCLUSIONPROPERTIES value; memset(&value,0,sizeof(value));
        voice.m_eax3BufferProperties.lOcclusion = 0;
        voice.m_eax3BufferProperties.flOcclusionLFRatio = 0;
        voice.m_eax3BufferProperties.flOcclusionRoomRatio = 0;
        voice.m_eax3BufferProperties.flOcclusionDirectRatio = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OCCLUSIONPARAMETERS,value,0,0);
        value.lOcclusion = 2;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OCCLUSIONPARAMETERS,value,1,1);
        assert(voice.m_eax3BufferProperties.lOcclusion == 2);
        value.flOcclusionLFRatio = 2;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OCCLUSIONPARAMETERS,value,1,1);
        assert(voice.m_eax3BufferProperties.flOcclusionLFRatio == 2);
        value.flOcclusionRoomRatio = 2;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OCCLUSIONPARAMETERS,value,1,1);
        assert(voice.m_eax3BufferProperties.flOcclusionRoomRatio == 2);
        value.flOcclusionDirectRatio = 2;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_OCCLUSIONPARAMETERS,value,1,1);
        assert(voice.m_eax3BufferProperties.flOcclusionDirectRatio == 2);
    }
    {
        EAXEXCLUSIONPROPERTIES value; memset(&value,0,sizeof(value));
        voice.m_eax3BufferProperties.lExclusion = 0;
        voice.m_eax3BufferProperties.flExclusionLFRatio = 0;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_EXCLUSIONPARAMETERS,value,0,0);
        value.lExclusion = 2;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_EXCLUSIONPARAMETERS,value,1,1);
        assert(voice.m_eax3BufferProperties.lExclusion == 2);
        value.flExclusionLFRatio = 2;
        check(DSPROPSETID_EAX30_BufferProperties,DSPROPERTY_EAX30BUFFER_EXCLUSIONPARAMETERS,value,1,1);
        assert(voice.m_eax3BufferProperties.flExclusionLFRatio == 2);
    }
    check(DSPROPSETID_EAX30_ListenerProperties, 99,integer,0,0);
    check(DSPROPSETID_EAX30_ListenerProperties, DSPROPERTY_EAX30LISTENER_NONE,integer,0,1);
    check(DSPROPSETID_EAX30_ListenerProperties, DSPROPERTY_EAX30LISTENER_NONE|DSPROPERTY_EAX30_DEFERRED,integer,0,0);
    {
        EAX30LISTENERPROPERTIES value;
        memset(&value,0,sizeof(value));

        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ALLPARAMETERS,value,0,1);
        value.lRoom=4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ALLPARAMETERS,value,0,1);
        assert(!memcmp(&value,&manager.m_listenerProperties,sizeof(value)));
    }
    {
        long value = 2;
        manager.m_listenerProperties.lRoom = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ROOM,value,0,1);
        assert(manager.m_listenerProperties.lRoom == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ROOM,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ROOM|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.lRoom == value);
    }
    {
        long value = 2;
        manager.m_listenerProperties.lRoomHF = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ROOMHF,value,0,1);
        assert(manager.m_listenerProperties.lRoomHF == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ROOMHF,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ROOMHF|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.lRoomHF == value);
    }
    {
        long value = 2;
        manager.m_listenerProperties.lRoomLF = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ROOMLF,value,0,1);
        assert(manager.m_listenerProperties.lRoomLF == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ROOMLF,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ROOMLF|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.lRoomLF == value);
    }
    {
        float value = 2;
        manager.m_listenerProperties.flDecayTime = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_DECAYTIME,value,0,1);
        assert(manager.m_listenerProperties.flDecayTime == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_DECAYTIME,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_DECAYTIME|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.flDecayTime == value);
    }
    {
        float value = 2;
        manager.m_listenerProperties.flDecayHFRatio = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_DECAYHFRATIO,value,0,1);
        assert(manager.m_listenerProperties.flDecayHFRatio == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_DECAYHFRATIO,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_DECAYHFRATIO|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.flDecayHFRatio == value);
    }
    {
        float value = 2;
        manager.m_listenerProperties.flDecayLFRatio = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_DECAYLFRATIO,value,0,1);
        assert(manager.m_listenerProperties.flDecayLFRatio == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_DECAYLFRATIO,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_DECAYLFRATIO|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.flDecayLFRatio == value);
    }
    {
        long value = 2;
        manager.m_listenerProperties.lReflections = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REFLECTIONS,value,0,1);
        assert(manager.m_listenerProperties.lReflections == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REFLECTIONS,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REFLECTIONS|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.lReflections == value);
    }
    {
        float value = 2;
        manager.m_listenerProperties.flReflectionsDelay = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REFLECTIONSDELAY,value,0,1);
        assert(manager.m_listenerProperties.flReflectionsDelay == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REFLECTIONSDELAY,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REFLECTIONSDELAY|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.flReflectionsDelay == value);
    }
    {
        long value = 2;
        manager.m_listenerProperties.lReverb = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REVERB,value,0,1);
        assert(manager.m_listenerProperties.lReverb == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REVERB,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REVERB|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.lReverb == value);
    }
    {
        float value = 2;
        manager.m_listenerProperties.flEnvironmentSize = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ENVIRONMENTSIZE,value,0,1);
        assert(manager.m_listenerProperties.flEnvironmentSize == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ENVIRONMENTSIZE,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ENVIRONMENTSIZE|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.flEnvironmentSize == value);
    }
    {
        float value = 2;
        manager.m_listenerProperties.flEnvironmentDiffusion = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ENVIRONMENTDIFFUSION,value,0,1);
        assert(manager.m_listenerProperties.flEnvironmentDiffusion == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ENVIRONMENTDIFFUSION,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ENVIRONMENTDIFFUSION|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.flEnvironmentDiffusion == value);
    }
    {
        float value = 2;
        manager.m_listenerProperties.flEchoTime = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ECHOTIME,value,0,1);
        assert(manager.m_listenerProperties.flEchoTime == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ECHOTIME,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ECHOTIME|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.flEchoTime == value);
    }
    {
        float value = 2;
        manager.m_listenerProperties.flEchoDepth = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ECHODEPTH,value,0,1);
        assert(manager.m_listenerProperties.flEchoDepth == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ECHODEPTH,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ECHODEPTH|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.flEchoDepth == value);
    }
    {
        float value = 2;
        manager.m_listenerProperties.flModulationTime = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_MODULATIONTIME,value,0,1);
        assert(manager.m_listenerProperties.flModulationTime == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_MODULATIONTIME,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_MODULATIONTIME|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.flModulationTime == value);
    }
    {
        float value = 2;
        manager.m_listenerProperties.flModulationDepth = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_MODULATIONDEPTH,value,0,1);
        assert(manager.m_listenerProperties.flModulationDepth == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_MODULATIONDEPTH,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_MODULATIONDEPTH|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.flModulationDepth == value);
    }
    {
        float value = 2;
        manager.m_listenerProperties.flAirAbsorptionHF = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_AIRABSORPTIONHF,value,0,1);
        assert(manager.m_listenerProperties.flAirAbsorptionHF == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_AIRABSORPTIONHF,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_AIRABSORPTIONHF|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.flAirAbsorptionHF == value);
    }
    {
        unsigned long value = 2;
        manager.m_listenerProperties.ulFlags = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_FLAGS,value,0,1);
        assert(manager.m_listenerProperties.ulFlags == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_FLAGS,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_FLAGS|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.ulFlags == value);
    }
    {
        float value = 2;
        manager.m_listenerProperties.flHFReference = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_HFREFERENCE,value,0,1);
        assert(manager.m_listenerProperties.flHFReference == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_HFREFERENCE,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_HFREFERENCE|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.flHFReference == value);
    }
    {
        float value = 2;
        manager.m_listenerProperties.flLFReference = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_LFREFERENCE,value,0,1);
        assert(manager.m_listenerProperties.flLFReference == value);
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_LFREFERENCE,value,0,0);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_LFREFERENCE|DSPROPERTY_EAX30_DEFERRED,value,0,0);
        assert(manager.m_listenerProperties.flLFReference == value);
    }
    {
        EAXVECTOR value = {0,0,0};
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REFLECTIONSPAN,value,0,0);
        value.x = 4; check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REFLECTIONSPAN,value,0,1);
        assert(manager.m_listenerProperties.vReflectionsPan.x == 4);
        value.y = 4; check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REFLECTIONSPAN,value,0,1);
        assert(manager.m_listenerProperties.vReflectionsPan.y == 4);
        value.z = 4; check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REFLECTIONSPAN,value,0,1);
        assert(manager.m_listenerProperties.vReflectionsPan.z == 4);
    }
    {
        EAXVECTOR value = {0,0,0};
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REVERBPAN,value,0,0);
        value.x = 4; check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REVERBPAN,value,0,1);
        assert(manager.m_listenerProperties.vReverbPan.x == 4);
        value.y = 4; check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REVERBPAN,value,0,1);
        assert(manager.m_listenerProperties.vReverbPan.y == 4);
        value.z = 4; check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REVERBPAN,value,0,1);
        assert(manager.m_listenerProperties.vReverbPan.z == 4);
    }
    {
        float value = 2; manager.m_listenerProperties.flDecayTime = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ROOMROLLOFFFACTOR,value,0,1);
        assert(manager.m_listenerProperties.flRoomRolloffFactor == 2 && manager.m_listenerProperties.flDecayTime == 2);
        manager.m_listenerProperties.flReverbDelay = 0;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REVERBDELAY,value,0,0);
        assert(manager.m_listenerProperties.flReverbDelay == 2);
        value = 4;
        check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_REVERBDELAY,value,0,1);
        assert(manager.m_listenerProperties.flReverbDelay == 4);
        for(unsigned long index=0; index<26; ++index) {
            check(DSPROPSETID_EAX30_ListenerProperties,DSPROPERTY_EAX30LISTENER_ENVIRONMENT,index,0,1);
            assert(manager.m_listenerProperties.ulEnvironment == index);
        }
        assert(manager.m_listenerProperties.lRoom == -1000 && manager.m_listenerProperties.flEnvironmentSize == 1);
    }
    puts("00c4f600_TEST PASS");
    return 0;
}