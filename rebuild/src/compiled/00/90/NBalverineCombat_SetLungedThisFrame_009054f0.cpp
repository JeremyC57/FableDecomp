// NBalverineCombat::SetLungedThisFrame() -- remember the current world frame as the frame of the last balverine lunge
// Leaf: ?SetLungedThisFrame@NBalverineCombat@@YIXXZ
// Retail: Fable.exe @ 0x009054F0, 11 bytes, __fastcall (no arguments), plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?SetLungedThisFrame@NBalverineCombat@@YIXXZ (catalog "_global::SetLungedThisFrame");
// its partner ?CanLungeThisFrame@NBalverineCombat@@YI_NABVCThingCreatureBase@@@Z reads the same global (landed 0x00905550
// binds it as g_lastLungeFrame). Its caller 0x008CC9A6 sits in a balverine lunge action.
// Callees: 0x0049D870 ?GetFrame@CWorld@@SIJXZ (static __fastcall).
// Globals: 0x013BB660 ?g_lastLungeFrame@@3JA (the existing binding; the real file-static name is not in the PDB name sets).
// Shape: `call ; mov [g],eax ; ret`.
class CWorld {
public:
    static long __fastcall GetFrame();                      // 0x0049D870
};

extern long g_lastLungeFrame;                               // 0x013BB660

namespace NBalverineCombat {
void __fastcall SetLungedThisFrame()
{
    g_lastLungeFrame = CWorld::GetFrame();
}
}