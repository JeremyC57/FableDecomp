// CTCQuestCard::OnSerialise(CPersistContext& context, bool flag) -- when asked, persist the card's state: the finished/replayable flags, feat index, quest/objective/region/teleporter/script names and the gold and renown rewards (defaulting to the def's)
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00702720, 232 bytes (`ret 8`). CTCQuestCard vtable slot 1 (catalog "vfunc_1"; PDB
//   ?OnSerialise@CTCQuestCard@@UAEXAAVCPersistContext@@_N@Z).
// Callees: CPersistContext::Transfer<bool> @ 0x004045C0, Transfer<long> @ 0x00410BE0, Transfer(const char*, CCharString*) @ 0x00411010.
// Layout: engine/CTCQuestCard.h (Ego_r.pdb: PDef +0x0c, Finished +0x10, Replayable +0x11, FeatIndex +0x14, the five CCharStrings
//   +0x18..+0x28, GoldReward +0x2c, RenownReward +0x30) -- the header flattens the strings into PStringData pointers and has no
//   virtuals, so the class is modelled locally with the same offsets; CQuestCardDef RenownReward +0x44, GoldReward +0x48.
// Shape: the literal defaults are const-ref temporaries homed in the dead `flag` parameter slot (`lea eax,[esp+10h]`); the
//   reward defaults pass the def fields' addresses (`mov edx,[esi+0Ch] ; add edx,48h`).
class CCharString { public: void* PStringData; };
class CPersistContext { public:
    void Transfer(const char* name, bool& value, const bool& defaultValue);     // 0x004045C0 Transfer<bool>
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x00410BE0 <long>
    void Transfer(const char* name, CCharString* value);                        // 0x00411010
};
class CQuestCardDef { public:
    unsigned char _pad00[0x44];
    long RenownReward;                                  // +0x44
    long GoldReward;                                    // +0x48
};
template <class T> class CDefPointer { public:
    const T* Object;
    const T* operator->() const { return Object; }
};
class CThing;
class CTCQuestCard { public:
    virtual void v00();
    virtual void OnSerialise(CPersistContext& context, bool flag);   // slot 1
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    bool          ToBeRemoved;                          // +0x08
    bool          IsInGlobalUpdateTCs;                  // +0x09
    unsigned char _pad0a[2];
    CDefPointer<CQuestCardDef> PDef;                    // +0x0c
    bool          Finished;                             // +0x10
    bool          Replayable;                           // +0x11
    long          FeatIndex;                            // +0x14
    CCharString   QuestName;                            // +0x18
    CCharString   ObjectiveName;                        // +0x1c
    CCharString   RegionName;                           // +0x20
    CCharString   TeleporterRegionName;                 // +0x24
    CCharString   ScriptQuestName;                      // +0x28
    long          GoldReward;                           // +0x2c
    long          RenownReward;                         // +0x30
};

void CTCQuestCard::OnSerialise(CPersistContext& context, bool flag)
{
    if (flag) {
        context.Transfer("Finished", Finished, false);
        context.Transfer("Replayable", Replayable, false);
        context.Transfer<long>("FeatIndex", FeatIndex, 0);
        context.Transfer("QuestName", &QuestName);
        context.Transfer("ObjectiveName", &ObjectiveName);
        context.Transfer("RegionName", &RegionName);
        context.Transfer("TeleporterRegionName", &TeleporterRegionName);
        context.Transfer("ScriptQuestName", &ScriptQuestName);
        context.Transfer("GoldReward", GoldReward, PDef->GoldReward);
        context.Transfer("RenownReward", RenownReward, PDef->RenownReward);
    }
}