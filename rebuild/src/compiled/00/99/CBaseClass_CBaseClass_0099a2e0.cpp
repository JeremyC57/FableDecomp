// CBaseClass::~CBaseClass() -- the root of every polymorphic engine class: re-install the CBaseClass vtable (0x01231710)
// Leaf: ??1CBaseClass@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A2E0: `mov dword ptr [ecx],0x01231710 ; ret`.
// Identity: the vtable stored is 0x01231710 (RTTI .?AVCBaseClass@@); Ego_r.pdb (0x00B01F30) and FableWin.pdb name the
//   public virtual ??1CBaseClass@@UAE@XZ right after ??0CBaseClass@@QAE@XZ (0x0099A2D0 here). Derived destructors
//   (CBaseObject 0x0099A430, CBaseClassNonCopyable 0x0099A300) inline the same store. The row was the address-named
//   free helper FableRestoreCBaseVTable_0099A2E0(CBase*).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

CBaseClass::~CBaseClass()
{
}
