#include <cstdio>
// CAIStateGroup_MinionKidGroupTag::OnActivate: add mode 1 if missing (remember it), register, set tagger when needed
struct Modes { bool has1; int added; bool HasMode(unsigned long m) { return m == 1 && has1; } void AddMode(unsigned long m) { if (m == 1) { has1 = true; ++added; } } };
struct Tag { void* players[4]; int count; void* tagger; int setCount;
    void AddPlayer(void* p) { players[count++] = p; } void* GetTagger() { return tagger; } void SetTagger(void* p) { tagger = p; ++setCount; } };
struct Group {
    void* creature; Modes* modes; Tag* tag; bool initiatedTag; bool standModeAdded;
    void OnActivate()
    {
        if (!modes->HasMode(1)) { standModeAdded = true; modes->AddMode(1); } else standModeAdded = false;
        tag->AddPlayer(creature);
        if (initiatedTag || tag->GetTagger() == 0) tag->SetTagger(creature);
    }
};
int main()
{
    int me, other; Modes m; m.has1 = false; m.added = 0; Tag t; t.count = 0; t.tagger = 0; t.setCount = 0;
    Group g; g.creature = &me; g.modes = &m; g.tag = &t; g.initiatedTag = false; g.standModeAdded = false;
    g.OnActivate();
    if (!g.standModeAdded || m.added != 1 || t.count != 1 || t.players[0] != &me || t.tagger != &me) { std::printf("00934120_TEST FAIL\n"); return 1; }
    t.tagger = &other; g.OnActivate();
    if (g.standModeAdded || m.added != 1 || t.count != 2 || t.tagger != &other) { std::printf("00934120_TEST FAIL\n"); return 1; }
    g.initiatedTag = true; g.OnActivate();
    if (t.tagger != &me || t.setCount != 2) { std::printf("00934120_TEST FAIL\n"); return 1; }
    std::printf("00934120_TEST PASS\n"); return 0;
}