// CThingBuilding::WorldUpdate() -- the game-object update, then a graphic refresh request while the building is flagged or any global debug/visibility switch is on
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D810, 91 bytes (+ int3 padding).
// Callees: CThingGameObject::WorldUpdate @ 0x00838BA0 (CThingGameObject vtable 0x01244F7C slot 19; the manifest names it
//          "CThingCreatureBase::EditWorldUpdate", propagated), CThing::UpdateGraphicAppearance(long) @ 0x004C7590
//          (forwards to the graphic appearance component at +0x64 when present; the "CRenderTarget::DetachTarget" label is
//          propagated). Globals: five bytes and one dword at 0x013B8637/38/3C/3D/3E and 0x013B86C6 (engine display
//          switches; read in the order 37, 38, 3D, 3C, 3E, C6).
// Identity: CThingBuilding vtable 0x0124509C slot 19.
// Shape: the thing flag word at +0x2c is tested with `test ah,4` (bit 10); the conditions form one `||` chain.
extern unsigned char g_displaySwitch_13b8637;     // 0x013B8637
extern long g_displayMode_13b8638;                // 0x013B8638
extern unsigned char g_displaySwitch_13b863d;     // 0x013B863D
extern unsigned char g_displaySwitch_13b863c;     // 0x013B863C
extern unsigned char g_displaySwitch_13b863e;     // 0x013B863E
extern unsigned char g_displaySwitch_13b86c6;     // 0x013B86C6

struct CThingGameObject {
    unsigned char _pad00[0x2c];
    unsigned long m_thingFlags;                   // +0x2c
    bool WorldUpdate();                           // 0x00838BA0
    void UpdateGraphicAppearance(long reason);    // 0x004C7590 (CThing)
};

struct CThingBuilding : CThingGameObject {
    bool WorldUpdate();
};

bool CThingBuilding::WorldUpdate()
{
    if (CThingGameObject::WorldUpdate())
        return true;
    if ((m_thingFlags & 0x400) || g_displaySwitch_13b8637 || g_displayMode_13b8638 || g_displaySwitch_13b863d
        || g_displaySwitch_13b863c || g_displaySwitch_13b863e || g_displaySwitch_13b86c6)
        UpdateGraphicAppearance(4);
    return false;
}
