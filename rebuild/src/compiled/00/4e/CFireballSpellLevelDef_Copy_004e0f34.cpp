#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CFireballSpellLevelDef.h"

#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

struct CFireballSpellLevelDef_Methods : CFireballSpellLevelDef {
    void Copy(CDefClassBase const* src);
};

void CFireballSpellLevelDef_Methods::Copy(CDefClassBase const* src)
{
    ((CDefClassBase*)(this))->operator=(*(const CDefClassBase*)(src));
    this->SpellLevel = *(unsigned long const*)((char const*)src + 0x28);
}
