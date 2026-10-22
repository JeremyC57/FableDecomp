// Behaviour test for CEditBox::AddCharacter (0x0053FE50): backslash and (when disallowed) a leading space are ignored,
// the allowed / forbidden sets and control characters filter input, a pending delete-all clears the text first,
// backspace deletes before the cursor, other characters insert at the cursor up to the maximum length, and the display
// refresh runs for every accepted key.
#include <cstdio>
#include <string>

struct EditBox {
    std::string Text; long CursorPos; bool DisallowSpaceAsFirstChar, DeleteAllOnKeyPress; long MaxLength;
    std::string AllowedCharacters, ForbiddenCharacters; int refreshes;
    void AddCharacter(char c)
    {
        std::string ch(1, c);
        if (std::string("\\").find(ch) != std::string::npos) return;
        if (DisallowSpaceAsFirstChar && CursorPos == 0 && c == ' ') return;
        if (c != 8) {
            if (!AllowedCharacters.empty() && AllowedCharacters.find(ch) == std::string::npos) return;
            if (!ForbiddenCharacters.empty() && ForbiddenCharacters.find(ch) != std::string::npos) return;
            if ((unsigned char)c < ' ') return;
        }
        if (DeleteAllOnKeyPress) { Text = ""; CursorPos = 0; DeleteAllOnKeyPress = false; }
        if (c == 8) {
            std::string before, after;
            if (CursorPos > 0) before = Text.substr(0, CursorPos - 1);
            if (CursorPos < (long)Text.size()) after = Text.substr(CursorPos);
            Text = before + after;
            if (--CursorPos < 0) CursorPos = 0;
        } else {
            if ((long)Text.size() >= MaxLength) return;
            std::string before, after;
            if (CursorPos > 0) before = Text.substr(0, CursorPos);
            if (CursorPos < (long)Text.size()) after = Text.substr(CursorPos);
            Text = before + c + after;
            ++CursorPos;
        }
        ++refreshes;
    }
};
int main()
{
    EditBox e;
    e.CursorPos = 0; e.DisallowSpaceAsFirstChar = true; e.DeleteAllOnKeyPress = false; e.MaxLength = 4; e.refreshes = 0;
    int bad = 0;
    e.AddCharacter(' ');  if (e.Text != "" || e.refreshes != 0) ++bad;          // leading space refused
    e.AddCharacter('a'); e.AddCharacter('c'); e.CursorPos = 1; e.AddCharacter('b');
    if (e.Text != "abc" || e.CursorPos != 2 || e.refreshes != 3) ++bad;           // insert at the cursor
    e.AddCharacter('\\'); e.AddCharacter(1); if (e.Text != "abc") ++bad;           // backslash / control refused
    e.AddCharacter(8); if (e.Text != "ac" || e.CursorPos != 1) ++bad;            // backspace before the cursor
    e.AddCharacter('d'); e.AddCharacter('e'); e.AddCharacter('f');
    if (e.Text != "adec" || e.CursorPos != 3) ++bad;                               // stops at MaxLength 4
    e.ForbiddenCharacters = "x"; e.AddCharacter(8); e.AddCharacter('x'); if (e.Text != "adc") ++bad;
    e.AllowedCharacters = "q"; e.AddCharacter('z'); if (e.Text != "adc") ++bad;
    e.DeleteAllOnKeyPress = true; e.AddCharacter('q'); if (e.Text != "q" || e.CursorPos != 1) ++bad;
    e.CursorPos = 0; e.AddCharacter(8); if (e.Text != "q" || e.CursorPos != 0) ++bad;   // backspace at start clamps
    if (bad == 0) std::printf("0053fe50_TEST PASS\n"); else std::printf("0053fe50_TEST FAIL %d\n", bad);
    return bad;
}