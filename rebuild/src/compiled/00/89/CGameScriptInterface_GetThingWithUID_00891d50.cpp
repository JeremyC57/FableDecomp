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
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x0051ED80 ?GetPThingWithUID@CThingManager@@QAEPAVCThing@@_K@Z (was ?GetTicks@CStopWatch@@QBEPAX_K@Z)
//   0x004C7E50 ?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ (was ?WrapForScript@CGSIGetThingWithUID_Thing@@QBE?AVCScriptThing@@XZ)
class CThingManager { public: CThing* GetPThingWithUID(unsigned __int64); };
class CThing { public: CScriptThing GetScriptThing() const; };
CScriptThing CGameScriptInterface::GetThingWithUID(
    unsigned __int64 uid) const
{
    const CGSIGetThingWithUID_InterfaceOverlay* const pThis =
        reinterpret_cast<const CGSIGetThingWithUID_InterfaceOverlay*>(this);

    void* const pThing =
        reinterpret_cast<CThingManager*>(pThis->m_pDispatch
            ->GetStopWatch())->GetPThingWithUID(uid);

    if (pThing != 0)
    {
        return reinterpret_cast<const CThing*>(reinterpret_cast<const CGSIGetThingWithUID_Thing*>(pThing))->GetScriptThing();
    }

    return CScriptThing();
}
