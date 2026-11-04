// Behaviour test for NUISystem::CEditBox::SetForbiddenCharacters (0x00540D40): the call replaces the box's forbidden-characters string with a copy of the
// argument (CWideString::operator=); the other filter string is untouched and later changes to the argument do not leak in.
#include <cstdio>
#include <string>
struct EditBox {
    std::wstring AllowedCharacters, ForbiddenCharacters;
    void SetAllowedCharacters(const std::wstring& c) { AllowedCharacters = c; }
    void SetForbiddenCharacters(const std::wstring& c) { ForbiddenCharacters = c; }
};
int main()
{
    EditBox box;
    box.AllowedCharacters = L"abc"; box.ForbiddenCharacters = L"xyz";
    std::wstring s = L"0123456789";
    box.SetForbiddenCharacters(s);
    s = L"changed";
    if (box.ForbiddenCharacters != L"0123456789" || box.AllowedCharacters != L"abc") { std::printf("00540d40_TEST FAIL\n"); return 1; }
    std::printf("00540d40_TEST PASS\n");
    return 0;
}