class CBaseObject;
class CScriptThing;
class CThing;

namespace NHeroInformationScreens
{
namespace N_0099a2d0 { class CBase
{
public:
    CBase();
    virtual ~CBase();
}; } using N_0099a2d0::CBase;
}

struct CCountedPointerWords
{
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};

class CScriptThing : public NHeroInformationScreens::CBase
{
public:
    CScriptThing() { }
    virtual ~CScriptThing();

private:
    CCountedPointerWords PImp;
};

class CBaseObject
{
public:
    CScriptThing BuildScriptThing();
};

class CBaseIntelligentPointer
{
public:
    CBaseObject* GetPItem() const;
};

class CVillageTaskCarryCrate
{
public:
    void AssignCreatureToTask(CThing* thing);

private:
    unsigned char m_Unused[0xA8];

public:
    CBaseIntelligentPointer m_MostRecentTarget;
};

struct CGSIGetMostRecentValidUsedTarget_BaseObject
{
    unsigned char m_Unused[0x91];
    unsigned char m_Flags;
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

extern CVillageTaskCarryCrate*
    CGSIGetMostRecentValidUsedTarget_Manager;

CScriptThing
CGameScriptInterface::GetMostRecentValidUsedTarget() const
{
    CBaseObject* const item =
        CGSIGetMostRecentValidUsedTarget_Manager
            ->m_MostRecentTarget.GetPItem();
    if (item != 0 &&
        (reinterpret_cast<
            const CGSIGetMostRecentValidUsedTarget_BaseObject*>(
                item)->m_Flags & 1) == 0)
    {
        CGSIGetMostRecentValidUsedTarget_Manager
            ->AssignCreatureToTask(0);
        return item->BuildScriptThing();
    }

    return CScriptThing();
}
