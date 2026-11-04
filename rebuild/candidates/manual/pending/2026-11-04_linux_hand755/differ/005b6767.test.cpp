// Behaviour test for CTCInventoryClothing::GetItemDescriptionArmourRating (0x005B6767), g++ model of the parked source.
#include <cstdio>
#include <string>
static std::wstring Rating(long item, long current)
{
    std::wstring text = L"Armour";
    long difference = item - current;
    wchar_t b[64];
    text += L" "; std::swprintf(b, 64, L"%ld", item); text += b; text += L" (";
    if (difference > 0) text += L"+";
    std::swprintf(b, 64, L"%ld", difference); text += b; text += L")";
    return text;
}
int main()
{
    int ok = Rating(12, 10) == L"Armour 12 (+2)" && Rating(5, 9) == L"Armour 5 (-4)" && Rating(7, 7) == L"Armour 7 (0)";
    std::printf(ok ? "005b6767_TEST PASS\n" : "005b6767_TEST FAIL\n");
    return ok ? 0 : 1;
}
