#ifndef FABLE_ENGINE_CTCHASNAME_H
#define FABLE_ENGINE_CTCHASNAME_H
#ifdef FABLE_RETAIL_CTCHASNAME_NPCSHORTNAME
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
class CHasNameDef;
class CTCHasName : public CTCBase {
public:
    CWideString GetNPCShortName();
#ifdef FABLE_RETAIL_TARGETING_PLAYER_TEST_PASS27
    CTCHasName(CThing& thing);
#endif
#ifdef FABLE_RETAIL_TARGETING_PLAYER_SET_PASS27
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_HAS_NAME; }
    CWideString GetNPCNameAndFullTitle();
#endif
private:
    CDefPointer<const CHasNameDef> PDef;
};
FABLE_STATIC_ASSERT_AT(13, sizeof(CTCHasName) == 0x10);
#endif
#endif
