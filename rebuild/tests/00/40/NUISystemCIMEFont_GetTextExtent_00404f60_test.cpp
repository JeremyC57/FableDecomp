#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[1024];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, " "); }
static void LogN(const char* s, long n) { char b[64]; sprintf(b, "%s%lx", s, n); Log(b); }
static void Reset() { g_log[0] = 0; }
static bool Is(const char* want) { return strcmp(g_log, want) == 0; }
class C2DVector { public: float X, Y; C2DVector(float x, float y) : X(x), Y(y) {} };
class CFontBank { public: int Refs; C2DVector GetTextExtent(const unsigned short* t, long) { long n = 0; while (t[n]) ++n; return C2DVector(n * 10.0f, 12.0f); } };
static CFontBank g_font;
struct CFontPointer { CFontBank* Data; CFontPointer(CFontBank* p) : Data(p) { ++p->Refs; } CFontPointer(const CFontPointer& o) : Data(o.Data) { ++Data->Refs; } ~CFontPointer() { --Data->Refs; } };
struct CIMEFont {
 unsigned long Height;
 void GetFontNameAndScale(const char*& name, float& scale) const { name = "ENG_ARIAL_24"; scale = Height / 12.0f; }
 CFontPointer GetFont(const char*) const { return CFontPointer(&g_font); }
 void GetTextExtent(const unsigned short* text, unsigned long* width, unsigned long* height)
 {
  const char* name;
  float scale;
  GetFontNameAndScale(name, scale);
  CFontPointer font = GetFont(name);
  C2DVector extent = font.Data->GetTextExtent(text, 0);
  *width = (unsigned long)(extent.X * scale);
  *height = (unsigned long)(extent.Y * scale);
 }
};
int main()
{
 unsigned short txt[4] = { 'a', 'b', 'c', 0 }; g_font.Refs = 1;
 CIMEFont f; f.Height = 24; unsigned long w = 0, h = 0;
 f.GetTextExtent(txt, &w, &h);
 if (w != 60 || h != 24 || g_font.Refs != 1) { printf("00404f60_TEST FAIL\n"); return 1; }
 printf("00404f60_TEST PASS\n");
 return 0;
}