// CThingSwitch::WorldUpdate() -- CThingSwitch vtable slot 19: after the game-object update, every fifth frame (staggered by UniqueID) runs the trigger test selected by the switch def's TriggerType
// Leaf: WorldUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083CD70, 220 bytes including the 5-entry jump table at 0x0083CE38. CThingSwitch
// vtable 0x012453FC slot 19 (the landed GetSwitchDef 0x0083C1E0 names this caller). Callees: CThingGameObject::WorldUpdate
// 0x00838BA0 (qualified base call, byte result), CThing::GetWorld 0x004C7AB0 (`[[0x013B8A1C]+24h]`, result unused:
// the frame getter is static), GFGetCurrentFrame 0x0049D870, __aullrem 0x00BFEBB0 (unsigned 64-bit % 5),
// CThingSwitch::GetSwitchDef 0x0083C1E0 (CDefPointer by value), the def's virtual slot 1 (+4) on release, and the
// five trigger handlers 0x0083CA90 / 0x0083C690 / 0x0083C800 / 0x0083C990 / 0x0083BE90.
// Layout: CThing::UniqueID (unsigned __int64) at +0x18 (PDB +0x18); CThingSwitchDef::TriggerType at retail +0x70
// (PDB +0x74; the landed GetSwitchDef callers read +0x70).
// Shape: `if (base == 1) return result;` returns the compared al; UniqueID is read into a named local after the
// GetWorld call (retail loads it into edi:ebx before the frame call); `switch (GetSwitchDef()->TriggerType)` --
// the def temporary in the switch condition releases the long way (pointee DecRef/GetRefCount) and KEEPS its
// null store (a `long type = GetSwitchDef()->TriggerType;` local lets VC7.1 drop it); a dense switch 1..5 with
// one `return 0` per case (VC merges the copies; the out-of-range jump lands on the last case's epilogue).
struct CRefCounted {                       // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                // slot 1
};

struct CDefBase : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

struct CThingSwitchDef : CDefBase {
    unsigned char _pad08[0x70 - 8];
    long TriggerType;                      // +0x70 (retail; ESwitchTriggerType)
};

struct CSwitchDefPointer {                 // CDefPointer<CThingSwitchDef>
    CThingSwitchDef* m_p;
    ~CSwitchDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
    CThingSwitchDef* operator->() const { return m_p; }
};

struct CWorld {
    static long __fastcall GetWorldFrame();          // 0x0049D870 (GFGetCurrentFrame)
};

enum ESwitchTriggerType {
    SWITCH_TRIGGER_PLAYER_ONLY_ONCE_IN_AREA = 1,
    SWITCH_TRIGGER_PLAYER_ONLY_MULTIPLE_TIMES_IN_AREA = 2,
    SWITCH_TRIGGER_PLAYER_RESET_WHEN_LEAVES = 3,
    SWITCH_TRIGGER_SET_PLAYER_AREA_NAME = 4,
    SWITCH_TRIGGER_SET_ENVIRONMENT = 5
};

struct CThingSwitch {
    void*            __vftable;            // +0x00
    unsigned char    _pad04[0x18 - 4];
    unsigned __int64 UniqueID;             // +0x18
    unsigned char GameObjectWorldUpdate();               // 0x00838BA0 (CThingGameObject::WorldUpdate)
    CWorld* GetWorld();                                  // 0x004C7AB0
    CSwitchDefPointer GetSwitchDef();                    // 0x0083C1E0
    void SwitchTriggerPlayerOnlyOnceInArea();            // 0x0083CA90
    void SwitchTriggerPlayerOnlyMultipleTimesInArea();   // 0x0083C690
    void TriggerPlayerResetWhenLeaves();                 // 0x0083C800
    void TriggerSetPlayerAreaName();                     // 0x0083C990
    void TriggerSetEnvironment();                        // 0x0083BE90
    unsigned char WorldUpdate();
};

unsigned char CThingSwitch::WorldUpdate()
{
    unsigned char result = GameObjectWorldUpdate();
    if (result == 1)
        return result;
    CWorld* world = GetWorld();
    unsigned __int64 id = UniqueID;
    if ((world->GetWorldFrame() + id) % 5 == 0) {
        switch (GetSwitchDef()->TriggerType) {
        case SWITCH_TRIGGER_PLAYER_ONLY_ONCE_IN_AREA:
            SwitchTriggerPlayerOnlyOnceInArea();
            return 0;
        case SWITCH_TRIGGER_PLAYER_ONLY_MULTIPLE_TIMES_IN_AREA:
            SwitchTriggerPlayerOnlyMultipleTimesInArea();
            return 0;
        case SWITCH_TRIGGER_PLAYER_RESET_WHEN_LEAVES:
            TriggerPlayerResetWhenLeaves();
            return 0;
        case SWITCH_TRIGGER_SET_PLAYER_AREA_NAME:
            TriggerSetPlayerAreaName();
            return 0;
        case SWITCH_TRIGGER_SET_ENVIRONMENT:
            TriggerSetEnvironment();
            return 0;
        }
    }
    return 0;
}
