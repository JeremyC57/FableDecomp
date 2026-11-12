// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x013BAD40 is the anonymous-namespace const long DEFAULT_UNARMED_COMBAT_IDLE_COUNT
//   (ConstantFPS * 5; Ego_r.pdb + FableWin.pdb `dynamic initializer for '`anonymous namespace'::DEFAULT_UNARMED_COMBAT_IDLE_COUNT'',
//   initializer 0x0121ED60 landed by agentFAM09); was g_7332d0 (an int). The stand-in fn_007332d0 / OH model is untouched.
namespace { extern const long DEFAULT_UNARMED_COMBAT_IDLE_COUNT; }   // 0x013BAD40
struct OH { char pad[0x1c]; int f1c; char pad2[0x50-0x20]; int f50; };
void __fastcall fn_007332d0(OH* combat, int _edx, int thing){ combat->f1c = DEFAULT_UNARMED_COMBAT_IDLE_COUNT; combat->f50 = -1; }