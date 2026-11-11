// CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer&) -- share the other pointer: ShareData(Data, Info), return *this
// Leaf: ??4?$CCountedPointer@VCScriptGameResourceObjectScriptedThingBase@@@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AB1E0, 72 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Set_008ab1e0 (stand-in name).
// T = CScriptGameResourceObjectScriptedThingBase, proven by: 0x00D27050 binds CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=; the scripted-thing handles the NScript Main functions copy hold PImp = CCountedPointer<CScriptGameResourceObjectScriptedThingBase> (Ego_r CScriptGameResourceObjectScriptedThingBase +8); FableWin.pdb ??4 name.
// Member: the in-class operator= of engine/CCountedPointer.h (loads Data / Info, then the ShareData body), emitted as its own COMDAT.
// Model: opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (ReleaseData zeroes Data first; its stores die under ShareData's own).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectScriptedThingBase;
template CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>&);
