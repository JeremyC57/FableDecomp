#ifndef FABLE_ENGINE_CTCNOISEDEF_H
#define FABLE_ENGINE_CTCNOISEDEF_H
#include "engine/CSubDefClassBase.h"
#ifdef _MSC_VER
enum ENoiseType;
#else
enum ENoiseType{};
#endif
class CPersistContext;
// Layout: complete original 0x1780d, both PDBs; size52.
class CTCNoiseDef:public CSubDefClassBase{
public:
 virtual void Copy(const CDefClassBase * p0);
 virtual unsigned long GetSizeofClass();
 static CDefClassBase * FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL StatGetTypeName();
 ENoiseType Type;
 float LifeTime;
 float DistanceCanBeHeardFrom;
 virtual void Transfer(CPersistContext & p0);
 CTCNoiseDef(const CTCNoiseDef & p0);
#ifdef FABLE_RETAIL_CTCNOISEDEF_CTOR_INLINE
 // NF83 opt-in (round 7, row 0x00749810 CTCNoise::CTCNoise): retail expands this ctor in the CTCNoise ctor (CResource::CResource
 // 0x009FBEC0, the CDefClassBase bits and PDefManager, then the CTCNoiseDef vtable 0x01231E9C; Type / LifeTime /
 // DistanceCanBeHeardFrom are left unset). Ego_r.pdb / FableWin.pdb ??0CTCNoiseDef@@QAE@XZ. Needs FABLE_RETAIL_DEF_CLASS_BASE_CTORS,
 // FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE and FABLE_RETAIL_CDEFCLASSBASE_CTOR_SUBDEF_FRIEND.
 CTCNoiseDef() { }
#else
 CTCNoiseDef();
#endif
#ifndef FABLE_RETAIL_CTCNOISEDEF_IMPLICIT_DTOR
 virtual ~CTCNoiseDef();
#else
 // NF355 opt-in (2026-11-24, row 0x0045C5F6 ??1CThingNoiseDef@@UAE@XZ): retail's ~CTCNoiseDef is the compiler-generated one, so this
 // class declares none. Retail never calls it out of line: 0x0045C5F6 (Noise +0x70) and 0x00749A70 (CTCNoise::Definition +0x0C) expand
 // it with no CTCNoiseDef vtable store, down to ~CDefPointeeBase's store of 0x01230BA0 and the ~CResource 0x009FC550 call. Only
 // FableWin.pdb (debug) has ??1CTCNoiseDef@@UAE@XZ; Ego_r.pdb inlines it. Without this macro the landed includers keep the declaration.
#endif
 CTCNoiseDef & operator=(const CTCNoiseDef & p0);
};
FABLE_STATIC_ASSERT_AT(26, sizeof(CTCNoiseDef)==52);
#endif
