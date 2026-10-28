// NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh::CTCGraphicAppearanceAnimatedMesh(CThing& thing) -- initialize and register the appearance component.
// Leaf: ??0CTCGraphicAppearanceAnimatedMesh@NGraphicAppearance@@QAE@AAVCThing@@@Z
// Identity: proven; RTTI primary vtable0x0126ACDC/secondary0x0126ACC0, PDB own CThing& constructor and PrimHandle+16; CAGraphicAppearanceComponent secondary+12/RegisterComponent0x4BD390.
// Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
// Layout: no target header. Real RTTI multiple inheritance supplies secondary vptr+12 and PrimHandle+16; CThing is quarantined, local named PTCGraphicAppearance view preserves+0x64.
namespace NGraphicAppearance { class CAGraphicAppearanceComponent { public: CAGraphicAppearanceComponent() {} virtual ~CAGraphicAppearanceComponent() {} virtual void OnAppearanceClear() {} }; class CTCGraphicAppearance { public: void RegisterComponent(CAGraphicAppearanceComponent*); void RemoveTintColour(void*); void ClearHighlighted(void*); }; class CPrimitive; }
class CThing { public: virtual ~CThing() {} unsigned char _unmodeled04[0x60]; NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance; };
class CThing;
class CBaseClass { public: virtual ~CBaseClass() {} }; class CBaseClassNonCopyable:public CBaseClass {};
class CTCBase:public CBaseClassNonCopyable { public: CTCBase(CThing&); CThing* ParentThing; bool ToBeRemoved,IsInGlobalUpdateTCs; };
namespace NGraphicAppearance { class CTCGraphicAppearanceAnimatedMesh:public CTCBase,public CAGraphicAppearanceComponent { public: CPrimitive* PrimHandle; CTCGraphicAppearanceAnimatedMesh(CThing&); };
CTCGraphicAppearanceAnimatedMesh::CTCGraphicAppearanceAnimatedMesh(CThing& thing):CTCBase(thing),PrimHandle(0) { ParentThing->PTCGraphicAppearance->RegisterComponent(this); } }