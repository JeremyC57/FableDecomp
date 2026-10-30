class CCharString;
class CDefinitionManager;
class CGameDefinitionManager;
class CEnvironment;

class CDefinitionManager
{
public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;
};

class CGameDefinitionManager : public CDefinitionManager
{
public:
    static CGameDefinitionManager& __fastcall Get();
};

class CEnvironment
{
public:
    void TransitionToTheme(
        int internalIndex,
        int unknown1,
        long themeGlobalIndex,
        float blendTime);
};

struct CGameScriptInterfaceEnvironmentOwnerOverlay
{
    char m_Pad00[0x18];
    CEnvironment* m_pEnvironment; // 0x18
};

struct CGameScriptInterfaceOverlay
{
    void* m_pVftable; // 0x00
    CGameScriptInterfaceEnvironmentOwnerOverlay* m_pEnvironmentOwner; // 0x04
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::TransitionToThemeAllInternals(
    const CCharString& themeName,
    float blendTime) const
{
    const long globalIndex =
        CGameDefinitionManager::Get().GetDefGlobalIndexFromName(themeName);

    if (0 < globalIndex)
    {
        const CGameScriptInterfaceOverlay* const pThis =
            reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);

        for (int internalIndex = 1; internalIndex <= 4; ++internalIndex)
        {
            pThis->m_pEnvironmentOwner->m_pEnvironment->TransitionToTheme(
                internalIndex,
                1,
                globalIndex,
                blendTime);
        }
    }
}