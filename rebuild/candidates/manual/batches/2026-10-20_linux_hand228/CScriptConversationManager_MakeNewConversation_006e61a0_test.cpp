#include <cstdio>
// Behaviour model of CScriptConversationManager::MakeNewConversation @ 0x006E61A0 (copy of the source's logic, stand-ins).
struct CThing { int id; };
struct CScriptConversationManager;
static int g_dtors = 0, g_copies = 0;
struct CScriptConversation {
    CScriptConversationManager* Manager; long ConversationIndex;
    CThing* people[4]; int peopleCount;
    long CurrentLine, FramesLeftBeforeNewLine; bool SoundIn2D, StartedCurrentLineSpeech, PlayDuringCutScenes;
    CScriptConversation(CScriptConversationManager& manager, long index, CThing& person, bool soundIn2D, bool playDuringCutScenes)
        : Manager(&manager), ConversationIndex(index), peopleCount(0), CurrentLine(-1), FramesLeftBeforeNewLine(0), SoundIn2D(soundIn2D),
          StartedCurrentLineSpeech(false), PlayDuringCutScenes(playDuringCutScenes)
    { AddPersonToConversation(person); }
    CScriptConversation(const CScriptConversation& o) { *this = o; ++g_copies; }
    ~CScriptConversation() { ++g_dtors; }
    void AddPersonToConversation(CThing& person) { people[peopleCount++] = &person; }
};
template<class T1, class T2> struct Pair {
    T1 first; T2 second;
    Pair(const T1& a, const T2& b) : first(a), second(b) {}
    template<class U1, class U2> Pair(const Pair<U1, U2>& p) : first(p.first), second(p.second) {}
};
template<class T1, class T2> Pair<T1, T2> MakePair(const T1& a, const T2& b) { return Pair<T1, T2>(a, b); }
struct CConversationMap {
    long keys[4]; CScriptConversation* values[4]; int count;
    bool insert(const Pair<const long, CScriptConversation>& v) { keys[count] = v.first; values[count] = new CScriptConversation(v.second); ++count; return true; }
};
struct CScriptConversationManager {
    CConversationMap Conversations; long NextConversationIndex;
    long MakeNewConversation(CThing& person, bool soundIn2D, bool playDuringCutScenes)
    {
        long index = NextConversationIndex++;
        CScriptConversation conversation(*this, index, person, soundIn2D, playDuringCutScenes);
        Conversations.insert(MakePair(index, conversation));
        return index;
    }
};
int main()
{
    static CScriptConversationManager m; m.Conversations.count = 0; m.NextConversationIndex = 7;
    CThing hero = { 1 }, guard = { 2 };
    long a = m.MakeNewConversation(hero, true, false);
    long b = m.MakeNewConversation(guard, false, true);
    CScriptConversation* c0 = m.Conversations.values[0];
    CScriptConversation* c1 = m.Conversations.values[1];
    int ok = a == 7 && b == 8 && m.NextConversationIndex == 9 && m.Conversations.count == 2
          && m.Conversations.keys[0] == 7 && m.Conversations.keys[1] == 8
          && c0->Manager == &m && c0->ConversationIndex == 7 && c0->peopleCount == 1 && c0->people[0] == &hero
          && c0->SoundIn2D && !c0->PlayDuringCutScenes && c0->CurrentLine == -1 && c0->FramesLeftBeforeNewLine == 0
          && c1->people[0] == &guard && !c1->SoundIn2D && c1->PlayDuringCutScenes && !c1->StartedCurrentLineSpeech;
    // every temporary built in the function is destroyed again: only the two stored copies survive
    ok = ok && g_dtors == g_copies;   // (2 + copies) constructed, all but the 2 stored copies destroyed
    delete c0; delete c1;
    if (!ok) { std::printf("006e61a0_TEST FAIL\n"); return 1; }
    std::printf("006e61a0_TEST PASS\n");
    return 0;
}
