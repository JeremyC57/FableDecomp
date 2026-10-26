#include "engine/CFireballSpellLevelDef.h"

class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CFireballSpellLevelDef_Methods : CFireballSpellLevelDef {
    void Copy(CDefClassBase const* src);
};

void CFireballSpellLevelDef_Methods::Copy(CDefClassBase const* src)
{
    ((CDefClassBase*)(this))->operator=(*(const CDefClassBase*)(src));
    this->SpellLevel = *(unsigned long const*)((char const*)src + 0x28);
}
