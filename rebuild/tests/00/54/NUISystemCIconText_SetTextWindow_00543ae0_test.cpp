#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[1024];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, " "); }
static void LogN(const char* s, long n) { char b[64]; sprintf(b, "%s%lx", s, n); Log(b); }
static void Reset() { g_log[0] = 0; }
static bool Is(const char* want) { return strcmp(g_log, want) == 0; }
class C2DBoxF { public: float Left, Top, Right, Bottom; };
struct CIconText { C2DBoxF TextWindow; void SetTextWindow(C2DBoxF& w) { TextWindow = w; } };
int main()
{
 C2DBoxF b; b.Left = 1; b.Top = 2; b.Right = 3; b.Bottom = 4; CIconText t; t.SetTextWindow(b);
 if (t.TextWindow.Left != 1 || t.TextWindow.Top != 2 || t.TextWindow.Right != 3 || t.TextWindow.Bottom != 4) { printf("00543ae0_TEST FAIL\n"); return 1; }
 printf("00543ae0_TEST PASS\n");
 return 0;
}