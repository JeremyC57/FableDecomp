// Test: CDriveManager::GetDriveFromPath -- the first character of the path's drive component (0 when the path has no drive)
#include <cstdio>
static wchar_t GetDriveFromPath(const wchar_t* path)
{
    wchar_t drive[3];
    drive[0] = 0;                                       // _wsplitpath's drive component: "X:" or ""
    if (path[0] && path[1] == L':') { drive[0] = path[0]; drive[1] = L':'; drive[2] = 0; }
    return drive[0];
}
int main()
{
    if (GetDriveFromPath(L"C:\\Games\\Fable") != L'C' || GetDriveFromPath(L"d:x.txt") != L'd' || GetDriveFromPath(L"\\data\\a.big") != 0) { std::printf("009e4c70_TEST FAIL\n"); return 1; }
    std::printf("009e4c70_TEST PASS\n");
    return 0;
}