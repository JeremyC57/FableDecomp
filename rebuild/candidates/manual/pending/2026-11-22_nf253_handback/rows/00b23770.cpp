// CIEngine::`scalar deleting destructor'(unsigned int) -- compiler-generated for CIEngine's inline virtual destructor
// Leaf: ??_GCIEngine@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23770, 36 bytes; referenced only from slot 0 of CIEngine's vtable 0x012A0D24
//   (.rdata; no E8 / E9 caller). A pick-any COMDAT: VC7.1 emits it beside the CIEngine vtable in every TU that stores that vptr.
// Retail bytes:  push esi ; mov esi,ecx ; mov dword ptr [esi],offset CIEngine::`vftable' ; call CInitBaseClass::~CInitBaseClass ;
//   test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// Callees: CInitBaseClass::~CInitBaseClass() @ 0x0099A410 (??1CInitBaseClass@@UAE@XZ), operator delete @ 0x00BFE9BC. Vtable 0x012A0D24 is
//   CIEngine's (rtti-vtables.tsv).
// Names: ??_GCIEngine@@UAEPAXI@Z in Ego_r.pdb, FableWin.pdb and Ego_d.pdb (raw grep); ??1CIEngine@@UAE@XZ (virtual) in FableWin.pdb + Ego_d.pdb;
//   vtable slot 0; engine/CIEngine.h FABLE_RETAIL_CIENGINE_INLINE_DTOR.
// NF253 (q12710, landing nf251 ruling 4): was a TU emitter over local CInitBaseClass / CIEngine models that also defined a strong
//   ??0CIEngine@@QAE@XZ (no PDB has the public spelling: FableWin + Ego_d spell it IAE), a strong ??0CInitBaseClass@@QAE@XZ and a global
//   ?MakeOne@@YAPAXXZ. Now the header's classes and the 0x00B23750 scaffolding: the TU emits CIEngine's inline constructor, vtable and
//   this deleting destructor as pick-any COMDATs, as every TU that uses them does (the CEngine ctor / dtor rows and 0x00B23750 carry
//   equal copies), so no object defines any of them strongly.
#define FABLE_RETAIL_CINITBASECLASS_VALIDATE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_INHERITED_VALIDATE
#define FABLE_RETAIL_CIENGINE_CTOR
#define FABLE_RETAIL_CIENGINE_INLINE_DTOR
#define FABLE_RETAIL_CIENGINE_RENDER
#include "fable_string.h"
#include "engine/CInitBaseClass.h"
#include "engine/CIEngine.h"

namespace {
// Fidelity: scaffolding, not retail: the smallest use of the header's protected inline constructor (a derived class's out-of-line
// constructor in the anonymous namespace; CIEngine is abstract, so it is never instantiated). The inlined CIEngine constructor stores
// CIEngine's vptr, so this TU emits the CIEngine vtable and the ??_GCIEngine it references as pick-any COMDATs. Internal to this TU
// (hashed names); the same scaffolding as 0x00B23750.
class CIEngineConstructorReference : public CIEngine {
public:
    CIEngineConstructorReference();
};
CIEngineConstructorReference::CIEngineConstructorReference()
{
}
}
