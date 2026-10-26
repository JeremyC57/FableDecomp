// Behaviour test for 0x00446720 GetActionName: Xbox bindings report their placeholder (false); mouse bindings map to the
// (abbreviated when shortName) TEXT_GUI_CONTROL_* text looked up in the game bank (or the front end's) and succeed only
// with non-empty text; keyboard shift/space map to TEXT_GUI_KEY_* (always true), other keys ask the keyboard for its key
// name; unbound or unknown controls report "NULL" (false).
#include <cstdio>
#include <cstring>

class CWideString { public:
    char s[64];                                        // narrow stand-in for the wide text
    CWideString() { s[0] = 0; }
    CWideString& operator=(const char* t) { std::strcpy(s, t); return *this; }
    bool operator!=(const char* t) const { return std::strcmp(s, t) != 0; }
};
class CCharString { public:
    char s[64];
    CCharString() { s[0] = 0; }
    CCharString& operator=(const char* t) { std::strcpy(s, t); return *this; }
    bool operator!=(const char* t) const { return std::strcmp(s, t) != 0; }
};
static int g_bankPrefix;                               // 0: echo the tag, 1: unknown tag -> empty text
struct CDataBank {
    const char* who;
    CWideString GetTextBySymbol(const CCharString& tag) const
    {
        CWideString r;
        if (!g_bankPrefix) { std::strcpy(r.s, who); std::strcat(r.s, ":"); std::strcat(r.s, tag.s); }
        return r;
    }
};
class CMainGameComponent { public: CDataBank* bank; };
class CNewFrontendGameComponent { public: CDataBank* bank; };
static CMainGameComponent* g_pMainGameComponent;
static CNewFrontendGameComponent* g_pNewFrontendGameComponent;
static bool KeyboardGetKeyName(long key, CWideString& name)
{
    if (key == 0x1e) { name = "A"; return true; }
    return false;
}
class CActionInputControl { public: long GameAction, ControllerType, KeyboardKey, XboxButton, MouseButton; float Dir[2]; };

// the function under test (shape of 0x00446720)
static bool GetActionName(const CActionInputControl& control, CWideString& name, bool shortName)
{
    switch (control.ControllerType) {
    case 1:
        name = "Assigned to Xbox pad - bugger";
        return false;
    case 2:
        if (control.KeyboardKey != 0) {
            CCharString tag;
            switch (control.KeyboardKey) {
            case 0x2a: tag = "TEXT_GUI_KEY_LEFTSHIFT"; break;
            case 0x36: tag = "TEXT_GUI_KEY_RIGHTSHIFT"; break;
            case 0x39: tag = "TEXT_GUI_KEY_SPACE"; break;
            }
            if (tag != "") {
                if (g_pMainGameComponent) name = g_pMainGameComponent->bank->GetTextBySymbol(tag);
                else if (g_pNewFrontendGameComponent) name = g_pNewFrontendGameComponent->bank->GetTextBySymbol(tag);
                return true;
            }
            if (KeyboardGetKeyName(control.KeyboardKey, name))
                return true;
        }
        break;
    case 3: {
        CCharString tag;
        switch (control.MouseButton) {
        case 1:  tag = shortName ? "TEXT_GUI_CONTROL_LMB_ABBREVIATED" : "TEXT_GUI_CONTROL_LMB"; break;
        case 2:  tag = shortName ? "TEXT_GUI_CONTROL_RMB_ABBREVIATED" : "TEXT_GUI_CONTROL_RMB"; break;
        case 3:  tag = shortName ? "TEXT_GUI_CONTROL_MMB_ABBREVIATED" : "TEXT_GUI_CONTROL_MMB"; break;
        case 8:  tag = shortName ? "TEXT_GUI_CONTROL_MB4_ABBREVIATED" : "TEXT_GUI_CONTROL_MB4"; break;
        case 9:  tag = shortName ? "TEXT_GUI_CONTROL_MB5_ABBREVIATED" : "TEXT_GUI_CONTROL_MB5"; break;
        case 10: tag = shortName ? "TEXT_GUI_CONTROL_MB6_ABBREVIATED" : "TEXT_GUI_CONTROL_MB6"; break;
        case 11: tag = shortName ? "TEXT_GUI_CONTROL_MB7_ABBREVIATED" : "TEXT_GUI_CONTROL_MB7"; break;
        case 12: tag = shortName ? "TEXT_GUI_CONTROL_MB8_ABBREVIATED" : "TEXT_GUI_CONTROL_MB8"; break;
        case 4:  tag = "TEXT_GUI_CONTROL_MOUSE_MOVEMENT"; break;
        case 5:  tag = "TEXT_GUI_CONTROL_MOUSE_WHEEL"; break;
        case 6:  tag = "TEXT_GUI_CONTROL_MOUSE_WHEEL_UP"; break;
        case 7:  tag = "TEXT_GUI_CONTROL_MOUSE_WHEEL_DOWN"; break;
        default: tag = ""; break;
        }
        if (tag != "") {
            if (g_pMainGameComponent) name = g_pMainGameComponent->bank->GetTextBySymbol(tag);
            else if (g_pNewFrontendGameComponent) name = g_pNewFrontendGameComponent->bank->GetTextBySymbol(tag);
            if (name != "")
                return true;
        }
        return false;
    }
    }
    name = "NULL";
    return false;
}

static int check(long type, long key, long button, bool shortName, bool want, const char* text)
{
    CActionInputControl c = { 0, type, key, 0, button, { 0, 0 } };
    CWideString n; n = "untouched";
    bool r = GetActionName(c, n, shortName);
    return r == want && std::strcmp(n.s, text) == 0;
}

int main()
{
    int ok = 1;
    CDataBank game = { "game" }, front = { "front" };
    CMainGameComponent mg = { &game };
    CNewFrontendGameComponent fe = { &front };
    g_pMainGameComponent = &mg; g_pNewFrontendGameComponent = &fe;
    ok &= check(1, 0, 0, false, false, "Assigned to Xbox pad - bugger");
    ok &= check(0, 0, 0, false, false, "NULL");
    ok &= check(7, 0, 0, false, false, "NULL");
    ok &= check(3, 0, 1, false, true, "game:TEXT_GUI_CONTROL_LMB");
    ok &= check(3, 0, 1, true, true, "game:TEXT_GUI_CONTROL_LMB_ABBREVIATED");
    ok &= check(3, 0, 12, true, true, "game:TEXT_GUI_CONTROL_MB8_ABBREVIATED");
    ok &= check(3, 0, 6, true, true, "game:TEXT_GUI_CONTROL_MOUSE_WHEEL_UP");
    ok &= check(3, 0, 0, false, false, "untouched");      // unbound mouse: false, name untouched
    ok &= check(3, 0, 13, false, false, "untouched");
    ok &= check(2, 0, 0, false, false, "NULL");             // unbound key
    ok &= check(2, 0x36, 0, true, true, "game:TEXT_GUI_KEY_RIGHTSHIFT");
    ok &= check(2, 0x1e, 0, false, true, "A");              // keyboard's own name
    ok &= check(2, 0x1f, 0, false, false, "NULL");          // keyboard has no name
    g_pMainGameComponent = 0;
    ok &= check(3, 0, 2, false, true, "front:TEXT_GUI_CONTROL_RMB");
    ok &= check(2, 0x39, 0, false, true, "front:TEXT_GUI_KEY_SPACE");
    g_pNewFrontendGameComponent = 0;
    ok &= check(3, 0, 2, false, true, "untouched");         // no bank: name kept, non-empty -> true
    ok &= check(2, 0x2a, 0, false, true, "untouched");      // special key with no bank: still true
    g_pMainGameComponent = &mg; g_bankPrefix = 1;
    ok &= check(3, 0, 3, false, false, "");                 // empty text -> false
    ok &= check(2, 0x2a, 0, false, true, "");               // keyboard special key: true even when empty
    std::printf(ok ? "00446720_TEST PASS\n" : "00446720_TEST FAIL\n");
    return ok ? 0 : 1;
}