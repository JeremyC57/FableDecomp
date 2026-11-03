#ifndef FABLE_ENGINE_CQ_RANSOMVICTIMCHIEFSHOUSESCRIPT_H
#define FABLE_ENGINE_CQ_RANSOMVICTIMCHIEFSHOUSESCRIPT_H
// PDB Ego_r TPI 0x10f46 and retail ctor 00debe10: CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>.
// Layout: native base 0x48; SetChiefsHut +0x48, total 0x4c. Native Init 00debe40 clears this flag.
// Native owner ctor 00debe10 and registered factory confirm this layout; donor containers are larger.
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_RansomVictimChiefsHouseScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    bool SetChiefsHut;
    class CTest : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_RansomVictimChiefsHouseScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CQ_RansomVictimChiefsHouseScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_RansomVictimChiefsHouseScript) == 0x4c);
#endif
