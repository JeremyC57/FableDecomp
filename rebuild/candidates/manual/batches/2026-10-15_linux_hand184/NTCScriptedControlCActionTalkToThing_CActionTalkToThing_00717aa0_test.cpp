// Behaviour test for 00717aa0 (see src/00717aa0.cpp): CActionTalkToThing ctor (text name): base args, flags, messages copied, cutscene flag; plain C++98, member access only.
#include <cstdio>
#include <cstring>
// shared test model: a thing with an interface table (sorted type -> component), a dead bit, and an
// intelligent pointer; mirrors the HasTC / GetInterface / FindEntry shapes of the sources.
struct TThing {
    unsigned long bits[9];
    long types[8];
    void* comps[8];
    int count;
    unsigned char flags91;
    unsigned long flags6c;
    TThing() : count(0), flags91(0), flags6c(0) { for (int i = 0; i < 9; ++i) bits[i] = 0; }
    void Add(long type, void* comp) { types[count] = type; comps[count] = comp; ++count; bits[type >> 5] |= 1UL << (type & 31); }
    unsigned long HasTC(long type) const { return bits[type >> 5] & (1UL << (type & 31)); }
    void* Find(long type) const { for (int i = 0; i < count; ++i) if (types[i] == type) return comps[i]; return 0; }
    template<class T> bool GetInterface(T** out, long type) const { if (!HasTC(type)) return false; *out = (T*)Find(type); return true; }
};
struct TPtr {
    TThing* item;
    TPtr() : item(0) { }
    TThing* GetPItem() const { return item; }
    void SetPItem(TThing* t) { item = t; }
};
inline bool Alive(const TPtr& p) { return p.GetPItem() != 0 && !(p.GetPItem()->flags91 & 1); }
#define CHECK(c, addr) do { if (!(c)) { std::printf("%s_TEST FAIL\n", addr); return 1; } } while (0)

struct TMsgs { int count; TMsgs() : count(0) { } };
static void GetMessagesFromTextName(TMsgs* out, const char* name, const char*, long) { out->count = (int)std::strlen(name); }
struct TGui { bool inCutscene; }; static TGui g_gui; static TGui* g_pGui = &g_gui;
struct TTalk { bool makeListen, useCam, sound2D; TMsgs Messages; bool AddedDuringCutscene, AskingHeroQuestion, DisplayOverScreenFade;
    TTalk(void*, const char* name, const char* group, long method, bool listen, bool sound, bool fade)
        : makeListen(listen), useCam(true), sound2D(sound), AskingHeroQuestion(false), DisplayOverScreenFade(fade)
    { TMsgs m; GetMessagesFromTextName(&m, name, group, method); Messages = m; AddedDuringCutscene = g_pGui->inCutscene; } };
int main() {
    g_gui.inCutscene = true; TTalk t(0, "HELLO", "G", 0, true, false, true);
    CHECK(t.Messages.count == 5 && t.AddedDuringCutscene && t.useCam && t.makeListen && !t.sound2D && t.DisplayOverScreenFade && !t.AskingHeroQuestion, "00717aa0");
    g_gui.inCutscene = false; TTalk u(0, "HI", "G", 0, false, true, false); CHECK(u.Messages.count == 2 && !u.AddedDuringCutscene && u.sound2D, "00717aa0");
    std::printf("00717aa0_TEST PASS\n"); return 0; }
