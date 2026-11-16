// CCountedPointer<CTexture>::~CCountedPointer() -- release one counted pointer
// Leaf: ??1?$CCountedPointer@VCTexture@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041BC90, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T). The catalog's "Release" placeholder and the earlier _Dest_val label name this body; the
// order of the two stores is the destructor's, not ReleaseData's (Info first).
// T (NF37): CTexture -- the one landed caller 0x00563920 (CTCHeroOnlineScoreboard::DisplayScoreboardGUI) destroys its GetCachedImage CCountedPointer<CTexture> result here (0x005643AD) and binds ??1?$CCountedPointer@VCTexture@@@@QAE@XZ.
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/CCountedPointer.h"
class CTexture;
template CCountedPointer<CTexture>::~CCountedPointer();
