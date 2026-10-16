// Behaviour test for 009cbc50: CBankFile::GetLastModifiedTime -- open, read the info, return LastWriteTime, close (twice: explicit + dtor).
#include <cstdio>
struct TTime { long Year; unsigned long Month, Day, Hour, Minute, Second, Millisecond; };
struct TInfo { TTime CreationTime, LastAccessTime, LastWriteTime; bool ReadOnly; };
static int g_opens = 0, g_closes = 0;
struct TFile {
    bool Opened;
    TFile() { Opened = false; }
    ~TFile() { Close(); }
    void Open(const char*, int, unsigned long) { ++g_opens; Opened = true; }
    TInfo GetFileInformation() { TInfo i = TInfo(); i.LastWriteTime.Year = 2004; i.LastWriteTime.Millisecond = 9; i.CreationTime.Year = 1; return i; }
    void Close() { ++g_closes; Opened = false; }
};
static TTime GetLastModifiedTime(const char* path)
{
    TFile file;
    file.Open(path, 1, 2);
    TTime time = file.GetFileInformation().LastWriteTime;
    file.Close();
    return time;
}
int main()
{
    TTime t = GetLastModifiedTime("x.big");
    if (t.Year != 2004 || t.Millisecond != 9 || g_opens != 1 || g_closes != 2) { std::printf("009cbc50_TEST FAIL\n"); return 1; }
    std::printf("009cbc50_TEST PASS\n");
    return 0;
}