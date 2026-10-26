// CTCVillage::UpdateSecuritySweepTasks() -- while a security sweep runs: a hero who is no longer a criminal ends it; otherwise the sweep is recalculated every 7.5 seconds or when the hero has moved more than 10 units from where it was last calculated
// Leaf: UpdateSecuritySweepTasks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065C08A, 188 bytes (plain `ret`, `leave`): frame-pointer size TU,
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)`. Ego_r.pdb `?UpdateSecuritySweepTasks@CTCVillage@@AAEXXZ`.
// Callees: CTCVillage::IsHeroCriminal() 0x0065BFCE; CTCVillage::InvalidateAndClearAllSecuritySweepTasks() 0x006590A0;
//          CTCBase::GetWorld() const 0x00686A80 (its result only feeds the static frame getter); CWorld::GetWorldFrame()
//          0x0049D870 (static); __ftol2 0x00BFEA70; CTCBase::GetPlayerManager() 0x00686D40; CPlayerManager::GetMainPlayer()
//          0x00449970 (Ego_r.pdb returns CPlayer&); CPlayer::GetHeroThing() 0x00487DC0; CThing::GetPosition() const
//          0x004C73D0; CTCVillage::CalculateSecuritySweepTasks() 0x00659800. Globals: g_framesPerSecond 0x01375550;
//          7.5f 0x0125AA1C, 100.0f 0x01230A08.
// Layout: retail CTCVillage FrameStartedSecuritySweep +0x10, PositionPlayerWasInWhenSecuritySweepCalculated +0x14,
//         SecuritySweepTasks +0x5c (bare list head pointer; PDB +0xc4).
// Shape: the world is taken into a local and the frame rate read into a `long` local before the static frame call (its
//        `fild` from the frame word); the recalculation flag is a bool in bl; the hero's planar distance is the
//        statement-by-statement `d = dx*dx; d += dy*dy;` against 100 (`test ah,41h`).
#pragma optimize("y", off)
#pragma optimize("s", on)
class C3DVector { public: float X, Y, Z; };
class CThing { public:
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
class CPlayer { public:
    CThing* GetHeroThing();                     // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                   // 0x00449970
};
class CWorld { public:
    static long GetWorldFrame();                // 0x0049D870
};
extern long g_framesPerSecond;                  // 0x01375550

struct SListNode { SListNode* _M_next; SListNode* _M_prev; };
struct CTaskList {                              // retail STLport list: the head node pointer
    SListNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};

class CTCVillage { public:
    void*         __vftable;                    // +0x00
    void*         ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x8];
    long          FrameStartedSecuritySweep;    // +0x10
    C3DVector     PositionPlayerWasInWhenSecuritySweepCalculated;   // +0x14
    unsigned char _pad_0x20[0x3c];
    CTaskList     SecuritySweepTasks;           // +0x5c list<CCountedPointer<CVillageTaskSecuritySweep> >
    CWorld* GetWorld() const;                   // 0x00686A80 (CTCBase)
    CPlayerManager* GetPlayerManager();         // 0x00686D40 (CTCBase)
    bool IsHeroCriminal();                      // 0x0065BFCE
    void InvalidateAndClearAllSecuritySweepTasks();   // 0x006590A0
    void CalculateSecuritySweepTasks();         // 0x00659800
    void UpdateSecuritySweepTasks();
};

void CTCVillage::UpdateSecuritySweepTasks()
{
    if (!SecuritySweepTasks.empty()) {
        if (!IsHeroCriminal()) {
            InvalidateAndClearAllSecuritySweepTasks();
            return;
        }
        bool recalculate = false;
        CWorld* world = GetWorld();
        long framesPerSecond = g_framesPerSecond;
        if ((world->GetWorldFrame() - FrameStartedSecuritySweep) % (long)(framesPerSecond * 7.5f) == 0)
            recalculate = true;
        CThing* hero = GetPlayerManager()->GetMainPlayer().GetHeroThing();
        if (hero) {
            const C3DVector& position = hero->GetPosition();
            float dx = PositionPlayerWasInWhenSecuritySweepCalculated.X - position.X;
            float dy = PositionPlayerWasInWhenSecuritySweepCalculated.Y - position.Y;
            float distanceSquared = dx * dx;
            distanceSquared += dy * dy;
            if (distanceSquared > 100.0f)
                recalculate = true;
        }
        if (recalculate) {
            InvalidateAndClearAllSecuritySweepTasks();
            CalculateSecuritySweepTasks();
        }
    }
}