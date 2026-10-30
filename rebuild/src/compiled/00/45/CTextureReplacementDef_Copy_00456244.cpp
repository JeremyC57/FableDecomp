#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
struct CSub { void Copy(const CSub* o); };
class CTextureReplacementDef { public:
    char base[0x28];
    CSub sub;
    void CTextureReplacementDef_Copy(const CTextureReplacementDef* o);
};

void CTextureReplacementDef::CTextureReplacementDef_Copy(const CTextureReplacementDef* o)
{
    ((CDefClassBase*)(this))->operator=(*(const CDefClassBase*)(o));
    this->sub.Copy(&o->sub);
}