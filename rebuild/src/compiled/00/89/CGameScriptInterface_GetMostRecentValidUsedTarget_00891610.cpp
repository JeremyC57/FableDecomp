// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).
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

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

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

#include "engine/CTBaseSingletonNoDefaultAlloc.h"
class CQuestManager;
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())

CScriptThing
CGameScriptInterface::GetMostRecentValidUsedTarget() const
{
    CBaseObject* const item =
        reinterpret_cast<CVillageTaskCarryCrate*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())
            ->m_MostRecentTarget.GetPItem();
    if (item != 0 &&
        (reinterpret_cast<
            const CGSIGetMostRecentValidUsedTarget_BaseObject*>(
                item)->m_Flags & 1) == 0)
    {
        reinterpret_cast<CVillageTaskCarryCrate*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())
            ->AssignCreatureToTask(0);
        return item->BuildScriptThing();
    }

    return CScriptThing();
}
