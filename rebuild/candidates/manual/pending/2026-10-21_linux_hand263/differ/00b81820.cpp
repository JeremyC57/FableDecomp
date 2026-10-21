// Grade: DIFFER(433v433) -- scheduling only: the saved-frame store `mov [esp+N],edx ; mov edx,[ecx+368h]` lands after `push ebx` (retail after `push ebx ; push esi`); tried named world local, const local, RAII frame scope
// CEngineSimulationManager::FrameStart(const CEngineComponentParams& params) -- per-frame simulation setup: advance the wind-noise time and offset, clamp the zoom, swap in the world's simulation frame global, clear the static particle sims, then run every pending step of the full-update, always-update and paused simulation lists
// Leaf: FrameStart
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B81820, length 448 bytes (body 0x1af + int3 padding), `ret 4` (virtual; params unused).
// Callees: ClearStaticParticleSims() @ 0x00B81450 (static: it reads only globals), CEngineSimulationBase virtual slot 6 (+0x18,
//   the simulation step) on every simulation of each info's CArray.
// Globals: the engine object @ 0x01436E98 (world pointer +0x10, whose +0x368 is the current simulation frame), the timer
//   @ 0x01436EA4 (double time +0x98), the camera-ish object @ 0x01436EA0 (float +0xd4), the simulation frame global @ 0x013D2D70
//   (saved, replaced and restored).
// Layout: Ego_r.pdb CEngineSimulationManager less 0x1c: retail STLport lists are one 4-byte head pointer (PDB 12 bytes) and the base
//   is 0xc bytes, so the lists sit at +0xc/+0x10/+0x14, WindNoiseTime +0x1024, WindNoiseOffset +0x1028, WindNoiseSpeed +0x102c,
//   Zoom +0x1064 and the three pending step counts +0x1068/+0x106c/+0x1070 (engine header offsets do not hold).
// Shape: `Zoom = 1.0f > z ? 1.0f : z` (fcomp ; test ah,41h); WindNoiseOffset is `sin(WindNoiseTime * 2pi)` in double with the
//   2pi double constant equal to (double)6.2831855f; each pending loop is `while (count) { walk; --count; }`.
#include <math.h>
#pragma intrinsic(sin)

struct CEngineComponentParams;

struct CEngineSimulationBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void Simulate();                       // slot 6 (+0x18)
};
template <class T> struct CCountedPointer {        // 8 bytes
    T*    PItem;
    long* PRefCount;
};
template <class T> struct CArray {                 // retail STLport vector, 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
};
struct CEnginePrimitiveSimulationInfo {
    unsigned char _pad[8];
    CArray<CCountedPointer<CEngineSimulationBase> > Simulations;   // +0x08
};
struct SInfoNode {                                 // STLport list node of CIVCountedPointer<CEnginePrimitiveSimulationInfo>
    SInfoNode* next;
    SInfoNode* prev;
    CEnginePrimitiveSimulationInfo* info;
};

struct CWorldView { unsigned char _pad[0x368]; long SimulationFrame; };
struct CEngineData_01436e98 { unsigned char _pad[0x10]; CWorldView* World; };
struct CTimer_01436ea4 { unsigned char _pad[0x98]; double Time; };
struct CCamera_01436ea0 { unsigned char _pad[0xd4]; float Zoom; };
extern CEngineData_01436e98* g_pEngineData_01436e98;   // 0x01436E98
extern CTimer_01436ea4*      g_pTimer_01436ea4;        // 0x01436EA4
extern CCamera_01436ea0*     g_pCamera_01436ea0;       // 0x01436EA0
extern long                  g_simulationFrame;        // 0x013D2D70

void ClearStaticParticleSims();                    // 0x00B81450

static __forceinline void SimulateList(SInfoNode* const& head)
{
    for (SInfoNode* node = head->next; node != head; node = node->next) {
        CEnginePrimitiveSimulationInfo* info = node->info;
        for (CCountedPointer<CEngineSimulationBase>* it = info->Simulations.m_pFirst; it != info->Simulations.m_pLast; ++it)
            if (it->PItem)
                it->PItem->Simulate();
    }
}

struct CEngineSimulationManager {
    virtual void FrameStart(const CEngineComponentParams& params);
    unsigned char _pad004[0x8];
    SInfoNode*    FullUpdateOnlySimulationInfoList;   // +0x0c
    SInfoNode*    AlwaysUpdateSimulationInfoList;     // +0x10
    SInfoNode*    PausedSimulationInfoList;           // +0x14
    unsigned char _pad018[0x1024 - 0x18];
    float         WindNoiseTime;                      // +0x1024
    float         WindNoiseOffset;                    // +0x1028
    float         WindNoiseSpeed;                     // +0x102c
    unsigned char _pad1030[0x1064 - 0x1030];
    float         Zoom;                               // +0x1064
    unsigned long FullUpdateOnlyStepsPending;         // +0x1068
    unsigned long AlwaysUpdateStepsPending;           // +0x106c
    unsigned long PausedStepsPending;                 // +0x1070
};

void CEngineSimulationManager::FrameStart(const CEngineComponentParams& params)
{
    if (g_pEngineData_01436e98->World == 0)
        return;
    WindNoiseTime = WindNoiseSpeed * g_pTimer_01436ea4->Time;
    WindNoiseOffset = sin(WindNoiseTime * 6.2831854820251465);
    Zoom = 1.0f > g_pCamera_01436ea0->Zoom ? 1.0f : g_pCamera_01436ea0->Zoom;
    long savedFrame = g_simulationFrame;
    g_simulationFrame = g_pEngineData_01436e98->World->SimulationFrame;
    ClearStaticParticleSims();
    while (FullUpdateOnlyStepsPending) {
        SimulateList(FullUpdateOnlySimulationInfoList);
        --FullUpdateOnlyStepsPending;
    }
    while (AlwaysUpdateStepsPending) {
        SimulateList(AlwaysUpdateSimulationInfoList);
        --AlwaysUpdateStepsPending;
    }
    while (PausedStepsPending) {
        SimulateList(PausedSimulationInfoList);
        --PausedStepsPending;
    }
    g_simulationFrame = savedFrame;
}
