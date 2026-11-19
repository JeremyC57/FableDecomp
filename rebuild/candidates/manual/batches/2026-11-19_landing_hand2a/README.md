# Landing hand2a (2026-11-19): 3 of hand1's parked rows (3,485 bytes)

The hand2 lane (owner: no parking; a row past 100 grades goes to the Fable advisor in the main chat) re-verified and closed three of
the hand1 lane's rows. vland --land with LANDVERIFY_STRICT_HEADERS=1: RELOCATION_MATCH + PASS for all three (`logs/vland_land.log`).

| row | what closed it |
|---|---|
| 0x00B0D360 NParticleEngine::CPSCUpdateNormal::DatabaseSerialise (2240) | hand1's Fable-advisor candidate unchanged: the visible CPersistContext::Transfer<T> body (CPSCLight 0x00B1A870 opt-in preamble) folds the bool temporary; 2-arg Transfer + GetDefaultVal, no defaultScale local |
| 0x008EEC70 Mass::Collide (1129) | new opt-in FABLE_RETAIL_C2DVECTOR_DOT_GROUP (engine/C2DVector.h): `return (X*vector.X+Y*vector.Y);` -- the PAREN tuple of the once-expanded Dot keeps `mov eax,esp` (0x008EEFA8) out of the 81-tuple scheduler region; test extended with rotating-body cases, 4/4 mutants killed |
| 0x00918A40 CAIStateGroup_GazeAtWork::IsRunnable (116) | duplicated `return FindFreeGazePos() != 0;` arms (early `!GetTC` return + shop-open return) that VC7.1 cross-jumps, giving retail's block boundary at 0x00918A85 / 0x00918A89; one block for ptc_shopkeeper / ptc_shop as in Ego_d; 3/3 mutants |

Held: 0x00680050 CThingObjectDef::Validate matches (RELOCATION_MATCH PASS) only with an include-order lever (CMeshDataBank.h before
CWorld.h) -- owner decision pending; source in hand2a's workspace, copied to `pending/2026-11-18_hand1/candidates/` on decision.
Open: 0x006E89B0 escalated to the Fable advisor (hand2a continuing). `NAME_FIXES.tsv`: three fixes queued (two callees of 0x006E89B0
proven from Ego_r + FableWin; the 0x00680050 catalog name). `LESSONS.md`, `hand2a_NOTES.md`: the agent's notes.
