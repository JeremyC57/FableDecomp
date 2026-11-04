// NScript::CQ_EndGameScript::CHookCoastTeleporter::Main() -- the Hook Coast teleporter entity script's main loop (not authored)
// Grade: DIFFER(not attempted) -- out of scope for this tiny-row chunk: a 1319-byte quest-script Main (398 instructions, 29 game-script-interface virtual calls, 9 CCharString literals, float timers against the script globals 0x0143E90C +0xBE4/+0xBE8/+0xBEC); 0 grades spent
// Leaf: ?Main@CHookCoastTeleporter@CQ_EndGameScript@NScript@@EAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D29880, 1319 bytes. Siblings landed: CHookCoastTeleporter::GetParentScript 0x00D29010, the
//   CQ_EndGameScript constructor 0x00D27370 / RegisterMain 0x00D273C0, engine/CQ_EndGameScript.h.
// Next idea: give it its own chunk; oracle the flow with Aeon's LUAGameflow EndGame port (work/aeon_lua_ports/) and model on the
//   landed CQ_EndGameBossBattleScript Main 0x00D2FB20 (same quest family, same pDSTGame virtual-call style).
