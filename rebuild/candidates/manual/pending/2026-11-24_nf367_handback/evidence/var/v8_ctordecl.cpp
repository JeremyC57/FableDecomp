// CTCNoise::CTCNoise(CThing&) -- CTCBase(owner), vtable, an inline-constructed Definition (CTCNoiseDef), zero timer and range, an empty Creator pointer
// Leaf: ??0CTCNoise@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00749810, length 79 bytes (vtable 0x01268A74).
// Identity: Ego_r.pdb / FableWin.pdb ??0CTCNoise@@QAE@AAVCThing@@@Z.
// Callees: CTCBase::CTCBase(CThing&) 0x00686800, CResource::CResource 0x009FBEC0 (the Definition's def root: CTCNoiseDef : CSubDefClassBase
//   : CDefClassBase : CDefPointeeBase : CResource, their ctors expanded with only the CTCNoiseDef vtable 0x01231E9C store left),
//   CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10 (Creator, vtable 0x01237F1C).
// Shape: CDefClassBase's inline ctor (FABLE_RETAIL_DEF_CLASS_BASE_CTORS: PDefManager 0, the three bits cleared with retail's split
//   read-modify-write `mov cl,[edi+24h] ; xor eax,eax ; and cl,0F8h`); Definition's address stays in edi; the scalars share eax's zero.
// NF83 (round 7, q11018 binder): ported onto engine/CTCNoise.h (FABLE_RETAIL_HEARING); the local CResourceRoot / CDefClassBase /
//   CTCNoiseDef / CTCNoise models are gone, so the def root ctor binds ??0CResource@@QAE@XZ. NF83 overlay opt-ins:
//   FABLE_RETAIL_CTCNOISEDEF_CTOR_INLINE (engine/CTCNoiseDef.h) and FABLE_RETAIL_CDEFCLASSBASE_CTOR_SUBDEF_FRIEND (engine/CDefClassBase.h).
#define FABLE_RETAIL_HEARING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CDEFCLASSBASE_CTOR_SUBDEF_FRIEND
#define FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE
#define FABLE_RETAIL_CTCNOISEDEF_CTOR_INLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CTCNoise.h"

CTCNoise::CTCNoise(CThing& owner)
    : CTCBase(owner), LifeTimer(0), DistanceCanBeHeardFrom(0.0f)
{
}
