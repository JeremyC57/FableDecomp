// Behaviour test for 0x00540180 NUISystem::CEditBox::SetText
#include <cstdio>
#include <string>
struct CEditBox {
    std::wstring Text; long CursorPos; long LeftMostCharacter;
    void SetText(const std::wstring& text)
    {
        LeftMostCharacter = 0;
        Text = text;
        CursorPos = (long)text.length();
    }
};
int main()
{
    CEditBox e; e.CursorPos = 9; e.LeftMostCharacter = 4; e.Text = L"old text";
    e.SetText(L"Hero");
    int ok = e.Text == L"Hero" && e.CursorPos == 4 && e.LeftMostCharacter == 0;
    e.SetText(e.Text);
    ok = ok && e.Text == L"Hero" && e.CursorPos == 4;
    e.SetText(L"");
    ok = ok && e.Text.empty() && e.CursorPos == 0;
    if (!ok) { std::printf("00540180_TEST FAIL\n"); return 1; }
    std::printf("00540180_TEST PASS\n");
    return 0;
}