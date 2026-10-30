// CBaseClass::CBaseClass() -- root polymorphic base constructor: stores the CBaseClass vtable
// Leaf: ??0CBaseClass
// Retail: Fable.exe @ 0x0099A2D0, 9 bytes. The vtable stored, 0x01231710, is CBaseClass by its RTTI locator
// (catalog label NHeroInformationScreens::CBase::CBase is wrong; GOTCHAS: 0x99A2D0/0x99A2E0 are the
// CBaseClass ctor/dtor, 0x99A2F0 the CBaseClassNonCopyable ctor).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

CBaseClass::CBaseClass()
{
}