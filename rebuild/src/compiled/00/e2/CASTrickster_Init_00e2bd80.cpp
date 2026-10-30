// CAS_Trickster::Init() -- set up the ambush-scam trickster: make the hero and the trickster mutual allies, wander around its current position (min 0, max the script def's AmbushScamTricksterWanderMax), scripting state group 4, both random-speech indices 5, and flag it as having information
// Leaf: ?Init@CAS_Trickster
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E2BD80, length 313 bytes.
// Callees (all virtual, through Game at +0x4 = CGameScriptInterface, slots from ghidra_out/cgsi_slots_aligned.tsv):
//          GetHero (70, +0x118) x2, EntitySetThingAsAllyOfThing (599, +0x95c) x2, SetWanderCentrePoint (763, +0xbec),
//          SetWanderMinDistance (766, +0xbf8), SetWanderMaxDistance (769, +0xc04), SetScriptingStateGroup (784, +0xc40),
//          SetThingHasInformation (360, +0x5a0); CScriptThing::GetPos (slot 6, +0x18) on Me. Data: pGlobalScriptDef @ 0x0143E90C
//          (retail AmbushScamTricksterWanderMax at +0x29c = PDB +0x2a0, the block sits 4 bytes below the PDB offsets).
// Layout: Ego_r.pdb CEntityScriptBase (vptr, Game +4, Me +8) + CAS_Trickster (ParentClass +0x14, PMasterData +0x18,
//          lastRandomSpeechIdx1 +0x1c, lastRandomSpeechIdx2 +0x20).
// Shape: the landed CAS_Ambusher::Init (0x00E2BC10) ally round trip (outer slot fetched first, `this` re-read); each by-value
//          CScriptThing is built in its argument slot by the IMPLICIT copy ctor (CBaseClass vtable 0x01231710, then CScriptThing
//          0x01238C8C) over a CCountedPointer member with a user copy ctor (Data, Info, AddRef) -- a user CScriptThing copy ctor
//          reads the source through the cached &Me register instead of `this`-relative; the C3DVector argument is evaluated
//          first (right to left) and copied from GetPos()'s reference by a memberwise USER copy ctor (the implicit one
//          reorders the Y store); the float distances travel as dwords.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
enum EScriptingStateGroups { SCRIPTING_STATE_GROUP_4 = 4 };
struct SCountInfo { long RefCount; };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
template <class T> class CCountedPointer { public:                       // CCountedPointer<CScriptThing>: {Data, Info}
    void*       Data;                              // +0x00
    SCountInfo* Info;                              // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
class CScriptDef { public:
    unsigned char _pad[0x29c];
    float AmbushScamTricksterWanderMax;            // +0x29c (PDB +0x2a0)
};
extern CScriptDef* pGlobalScriptDef;                   // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CAS_Trickster {
    virtual void v00();
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    void*                 ParentClass;             // +0x14 CV_AmbushScamScript*
    void*                 PMasterData;             // +0x18 CQ_SunnyvaleMasterData*
    long                  lastRandomSpeechIdx1;    // +0x1c
    long                  lastRandomSpeechIdx2;    // +0x20
    void Init();
};

void CAS_Trickster::Init()
{
    Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
    Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
    Game->SetWanderCentrePoint(Me, Me.GetPos());
    Game->SetWanderMinDistance(Me, 0.0f);
    Game->SetWanderMaxDistance(Me, pGlobalScriptDef->AmbushScamTricksterWanderMax);
    Game->SetScriptingStateGroup(Me, SCRIPTING_STATE_GROUP_4);
    lastRandomSpeechIdx1 = 5;
    lastRandomSpeechIdx2 = 5;
    Game->SetThingHasInformation(Me, true, false, false);
}