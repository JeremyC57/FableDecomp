#ifndef FABLE_ENGINE_NGRAPHICAPPEARANCE_CPRIMITIVE_H
#define FABLE_ENGINE_NGRAPHICAPPEARANCE_CPRIMITIVE_H
#include "engine/CEnginePrimitiveHandle.h"
namespace NGraphicAppearance {
#ifdef _MSC_VER
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
 CPrimitive(EPrimitiveType type,long bank_index=0):Type(type),BankIndex(bank_index){}
 ~CPrimitive();
};
}
FABLE_STATIC_ASSERT(sizeof(NGraphicAppearance::CPrimitive)==16);
#endif
