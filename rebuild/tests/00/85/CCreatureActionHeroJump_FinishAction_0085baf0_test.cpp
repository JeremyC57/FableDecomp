#include "rebuild_abi.h"
#include <cstdio>
struct FableDestVal_0085baf0_t
{ char pad[0x8]; };
// Naming (NF442, ruling q2): the row now calls the callee by its PDB spelling, so this double is respelt with it (callee only).
class CCombatActionBase { protected: virtual void FinishAction(); };
class CBaseObject;
class CBaseIntelligentPointer { public: CBaseObject* GetPItem() const; };   // 0x00A01B50 (Ego_r.pdb)

static int g_inner = 0, g_outer = 0;
CBaseObject* CBaseIntelligentPointer::GetPItem() const { g_inner++; return 0; }
void CCombatActionBase::FinishAction() { g_outer++; }
extern "C" void FABLE_FASTCALL CCreatureActionHeroJump_FinishAction(FableDestVal_0085baf0_t* action);
int main()
{
    char buf[0x8 + 8];
    CCreatureActionHeroJump_FinishAction((FableDestVal_0085baf0_t*)buf);
    if (g_inner != 1) { std::printf("BAD_INNER\n"); return 1; }
    if (g_outer != 1) { std::printf("BAD_OUTER\n"); return 1; }
    std::printf("DESTVAL_0085baf0_OK\n"); return 0;
}