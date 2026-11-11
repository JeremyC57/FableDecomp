// CEAXManager::SetEAXProperty(const GUID* propertySet, unsigned long id, CEAXVoice* voice, void* data, unsigned long size) -- the EAX property setter: classify the property-set GUID (EAX 4.0 FX slots 0..3 / context / source, EAX 3.0 listener / buffer), store one EAX 3.0 buffer property into the voice's cached EAX30BUFFERPROPERTIES or one listener property (or a whole preset environment from the 26-entry I3DL2 table) into the manager's cached EAX30LISTENERPROPERTIES, and unless the id carries the deferred bit push a changed value on (voice: ApplyVoiceBufferProperties; then, for both sets, ApplyListenerProperties); always 0
// Grade: DIFFER(9204v9204) -- unchanged in chunk H40c (no grade spent; 13 s c2prio --rotation probes only): only the three integer listener cases ROOM / ROOMHF / ROOMLF (0x00C5137E..0x00C513DC) differ, retail reloads `data` into edx, eax, ecx, ours into ecx, edx, eax -- one expression-temporary rotation step between the buffer-apply call (0x00C512FD, `mov ecx,[size]`) and ROOM; the ALLPARAMETERS memcmp temporary walks to ebp and leaves the pointer where it was (ecx)
// Next: the catalog name PropertySerializer_GetPropertyTypeCode is wrong (this is CEAXManager::SetEAXProperty, LABELS.tsv). Probed in H40c without moving the pointer (rotation listing lines 239-248): memcpy intrinsic for the copy, `!= 0`, a typed local pointer, `changed = true` first / in both arms, `changed = memcmp(...) != 0`, the apply-and-return tail written inside the case, a named `property` for the switch value. The two levers that closed such a step in this chunk were (a) statements written in BOTH arms of an if / else that c2 cross-jumps away after code generation (0x00E64FB0) and (b) a variable zeroed earlier than its loop, which keeps a scratch register busy (0x00D5AE70): look for either in the buffer branch's tail (`if (changed && !deferred) ApplyVoiceBufferProperties(...)` followed by the shared listener apply) and in the NONE / ALLPARAMETERS cases H40f (1 baseline grade, 30 rotation probes at 15 s, tools/sd.py self-diffs; work/c4/): what retail needs is ONE more rotation step between the buffer apply (two temporaries: size -> ecx, the id argument -> eax, pointer back at ecx) and ROOM, i.e. a temporary that takes ecx and leaves no code. Dead ends, all with the pointer still at ecx or with visible code: the listener tail duplicated in the buffer arm (other allocation regime, 9104 bytes), NONE / ALLPARAMETERS with a direct `if (!deferred) Apply` tail (NONE: pointer right but an unmerged `lea ecx` pad, 9220 bytes; ALLPARAMETERS: the address is the memcmp CSE, no temporary), NONE moved last, NONE as fall-through of ALLPARAMETERS, the equal / unequal arms written out, a typed pointer, a named switch value, `if (changed) if (!deferred)`, `(long)id >= 0`, an early `if (!changed) return 0`, the call through a named `manager` / `(*this).` / a reference / a base class at +0 / a first member at +0, `propertySetType` as the version argument (c2 folds it to `push 2`, byte-identical), an unshared rep count. Passing 0 for id or size gives ROOM = edx but another regime (9388 bytes). H40k (1 grade): baseline 85, unchanged; no new spelling tried (30 rotation probes of earlier passes stand). Header debt: row-local CEAXManager / CEAXVoice models.
// Leaf: ?SetEAXProperty@CEAXManager@@QAEJPBU_GUID@@KPAUCEAXVoice@@PAXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4F600, 9204 bytes = code to 0x00C5192C + `lea ecx,[ecx]` + the 23-entry buffer
// case table at 0x00C51930 and the 26-entry listener case table at 0x00C5198C (one function); thiscall `ret 14h`,
// `sub esp,0AFCh` (the 26 x 0x6C-byte preset table is a local).
// Identity: the landed CEAXManager::QueueCommit 0x00C51A20 declares and calls it; its callees are the landed
// ApplyVoiceBufferProperties 0x00C4EA30 (buffer set, `push 2`) and ApplyListenerProperties 0x00C4E3C0 (`lea edx,[ebx+74h]`).
// GUIDs: DSPROPSETID_EAX30_ListenerProperties 0x012B9418, _BufferProperties 0x012B9428, EAXPROPERTYID_EAX40_Context
// 0x012B9448, FXSlot0..3 0x012B9468..0x012B9498, Source 0x012B94A8 (memcmp intrinsic: `repe cmpsd`).
// Shape: `bool changed = false` and ONE trailing `if (changed && !deferred) ApplyListenerProperties(...)` after the
// buffer / listener if-else: retail keeps `changed` in bl across the ApplyVoiceBufferProperties call and jumps to that
// listener test (0x00C51313 -> 0x00C51900), so a non-deferred buffer change also re-applies the listener; with the apply
// inside the listener branch the zero constant stays live past the GUID ladder (ebx, `cmp al,bl`) and every register
// rotates. Retail bugs kept: ROOMROLLOFFFACTOR stores the factor and falls into DECAYTIME; REVERBDELAY tests the room
// rolloff factor and stores the reverb delay; ENVIRONMENT indexes the table unchecked.
#include "engine/EAXNativeTypes.h"

extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int size);
#pragma intrinsic(memcmp)

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
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -100, 0, 1.49f, 0.83f, 1.0f, -2602, 0.007f, 200, 0.011f, 7.5f, 1.0f, 1.0f, 0.25f, 0.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 0
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -6000, 0, 0.17f, 0.1f, 1.0f, -1204, 0.001f, 207, 0.002f, 1.4f, 1.0f, 1.0f, 0.25f, 0.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 1
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -454, 0, 0.4f, 0.83f, 1.0f, -1646, 0.002f, 53, 0.003f, 1.9f, 1.0f, 1.0f, 0.25f, 0.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 2
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -1200, 0, 1.49f, 0.54f, 1.0f, -370, 0.007f, 1030, 0.011f, 1.4f, 1.0f, 1.0f, 0.25f, 0.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 3
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -6000, 0, 0.5f, 0.1f, 1.0f, -1376, 0.003f, -1104, 0.004f, 2.5f, 1.0f, 1.0f, 0.25f, 0.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 4
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -300, 0, 2.31f, 0.64f, 1.0f, -711, 0.012f, 83, 0.017f, 11.6f, 1.0f, 1.0f, 0.25f, 0.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 5
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -476, 0, 4.32f, 0.37f, 1.0f, -789, 0.02f, -289, 0.03f, 21.6f, 1.0f, 1.0f, 0.25f, 0.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 6
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -500, 0, 3.92f, 0.4f, 1.0f, -1230, 0.02f, -2, 0.029f, 19.6f, 1.0f, 1.0f, 0.25f, 0.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 7
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, 0, 0, 2.91f, 1.3f, 1.0f, -602, 0.015f, -302, 0.022f, 14.6f, 1.0f, 1.0f, 0.25f, 0.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 8
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -698, 0, 7.24f, 0.22f, 1.0f, -1166, 0.02f, 16, 0.03f, 36.2f, 1.0f, 1.0f, 0.25f, 0.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 9
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -1000, 0, 10.05f, 0.16f, 1.0f, -602, 0.02f, 198, 0.03f, 50.3f, 1.0f, 1.0f, 0.25f, 0.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 10
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -4000, 0, 0.3f, 0.1f, 1.0f, -1831, 0.002f, -1630, 0.03f, 1.9f, 1.0f, 1.0f, 0.25f, 0.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 11
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -300, 0, 1.49f, 0.59f, 1.0f, -1219, 0.007f, 441, 0.011f, 1.8f, 1.0f, 1.0f, 0.25f, 0.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 12
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -237, 0, 2.7f, 0.59f, 1.0f, -1214, 0.013f, 395, 0.02f, 13.5f, 1.0f, 1.0f, 0.25f, 0.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 13
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -270, 0, 1.49f, 0.86f, 1.0f, -1204, 0.007f, -4, 0.011f, 7.5f, 0.3f, 1.0f, 0.125f, 0.95f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 14
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -3300, 0, 1.49f, 0.54f, 1.0f, -2560, 0.162f, -229, 0.088f, 38.0f, 0.3f, 1.0f, 0.125f, 1.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 15
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -800, 0, 1.49f, 0.67f, 1.0f, -2273, 0.007f, -1691, 0.011f, 7.5f, 0.5f, 1.0f, 0.25f, 0.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 16
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -2500, 0, 1.49f, 0.21f, 1.0f, -2780, 0.3f, -1434, 0.1f, 100.0f, 0.27f, 1.0f, 0.25f, 1.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 17
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -1000, 0, 1.49f, 0.83f, 1.0f, -10000, 0.061f, 500, 0.025f, 17.5f, 1.0f, 1.0f, 0.125f, 0.7f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 18
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -2000, 0, 1.49f, 0.5f, 1.0f, -2466, 0.179f, -1926, 0.1f, 42.5f, 0.21f, 1.0f, 0.25f, 1.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 19
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, 0, 0, 1.65f, 1.5f, 1.0f, -1363, 0.008f, -1153, 0.012f, 8.3f, 1.0f, 1.0f, 0.25f, 0.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 20
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -1000, 0, 2.81f, 0.14f, 1.0f, 429, 0.014f, 1023, 0.021f, 1.7f, 0.8f, 1.0f, 0.25f, 0.0f, 0.25f, 0.0f, 5000, 250, 0.0f, -5 },   // 21
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -4000, 0, 1.49f, 0.1f, 1.0f, -449, 0.007f, 1700, 0.011f, 1.8f, 1.0f, 1.0f, 0.25f, 0.0f, 1.18f, 0.35f, 5000, 250, 0.0f, -5 },   // 22
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, 0, 0, 8.39f, 1.39f, 1.0f, -115, 0.002f, 985, 0.03f, 1.9f, 0.5f, 1.0f, 0.25f, 0.0f, 0.25f, 1.0f, 5000, 250, 0.0f, -5 },   // 23
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -400, 0, 17.23f, 0.56f, 1.0f, -1713, 0.02f, -613, 0.03f, 1.8f, 0.6f, 1.0f, 0.25f, 1.0f, 0.81f, 0.31f, 5000, 250, 0.0f, -5 },   // 24
        { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, -1000, -151, 0, 7.56f, 0.91f, 1.0f, -626, 0.02f, 774, 0.03f, 1.0f, 0.5f, 1.0f, 0.25f, 0.0f, 4.0f, 1.0f, 5000, 250, 0.0f, -5 },   // 25
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
                if (voice->m_eax3BufferProperties.lObstruction == obstruction->lObstruction
                    && voice->m_eax3BufferProperties.flObstructionLFRatio == obstruction->flObstructionLFRatio)
                    return 0;
                voice->m_eax3BufferProperties.lObstruction = obstruction->lObstruction;
                voice->m_eax3BufferProperties.flObstructionLFRatio = obstruction->flObstructionLFRatio;
                changed = true;
                break;
            }
            case DSPROPERTY_EAX30BUFFER_OCCLUSIONPARAMETERS: {
                EAXOCCLUSIONPROPERTIES* occlusion = (EAXOCCLUSIONPROPERTIES*)data;
                if (voice->m_eax3BufferProperties.lOcclusion == occlusion->lOcclusion
                    && voice->m_eax3BufferProperties.flOcclusionLFRatio == occlusion->flOcclusionLFRatio
                    && voice->m_eax3BufferProperties.flOcclusionRoomRatio == occlusion->flOcclusionRoomRatio
                    && voice->m_eax3BufferProperties.flOcclusionDirectRatio == occlusion->flOcclusionDirectRatio)
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
                if (voice->m_eax3BufferProperties.lExclusion == exclusion->lExclusion
                    && voice->m_eax3BufferProperties.flExclusionLFRatio == exclusion->flExclusionLFRatio)
                    return 0;
                voice->m_eax3BufferProperties.lExclusion = exclusion->lExclusion;
                voice->m_eax3BufferProperties.flExclusionLFRatio = exclusion->flExclusionLFRatio;
                changed = true;
                break;
            }
            case DSPROPERTY_EAX30BUFFER_DIRECT:
                changed = voice->m_eax3BufferProperties.lDirect != *(long*)data; voice->m_eax3BufferProperties.lDirect = *(long*)data; break;
            case DSPROPERTY_EAX30BUFFER_DIRECTHF:
                changed = voice->m_eax3BufferProperties.lDirectHF != *(long*)data; voice->m_eax3BufferProperties.lDirectHF = *(long*)data; break;
            case DSPROPERTY_EAX30BUFFER_ROOM:
                changed = voice->m_eax3BufferProperties.lRoom != *(long*)data; voice->m_eax3BufferProperties.lRoom = *(long*)data; break;
            case DSPROPERTY_EAX30BUFFER_ROOMHF:
                changed = voice->m_eax3BufferProperties.lRoomHF != *(long*)data; voice->m_eax3BufferProperties.lRoomHF = *(long*)data; break;
            case DSPROPERTY_EAX30BUFFER_OBSTRUCTION:
                changed = voice->m_eax3BufferProperties.lObstruction != *(long*)data; voice->m_eax3BufferProperties.lObstruction = *(long*)data; break;
            case DSPROPERTY_EAX30BUFFER_OBSTRUCTIONLFRATIO:
                changed = voice->m_eax3BufferProperties.flObstructionLFRatio != *(float*)data; voice->m_eax3BufferProperties.flObstructionLFRatio = *(float*)data; break;
            case DSPROPERTY_EAX30BUFFER_OCCLUSION:
                changed = voice->m_eax3BufferProperties.lOcclusion != *(long*)data; voice->m_eax3BufferProperties.lOcclusion = *(long*)data; break;
            case DSPROPERTY_EAX30BUFFER_OCCLUSIONLFRATIO:
                changed = voice->m_eax3BufferProperties.flOcclusionLFRatio != *(float*)data; voice->m_eax3BufferProperties.flOcclusionLFRatio = *(float*)data; break;
            case DSPROPERTY_EAX30BUFFER_OCCLUSIONROOMRATIO:
                changed = voice->m_eax3BufferProperties.flOcclusionRoomRatio != *(float*)data; voice->m_eax3BufferProperties.flOcclusionRoomRatio = *(float*)data; break;
            case DSPROPERTY_EAX30BUFFER_OCCLUSIONDIRECTRATIO:
                changed = voice->m_eax3BufferProperties.flOcclusionDirectRatio != *(float*)data; voice->m_eax3BufferProperties.flOcclusionDirectRatio = *(float*)data; break;
            case DSPROPERTY_EAX30BUFFER_EXCLUSION:
                changed = voice->m_eax3BufferProperties.lExclusion != *(long*)data; voice->m_eax3BufferProperties.lExclusion = *(long*)data; break;
            case DSPROPERTY_EAX30BUFFER_EXCLUSIONLFRATIO:
                changed = voice->m_eax3BufferProperties.flExclusionLFRatio != *(float*)data; voice->m_eax3BufferProperties.flExclusionLFRatio = *(float*)data; break;
            case DSPROPERTY_EAX30BUFFER_OUTSIDEVOLUMEHF:
                changed = voice->m_eax3BufferProperties.lOutsideVolumeHF != *(long*)data; voice->m_eax3BufferProperties.lOutsideVolumeHF = *(long*)data; break;
            case DSPROPERTY_EAX30BUFFER_DOPPLERFACTOR:
                changed = voice->m_eax3BufferProperties.flDopplerFactor != *(float*)data; voice->m_eax3BufferProperties.flDopplerFactor = *(float*)data; break;
            case DSPROPERTY_EAX30BUFFER_ROLLOFFFACTOR:
                changed = voice->m_eax3BufferProperties.flRolloffFactor != *(float*)data; voice->m_eax3BufferProperties.flRolloffFactor = *(float*)data; break;
            case DSPROPERTY_EAX30BUFFER_ROOMROLLOFFFACTOR:
                changed = voice->m_eax3BufferProperties.flRoomRolloffFactor != *(float*)data; voice->m_eax3BufferProperties.flRoomRolloffFactor = *(float*)data; break;
            case DSPROPERTY_EAX30BUFFER_AIRABSORPTIONFACTOR:
                changed = voice->m_eax3BufferProperties.flAirAbsorptionFactor != *(float*)data; voice->m_eax3BufferProperties.flAirAbsorptionFactor = *(float*)data; break;
            case DSPROPERTY_EAX30BUFFER_FLAGS:
                changed = voice->m_eax3BufferProperties.ulFlags != *(unsigned long*)data; voice->m_eax3BufferProperties.ulFlags = *(unsigned long*)data; break;
            default:
                return 0;
            }
            if (changed && !(id & DSPROPERTY_EAX30_DEFERRED))
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
                changed = m_listenerProperties.lRoom != *(long*)data; m_listenerProperties.lRoom = *(long*)data; break;
            case DSPROPERTY_EAX30LISTENER_ROOMHF:
                changed = m_listenerProperties.lRoomHF != *(long*)data; m_listenerProperties.lRoomHF = *(long*)data; break;
            case DSPROPERTY_EAX30LISTENER_ROOMLF:
                changed = m_listenerProperties.lRoomLF != *(long*)data; m_listenerProperties.lRoomLF = *(long*)data; break;
            case DSPROPERTY_EAX30LISTENER_ROOMROLLOFFFACTOR:
                m_listenerProperties.flRoomRolloffFactor = *(float*)data;
                // falls through (retail): the decay-time test and store follow
            case DSPROPERTY_EAX30LISTENER_DECAYTIME:
                changed = m_listenerProperties.flDecayTime != *(float*)data; m_listenerProperties.flDecayTime = *(float*)data; break;
            case DSPROPERTY_EAX30LISTENER_DECAYHFRATIO:
                changed = m_listenerProperties.flDecayHFRatio != *(float*)data; m_listenerProperties.flDecayHFRatio = *(float*)data; break;
            case DSPROPERTY_EAX30LISTENER_DECAYLFRATIO:
                changed = m_listenerProperties.flDecayLFRatio != *(float*)data; m_listenerProperties.flDecayLFRatio = *(float*)data; break;
            case DSPROPERTY_EAX30LISTENER_REFLECTIONS:
                changed = m_listenerProperties.lReflections != *(long*)data; m_listenerProperties.lReflections = *(long*)data; break;
            case DSPROPERTY_EAX30LISTENER_REFLECTIONSDELAY:
                changed = m_listenerProperties.flReflectionsDelay != *(float*)data; m_listenerProperties.flReflectionsDelay = *(float*)data; break;
            case DSPROPERTY_EAX30LISTENER_REFLECTIONSPAN: {
                EAXVECTOR* pan = (EAXVECTOR*)data;
                if (m_listenerProperties.vReflectionsPan.x == pan->x && m_listenerProperties.vReflectionsPan.y == pan->y
                    && m_listenerProperties.vReflectionsPan.z == pan->z)
                    return 0;
                m_listenerProperties.vReflectionsPan = *pan;
                changed = true;
                break;
            }
            case DSPROPERTY_EAX30LISTENER_REVERB:
                changed = m_listenerProperties.lReverb != *(long*)data; m_listenerProperties.lReverb = *(long*)data; break;
            case DSPROPERTY_EAX30LISTENER_REVERBDELAY:
                // retail tests the room rolloff factor here and stores the reverb delay
                changed = m_listenerProperties.flRoomRolloffFactor != *(float*)data; m_listenerProperties.flReverbDelay = *(float*)data; break;
            case DSPROPERTY_EAX30LISTENER_REVERBPAN: {
                EAXVECTOR* pan = (EAXVECTOR*)data;
                if (m_listenerProperties.vReverbPan.x == pan->x && m_listenerProperties.vReverbPan.y == pan->y
                    && m_listenerProperties.vReverbPan.z == pan->z)
                    return 0;
                m_listenerProperties.vReverbPan = *pan;
                changed = true;
                break;
            }
            case DSPROPERTY_EAX30LISTENER_ENVIRONMENT:
                m_listenerProperties.ulEnvironment = *(unsigned long*)data;
                m_listenerProperties.flAirAbsorptionHF = (float)environments[m_listenerProperties.ulEnvironment].m_airAbsorption;
                m_listenerProperties.flDecayHFRatio = environments[m_listenerProperties.ulEnvironment].m_decayHFRatio;
                m_listenerProperties.flDecayTime = environments[m_listenerProperties.ulEnvironment].m_decayTime;
                m_listenerProperties.flEnvironmentDiffusion = environments[m_listenerProperties.ulEnvironment].m_environmentDiffusion;
                m_listenerProperties.flEnvironmentSize = environments[m_listenerProperties.ulEnvironment].m_environmentSize;
                m_listenerProperties.flReflectionsDelay = environments[m_listenerProperties.ulEnvironment].m_reflectionsDelay;
                m_listenerProperties.flReverbDelay = environments[m_listenerProperties.ulEnvironment].m_reverbDelay;
                m_listenerProperties.flRoomRolloffFactor = environments[m_listenerProperties.ulEnvironment].m_roomRolloffFactor;
                m_listenerProperties.lReflections = environments[m_listenerProperties.ulEnvironment].m_reflections;
                m_listenerProperties.lReverb = environments[m_listenerProperties.ulEnvironment].m_reverb;
                m_listenerProperties.lRoom = environments[m_listenerProperties.ulEnvironment].m_room;
                m_listenerProperties.lRoomHF = environments[m_listenerProperties.ulEnvironment].m_roomHF;
                changed = true;
                break;
            case DSPROPERTY_EAX30LISTENER_ENVIRONMENTSIZE:
                changed = m_listenerProperties.flEnvironmentSize != *(float*)data; m_listenerProperties.flEnvironmentSize = *(float*)data; break;
            case DSPROPERTY_EAX30LISTENER_ENVIRONMENTDIFFUSION:
                changed = m_listenerProperties.flEnvironmentDiffusion != *(float*)data; m_listenerProperties.flEnvironmentDiffusion = *(float*)data; break;
            case DSPROPERTY_EAX30LISTENER_ECHOTIME:
                changed = m_listenerProperties.flEchoTime != *(float*)data; m_listenerProperties.flEchoTime = *(float*)data; break;
            case DSPROPERTY_EAX30LISTENER_ECHODEPTH:
                changed = m_listenerProperties.flEchoDepth != *(float*)data; m_listenerProperties.flEchoDepth = *(float*)data; break;
            case DSPROPERTY_EAX30LISTENER_MODULATIONTIME:
                changed = m_listenerProperties.flModulationTime != *(float*)data; m_listenerProperties.flModulationTime = *(float*)data; break;
            case DSPROPERTY_EAX30LISTENER_MODULATIONDEPTH:
                changed = m_listenerProperties.flModulationDepth != *(float*)data; m_listenerProperties.flModulationDepth = *(float*)data; break;
            case DSPROPERTY_EAX30LISTENER_AIRABSORPTIONHF:
                changed = m_listenerProperties.flAirAbsorptionHF != *(float*)data; m_listenerProperties.flAirAbsorptionHF = *(float*)data; break;
            case DSPROPERTY_EAX30LISTENER_FLAGS:
                changed = m_listenerProperties.ulFlags != *(unsigned long*)data; m_listenerProperties.ulFlags = *(unsigned long*)data; break;
            case DSPROPERTY_EAX30LISTENER_HFREFERENCE:
                changed = m_listenerProperties.flHFReference != *(float*)data; m_listenerProperties.flHFReference = *(float*)data; break;
            case DSPROPERTY_EAX30LISTENER_LFREFERENCE:
                changed = m_listenerProperties.flLFReference != *(float*)data; m_listenerProperties.flLFReference = *(float*)data; break;
            default:
                return 0;
            }
        }
    }
    if (changed && !(id & DSPROPERTY_EAX30_DEFERRED))
        ApplyListenerProperties(&m_listenerProperties);
    return 0;
}
