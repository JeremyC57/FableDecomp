#ifndef FABLE_ENGINE_NGRAPHICAPPEARANCE_CPRIMITIVE_H
#define FABLE_ENGINE_NGRAPHICAPPEARANCE_CPRIMITIVE_H
#include "engine/CEnginePrimitiveHandle.h"
namespace NGraphicAppearance {
#ifdef FABLE_RETAIL_NGRAPHICAPPEARANCE_PASS22_PRIMITIVETYPE
// Retail opt-in (Codex pass22): original Ego_r TPI0x2795e and FableWin enum.
enum EPrimitiveType { GAPT_NONE=0,GAPT_STATIC_MESH=1,GAPT_ANIMATING_MESH=2,GAPT_MISC=3 };
#elif defined(_MSC_VER)
enum EPrimitiveType;
#else
enum EPrimitiveType {};
#endif
// Layout: both original PDBs; native Type0,PrimitiveHandle4,BankIndex12,size16.
class CPrimitive {
public:
 EPrimitiveType Type;
 CEnginePrimitiveHandle PrimitiveHandle;
 long BankIndex;
#ifdef FABLE_RETAIL_NGRAPHICAPPEARANCE_CPRIMITIVE_EYEDRAW_PASS24
 // Both PDBs and the two native 0x00642390 calls in 0x007542B0.
 CPrimitive(EPrimitiveType type, long bank_index = 0);
#else
 CPrimitive(EPrimitiveType type,long bank_index=0):Type(type),BankIndex(bank_index){}
#endif
#ifdef FABLE_RETAIL_HERO_MODIFIER_DRAW_PASS29
 ~CPrimitive() {}
#else
 ~CPrimitive();
#endif
};
}
FABLE_STATIC_ASSERT_AT(20, sizeof(NGraphicAppearance::CPrimitive)==16);
#endif
