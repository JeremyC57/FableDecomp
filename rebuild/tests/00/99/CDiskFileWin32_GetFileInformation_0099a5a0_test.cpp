// Behaviour test for CDiskFileWin32::GetFileInformation (0x0099A5A0): success converts the three FILETIMEs and the
// read-only bit; failure returns zeroed times and ReadOnly false.
#include <cstdio>
struct _FILETIME { unsigned long dwLowDateTime, dwHighDateTime; };
struct BY_HANDLE_FILE_INFORMATION {
    unsigned long dwFileAttributes; _FILETIME ftCreationTime; _FILETIME ftLastAccessTime; _FILETIME ftLastWriteTime;
    unsigned long rest[6];
};
static int GetFileInformationByHandle(void* file, BY_HANDLE_FILE_INFORMATION* info)
{
    if (!file) return 0;
    info->dwFileAttributes = 0x21;   // archive | read-only
    info->ftCreationTime.dwLowDateTime = 1; info->ftLastAccessTime.dwLowDateTime = 2; info->ftLastWriteTime.dwLowDateTime = 3;
    return 1;
}
struct CDateAndTime {
    long Year; unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    CDateAndTime() : Year(0), Month(0), Day(0), Hour(0), Minute(0), Second(0), Millisecond(0) {}
    void Initialise(const _FILETIME* t) { Year = 2000 + (long)t->dwLowDateTime; Month = t->dwLowDateTime; }
};
struct CFileInformation { CDateAndTime CreationTime, LastAccessTime, LastWriteTime; bool ReadOnly; };
struct CDiskFileWin32 {
    void* WinFileHandle;
    CFileInformation GetFileInformation();
};
CFileInformation CDiskFileWin32::GetFileInformation()
{
    CFileInformation info;
    BY_HANDLE_FILE_INFORMATION data;
    if (GetFileInformationByHandle(WinFileHandle, &data)) {
        info.CreationTime.Initialise(&data.ftCreationTime);
        info.LastAccessTime.Initialise(&data.ftLastAccessTime);
        info.LastWriteTime.Initialise(&data.ftLastWriteTime);
        info.ReadOnly = (data.dwFileAttributes & 1) != 0;
    } else {
        info.CreationTime = CDateAndTime();
        info.LastAccessTime = CDateAndTime();
        info.LastWriteTime = CDateAndTime();
        info.ReadOnly = false;
    }
    return info;
}
int main()
{
    int handle = 0; CDiskFileWin32 f; f.WinFileHandle = &handle;
    int bad = 0;
    CFileInformation a = f.GetFileInformation();
    if (a.CreationTime.Year != 2001 || a.LastAccessTime.Year != 2002 || a.LastWriteTime.Month != 3 || !a.ReadOnly) bad = 1;
    f.WinFileHandle = 0;
    CFileInformation b = f.GetFileInformation();
    if (b.CreationTime.Year || b.LastAccessTime.Month || b.LastWriteTime.Year || b.ReadOnly) bad = 2;
    if (bad) { std::printf("0099a5a0_TEST FAIL\n"); return 1; }
    std::printf("0099a5a0_TEST PASS\n");
    return 0;
}