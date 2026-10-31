// CCombatActionManager_00c3db80::CreateIcicleAction(unsigned long flags) -- EH-framed action factory of the 0x00C3Cxxx combat-action module: reject flags outside the two global masks (error -8); with a positive budget at +0x70 reuse the free head of the pool at +0x30 when it is an icicle action (error -9 when the pool is empty); with a budget of -1 allocate a 0x1dc-byte icicle action; any other budget is error -9; then initialise it with the flags (recycle on failure), pop the free node for a positive budget and push the action on the active chain
// Leaf: CreateIcicleAction
// Grade: DIFFER(335v335) (retry wave 2, improved) -- the parked file carried /EHsc only inside its Grade text, so the
//   grader built it without the EH frame (245 bytes); with a real Flags line only the two vtable registers differ
//   (retail eax for IsIcicle slot 1, edx for Init slot 3; ours edx/eax). Tried: swapped if/else, named Init result
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3DB80, 335 bytes (+ int3 padding), `ret 4`. Flags: /EHsc.
// Label: the catalog's "CTCHeroStats::CreateIceTrollIcicleAction" is propagated onto this library module (the landed
//   siblings 0x00C3DCD0 / 0x00C3DDE0 keep the same propagated class); the 0x1dc-byte object is built by the
//   CIcicleData base ctor @ 0x00C42D60 (manifest name) and gets vtable 0x012B5AB0.
// Callees: CCombatActionPool::GetFreeHead @ 0x00C3C9A0 (`return [this+8]`), CCombatActionPool::PopFree @ 0x00C3CA30,
//   CCombatActionPool::PushActive @ 0x00C3C9B0, CCombatActionManager::RecycleAction @ 0x00C3D610 (landed), the module
//   allocator @ 0x00C3C360 (class operator new), CIcicleData ctor @ 0x00C42D60, the action's virtual slots 1 (+0x04,
//   is-icicle test) and 3 (+0x0c, Init(flags) -> bool).
// Data: the two allowed-flag masks @ 0x0139C9A8 / 0x0139C9D8, the module's last error @ 0x0143BB48 and its report hook
//   @ 0x0143BBD4 (as the landed siblings).
// Shape: as the landed sibling 0x00C3DCD0; the pooled node is cast with `IsIcicle() ? node : 0` (neg/sbb/and) and only
//   the pooled path has the empty-pool error; both -9 reports and the -8 report share one `push e ; call eax` tail.
typedef void (__stdcall* PFNREPORT_00c3db80)(long error);
extern long g_lastError_0143bb48;                  // 0x0143BB48
extern PFNREPORT_00c3db80 g_pfnReport_0143bbd4;    // 0x0143BBD4
extern unsigned long g_allowedFlagsA_0139c9a8;     // 0x0139C9A8
extern unsigned long g_allowedFlagsB_0139c9d8;     // 0x0139C9D8

struct CIcicleAction_00c3db80 {                    // the pooled combat action
    virtual void v00();
    virtual bool IsIcicle();                       // slot 1 (+0x04)
    virtual void v02();
    virtual bool Init(unsigned long flags);        // slot 3 (+0x0c)
};

struct CIcicleData : CIcicleAction_00c3db80 {      // 0x1dc bytes
    unsigned char _body[0x1dc - 4];
    CIcicleData();                                 // 0x00C42D60
};

struct CIcicleActionImpl_00c3db80 : CIcicleData {  // vtable 0x012B5AB0
    virtual void v00();
    CIcicleActionImpl_00c3db80() { }
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
};

struct CCombatActionPool_00c3db80 {                // 20 bytes
    unsigned char _body[0x14];
    CIcicleAction_00c3db80* GetFreeHead();         // 0x00C3C9A0
    void PopFree();                                // 0x00C3CA30
    void PushActive(CIcicleAction_00c3db80* action);   // 0x00C3C9B0
};

struct CCombatActionManager_00c3db80 {
    unsigned char _pad[0x30];
    CCombatActionPool_00c3db80 m_pool;             // +0x30
    unsigned char _pad44[0x70 - 0x44];
    long m_budget;                                 // +0x70
    void RecycleAction(CIcicleAction_00c3db80* action);   // 0x00C3D610
    CIcicleAction_00c3db80* CreateIcicleAction(unsigned long flags);
};

static inline void ReportError(long error)
{
    if (g_lastError_0143bb48 == 0) {
        g_lastError_0143bb48 = error;
        if (g_pfnReport_0143bbd4)
            g_pfnReport_0143bbd4(error);
    }
}

CIcicleAction_00c3db80* CCombatActionManager_00c3db80::CreateIcicleAction(unsigned long flags)
{
    unsigned long invalidFlags = flags & ~(g_allowedFlagsA_0139c9a8 | g_allowedFlagsB_0139c9d8);
    if (invalidFlags) {
        ReportError(-8);
        return 0;
    }
    CIcicleAction_00c3db80* action = 0;
    if (m_budget > 0) {
        CIcicleAction_00c3db80* node = m_pool.GetFreeHead();
        if (node)
            action = node->IsIcicle() ? node : 0;
        else
            ReportError(-9);
    } else if (m_budget == -1) {
        action = new CIcicleActionImpl_00c3db80;
    } else {
        ReportError(-9);
    }
    if (!action)
        return 0;
    if (!action->Init(flags)) {
        RecycleAction(action);
        action = 0;
    }
    if (m_budget > 0)
        m_pool.PopFree();
    m_pool.PushActive(action);
    return action;
}
