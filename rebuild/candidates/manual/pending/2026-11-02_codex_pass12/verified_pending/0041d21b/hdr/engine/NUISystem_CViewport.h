#ifndef FABLE_ENGINE_NUISYSTEM_CVIEWPORT_H
#define FABLE_ENGINE_NUISYSTEM_CVIEWPORT_H
// Retail opt-in (Codex pass 12): original CChangingStateComponent base; full native0x174 and AddFakeChild slot142.
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/CEnginePrimitiveHandle.h"
namespace NUISystem {class CViewport:public CChangingStateComponent {
public:
 CViewport(long);CViewport(const CViewport&);
 virtual ~CViewport();
 virtual void Draw(CIEngine&,CEnginePrimitiveHandle*,long,long*,CComponent*);
 virtual C2DVector GetSize()const;
 virtual C2DVector GetDynamicSize()const;
 void GetDataFromDef();
 virtual void Initialise();
 virtual void AddFakeChild(CComponent*);
 virtual void FakeChildDying(CComponent*);
 virtual void ClearHandle();
protected:
 CEnginePrimitiveHandle Handle;
 std::vector<CComponent*> FakeChildren;
 unsigned long GraphicIndex;
};FABLE_STATIC_ASSERT(sizeof(CViewport)==0x174);}
#endif
