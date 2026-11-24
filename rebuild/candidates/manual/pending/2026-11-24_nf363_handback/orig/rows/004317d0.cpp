// NUISystem::CUIMiscThingsDef::~CUIMiscThingsDef() -- release the UI maps and strings, then the parent definition resource.
// Leaf: ??1CUIMiscThingsDef@NUISystem@@UAE@XZ
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPARENTDEFCLASSBASE
#include "engine/NUISystem_CUIMiscThingsDef.h"
// The original factory makes the implicit destructor available to the compiler.
// PDB exact factory plus native00432c40: new sizeof0x260, constructor004304e0.
CDefClassBase* __fastcall NUISystem::CUIMiscThingsDef::Alloc()
{
    return new CUIMiscThingsDef;
}