class CStopWatch;

namespace NHeroInformationScreens
{
namespace N_0099a2d0 { class CBase
{
public:
    CBase();
    virtual ~CBase();
}; } using N_0099a2d0::CBase;
}

class CScriptThing : public NHeroInformationScreens::CBase
{
public:
    CScriptThing()
        : m_Field04(0),
          m_Field08(0)
    {
    }

    virtual ~CScriptThing();

private:
    unsigned long m_Field04;
    unsigned long m_Field08;
};

class CStopWatch
{
public:
    void* GetTicks(unsigned __int64 uid) const;
};

class CGSIGetThingWithUID_Dispatch
{
public:
    virtual void Reserved00();
    virtual void Reserved04();
    virtual void Reserved08();
    virtual CStopWatch* GetStopWatch();
};

struct CGSIGetThingWithUID_InterfaceOverlay
{
    unsigned char m_Pad00[0x04];
    CGSIGetThingWithUID_Dispatch* m_pDispatch;
};

class CGSIGetThingWithUID_Thing
{
public:
    CScriptThing WrapForScript() const;
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

CScriptThing CGameScriptInterface::GetThingWithUID(
    unsigned __int64 uid) const
{
    const CGSIGetThingWithUID_InterfaceOverlay* const pThis =
        reinterpret_cast<const CGSIGetThingWithUID_InterfaceOverlay*>(this);

    void* const pThing =
        pThis->m_pDispatch
            ->GetStopWatch()
            ->GetTicks(uid);

    if (pThing != 0)
    {
        return reinterpret_cast<const CGSIGetThingWithUID_Thing*>(pThing)
            ->WrapForScript();
    }

    return CScriptThing();
}
