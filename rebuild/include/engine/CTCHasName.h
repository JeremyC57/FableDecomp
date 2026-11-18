#ifndef FABLE_ENGINE_CTCHASNAME_H
#define FABLE_ENGINE_CTCHASNAME_H
#ifdef FABLE_RETAIL_CTCHASNAME_NPCSHORTNAME
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
class CHasNameDef;
class CTCHasName : public CTCBase {
public:
    CWideString GetNPCShortName();
private:
    CDefPointer<const CHasNameDef> PDef;
};
FABLE_STATIC_ASSERT(sizeof(CTCHasName) == 0x10);
#endif
#endif
