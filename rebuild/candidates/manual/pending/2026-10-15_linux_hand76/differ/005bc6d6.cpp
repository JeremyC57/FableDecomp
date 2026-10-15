// Grade: DIFFER(ours v retail) -- 20v23: the float argument is re-pushed with `mov eax,[esp+4] ; push eax`, retail copies it through the FPU (`fld [esp+4] ; push ecx ; fstp [esp]`); /Op, a float wrapper with a user copy constructor and optimize("g",off) do not give it
// CUIBackgroundOwner_005bc6d6::RenderBackground(float alpha) -- render the user-profile manager's UI background at `alpha` (0x004075C0 on the singleton from 0x0040D2A0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC6D6, length 23 bytes.
// Leaf: RenderBackground
// The chunk label (CSpellContainerList::AddChild, a CCountedPointer argument) is propagated garbage: the body
// takes one float and never reads `this`; named by address.
// Callees: 0x0040D2A0 (CTBaseSingleton<CUserProfileManager>::Get, no arguments, the manager in eax),
// 0x004075C0 (CUserProfileManager::RenderUIBackground(float), thiscall).
// Shape: the float argument re-pushed through the FPU (`fld [esp+4] ; push ecx ; fstp [esp]`) BEFORE the
// singleton call (VC evaluates the argument first), `mov ecx,eax`, a plain call (no tail jump: the float is
// copied into a new slot) and `ret 4`: a thiscall member whose `this` is unused.
class CUserProfileManager {
public:
    void RenderUIBackground(float alpha);       // 0x004075C0
};

CUserProfileManager& GetUserProfileManager();   // 0x0040D2A0 (CTBaseSingleton<CUserProfileManager>::Get)

struct CUIBackgroundOwner_005bc6d6 {
    void RenderBackground(float alpha);
};

void CUIBackgroundOwner_005bc6d6::RenderBackground(float alpha)
{
    GetUserProfileManager().RenderUIBackground(alpha);
}
