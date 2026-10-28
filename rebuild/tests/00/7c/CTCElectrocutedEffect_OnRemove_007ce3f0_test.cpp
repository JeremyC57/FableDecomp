#include <stdio.h>
namespace NGraphicAppearance { class CAGraphicAppearanceComponent { public: CAGraphicAppearanceComponent() {} virtual ~CAGraphicAppearanceComponent() {} virtual void OnAppearanceClear() {} }; class CTCGraphicAppearance { public: void RegisterComponent(CAGraphicAppearanceComponent*); void RemoveTintColour(void*); void ClearHighlighted(void*); }; class CPrimitive; }
class CThing { public: virtual ~CThing() {} unsigned char _unmodeled04[0x60]; NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance; };
class CThing;
class CBaseClass { public: virtual ~CBaseClass() {} }; class CBaseClassNonCopyable:public CBaseClass {};
class CTCBase:public CBaseClassNonCopyable { public: CTCBase(CThing&); CThing* ParentThing; bool ToBeRemoved,IsInGlobalUpdateTCs; };
class CTCElectrocutedEffect:public CTCBase { public: CTCElectrocutedEffect(CThing& thing):CTCBase(thing) {}  public: virtual void OnRemove(); };
void CTCElectrocutedEffect::OnRemove() { NGraphicAppearance::CTCGraphicAppearance* graphic=ParentThing->PTCGraphicAppearance; if(graphic) { graphic->RemoveTintColour(this); graphic->ClearHighlighted(this); } }
static int calls,bad; static void* expectedEffect; static NGraphicAppearance::CTCGraphicAppearance* expectedGraphic;
CTCBase::CTCBase(CThing& thing) { ParentThing=&thing; }
void NGraphicAppearance::CTCGraphicAppearance::RemoveTintColour(void* effect) { if(this!=expectedGraphic || effect!=expectedEffect || calls++) bad=1; }
void NGraphicAppearance::CTCGraphicAppearance::ClearHighlighted(void* effect) { if(this!=expectedGraphic || effect!=expectedEffect || calls++!=1) bad=1; }
int main() { CThing thing; NGraphicAppearance::CTCGraphicAppearance graphic; CTCElectrocutedEffect effect(thing); expectedEffect=&effect; expectedGraphic=&graphic; thing.PTCGraphicAppearance=&graphic; effect.OnRemove(); if(bad || calls!=2) goto fail; thing.PTCGraphicAppearance=0; calls=0; effect.OnRemove(); if(bad || calls) goto fail; puts("007ce3f0_TEST PASS"); return 0; fail: puts("007ce3f0_TEST FAIL"); return 1; }