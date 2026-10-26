// CTCTrophy::OnCreate() -- fetches the trophy def from the owner's def, empties the available-witness list, and makes the trophy mountable unless the def is a fake trophy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DDA20, length 43 bytes.
// Callees: CDefClassBase::GetSubDef<CTrophyDef>(CDefPointer<const CTrophyDef>*) @ 0x005DA300 (on the owner thing's def
// at +0x70; the manifest template argument "CAICreatureWillPowerIndicatorDef" is propagated), std::list<
// CIntelligentPointer<CThingAICreature> >::clear() @ 0x00510D40 (manifest "CGuiControlTreePane::SortTreeRecursively"
// is propagated; the body is the VC6-era list clear: destroy + free each node, relink the sentinel).
// Shape: &PDef stays in edi across both calls and its Object is re-read; `test al,al ; sete dl` is the bool `!FakeTrophy`.
// Layout: Ego_r.pdb CTCTrophy with the retail one-pointer std::list (PDB 12 bytes -> 4): PDef +0x0C,
// AvailableWitnesses +0x34, WitnessesAchieved +0x38, the Waiting* flags +0x3C, IsGlobalTC +0x40,
// CurrentInstructionsPage +0x44, Mountable +0x48. CTrophyDef (PDB): EvilTrophy +0x34, FakeTrophy +0x35. No engine header.
class CTrophyDef { public:
    unsigned char _pad[0x34];
    bool          EvilTrophy;               // +0x34
    bool          FakeTrophy;               // +0x35
};

struct CDefPointerTrophyDef {
    const CTrophyDef* Object;
};

class CDefClassBase { public:
    bool GetSubDef(CDefPointerTrophyDef* out);   // 0x005DA300
};

class CThing { public:
    unsigned char  _pad[0x70];
    CDefClassBase* Def;                     // +0x70
};

struct CWitnessList {                       // retail std::list<CIntelligentPointer<CThingAICreature> >, sentinel pointer only
    void* _M_head;
    void clear();                           // 0x00510D40
};

class CTCTrophy { public:
    virtual void v00();
    CThing*              Thing;             // +0x04
    unsigned char        _pad_0x08[0x4];
    CDefPointerTrophyDef PDef;              // +0x0C
    unsigned char        _pad_0x10[0x24];   // +0x10 NonMinigameDeedHandle .. FrameForNextGainedWitnessSound
    CWitnessList         AvailableWitnesses;   // +0x34
    CWitnessList         WitnessesAchieved;    // +0x38
    bool                 WaitingForConfirmation;   // +0x3C
    bool                 WaitingForScore;
    bool                 WaitingForTerminate;
    bool                 WaitingForNextInstructionsPage;
    bool                 IsGlobalTC;        // +0x40
    long                 CurrentInstructionsPage;   // +0x44
    bool                 Mountable;         // +0x48
    void OnCreate();
};

void CTCTrophy::OnCreate()
{
    Thing->Def->GetSubDef(&PDef);
    AvailableWitnesses.clear();
    Mountable = !PDef.Object->FakeTrophy;
}