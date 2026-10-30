#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
struct CBase {  unsigned char _b[0x30]; };
class CGuildMasterDef : public CBase { public: void Copy(const CGuildMasterDef* other); };
void CGuildMasterDef::Copy(const CGuildMasterDef* other) {
    ((CDefClassBase*)(this))->operator=(*(const CDefClassBase*)other);
    *(unsigned long*)((char*)this + 0x28) = *(const unsigned long*)((const char*)other + 0x28);
    *(unsigned long*)((char*)this + 0x2c) = *(const unsigned long*)((const char*)other + 0x2c);
}