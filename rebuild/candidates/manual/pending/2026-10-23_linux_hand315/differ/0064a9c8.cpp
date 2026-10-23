// Grade: DIFFER(1v1182) -- parked unreached (skeleton only): /Os ebp-frame message queue pop (+0x68), frame-counter timing, guild-seal sound (HUD_ORB_QUEST_CORE / OBJECTIVE_UPDATE), text lookups, health-low/crit quest text and an x87 alpha tail (ftol 0xBFEA70); not reached beyond decode.
// CDrawGuildSeal::DisplayNextMessage() -- PARK SKELETON (decoded only)
// Leaf: DisplayNextMessage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064A9C8, length 1182 bytes.
// Decode: /Os ebp-frame message queue pop (+0x68), frame-counter timing, guild-seal sound (HUD_ORB_QUEST_CORE / OBJECTIVE_UPDATE), text lookups, health-low/crit quest text and an x87 alpha tail (ftol 0xBFEA70); not reached beyond decode.
#pragma optimize("s", on)
struct CDrawGuildSeal {
    unsigned char _pad[0x100];
    void DisplayNextMessage();
};

void CDrawGuildSeal::DisplayNextMessage()
{
}
