// Grade: DIFFER(24v1285) -- first-pass stub only (one grade); the body is not reconstructed yet, see the header for the decoded shape
// CMainGameComponent::Init() -- the game's start-up sequence: progress bar, text, console, definition manager (new
//   CGameDefinitionManager), then, each behind a DisplayProgress(<step name>) call, thing components, subtitled messages,
//   console variables, conversation attitude, player manager, player interface, world, display engine, player creation,
//   sound banks and particles; checkpoint-save cleanup and the world checksum peek
// Leaf: ?Init@CMainGameComponent@@
// Stub: first-pass skeleton only (the first progress step); the 19 step strings and calls are not reconstructed yet.
//   Frame-pointer /GS TU (cookie check at the end).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void __cdecl StartProgress(long a, long b, long c);    // 0x009E9EF0 (descriptive)
class CMainGameComponent {
public:
    virtual void Init();
    void InitialiseText();                             // 0x00416832
};
#pragma optimize("s", on)
#pragma optimize("y", off)
void CMainGameComponent::Init()
{
    StartProgress(0, 0, 0);
    InitialiseText();
}
