// COpinionReactionManager::GetAttitudeType(const CThing* thing, CTCOpinionSource* source) const -- with an opinion source, the attitude found by the shared attitude/reaction lookup under the attitude mask; without one, the thing's attitude to the player
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00681500, length 44 bytes (`ret 8`). Callees:
// COpinionReactionManager::GetAttitudeOrReactionType @ 0x00701010 (thing, true, mask, source),
// COpinionReactionManager::GetThingAttitudeToPlayer @ 0x00701840 (thing).
// Layout: the PDB places AttitudeMask (bool[79]) at +0x55C and DeedReactionSpeechPlaying (long[92]) at +0x5AC; retail
// reads them at +0x554 / +0x5A4 (8 bytes lower: two earlier containers are narrower in retail), so the class is modelled
// locally with the retail offsets. Shape: the source is tested first; the found arm pushes source, the mask address
// (`lea eax,[ecx+554h]`), the `true` immediate and the thing right to left; each arm ends in its own `ret 8` (no
// sibling jump: the argument counts differ).
// Leaf: GetAttitudeType
class CThing;
class CTCOpinionSource;
enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE = 0 };
enum EOpinionReactionType { OPINION_REACTION_NONE = 0 };

class COpinionReactionManager {
public:
    unsigned char _pad000[0x554];
    bool          AttitudeMask[79];                // +0x554 (PDB +0x55C)
    unsigned char _pad5a3[0x1];
    long          DeedReactionSpeechPlaying[92];   // +0x5A4 (PDB +0x5AC)

    EOpinionReactionType GetAttitudeOrReactionType(const CThing* thing, bool attitude, const bool* mask, CTCOpinionSource* source) const;   // 0x00701010
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing* thing) const;                                                            // 0x00701840
    EOpinionAttitudeType GetAttitudeType(const CThing* thing, CTCOpinionSource* source) const;
};

EOpinionAttitudeType COpinionReactionManager::GetAttitudeType(const CThing* thing, CTCOpinionSource* source) const
{
    if (source)
        return (EOpinionAttitudeType)GetAttitudeOrReactionType(thing, true, AttitudeMask, source);
    return GetThingAttitudeToPlayer(thing);
}