#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[1024];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, " "); }
static void LogN(const char* s, long n) { char b[64]; sprintf(b, "%s%lx", s, n); Log(b); }
static void Reset() { g_log[0] = 0; }
static bool Is(const char* want) { return strcmp(g_log, want) == 0; }
class C2DVector { public: float X, Y; };
struct CManager { C2DVector RightStickForce; void SetRightStickForce(C2DVector& f) { RightStickForce = f; } };
int main()
{
 CManager m; C2DVector f; f.X = 0.25f; f.Y = -0.75f; m.SetRightStickForce(f);
 if (m.RightStickForce.X != 0.25f || m.RightStickForce.Y != -0.75f) { printf("0042d592_TEST FAIL\n"); return 1; }
 printf("0042d592_TEST PASS\n");
 return 0;
}