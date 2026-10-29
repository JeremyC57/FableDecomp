// NUISystem::CManager::ShowErrorMessageOnInstance(long message) -- static: forwards to the front-end UI manager singleton's ShowErrorMessage
// Retail: Fable.exe @ 0x0041E61B, 14 bytes, static `__fastcall` (argument in ecx). Catalog label
// NUISystem::CSpellContainerList::AddChild is wrong. Evidence: both callers (CMainGameComponent::Update 0x0041834C and
// CNewFrontendGameComponent::Update 0x0042DCE7) do `xor ecx,ecx ; call`; the body pushes that argument, fetches the
// singleton with GetInstance 0x0041E5F2 (allocates 0xD0 bytes and runs ctor 0x0041E3F6, which stores vtable
// 0x01230134 = NUISystem::CManager by RTTI) and calls CManager::ShowErrorMessage 0x0041DB84 (thiscall, `ret 4`).
// The static's name is descriptive. Shape: VC7.1 pushes the call argument before evaluating the object expression.
namespace NUISystem {
class CManager {
public:
    static CManager* __fastcall GetInstance();                      // 0x0041E5F2
    void ShowErrorMessage(long message);                 // 0x0041DB84
    static void __fastcall ShowErrorMessageOnInstance(long message);
};

void __fastcall CManager::ShowErrorMessageOnInstance(long message)
{
    GetInstance()->ShowErrorMessage(message);
}
}