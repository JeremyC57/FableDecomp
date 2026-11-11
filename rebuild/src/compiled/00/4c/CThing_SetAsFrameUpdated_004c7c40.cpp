// Leaf: ?SetAsFrameUpdated@CThing@@QAEXXZ
// agentNF15: respelled from the free-function stand-in `CThing_SetAsFrameUpdated(CThing*)` (__fastcall) to the catalog / Ego_r.pdb
//   member CThing::SetAsFrameUpdated() (?SetAsFrameUpdated@CThing@@QAEXXZ; __thiscall carries `this` in ecx exactly as the
//   stand-in's first fastcall argument did). The global stand-in `g_mgr` (GlobalMgr*) at 0x013B8A1C is the private static
//   CThing::PCurrentThingManager (?PCurrentThingManager@CThing@@0PAVCThingManager@@A, engine/CThing.h, 65 landed rows), and the
//   callee GlobalMgr::Notify @ 0x0051EC50 is CThingManager::AddToUpdateList(CThing&) (Ego_r.pdb
//   ?AddToUpdateList@CThingManager@@QAEXAAVCThing@@@Z; landed callers name 0x0051EC50 AddToUpdateList).
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7C40, 22 bytes: `test byte [ecx+91h],1 ; jne ; push ecx ; mov ecx,[013B8A1Ch] ;
//   call 0051EC50 ; ret`.
class CThing;
class CThingManager { public:
    void AddToUpdateList(CThing& thing);            // 0x0051EC50
};
class CThing {
private:
    static CThingManager* PCurrentThingManager;      // 0x013B8A1C ?PCurrentThingManager@CThing@@0PAVCThingManager@@A (private static)
public:
    unsigned char pad[0x91];
    unsigned char flags;                            // +0x91 (bit 0 tested)
    void SetAsFrameUpdated();
};
void CThing::SetAsFrameUpdated()
{
    if (!(flags & 1))
        PCurrentThingManager->AddToUpdateList(*this);
}
