#include <stdio.h>
namespace NGraphicAppearance { class CAGraphicAppearanceComponent { public: CAGraphicAppearanceComponent() {} virtual ~CAGraphicAppearanceComponent() {} virtual void OnAppearanceClear() {} }; class CTCGraphicAppearance { public: void RegisterComponent(CAGraphicAppearanceComponent*); void RemoveTintColour(void*); void ClearHighlighted(void*); }; class CPrimitive; }
class CThing { public: virtual ~CThing() {} unsigned char _unmodeled04[0x60]; NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance; };
class CThing;
class CBaseClass { public: virtual ~CBaseClass() {} }; class CBaseClassNonCopyable:public CBaseClass {};
class CTCBase:public CBaseClassNonCopyable { public: CTCBase(CThing&); CThing* ParentThing; bool ToBeRemoved,IsInGlobalUpdateTCs; };
namespace NGraphicAppearance { class CTCGraphicAppearanceStaticMesh:public CTCBase,public CAGraphicAppearanceComponent { public: CPrimitive* PrimHandle; CTCGraphicAppearanceStaticMesh(CThing&); };
CTCGraphicAppearanceStaticMesh::CTCGraphicAppearanceStaticMesh(CThing& thing):CTCBase(thing),PrimHandle(0) { ParentThing->PTCGraphicAppearance->RegisterComponent(this); } }
static int calls,bad; static CThing* expectedThing; static NGraphicAppearance::CTCGraphicAppearance* expectedAppearance; static NGraphicAppearance::CAGraphicAppearanceComponent* component;
CTCBase::CTCBase(CThing& thing) { if(&thing!=expectedThing || calls++) bad=1; ParentThing=&thing; }
void NGraphicAppearance::CTCGraphicAppearance::RegisterComponent(CAGraphicAppearanceComponent* value) { if(this!=expectedAppearance || calls++!=1) bad=1; component=value; }
int main() { CThing thing; NGraphicAppearance::CTCGraphicAppearance appearance; expectedThing=&thing; expectedAppearance=&appearance; thing.PTCGraphicAppearance=&appearance; NGraphicAppearance::CTCGraphicAppearanceStaticMesh object(thing); if(bad || calls!=2 || component!=static_cast<NGraphicAppearance::CAGraphicAppearanceComponent*>(&object) || object.ParentThing!=&thing || object.PrimHandle) { puts("0077b9b0_TEST FAIL"); return 1; } puts("0077b9b0_TEST PASS"); return 0; }