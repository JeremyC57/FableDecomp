// Behaviour test for 0x009EC890: CConsole::RunScript -- a missing file runs nothing; an existing one is read whole into a
// NUL-terminated buffer, stripped of // comments (to \n, \r or the end) and /* */ comments, and run as one text command.
#include <cstdio>
#include <stdlib.h>
#include <string.h>

struct SFile { const char* Name; const char* Text; };
static const SFile g_files[] = { { "startup.txt", "SetFov 60; // comment\nShowFPS 1 /* block */;// tail" } };
static char g_ran[128];
static int  g_runs;

static const SFile* PathExists(const char* name)
{
    for (unsigned int i = 0; i < sizeof(g_files) / sizeof(g_files[0]); ++i)
        if (strcmp(g_files[i].Name, name) == 0) return &g_files[i];
    return 0;
}
// stand-in for GFRemoveComments(text, start, end): blank every start..end span (an empty end marker runs to the end;
// the end marker itself is kept unless it is the block-comment close)
static void RemoveComments(char* text, const char* start, const char* end)
{
    for (char* p = strstr(text, start); p; p = strstr(p, start)) {
        char* e = *end ? strstr(p + strlen(start), end) : 0;
        char* stop = e ? (strcmp(end, "*/") == 0 ? e + 2 : e) : text + strlen(text);
        for (char* q = p; q < stop; ++q) *q = ' ';
        p = stop;
    }
}
static void RunTextCommand(const char* text) { strcpy(g_ran, text); ++g_runs; }

static void RunScript(const char* filename)
{
    const SFile* file = PathExists(filename);
    if (!file)
        return;
    long length = (long)strlen(file->Text);
    char* buffer = (char*)calloc(length + 1, 1);     // the zero-filled CArray<char>(length + 1)
    memcpy(buffer, file->Text, length);              // CDiskFileWin32::Read
    buffer[length] = 0;
    RemoveComments(buffer, "//", "\n");
    RemoveComments(buffer, "//", "\r");
    RemoveComments(buffer, "//", "");
    RemoveComments(buffer, "/*", "*/");
    RunTextCommand(buffer);
    free(buffer);
}

int main()
{
    RunScript("missing.txt");
    if (g_runs != 0) { std::printf("009ec890_TEST FAIL\n"); return 1; }
    RunScript("startup.txt");
    if (g_runs != 1 || strstr(g_ran, "comment") || strstr(g_ran, "block") || strstr(g_ran, "tail")
        || !strstr(g_ran, "SetFov 60;") || !strstr(g_ran, "ShowFPS 1") || !strchr(g_ran, '\n')) { std::printf("009ec890_TEST FAIL\n"); return 1; }
    std::printf("009ec890_TEST PASS\n");
    return 0;
}