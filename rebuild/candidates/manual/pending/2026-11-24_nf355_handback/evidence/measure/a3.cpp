// NF355 measurement (NOT a candidate): Alloc form, declared virtual ~CThingBaseDef, opaque Components, no pragma
// Leaf: ??1CThingNoiseDef@@UAE@XZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCNOISEDEF_IMPLICIT_DTOR
#define FABLE_RETAIL_CTCNOISEDEF_CTOR_INLINE
#include "fable_string.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CTCNoiseDef.h"
class CThingComponentSet {
public:
    ~CThingComponentSet();                                   // 0x00450BA0
    unsigned char Entries[8];
};
#pragma pack(push, 1)
class CThingBaseDef : public CParentDefClassBase {
public:
    CThingBaseDef();                                         // 0x006772D0
    virtual ~CThingBaseDef();
    unsigned char Components[8];                                                      // +0x39
    unsigned char _rest[0x37 - 8];
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CThingBaseDef) == 0x70);
class CPersistContext;
class CThingNoiseDef : public CThingBaseDef {
public:
    CThingNoiseDef() { }
    static CDefClassBase* FABLE_FASTCALL Alloc();
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    CTCNoiseDef Noise;                                       // +0x70
};
FABLE_STATIC_ASSERT(sizeof(CThingNoiseDef) == 0xa4);
CDefClassBase* CThingNoiseDef::Alloc() { return new CThingNoiseDef; }

