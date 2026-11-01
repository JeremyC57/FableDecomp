// Naming wave 18: the global stand-in g_root (0x013B8A1C) is the private static CThing::PCurrentThingManager, read through the inline
//   static accessor GetPCurrentThingManager() as retail code outside CThing does (name-only: the bytes are unchanged).
class CDisplayEngine;
class CThingManager { public: char pad[0x1c]; CDisplayEngine* engine; };
class CThingManager;
class CThing {
    static CThingManager* PCurrentThingManager;      // 0x013B8A1C ?PCurrentThingManager@CThing@@0PAVCThingManager@@A (private static)
public:
    static CThingManager* __fastcall GetPCurrentThingManager() { return PCurrentThingManager; }   // Ego_r / FableWin ?GetPCurrentThingManager@CThing@@SIPAVCThingManager@@XZ (inline)
};
struct CViewBase {};
CDisplayEngine* __fastcall CViewBase_GetDisplayEngine(CViewBase* self)
{
    (void)self;
    return CThing::GetPCurrentThingManager()->engine;
}