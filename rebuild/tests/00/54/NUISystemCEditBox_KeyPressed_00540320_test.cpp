// Behaviour test for 0x00540320 NUISystem::CEditBox::KeyPressed: the row's defines, includes and body verbatim over the shared
// headers, run on a zeroed CEditBox image whose vptr points at a test table (slot 144 GetTextComponent, slot 147 UpdateDisplayText),
// a fake text component (slot 148 GetTextWindow) and a fake CManager (slot 14 Action). CWideString is doubled with a small buffer
// (GetSubString takes [start, end)). Covers: left (decrement, clamp at 0, scroll by -6 only left of LeftMostCharacter), right
// (increment, clamp at the length, scroll by +6 only right of RightMostCharacter - 1), the IME gate (no change, delete-all mode kept),
// escape / enter action lists (each node passed in order), delete in delete-all mode (empty text, cursor 0) and the cut of the
// character after the cursor (at 0, middle, end), home (cursor 0, width walk stops at the first char whose running width passes the
// window's BRX, unsigned widths >= 2^31 count as large, Adjustment clamped to 0), end (cursor to length, Adjustment 0), other keys
// (no change, delete-all kept), and UpdateDisplayText called exactly once on every path.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
// ---- row body ----
// NUISystem::CEditBox::KeyPressed(wchar_t ch) -- the edit box's key handler (vtable slot 146): left/right (0x6f/0x70) move the cursor and nudge the scroll Adjustment by 6 past the visible window; otherwise, unless the IME is active, escape (1) runs the escape actions, delete (0x75) clears the text (delete-all mode) or cuts the character after the cursor and refreshes, home (0x6c) puts the cursor at 0 and scrolls so the text that fits the text component's window shows, end (0x71) puts the cursor at the end, enter (0x1c) runs the enter actions; a handled key ends delete-all mode; finally the display text is rebuilt
// Grade: RELOCATION_MATCH (hand4d 2026-11-19). Closed by porting FableWin.pdb's /Od statement order (0x0225E8F0): every handled
//   branch clears DeleteAllOnKeyPress itself, the IME test wraps the inner chain, the delete-cut branch falls through to the shared
//   UpdateDisplayText (retail duplicates that tail at 0x005404EF), Ego_d.pdb locals (ch; it; part1 / part2; width, ptext, i) and the
//   home scroll spelled as FableWin computes it, Adjustment = -(Text.GetLength() - i + 5) (FableWin 0x0225EE1C sub / add 5 / neg;
//   retail 0x0054058B sub edi,eax ; add edi,-5). The old draft's three eax/edx vptr swaps closed with the same port.
// VC6 for scope: FableWin opens the escape, home and enter loops with `xor reg,reg ; je` (0x0225EA83, 0x0225ED7A, 0x0225EEC5), the
//   TLC `#define for if (0) {} else for` idiom; spelled `if (false) {} else for (...)` with the loop variable declared before it
//   (landed precedent 0x007C4430, GOTCHAS "VC6 for-scope idiom").
// Leaf: ?KeyPressed@CEditBox@NUISystem@@UAEXG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00540320, 755 bytes, `ret 4`.
// Callees: CWideString GetLength 0x0099B220, operator const wchar_t* 0x0099AF00, CWideString() 0x0099AED0, operator=(const wchar_t*)
//   0x0099B800 (L"" 0x0122D70C), operator=(const CWideString&) 0x0099B7D0, ~CWideString 0x0099B510, GetSubString 0x0099BDB0,
//   operator+(const CWideString&, const CWideString&) 0x0099BE70 (__fastcall); CIME::Get 0x00404A80 / IsIMEActive 0x00404C50;
//   CManager::GetInstance 0x0041E5F2 then CManager::Action (slot 14, +0x38); CText::GetCharWidthW 0x0054F4B0; own virtuals
//   GetTextComponent (slot 144, +0x240) and UpdateDisplayText (slot 147, +0x24c); CText::GetTextWindow (slot 148, +0x250).
//   Constant 2^32 0x0122DCB4 (unsigned width to float).
// Layout: shared engine/NUISystem_CEditBox.h (ActionsOnEnter +0x15c, ActionsOnEscape +0x160, Text +0x164, CursorPos +0x168,
//   LeftMostCharacter +0x16c, RightMostCharacter +0x170, Adjustment +0x174, DeleteAllOnKeyPress +0x17c); text component
//   engine/NUISystem_CText.h. Repo opt-ins (no overlay): FABLE_RETAIL_CIME_ISIMEACTIVE (fable_ime.h), FABLE_RETAIL_CTEXT_GETCHARWIDTHW,
//   FABLE_RETAIL_CTEXT_WINDOW_SLOTS (retail order of CText slots 147..150; now the default, kept harmless), FABLE_RETAIL_C2DBOXF
//   (inline GetBRX, FableWin calls ?GetBRX@C2DBoxF@@QBEMXZ at 0x0225EDF6; retail reads the box +8 at 0x0054056E).
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_SUBSTRING
#define FABLE_RETAIL_CIME
#define FABLE_RETAIL_CIME_ISIMEACTIVE
#define FABLE_RETAIL_CTEXT_GETCHARWIDTHW
#define FABLE_RETAIL_CTEXT_WINDOW_SLOTS
#include "engine/NUISystem_CEditBox.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CText.h"
#include "fable_ime.h"
namespace NUISystem {
void CEditBox::KeyPressed(wchar_t ch)
{
    if (ch == 0x6f) {
        --CursorPos;
        if (CursorPos < 0)
            CursorPos = 0;
        if (CursorPos < LeftMostCharacter)
            Adjustment -= 6;
        DeleteAllOnKeyPress = false;
    } else if (ch == 0x70) {
        ++CursorPos;
        if (CursorPos > Text.GetLength())
            CursorPos = Text.GetLength();
        if (CursorPos > RightMostCharacter - 1)
            Adjustment += 6;
        DeleteAllOnKeyPress = false;
    } else if (!CIME::Get().IsIMEActive()) {
        if (ch == 1) {
            std::list<CCountedPointer<CAction> >::iterator it;
            if (false) {} else for (it = ActionsOnEscape.begin(); it != ActionsOnEscape.end(); ++it)
                CManager::GetInstance()->Action(*it);
            DeleteAllOnKeyPress = false;
        } else if (ch == 0x75) {
            if (DeleteAllOnKeyPress) {
                Text = L"";
                CursorPos = 0;
                DeleteAllOnKeyPress = false;
            } else {
                CWideString part1;
                CWideString part2;
                if (CursorPos > 0)
                    part1 = Text.GetSubString(0, CursorPos);
                if (CursorPos < Text.GetLength())
                    part2 = Text.GetSubString(CursorPos + 1, Text.GetLength());
                Text = part1 + part2;
            }
        } else if (ch == 0x6c) {
            CursorPos = 0;
            float width = 0.0f;
            CText* ptext = static_cast<CText*>(GetTextComponent());
            long i;
            if (false) {} else for (i = 0; i != Text.GetLength(); ++i) {
                width += (float)CText::GetCharWidthW(((const wchar_t*)Text)[i]);
                if (width > ptext->GetTextWindow().GetBRX())
                    break;
            }
            Adjustment = -(Text.GetLength() - i + 5);
            if (Adjustment < 0)
                Adjustment = 0;
            DeleteAllOnKeyPress = false;
        } else if (ch == 0x71) {
            CursorPos = Text.GetLength();
            Adjustment = 0;
            DeleteAllOnKeyPress = false;
        } else if (ch == 0x1c) {
            std::list<CCountedPointer<CAction> >::iterator it;
            if (false) {} else for (it = ActionsOnEnter.begin(); it != ActionsOnEnter.end(); ++it)
                CManager::GetInstance()->Action(*it);
            DeleteAllOnKeyPress = false;
        }
    }
    UpdateDisplayText();
}
}
// ---- doubles ----
struct TStr { long len; unsigned short buf[64]; };
static TStr* TS(const void* p) { return (TStr*)p; }
CWideString::CWideString() { PStringData = 0; }
CWideString::CWideString(const CWideString& other) { PStringData = 0; *this = other; }
CWideString::~CWideString() { if (PStringData) free(PStringData); PStringData = 0; }
long CWideString::GetLength() const { return PStringData ? TS(PStringData)->len : 0; }
CWideString::operator const wchar_t*() const { static unsigned short e = 0; return PStringData ? (const wchar_t*)TS(PStringData)->buf : (const wchar_t*)&e; }
const CWideString& CWideString::operator=(const wchar_t* text)
{
    TStr* t = (TStr*)calloc(1, sizeof(TStr));
    const unsigned short* s = (const unsigned short*)text;
    while (s[t->len]) { t->buf[t->len] = s[t->len]; ++t->len; }
    if (PStringData) free(PStringData);
    PStringData = (CWideStringData*)t;
    return *this;
}
const CWideString& CWideString::operator=(const CWideString& other) { return *this = (const wchar_t*)other; }
CWideString CWideString::GetSubString(unsigned long start, unsigned long end) const
{
    unsigned short tmp[64]; unsigned long n = 0;
    for (unsigned long k = start; k < end && k < (unsigned long)GetLength(); ++k) tmp[n++] = TS(PStringData)->buf[k];
    tmp[n] = 0;
    CWideString r; r = (const wchar_t*)tmp; return r;
}
CWideString __fastcall operator+(const CWideString& left, const CWideString& right)
{
    unsigned short tmp[128]; long n = 0;
    const unsigned short* a = (const unsigned short*)(const wchar_t*)left; const unsigned short* b = (const unsigned short*)(const wchar_t*)right;
    for (long k = 0; k < left.GetLength(); ++k) tmp[n++] = a[k];
    for (long k2 = 0; k2 < right.GetLength(); ++k2) tmp[n++] = b[k2];
    tmp[n] = 0;
    CWideString r; r = (const wchar_t*)tmp; return r;
}
static bool g_ime = false;
static unsigned char g_imeObj[0x400];
NUISystem::CIME& FABLE_FASTCALL NUISystem::CIME::Get() { return *(NUISystem::CIME*)g_imeObj; }
bool NUISystem::CIME::IsIMEActive() const { return g_ime; }
static unsigned int g_widths[64];
static int g_widthCalls = 0;
unsigned int FABLE_FASTCALL NUISystem::CText::GetCharWidthW(wchar_t c) { ++g_widthCalls; return g_widths[(unsigned short)c - 'a']; }
static C2DBoxF g_window;
static int g_updates = 0, g_windowCalls = 0, g_actions = 0;
static void* g_actionArg[8];
static void* g_textObj[2];
struct Hooks {
    void* GetTextComponent() { return g_textObj; }
    void UpdateDisplayText() { ++g_updates; }
    C2DBoxF* GetTextWindow() { ++g_windowCalls; return &g_window; }
    void Action(void* ref) { if (g_actions < 8) g_actionArg[g_actions] = ref; ++g_actions; }
};
union PMF0 { void* (Hooks::*f)(); void* p; };
union PMF1 { void (Hooks::*f)(); void* p; };
union PMF2 { C2DBoxF* (Hooks::*f)(); void* p; };
union PMF3 { void (Hooks::*f)(void*); void* p; };
static void* g_boxVt[160];
static void* g_textVt[160];
static void* g_mgrVt[32];
static void* g_mgrObj[2];
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance() { return (NUISystem::CManager*)g_mgrObj; }
// accessor for the protected fields (never instantiated)
struct Probe : NUISystem::CEditBox {
    static Probe* Of(void* p) { return (Probe*)p; }
    long& Cur() { return CursorPos; } long& Left() { return LeftMostCharacter; } long& Right() { return RightMostCharacter; }
    long& Adj() { return Adjustment; } bool& Del() { return DeleteAllOnKeyPress; } CWideString& Txt() { return Text; }
    void** EnterHead() { return (void**)&ActionsOnEnter; } void** EscHead() { return (void**)&ActionsOnEscape; }
};
static void* g_box[0x18c / 4 + 1];
static int g_bad = 0;
static void Check(int cond, const char* what) { if (!cond) { printf("FAIL %s\n", what); g_bad = 1; } }
struct Node { Node* next; Node* prev; void* data[2]; };
static Node g_escHead, g_escA, g_escB, g_entHead, g_entA;
static Probe* Fresh(const char* text, long cur, long left, long right, long adj, bool del)
{
    Probe* p = Probe::Of(g_box);
    p->Txt().~CWideString();
    memset(g_box, 0, sizeof(g_box));
    *(void**)g_box = g_boxVt;
    *p->EscHead() = &g_escHead; *p->EnterHead() = &g_entHead;
    unsigned short w[64]; int n = 0; while (text[n]) { w[n] = (unsigned short)text[n]; ++n; } w[n] = 0;
    p->Txt() = (const wchar_t*)w;
    p->Cur() = cur; p->Left() = left; p->Right() = right; p->Adj() = adj; p->Del() = del;
    g_updates = 0; g_actions = 0; g_widthCalls = 0; g_windowCalls = 0;
    return p;
}
static int TextIs(Probe* p, const char* s)
{
    long n = (long)strlen(s); if (p->Txt().GetLength() != n) return 0;
    const unsigned short* b = (const unsigned short*)(const wchar_t*)p->Txt();
    for (long k = 0; k < n; ++k) if (b[k] != (unsigned short)s[k]) return 0;
    return 1;
}
static void Key(Probe* p, unsigned short k) { p->NUISystem::CEditBox::KeyPressed((wchar_t)k); }   // qualified: non-virtual call
int main()
{
    PMF0 a; a.f = &Hooks::GetTextComponent; g_boxVt[144] = a.p;
    PMF1 b; b.f = &Hooks::UpdateDisplayText; g_boxVt[147] = b.p;
    PMF2 c; c.f = &Hooks::GetTextWindow; g_textVt[148] = c.p;
    PMF3 d; d.f = &Hooks::Action; g_mgrVt[14] = d.p;
    g_textObj[0] = g_textVt; g_mgrObj[0] = g_mgrVt;
    g_escHead.next = &g_escA; g_escA.next = &g_escB; g_escB.next = &g_escHead;
    g_entHead.next = &g_entA; g_entA.next = &g_entHead;
    Probe* p;
    // left
    p = Fresh("abcdef", 3, 5, 9, 20, true); Key(p, 0x6f);
    Check(p->Cur() == 2 && p->Adj() == 14 && !p->Del() && g_updates == 1, "left scroll");
    p = Fresh("abcdef", 0, 0, 9, 20, true); Key(p, 0x6f);
    Check(p->Cur() == 0 && p->Adj() == 20 && !p->Del() && g_updates == 1, "left clamp");
    p = Fresh("abcdef", 6, 5, 9, 20, true); Key(p, 0x6f);
    Check(p->Cur() == 5 && p->Adj() == 20, "left no scroll at LeftMost");
    // right
    p = Fresh("abcdef", 2, 0, 4, 20, true); Key(p, 0x70);
    Check(p->Cur() == 3 && p->Adj() == 20 && !p->Del() && g_updates == 1, "right no scroll at Right-1");
    p = Fresh("abcdef", 3, 0, 4, 20, true); Key(p, 0x70);
    Check(p->Cur() == 4 && p->Adj() == 26, "right scroll");
    p = Fresh("abcdef", 6, 0, 20, 20, true); Key(p, 0x70);
    Check(p->Cur() == 6 && p->Adj() == 20, "right clamp at length");
    // IME active: nothing but the refresh
    g_ime = true;
    p = Fresh("abcdef", 3, 0, 9, 20, true); Key(p, 0x75);
    Check(p->Cur() == 3 && TextIs(p, "abcdef") && p->Del() && g_updates == 1, "ime gate");
    p = Fresh("abcdef", 3, 0, 9, 20, true); Key(p, 0x6f);
    Check(p->Cur() == 2 && !p->Del(), "ime does not gate left");
    g_ime = false;
    // escape / enter
    p = Fresh("ab", 1, 0, 9, 20, true); Key(p, 1);
    Check(g_actions == 2 && g_actionArg[0] == g_escA.data && g_actionArg[1] == g_escB.data && !p->Del() && g_updates == 1, "escape");
    p = Fresh("ab", 1, 0, 9, 20, true); Key(p, 0x1c);
    Check(g_actions == 1 && g_actionArg[0] == g_entA.data && !p->Del() && g_updates == 1, "enter");
    // delete
    p = Fresh("abcdef", 3, 0, 9, 20, true); Key(p, 0x75);
    Check(TextIs(p, "") && p->Cur() == 0 && !p->Del() && g_updates == 1, "delete all");
    p = Fresh("abcdef", 2, 0, 9, 20, false); Key(p, 0x75);
    Check(TextIs(p, "abdef") && p->Cur() == 2 && g_updates == 1, "delete middle");
    p = Fresh("abcdef", 0, 0, 9, 20, false); Key(p, 0x75);
    Check(TextIs(p, "bcdef") && p->Cur() == 0, "delete at 0");
    p = Fresh("abcdef", 6, 0, 9, 20, false); Key(p, 0x75);
    Check(TextIs(p, "abcdef") && g_updates == 1, "delete at end");
    // home
    for (int k = 0; k < 26; ++k) g_widths[k] = 10;
    g_window.BRX = 35.0f;
    p = Fresh("abcdefgh", 5, 0, 9, 20, true); Key(p, 0x6c);
    Check(p->Cur() == 0 && p->Adj() == 0 && !p->Del() && g_widthCalls == 4 && g_windowCalls == 4 && g_updates == 1, "home walk");
    g_window.BRX = 30.0f;
    p = Fresh("abcdefgh", 5, 0, 9, 20, true); Key(p, 0x6c);
    Check(g_widthCalls == 4 && p->Adj() == 0, "home walk equal width continues");
    g_window.BRX = 1000.0f;
    p = Fresh("abc", 2, 0, 9, 20, true); Key(p, 0x6c);
    Check(g_widthCalls == 3 && g_windowCalls == 3 && p->Adj() == 0 && p->Cur() == 0, "home whole text");
    p = Fresh("", 2, 0, 9, 20, true); Key(p, 0x6c);
    Check(g_widthCalls == 0 && p->Adj() == 0 && p->Cur() == 0, "home empty");
    g_widths[0] = 0x80000000u;
    p = Fresh("abc", 2, 0, 9, 20, true); Key(p, 0x6c);
    Check(g_widthCalls == 1 && p->Adj() == 0, "home unsigned width");
    g_widths[0] = 10;
    // end
    p = Fresh("abcdef", 2, 0, 9, 20, true); Key(p, 0x71);
    Check(p->Cur() == 6 && p->Adj() == 0 && !p->Del() && g_updates == 1, "end");
    // other key
    p = Fresh("abcdef", 2, 0, 9, 20, true); Key(p, 'x');
    Check(p->Cur() == 2 && p->Adj() == 20 && p->Del() && TextIs(p, "abcdef") && g_updates == 1, "other key");
    if (g_bad) return 1;
    printf("00540320_TEST PASS\n");
    return 0;
}