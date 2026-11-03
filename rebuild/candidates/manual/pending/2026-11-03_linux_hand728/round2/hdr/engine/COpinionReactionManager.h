#ifndef FABLE_ENGINE_COPINIONREACTIONMANAGER_H
#define FABLE_ENGINE_COPINIONREACTIONMANAGER_H
// COpinionReactionManager (chunk 728 round 2, row 0x006D1E30 CTCWife::FrameUpdate): declarations only (layout not modelled).
// Ego_r.pdb / FableWin.pdb ?GetThingAttitudeToPlayer@COpinionReactionManager@@QBE?AW4EOpinionAttitudeType@@ABVCThing@@@Z (0x00701840)
// and ?GetReactionMatchStrength@COpinionReactionManager@@QBEMW4EOpinionReactionType@@ABVCThingAICreature@@@Z (0x00701590).
class CThing;
class CThingAICreature;
enum EOpinionAttitudeType;
enum EOpinionReactionType;
class COpinionReactionManager {
public:
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing& thing) const;                              // 0x00701840
    float GetReactionMatchStrength(EOpinionReactionType reaction, const CThingAICreature& creature) const; // 0x00701590
};
#endif
