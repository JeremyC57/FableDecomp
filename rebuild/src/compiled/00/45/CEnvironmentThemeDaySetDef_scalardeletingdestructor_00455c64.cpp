// CEnvironmentThemeDaySetDef::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor: the implicit ~CEnvironmentThemeDaySetDef expanded (the TimeTheme array's out-of-line destructor, then the out-of-line ~CParentDefClassBase 0x00430300), then operator delete when bit 0 of the flags is set
// Leaf: ??_GCEnvironmentThemeDaySetDef@@UAEPAXI@Z
#pragma optimize("s",on)
// Retail: Fable.exe @ 0x00455C64 (size-optimised def TU: `pop ecx` after the delete). Identity: slot 0 of vtable 0x0123360C, RTTI
//   .?AVCEnvironmentThemeDaySetDef@@ (the catalog name agrees); it follows the constructor 0x00455C43 in the same TU.
// Callees: the TimeTheme destructor 0x0045359E (CArray<CEnvironmentThemeDef>, the vector's out-of-line COMDAT), CParentDefClassBase::
//   ~CParentDefClassBase 0x00430300 (Ego_r.pdb / FableWin.pdb ??1CParentDefClassBase@@UAE@XZ), operator delete 0x00BFE9BC.
// Model: the shared engine/CEnvironmentThemeDaySetDef.h (hdr overlay opt-in FABLE_RETAIL_CENVIRONMENTTHEMEDAYSETDEF) over
//   engine/CParentDefClassBase.h. The TU's constructor (0x00455C43, the landed row) is defined here too, so the vtable and its
//   deleting destructor are emitted as in retail.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENVIRONMENTTHEMEDAYSETDEF
#include "engine/CEnvironmentThemeDaySetDef.h"

CEnvironmentThemeDaySetDef::CEnvironmentThemeDaySetDef()
{
}
