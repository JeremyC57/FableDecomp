#ifndef FABLE_ENGINE_NUISYSTEM_CZOOMINGCOMPONENT_H
#define FABLE_ENGINE_NUISYSTEM_CZOOMINGCOMPONENT_H
// NUISystem::CZoomingComponent (new, chunk 765): RTTI .?AVCZoomingComponent@NUISystem@@, single direct base NUISystem::CChangingStateComponent at +0 (ghidra_out/rtti_bases.tsv).
// Vtables 0x01247DC4 (+0, slot 0 the ??_G 0x00546C20), 0x01247DA0 (+4, CObserver) and 0x01247D98 (+0x18, CActionParam) (rtti-vtables.tsv).
// Name set: Ego_r.pdb / FableWin.pdb ??0CZoomingComponent@NUISystem@@QAE@J@Z, ??0CZoomingComponent@NUISystem@@QAE@ABV01@@Z, ??1CZoomingComponent@NUISystem@@UAE@XZ.
// The destructor 0x00546660 removes the observer from CManager, then ~CChangingStateComponent.
// Data members are not modelled (no row needs them), hence no sizeof assertion. Overrides are not declared.
#include "engine/NUISystem_CChangingStateComponent.h"
#ifdef FABLE_RETAIL_CZOOMINGCOMPONENT_MEMBERS
#include "engine/C2DVector.h"
#endif
namespace NUISystem {
class CZoomingComponent : public CChangingStateComponent {
public:
    CZoomingComponent(long);
    CZoomingComponent(const CZoomingComponent&);
    virtual ~CZoomingComponent();
    void GetDataFromDef();
#ifdef FABLE_RETAIL_CZOOMINGCOMPONENT_VSLOTS
// agentFAM04 (2026-11-11): retail virtuals of vtable 0x01247DC4 the declarations above lack (Ego_r / FableWin
// ?Pan@CZoomingComponent@NUISystem@@UAEXVC2DVector@@@Z slot 142, ?GetZoomCenter@CZoomingComponent@NUISystem@@UAE?AVC2DVector@@XZ slot 146). Declarations only.
public:
    virtual void Pan(C2DVector);                                    // slot 142, 0x005466B0
    virtual C2DVector GetZoomCenter();                              // slot 146, 0x00546BE0
#endif // FABLE_RETAIL_CZOOMINGCOMPONENT_VSLOTS
#ifdef FABLE_RETAIL_CZOOMINGCOMPONENT_MEMBERS
    // Retail opt-in (chunk 849, row 0x0041D21B NUISystem::CManager::CreateComponent: `push 0x184 ; call operator new` before the
    // constructor 0x00546B00): the Ego_r.pdb data members (struct_layouts_egor.tsv) 0x30 lower in retail (STLport 4.6 container sizes),
    // so sizeof is retail's 0x184. Members only: no row reads them yet.
protected:
    float LastInputTime;                         // +0x15c (Ego_r +396)
    C2DVector ZoomCenter;                        // +0x160
    C2DVector MinZoom;                           // +0x168
    C2DVector MaxZoom;                           // +0x170
    bool Panning;                                // +0x178
    bool Zooming;                                // +0x179
    float InputDelay;                            // +0x17c
    float ScrollingSpeed;                        // +0x180
#endif
};
}
#ifdef FABLE_RETAIL_CZOOMINGCOMPONENT_MEMBERS
FABLE_STATIC_ASSERT(sizeof(NUISystem::CZoomingComponent) == 0x184);
#endif
#endif
