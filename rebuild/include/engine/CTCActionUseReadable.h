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
private:
    CDefPointer<const CReadableDef> PDef;
    CWideString TextTag;
};
FABLE_STATIC_ASSERT(sizeof(CTCActionUseReadable) == 0x1c);
#endif
#endif
