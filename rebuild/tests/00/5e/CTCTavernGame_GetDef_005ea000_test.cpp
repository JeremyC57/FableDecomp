// Behaviour test for CTCTavernGame::GetDef 0x005EA000: the tavern game reads its CTavernGameDef sub-def from the parent thing's
// base def (`mov ecx,[ecx+4] ; mov ecx,[ecx+70h] ; jmp GetSubDef`), writing it into the caller's def pointer.
#include <cstdio>
struct CTavernGameDef { int Stake; };
template <class T> struct CDefPointer { T* Object; };
struct CDef { CTavernGameDef* Sub; bool GetSubDef(CDefPointer<const CTavernGameDef>* out) const { out->Object = Sub; return Sub != 0; } };
struct CThing { CDef* PBaseDef; const CDef& PeekBaseDef() const { return *PBaseDef; } };
struct CTCTavernGame { CThing* ParentThing; void GetDef(CDefPointer<const CTavernGameDef>* def) { ParentThing->PeekBaseDef().GetSubDef(def); } };
int main()
{
    CTavernGameDef sub = { 50 };
    CDef def = { &sub };
    CThing thing = { &def };
    CTCTavernGame game = { &thing };
    CDefPointer<const CTavernGameDef> out = { 0 };
    game.GetDef(&out);
    if (out.Object != &sub || out.Object->Stake != 50) { std::printf("005ea000_TEST FAIL\n"); return 1; }
    std::printf("005ea000_TEST PASS\n");
    return 0;
}