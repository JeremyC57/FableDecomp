// CTCCreatureOpinionOfHero::IsWillingToAttackHeroOnOpinion() const -- hero enemies always; otherwise by the creature's attitude to the player (attitudes 2, 4-6 and 12-17 refuse)
// Grade: DIFFER(84v84) -- only the switch bias: retail `sub eax,2` (83 e8 02), ours `add eax,-2` (83 c0 fe); every other byte equal
// Leaf: ?IsWillingToAttackHeroOnOpinion@CTCCreatureOpinionOfHero@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006817F0, 59 bytes of code + the 2-entry jump table and 16-entry byte index table
//   (84 in all). Caller 0x00864AE6. Name: Ego_r.pdb ?IsWillingToAttackHeroOnOpinion@CTCCreatureOpinionOfHero@@QBE_NXZ (reloc_bindings).
// Callees: CTCBase::PeekWorld() const 0x00686A80 (Ego_r.pdb, protected; the landed spelling), COpinionReactionManager::
//   GetThingAttitudeToPlayer(const CThing&) const 0x00701840 (Ego_r.pdb ...QBE?AW4EOpinionAttitudeType@@ABVCThing@@@Z) on the
//   world's counted POpinionReactionManager (+0x74).
// Shape: HeroOpinionEnemy (+0x342) short-circuits to the shared `mov al,1` exit; ParentThing is read into esi before the world call.
//   The switch over attitudes 2..17 is a byte-indexed two-entry table. Both retail `sub`-biased switches in the exe sit in this
//   TU (0x0068180E, and 0x0068186E in the next function); every other byte-indexed switch uses `add eax,-N`.
//   Tried (this wave and before): long / unsigned long / enum / inline-helper / explicit `switch (x - 2)`, all accepting cases
//   enumerated, an if-chain (compare chain, 87 bytes), the QFE-4035 compiler, /Os, /O1 (binary search), /Ox, /Op, /Oa, /Ow,
//   /G5, /G6, /Gy, /Ob1, `optimize("g", off)`. /GL could not be graded (LTCG object): a link-time-codegen TU is the open lead.
// Layout: engine/CTCCreatureOpinionOfHero.h (PDB, Dinkumware lists 12 bytes) puts HeroOpinionEnemy at +0x3E2; retail STLport lists
//   are 4 bytes, so it sits at +0x342 (the landed SetHeroOpinionEnemy 0x006815D0 agrees): local model, header debt in NOTES.md.
class CThing;
enum EOpinionAttitudeType { };

class COpinionReactionManager {
public:
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing& thing) const;   // 0x00701840
};

template <class T> class CCountedPointer {
public:
    T* Data;
    T* operator->() const { return Data; }
};

class CWorld {
public:
    unsigned char _pad00[0x74];
    CCountedPointer<COpinionReactionManager> POpinionReactionManager;   // +0x74
};

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
protected:
    const CWorld& PeekWorld() const;        // 0x00686A80 (Ego_r.pdb CTCBase, protected)
};

class CTCCreatureOpinionOfHero : public CTCBase {
public:
    unsigned char _pad0c[0x342 - 0x0c];     // +0x0c .. NumberOfTimesHit, defs, masks, deed lists (retail STLport sizes)
    bool HeroOpinionEnemy;                  // +0x342
    bool IsWillingToAttackHeroOnOpinion() const;
};

bool CTCCreatureOpinionOfHero::IsWillingToAttackHeroOnOpinion() const
{
    if (HeroOpinionEnemy)
        return true;
    const CThing* thing = ParentThing;
    const COpinionReactionManager* manager = PeekWorld().POpinionReactionManager.Data;
    switch (manager->GetThingAttitudeToPlayer(*thing)) {
    case 2:
    case 4:
    case 5:
    case 6:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
        return false;
    default:
        return true;
    }
}
