#ifndef FABLE_ENGINE_CTCACTIONUSEREADABLE_H
#define FABLE_ENGINE_CTCACTIONUSEREADABLE_H
#ifdef FABLE_RETAIL_CTCACTIONUSEREADABLE_PASS24
#include "engine/CTCActionUseBase.h"
#include "engine/CReadableDef.h"
#include "fable_string.h"
// Original action-use subtype and its two members. Native +0x14 reads PDef.
class CTCActionUseReadable : public CTCActionUseBase {
public:
    bool UsesInfoDisplay() const { return PDef->UseInfoDisplay; }
#ifdef FABLE_RETAIL_TARGETING_PLAYER_TEST_PASS27
    CTCActionUseReadable(CThing& thing);
#endif
#ifdef FABLE_RETAIL_TARGETING_PLAYER_SET_PASS27
    static CCharString FABLE_FASTCALL StatGetClassTypeName() { return CCharString("CTCActionUseReadable"); }
    const CReadableDef& PeekDef() const { return *PDef; }
    const CWideString& GetTextTag() const { return TextTag; }
#endif
private:
    CDefPointer<const CReadableDef> PDef;
    CWideString TextTag;
};
FABLE_STATIC_ASSERT_AT(15, sizeof(CTCActionUseReadable) == 0x1c);
#endif
#endif
