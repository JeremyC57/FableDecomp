#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[1024];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, " "); }
static void LogN(const char* s, long n) { char b[64]; sprintf(b, "%s%lx", s, n); Log(b); }
static void Reset() { g_log[0] = 0; }
static bool Is(const char* want) { return strcmp(g_log, want) == 0; }
struct CIMEFont { float Position[2];
 void SetPosition(int x, int y) { Position[0] = (float)x; Position[1] = (float)y; } };
int main()
{
 CIMEFont f; f.SetPosition(12, -7);
 if (f.Position[0] != 12.0f || f.Position[1] != -7.0f) { printf("00404950_TEST FAIL\n"); return 1; }
 printf("00404950_TEST PASS\n");
 return 0;
}