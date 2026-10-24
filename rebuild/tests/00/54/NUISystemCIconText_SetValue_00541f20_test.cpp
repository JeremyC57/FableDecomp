#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static char g_log[1024];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, " "); }
static void LogN(const char* s, long n) { char b[64]; sprintf(b, "%s%lx", s, n); Log(b); }
static void Reset() { g_log[0] = 0; }
static bool Is(const char* want) { return strcmp(g_log, want) == 0; }
// strings modelled by plain char buffers: the text bank maps a symbol to its text or to "" when missing
struct CText { char s[64]; };
static const char* Lookup(const char* sym) { return strcmp(sym, "TEXT_OK") == 0 ? "Okay" : ""; }
static int g_game, g_system;
struct CIconText {
 CText Text; int Updates;
 void UpdateText() { ++Updates; }
 void SetValue(const char* symbol)
 {
  if (g_game) {
   const char* text = Lookup(symbol);
   if (strcmp(text, "") != 0) strcpy(Text.s, text); else strcpy(Text.s, symbol);
  } else if (g_system) {
   const char* text = Lookup(symbol);
   if (strcmp(text, "") != 0) strcpy(Text.s, text); else strcpy(Text.s, symbol);
  } else {
   strcpy(Text.s, symbol);
  }
  UpdateText();
 }
};
int main()
{
 CIconText t; t.Updates = 0; g_game = 1;
 t.SetValue("TEXT_OK"); if (strcmp(t.Text.s, "Okay") || t.Updates != 1) { printf("00541f20_TEST FAIL\n"); return 1; }
 t.SetValue("RAW"); if (strcmp(t.Text.s, "RAW")) { printf("00541f20_TEST FAIL\n"); return 1; }
 g_game = 0; g_system = 1; t.SetValue("TEXT_OK"); if (strcmp(t.Text.s, "Okay")) { printf("00541f20_TEST FAIL\n"); return 1; }
 g_system = 0; t.SetValue("TEXT_OK"); if (strcmp(t.Text.s, "TEXT_OK") || t.Updates != 4) { printf("00541f20_TEST FAIL\n"); return 1; }
 printf("00541f20_TEST PASS\n");
 return 0;
}