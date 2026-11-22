#ifndef FABLE_ENGINE_CAISTATEGROUP_CREATURESEEKENEMYUNDERGROUND_H
#define FABLE_ENGINE_CAISTATEGROUP_CREATURESEEKENEMYUNDERGROUND_H
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
// Both original PDBs and complete Ego_r layout: base0x14, PEnemy0x14, RisePos0x1c, size0x28.
class CAIStateGroup_CreatureSeekEnemyUnderground:public CAIStateGroupBase {
protected:
 virtual void Update();
 bool IsStillRunnable(){if(IsStopping())return false;return true;}
private:
 CIntelligentPointer<const CThing> PEnemy;
 C3DVector RisePos;
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_CreatureSeekEnemyUnderground)==0x28);
#endif
