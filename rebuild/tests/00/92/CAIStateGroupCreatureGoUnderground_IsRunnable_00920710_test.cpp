// Behaviour test for 0x00920710 CAIStateGroup_CreatureGoUnderground::IsRunnable(): runnable exactly when NCreatureMode::IsUnderground(creature) is false,
// with the creature read from PBrain (+0x04) -> Creature (+0x18).
#include <cstdio>
class CThing { public: int tag; };
static const CThing* gSeen = 0; static bool gUnder = false;
namespace NCreatureMode { bool __fastcall IsUnderground(const CThing& thing) { gSeen = &thing; return gUnder; } }
class CThingAICreature : public CThing { };
class CAIBrain { public: unsigned char _pad[0x18]; CThingAICreature* Creature; CThingAICreature& GetCreature() const { return *Creature; } };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() { }
    CAIBrain* PBrain;
protected:
    virtual bool IsRunnable() { return true; }
    CThingAICreature& GetCreature() const { return PBrain->GetCreature(); }
};
class CAIStateGroup_CreatureGoUnderground : public CAIStateGroupBase { public:
    bool Run() { return IsRunnable(); }
protected:
    virtual bool IsRunnable();
};
bool CAIStateGroup_CreatureGoUnderground::IsRunnable()
{
    if (NCreatureMode::IsUnderground(GetCreature()))
        return false;
    return true;
}
int main()
{
    CThingAICreature c; CAIBrain b; b.Creature = &c; CAIStateGroup_CreatureGoUnderground g; g.PBrain = &b;
    gUnder = false; if (!g.Run() || gSeen != &c) { std::printf("00920710_TEST FAIL\n"); return 1; }
    gUnder = true; if (g.Run()) { std::printf("00920710_TEST FAIL\n"); return 1; }
    std::printf("00920710_TEST PASS\n"); return 0;
}
