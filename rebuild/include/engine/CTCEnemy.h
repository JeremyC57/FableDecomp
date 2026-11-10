#ifndef FABLE_ENGINE_CTCENEMY_H
#define FABLE_ENGINE_CTCENEMY_H
// Retail opt-in (Codex pass18): original CTCBase hierarchy and every PDB field; native STLport list layout.
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CFaction;
class CTCEnemy : public CTCBase {
public:
    CTCEnemy(CThing& thing);
    virtual ~CTCEnemy();
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    virtual void OnInitialActivate();
    virtual bool IsComponentSerialisable() const;
    void SetFaction(const CCharString& faction);
protected:
    virtual void OnSerialise(CPersistContext&,bool);
private:
    bool FriendsWithEverythingFlag;
    bool EnableFollowersEnemyProxy;
    std::list<CIntelligentPointer<const CThing> > EnemyThings;
    std::list<CIntelligentPointer<const CThing> > AllyThings;
    const CFaction* PFaction;
};
FABLE_STATIC_ASSERT(sizeof(CTCEnemy)==28);
#endif
