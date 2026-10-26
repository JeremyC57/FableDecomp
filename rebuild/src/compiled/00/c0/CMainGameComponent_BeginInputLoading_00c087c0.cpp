// CMainGameComponent::BeginInputLoading(void* window, bool flag) -- start the input loader through the engine routine 0x00C0AEE0 with (window, flag, this, 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C087C0, length 19 bytes.
// Callees: 0x00C0AEE0 (`__fastcall`: the window in ecx, the flag in dl, two stack words, `ret 8`; it forwards (owner, 0, 0x10) to 0x00C0AE70; the manifest label "ConstructString" is propagated). Shape: `mov dl ; push 0 ; push ecx ; mov ecx,[esp+0Ch]`, `ret 8`.
class CMainGameComponent;
void __fastcall BeginLoading_00c0aee0(void* window, bool flag, CMainGameComponent* owner, long context);   // 0x00C0AEE0

class CMainGameComponent { public:
    void BeginInputLoading(void* window, bool flag);
};

void CMainGameComponent::BeginInputLoading(void* window, bool flag)
{
    BeginLoading_00c0aee0(window, flag, this, 0);
}