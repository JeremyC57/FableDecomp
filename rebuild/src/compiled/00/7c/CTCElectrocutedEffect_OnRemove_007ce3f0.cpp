// CTCElectrocutedEffect::OnRemove() -- remove this effect's tint and highlighting from the parent appearance.
// Leaf: ?OnRemove@CTCElectrocutedEffect@@UAEXXZ
// Identity: proven owner/method; PDB own OnRemove prototype; OnKill RTTI slot14, OnRemove slot2 and private FinalCleanup adjacent identical body; GraphicAppearance+0x64; RemoveTintColour0x4BFAE0 proved hand951G005. ClearHighlighted0x4BFFC0 spelling probable from PDB API/order and tail-jump to ClearMeshEffect0x4BFE20.
// Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
// Layout: no target header; real CTCBase inheritance retains ParentThing+4. Quarantined CThing requires local named PTCGraphicAppearance+0x64 view.
namespace NGraphicAppearance { class CAGraphicAppearanceComponent { public: CAGraphicAppearanceComponent() {} virtual ~CAGraphicAppearanceComponent() {} virtual void OnAppearanceClear() {} }; class CTCGraphicAppearance { public: void RegisterComponent(CAGraphicAppearanceComponent*); void RemoveTintColour(void*); void ClearHighlighted(void*); }; class CPrimitive; }
class CThing { public: virtual ~CThing() {} unsigned char _unmodeled04[0x60]; NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance; };
class CThing;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
 class CBaseClassNonCopyable:public CBaseClass {};
class CTCBase:public CBaseClassNonCopyable { public: CTCBase(CThing&); CThing* ParentThing; bool ToBeRemoved,IsInGlobalUpdateTCs; };
class CTCElectrocutedEffect:public CTCBase { public: virtual void OnRemove(); };
void CTCElectrocutedEffect::OnRemove() { NGraphicAppearance::CTCGraphicAppearance* graphic=ParentThing->PTCGraphicAppearance; if(graphic) { graphic->RemoveTintColour(this); graphic->ClearHighlighted(this); } }