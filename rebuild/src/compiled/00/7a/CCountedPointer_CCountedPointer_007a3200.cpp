// CCountedPointer<CTCWillLightning::CLightningBeam>::~CCountedPointer() -- release one counted pointer
// Leaf: ??1?$CCountedPointer@VCLightningBeam@CTCWillLightning@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A3200, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T). The catalog's "Release" placeholder and the earlier _Dest_val label name this body; the
// order of the two stores is the destructor's, not ReleaseData's (Info first).
// T (NF37): CTCWillLightning::CLightningBeam -- GetTargets 0x007A0D90 destroys its CCountedPointer<CLightningBeam> local here (0x007A1343) after building it with 0x007A4210, and both landed callers (0x007A0D90, 0x007A1B20) bind ??1?$CCountedPointer@VCLightningBeam@CTCWillLightning@@@@QAE@XZ.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
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
template CCountedPointer<CTCWillLightning::CLightningBeam>::~CCountedPointer();
