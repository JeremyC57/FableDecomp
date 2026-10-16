// Behaviour test for 0093c600 (see src/0093c600.cpp); plain C++98, member access only.
#include <cstdio>
// shared test model: a thing with an interface table (sorted type -> component), a dead bit, and an
// intelligent pointer; mirrors the HasTC / GetInterface / FindEntry shapes of the sources.
struct TThing {
    unsigned long bits[9];
    long types[8];
    void* comps[8];
    int count;
    unsigned char flags91;
    TThing() : count(0), flags91(0) { for (int i = 0; i < 9; ++i) bits[i] = 0; }
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
inline bool AliveT(const TThing* t) { return t != 0 && !(t->flags91 & 1); }
#define CHECK(c, addr) do { if (!(c)) { std::printf("%s_TEST FAIL\n", addr); return 1; } } while (0)
// StealthSeekEnemy::SetWhatsThatSound: with the sound component, play the WHATS_THAT pair with default parameters
#include <cstring>
struct TParams { float Volume, Pitch, Delay; long Loop, Priority, Pan, Flags, Handle;
    TParams() : Volume(1.0f), Pitch(1.0f), Delay(0.0f), Loop(1), Priority(1), Pan(0), Flags(0), Handle(-1) { } };
struct TPlayer { long played; TParams last; void Play(long sound, const TParams& p) { played = sound; last = p; } };
static long GetSoundPair(const char* name) { return std::strcmp(name, "WHATS_THAT") == 0 ? 77 : 0; }
static void SetWhatsThatSound(TThing* creature) { TPlayer* p = 0; if (creature->GetInterface(&p, 0x10)) p->Play(GetSoundPair("WHATS_THAT"), TParams()); }
int main() {
    TThing me; TPlayer p; p.played = 0; SetWhatsThatSound(&me); CHECK(p.played == 0, "0093c600");
    me.Add(0x10, &p); SetWhatsThatSound(&me); CHECK(p.played == 77 && p.last.Handle == -1 && p.last.Volume == 1.0f && p.last.Loop == 1, "0093c600");
    std::printf("0093c600_TEST PASS\n"); return 0; }
