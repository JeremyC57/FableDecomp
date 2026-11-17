// CTCActionUseReadable::OnSerialise(CPersistContext& context, bool loading) -- transfer the TextTag wide string under the name "GameTextDefName" with an empty default
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E8160, length 77 bytes.
// Leaf: OnSerialise
// Callees: CWideString::CWideString() @ 0x0099AED0, CWideString::CWideString(const CWideString&) @ 0x0099B720,
// CWideString::~CWideString @ 0x0099B510, CPersistContext::Transfer(const char*, T&, const T&) @ 0x00410AC0 (the ICF-shared
// template body; the manifest names its CIntelligentPointer<CThing> instantiation). Literal 0x01270BDC = "GameTextDefName".
// Shape: two frame strings -- A default-constructed, B copy-constructed from A, A destroyed BEFORE the call, B passed by
// const reference and destroyed after: the default value comes from an inline helper that returns a NAMED local by value
// (VC7.1 has no NRVO, so the local is copied into the return temporary and destroyed at the helper's end). The base
// OnSerialise is not called; `loading` is unused.
// Name fix (agentNF54): the local CWideString / CPersistContext stand-ins replaced by the shared fable_string.h and
//   engine/CPersistContext.h: 0x00410AC0 is the member template instance ??$Transfer@VCWideString@@@CPersistContext@@QAEXPBDAAVCWideString@@ABV1@@Z
//   (Ego_r 0x0040D1C0 + FableWin 0x01895750; was the non-template ?Transfer@CPersistContext@@QAEXPBDAAVCWideString@@ABV2@@Z).
#define FABLE_RETAIL_CWIDESTRING
#include "fable_string.h"
#include "engine/CPersistContext.h"

inline CWideString EmptyWideString()
{
    CWideString empty;
    return empty;
}

class CThing;

class CTCBase { public:                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
};

class CTCActionUseBase : public CTCBase { public:
    unsigned char _pad0c[8];                   // +0x0c PActionUseDef, +0x10 Usable
};

class CTCActionUseReadable : public CTCActionUseBase { public:
    void*       PDef;                          // +0x14 CDefPointer<CReadableDef const>
    CWideString TextTag;                       // +0x18
    virtual void OnSerialise(CPersistContext& context, bool loading);
};

void CTCActionUseReadable::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("GameTextDefName", TextTag, EmptyWideString());
}