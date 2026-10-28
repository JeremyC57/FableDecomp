// CQuestCard::CQuestCard(const CCharString& defName, const CCharString& scriptName, long featIndex) -- build a non-replayable quest card from its def and script names and feat index
// Leaf: CQuestCard
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AEE30, 61 bytes (`ret 0Ch`).
// Callees: CCharString::CCharString() 0x0099E4B0 (both names), CCharString::operator= 0x0099EFB0.
// Identity: the catalog label GetDefaultVal<std::list<COpinionTransientOffset> > is propagated across
// byte-identical bodies. The body default-constructs two CCharStrings at +0/+4, assigns them from the first
// two arguments, stores a bool at +8 and a long at +0xc: Ego_r.pdb's CQuestCard {CCharString DefName +0,
// CCharString ScriptName +4, bool Replayable +8, long FeatIndex +0xc} (16 bytes), next to the quest manager
// code at 0x004AF3C0; the overload at 0x004AEDF0 takes the replayable flag instead.
// Model: no engine header exists for CQuestCard or CCharString; local models carry the PDB names.
// Shape: the names are assigned in the body (default construction, then operator=), so the two scalar
// stores follow the assignments.

class CCharString { public:
    char* m_data;
    CCharString();                                    // 0x0099E4B0
    const CCharString& operator=(const CCharString& other); // 0x0099EFB0
};

struct CQuestCard {
    CCharString DefName;                              // +0x00
    CCharString ScriptName;                           // +0x04
    bool        Replayable;                           // +0x08
    long        FeatIndex;                            // +0x0c
    CQuestCard(const CCharString& defName, const CCharString& scriptName, long featIndex);
};

CQuestCard::CQuestCard(const CCharString& defName, const CCharString& scriptName, long featIndex)
{
    DefName = defName;
    ScriptName = scriptName;
    Replayable = false;
    FeatIndex = featIndex;
}