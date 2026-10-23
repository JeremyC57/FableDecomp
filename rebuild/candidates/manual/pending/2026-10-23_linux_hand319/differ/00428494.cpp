// Grade: DIFFER(45v1062) -- first-pass stub only (one grade); the body is not reconstructed yet, see the header for the decoded shape
// NUISystem::CManager::CreateMenuEntryComponent(...) (8 dword arguments, `ret 0x20`) -- build a menu entry: look up the
//   entry template, create the entry component (MENU_ENTRY_TEXT / PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE /
//   UI_WEAPON_TEXT_TEMPLATE variants through CreateComponent(const CCharString&, bool)), set the state-map positions and
//   alpha (_ftol of the scaled alpha), attach the text / icon children and return the counted pointer
// Leaf: CreateMenuEntryComponent
// Stub: first-pass skeleton only; the argument list and the body are not reconstructed yet (frame-pointer /Os TU, as the
//   landed CScrollableList / CList models).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
namespace NUISystem {
struct CComponent;
class CManager {
public:
    unsigned char _pad00[0x88];
    void*         Templates;                            // +0x88
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    void CreateMenuEntryComponent(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8);
};
#pragma optimize("s", on)
#pragma optimize("y", off)
void CManager::CreateMenuEntryComponent(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8)
{
    CreateComponent(CCharString("MENU_ENTRY_TEXT"), false);
}
}
