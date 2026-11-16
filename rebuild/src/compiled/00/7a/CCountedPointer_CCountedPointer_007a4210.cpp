// CCountedPointer<CTCWillLightning::CLightningBeam>::CCountedPointer(T*) -- take ownership of a new beam: a fresh count block (count 1,
// DeleteData 0x007A4120, Data) when non-null
// Leaf: ??0?$CCountedPointer@VCLightningBeam@CTCWillLightning@@@@QAE@PAVCLightningBeam@CTCWillLightning@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A4210. Callees: operator new 0x00BFEA1A.
// NF39 (2026-11-16): moved off its address-suffixed stand-in (CCountedPointer_007a4120 with a local count block) onto the shared
//   engine/CCountedPointer.h (the in-class (T*) constructor, emitted out of line as its own COMDAT, `ret 4`): Ego_r.pdb 0x007A93C0
//   ??0?$CCountedPointer@VCLightningBeam@CTCWillLightning@@@@QAE@PAVCLightningBeam@CTCWillLightning@@@Z; GetTargets 0x007A0D90 passes the new
//   CLightningBeam to it and binds this symbol. Preamble from the landed 0x007A3200 (~CCountedPointer<CLightningBeam>).
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCWILLLIGHTNING_CLASS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCWillLightning.h"   // NF37: the nested CLightningBeam (Ego_r.pdb)

template CCountedPointer<CTCWillLightning::CLightningBeam>::CCountedPointer(CTCWillLightning::CLightningBeam*);
