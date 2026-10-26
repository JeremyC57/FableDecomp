// CThingAICreatureInit::CThingAICreatureInit(CThingPhysical* original, CThingPhysical* previous, bool playerCharacter, const CCharString& scriptName, const CCharString& overridingBrainName, bool drawable) -- build the creature-init record: the two things, copies of the two names, the flags, placed at the end of the list, no auto-placing, no home or work building
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FACD0, 77 bytes. Callees: CCharString::CCharString(const CCharString&)
// @ 0x0099EC30 (out of line, so CCharString is modelled locally rather than through fable_string.h).
// Shape: layout as the PDB (0x1c bytes): OriginalThing +0, PreviousThing +4, ScriptName +8, OverridingBrainName +0xc,
// PlayerCharacter +0x10, Drawable +0x11, PlaceAtEndOfList +0x12, EnableCreatureAutoPlacing +0x13, HomeBuilding +0x14,
// WorkBuilding +0x18. `ret 18h` = six arguments.
class CThingPhysical;
class CThingBuilding;

class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);           // 0x0099EC30
};

class CThingAICreatureInit { public:
    CThingPhysical* OriginalThing;                   // +0x00
    CThingPhysical* PreviousThing;                   // +0x04
    CCharString     ScriptName;                      // +0x08
    CCharString     OverridingBrainName;             // +0x0c
    bool            PlayerCharacter;                 // +0x10
    bool            Drawable;                        // +0x11
    bool            PlaceAtEndOfList;                // +0x12
    bool            EnableCreatureAutoPlacing;       // +0x13
    CThingBuilding* HomeBuilding;                    // +0x14
    CThingBuilding* WorkBuilding;                    // +0x18
    CThingAICreatureInit(CThingPhysical* original, CThingPhysical* previous, bool playerCharacter,
                         const CCharString& scriptName, const CCharString& overridingBrainName, bool drawable);
};

CThingAICreatureInit::CThingAICreatureInit(CThingPhysical* original, CThingPhysical* previous, bool playerCharacter,
                                           const CCharString& scriptName, const CCharString& overridingBrainName,
                                           bool drawable)
    : OriginalThing(original), PreviousThing(previous), ScriptName(scriptName),
      OverridingBrainName(overridingBrainName), PlayerCharacter(playerCharacter), Drawable(drawable),
      PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false), HomeBuilding(0), WorkBuilding(0)
{
}