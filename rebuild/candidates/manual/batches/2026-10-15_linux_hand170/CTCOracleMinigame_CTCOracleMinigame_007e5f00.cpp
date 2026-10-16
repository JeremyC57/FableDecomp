// CTCOracleMinigame::CTCOracleMinigame(CThing* thing) -- CTCBase(thing), level 1, stage 0, not yet won, an empty pending-input list, then fetch the oracle minigame sub-def
// Leaf: ??0CTCOracleMinigame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E5F00, length 101 bytes (`ret 4`).
// Label correction: the manifest calls this CTCSummonableCreature::CTCSummonableCreature, but it stores vtable 0x012707B4 = CTCOracleMinigame
// (rtti-vtables.tsv) and touches offsets up to +0xE8 (PDB CTCSummonableCreature is 28 bytes; PDB CTCOracleMinigame 244 bytes fits).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, malloc @ 0x00BFEA0E (the list's 12-byte sentinel node),
// CDefClassBase::GetSubDef<COracleMinigameDef>(CDefPointer<const COracleMinigameDef>*) @ 0x007E6E00 (owner def +0x70; manifest instantiation
// "<CAICreatureWillPowerIndicatorDef>" is a byte twin).
// Layout (Ego_r.pdb CTCOracleMinigame, retail with the one-pointer STLport list): CurrentText +0x0C, Level +0x10, Stage +0x14, PostWaitStage
// +0x18 .. Output[32] +0x28, Success +0xA8, NextOutputFrame +0xAC, Symbols +0xB0, SymbolsFiring +0xC0, FramesFired +0xD0, InputsPending +0xE0
// (list<EOracleMinigameInput>, one sentinel pointer), SpeechMarker +0xE4 (PDB +0xEC), PDef +0xE8 (PDB +0xF0).
// Shape: init list in declaration order; the list ctor nulls the pointer, allocates and self-links the node, then stores it.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CThing;
struct COracleMinigameDef;
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<const COracleMinigameDef>* out) const;   // 0x007E6E00
};
struct CThingDefHolder {
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                        // +0x70
};
struct CInputList {                             // STLport list<EOracleMinigameInput>: one sentinel pointer
    struct node { node* next; node* prev; long value; };
    node* head;
    CInputList() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};
struct CTCBase {
    virtual ~CTCBase();
    CThingDefHolder* ParentThing;               // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    CTCBase(CThing* thing);                     // 0x00686800
};
struct CTCOracleMinigame : CTCBase {            // vtable 0x012707B4
    unsigned long CurrentText;                  // +0x0c
    long  Level;                                // +0x10
    long  Stage;                                // +0x14 EOracleMinigameState
    long  PostWaitStage;                        // +0x18
    long  WaitTime;                             // +0x1c
    float TimeBetweenOutputs;                   // +0x20
    long  NumOutputsFired;                      // +0x24
    long  Output[32];                           // +0x28
    bool  Success;                              // +0xa8
    long  NextOutputFrame;                      // +0xac
    CThing* Symbols[4];                         // +0xb0
    long  SymbolsFiring[4];                     // +0xc0
    long  FramesFired[4];                       // +0xd0
    CInputList InputsPending;                   // +0xe0
    CThing* SpeechMarker;                       // +0xe4
    CDefPointer<const COracleMinigameDef> PDef; // +0xe8
    CTCOracleMinigame(CThing* thing);
    virtual ~CTCOracleMinigame();
};

CTCOracleMinigame::CTCOracleMinigame(CThing* thing)
    : CTCBase(thing)
    , Level(1)
    , Stage(0)
    , Success(false)
{
    ParentThing->PDef->GetSubDef(&PDef);
}
