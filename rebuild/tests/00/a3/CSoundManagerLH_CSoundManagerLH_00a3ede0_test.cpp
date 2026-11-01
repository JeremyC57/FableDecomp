// Test for CSoundManagerLH::~CSoundManagerLH (0x00A3EDE0): the base banks are cleared first, the sound system is shut
// down (slot 13) and its pointer cleared, then the name map's strings, the name-pair map's strings, the sound list's nodes
// are released, and the CASoundManager base runs last.
#include <cstdio>
#include <cstdlib>
static char g_log[64]; static int g_n = 0; static int g_strings = 0, g_frees = 0;
struct String { int id; ~String() { ++g_strings; } };
struct NameEntry { String first; void* second; };
struct PairEntry { String a; String b; void* second; };
struct Node { Node* next; Node* prev; void* data; };
struct ASoundManager { virtual ~ASoundManager() { g_log[g_n++] = 'A'; } void ClearBanks() { g_log[g_n++] = 'C'; } };
struct SoundSystem { virtual void Shutdown() { g_log[g_n++] = 'S'; } };
template <class E> struct Vec {
    E* start; E* finish; E* eos;
    ~Vec() { for (E* e = start; e != finish; ++e) e->~E(); if (start) { std::free(start); ++g_frees; } }
};
struct List {
    Node* head;
    ~List() { Node* c = head->next; while (c != head) { Node* t = c; c = c->next; std::free(t); ++g_frees; } std::free(head); ++g_frees; g_log[g_n++] = 'L'; }
};
struct SoundManagerLH : ASoundManager {
    SoundSystem* System; void* Main; long m48; List Sounds; long NextEmitter;
    Vec<PairEntry> NamePairMap; bool d1; Vec<NameEntry> NameMap; bool d2;
    ~SoundManagerLH() { ClearBanks(); if (System) { System->Shutdown(); System = 0; } }
};
template <class E> static void Fill(Vec<E>& v, int n) { v.start = (E*)std::malloc(n * sizeof(E)); v.finish = v.start + n; v.eos = v.finish; }
int main()
{
    SoundSystem sys;
    SoundManagerLH* m = new SoundManagerLH;
    m->System = &sys;
    m->Sounds.head = (Node*)std::malloc(sizeof(Node)); m->Sounds.head->next = m->Sounds.head->prev = m->Sounds.head;
    Node* n1 = (Node*)std::malloc(sizeof(Node)); n1->next = m->Sounds.head; n1->prev = m->Sounds.head; m->Sounds.head->next = n1; m->Sounds.head->prev = n1;
    Fill(m->NamePairMap, 2); Fill(m->NameMap, 3);
    delete m;
    if (g_n != 4 || g_log[0] != 'C' || g_log[1] != 'S' || g_log[2] != 'L' || g_log[3] != 'A') { std::printf("00a3ede0_TEST FAIL\n"); return 1; }
    if (g_strings != 3 + 2 * 2 || g_frees != 4) { std::printf("00a3ede0_TEST FAIL\n"); return 1; }
    m = new SoundManagerLH; m->System = 0;
    m->Sounds.head = (Node*)std::malloc(sizeof(Node)); m->Sounds.head->next = m->Sounds.head->prev = m->Sounds.head;
    m->NamePairMap.start = m->NamePairMap.finish = 0; m->NameMap.start = m->NameMap.finish = 0;
    delete m;
    if (g_n != 7 || g_log[4] != 'C' || g_log[5] != 'L' || g_log[6] != 'A' || g_frees != 5) { std::printf("00a3ede0_TEST FAIL\n"); return 1; }
    std::printf("00a3ede0_TEST PASS\n");
    return 0;
}