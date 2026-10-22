// CAIStateGroup_HoverHornetQueen::GoToSpawnPoint() -- switch the queen's navigator to hover, pick deposit position 1 (first time) or a random 1..4, find the "QueenDepositPos<n>" marker within 60 units (else use the creature's start position), look at the target, then fly to the marker facing the target until arrived; restore the navigator, or terminate when the state stops being runnable
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095B020, length 599 bytes (+ int3 padding), plain `ret`, /GS cookie (sprintf buffer).
// Callees: GetPWorld @ 0x008FCFC0, CWorld random-seed getter @ 0x0049D810 (labelled CVertexBufferWin32::DoSizeof; result unused),
// LowerBound @ 0x0040F020, SetNavigatorToType @ 0x006CBA80, GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, sprintf @ 0x00BFEB10,
// CCharString @ 0x0099EBF0 / 0x0099EC30 / ~ @ 0x0099EAE0, CThing::GetPosition @ 0x004C73D0, _ftol @ 0x00BFEA70,
// PeekThingSearchTools @ 0x008FD000, GetPFirstThingInRoughArea<CThingFilter_HasDefGlobalIndex,CThing> @ 0x007FD110,
// IsStillRunnable @ 0x0095A420, GetPItem @ 0x00A01B50, AIAction_LookAtPos @ 0x0097D580,
// AIAction_MoveToPosProximityAndFacePos @ 0x0091D510, Terminate @ 0x008FCF40. String "QueenDepositPos%d" @ 0x01294EE4.
// Shape: the search rectangle is a function-scope local written member by member (Left/Top/Right/Bottom); a ctor or a
// block-scoped rectangle shares the look-at vector's frame words (frame 0x54) and schedules x+60 first.
#include <stdio.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CCharString {
    void* PData;
    CCharString(const char* text, long length);  // 0x0099EBF0
    CCharString(const CCharString& other);       // 0x0099EC30
    ~CCharString();                              // 0x0099EAE0
};
struct CThingFilter_HasDefGlobalIndex {
    CCharString Name;
    CThingFilter_HasDefGlobalIndex(const CCharString& name) : Name(name) { }
};
struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
struct CRectI {
    long Left, Top, Right, Bottom;
    CRectI() { }
};
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0xe8 - 0x4c];
    C3DVector StartPos;                         // +0xe8
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingCreatureBase : CThing { };
struct CTCCreatureNavigation {
    void SetNavigatorToType(long type);         // 0x006CBA80
};
extern long g_NavigatorTypeHover;               // 0x0129CB44
extern long g_NavigatorTypeDefault;             // 0x0129CBA4
struct CWorld {
    unsigned long& GetRandomSeed();             // 0x0049D810
};
struct CThingSearchTools {
    CThing* GetPFirstThingInRoughArea(const CRectI& area, const CThingFilter_HasDefGlobalIndex& filter, CThing** found);   // 0x007FD110
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1, AI_ACTION_REPEAT_POLL = 2 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    unsigned long& GetPWorldSeed();             // 0x008FCFD0
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    void Terminate();                           // 0x008FCF40
    EAIActionReturn AIAction_LookAtPos(EAIActionRepeat repeat, C3DVector pos, long flags, float tolerance);   // 0x0097D580
};
struct CAIStateGroup_HoverMovementBase : CAIStateGroupBase {
    EAIActionReturn AIAction_MoveToPosProximityAndFacePos(EAIActionRepeat repeat, const C3DVector& pos, float proximity,
        const C3DVector& facePos, bool& arrived, long movementType);   // 0x0091D510
};
struct CAIStateGroup_HoverHornetQueen : CAIStateGroup_HoverMovementBase {
    CBaseIntelligentPointer PTarget;            // +0x14
    C3DVector StartPos;                         // +0x1c
    C3DVector ArenaCentrePos;                   // +0x28
    bool FirstSpawn;                            // +0x34
    bool IsStillRunnable();                     // 0x0095A420
    void GoToSpawnPoint();
};

void CAIStateGroup_HoverHornetQueen::GoToSpawnPoint()
{
    GetPWorld()->GetRandomSeed();
    CThingCreatureBase* creature = GetCreature();
    CTCCreatureNavigation* nav;
    bool has = (creature->m_presentBits[0] >> 7) & 1;
    if (has) {
        long type = 7;
        CInterfaceMap* map = &creature->m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        nav = (CTCCreatureNavigation*)((it == map->last || 7 < it->type) ? map->last : it)->component;
    }
    nav->SetNavigatorToType(g_NavigatorTypeHover);
    CThing* found = 0;
    long index;
    if (FirstSpawn) {
        index = 1;
        FirstSpawn = false;
    } else {
        unsigned long& seed = GetPWorldSeed();
        seed = seed * 0x24A1 + 0x24DF;
        seed = GFROR13(seed);
        index = seed % 4 + 1;
    }
    char text[32];
    sprintf(text, "QueenDepositPos%d", index);
    CRectI area;
    C3DVector target;
    {
        CCharString name(text, -1);
        CThingFilter_HasDefGlobalIndex filter(name);
        const C3DVector& pos = GetCreature()->GetPosition();
        C2DCoordI centre((long)pos.X, (long)pos.Y);
        area.Left = centre.X - 60;
        area.Top = centre.Y - 60;
        area.Right = centre.X + 60;
        area.Bottom = centre.Y + 60;
        PeekThingSearchTools()->GetPFirstThingInRoughArea(area, filter, &found);
    }
    if (found)
        target = found->GetPosition();
    else
        target = GetCreature()->StartPos;
    if (IsStillRunnable()) {
        C3DVector look = PTarget.GetPItem()->GetPosition();
        if (AIAction_LookAtPos(AI_ACTION_REPEAT_UNTIL_DONE, look, 0x11, 0.05f) != AI_ACTION_DONE) {
            Terminate();
            return;
        }
        if (IsStillRunnable()) {
            EAIActionReturn result;
            do {
                bool arrived;
                result = AIAction_MoveToPosProximityAndFacePos(AI_ACTION_REPEAT_POLL, target, 0.3f, look, arrived, 10);
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
            } while (result == AI_ACTION_RUNNING);
            nav->SetNavigatorToType(g_NavigatorTypeDefault);
            return;
        }
    }
    Terminate();
}