// CTCHeroCentre::FrameUpdate() -- (triage only, no draft) hero-centre update: posts a game event when the pending flag at +0x30 is set and the GUI/world allow it, else kills the stale spawned feat cards (+0x18 vector of intelligent pointers), rebuilds them from the creatures returned by GetAllLeafThings for the hero's region, matching each to its CTC 0x6c card by def index / name compare, and refreshes the card text
// Grade: DIFFER(-v1504) -- not drafted: lane gap /GS per-return epilogue (the early game-event return at 0x0077D683 checks the cookie before the register pops, as 007EC710; not reproduced under RTM or QFE-4035); ebp-aligned frame (and esp,-8), 1504 bytes
// Retail: Fable.exe @ 0x0077D5B0, 1504 bytes. Callees seen: GetWorld const 0x00686A80, GetPlayerManager 0x00686D50, GetMainPlayer
// 0x00449960, GetCharacter 0x00487DC0, CMemoryBuffer copy 0x009F1760, CWorld 0x00449700, AddGameEvent 0x0041649C,
// GetPItem 0x00A01B50, CThing::Kill 0x004C9B80, vector clear 0x004AC0F0, reserve 0x0077E580, GetAllLeafThings 0x0077EC20,
// GetDefGlobalIndexFromName 0x009AD410, Create 0x00703210, LowerBound 0x0040F020 / 0x004365B0, Compare 0x00411570,
// CCharString::operator= 0x0099EFB0, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10,
// vector insert 0x004AC940, __security_check_cookie 0x00BFE9F9.
void CTCHeroCentre_FrameUpdate_Triage() { }
