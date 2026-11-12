// CFireHeartPatternDef::CFireHeartPatternDef() -- default constructor: CDefClassBase(), this class's vtable 0x012CCC50, then the
// empty STLport vector<bool> PadActive (+0x28..+0x30)
// Leaf: ??0CFireHeartPatternDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F33DC0, length 29 bytes.
// Identity: stores vtable 0x012CCC50, whose slots 18 / 19 are the landed CFireHeartPatternDef::Transfer 0x00D35070 / Copy 0x00D34FB0
//   and slot 0 the deleting destructor 0x00D34F60 (calls the landed ~CFireHeartPatternDef 0x00D34F80); the vector code that
//   default-constructs elements (CPersistContext_TransferableVectorOfSubComponents<CFireHeartPatternDef>::Resize 0x00F342B0,
//   vector<CFireHeartPatternDef>::_M_fill_insert 0x00F33EF0) store the same vtable; the zeroed block is PadActive (Ego_r.pdb
//   CFireHeartPatternDef, engine/CFireHeartPatternDef.h). Ego_r.pdb / FableWin.pdb ??0CFireHeartPatternDef@@QAE@XZ.
//   One landed binder spells it ??0CDefRecord34@@QAE@XZ (caller 0x00F33D90; NAME_FIXES.tsv).
// Callee: CDefClassBase::CDefClassBase() 0x0042D9E0 (FableWin.pdb ??0CDefClassBase@@AAE@XZ): CSubDefClassBase's constructor is
//   expanded around it, its vtable store dead (opt-ins FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL / _CSUBDEFCLASSBASE_CTOR_INLINE).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE
#include "engine/CFireHeartPatternDef.h"

CFireHeartPatternDef::CFireHeartPatternDef()
{
}
