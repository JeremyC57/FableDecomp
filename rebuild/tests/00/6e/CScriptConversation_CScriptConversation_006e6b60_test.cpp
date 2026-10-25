// Behaviour test for 0x006E6B60 CScriptConversation copy constructor.
#include <cstdio>
#include <cstring>

class CScriptConversationManager { public: int Id; };
class CCountedVector {                    // stands in for the two STLport vectors: counts copies
public:
    int Size;
    static int s_copies;
    CCountedVector(int n) : Size(n) {}
    CCountedVector(const CCountedVector& o) : Size(o.Size) { ++s_copies; }
};
int CCountedVector::s_copies = 0;
class CScriptConversation {
public:
    CScriptConversationManager* Manager;
    long ConversationIndex;
    CCountedVector PeopleInConversation;
    CCountedVector Conversation;
    long CurrentLine;
    long FramesLeftBeforeNewLine;
    bool SoundIn2D;
    bool StartedCurrentLineSpeech;
    bool PlayDuringCutScenes;
    CScriptConversation(CScriptConversationManager* m)
        : Manager(m), ConversationIndex(17), PeopleInConversation(2), Conversation(5), CurrentLine(3),
          FramesLeftBeforeNewLine(45), SoundIn2D(true), StartedCurrentLineSpeech(false), PlayDuringCutScenes(true) {}
    CScriptConversation(const CScriptConversation& other)
        : Manager(other.Manager), ConversationIndex(other.ConversationIndex),
          PeopleInConversation(other.PeopleInConversation), Conversation(other.Conversation),
          CurrentLine(other.CurrentLine), FramesLeftBeforeNewLine(other.FramesLeftBeforeNewLine),
          SoundIn2D(other.SoundIn2D), StartedCurrentLineSpeech(other.StartedCurrentLineSpeech),
          PlayDuringCutScenes(other.PlayDuringCutScenes) {}
};

int main()
{
    CScriptConversationManager mgr; mgr.Id = 1;
    CScriptConversation a(&mgr);
    CCountedVector::s_copies = 0;
    CScriptConversation b(a);
    if (!(CCountedVector::s_copies == 2 && b.Manager == &mgr && b.ConversationIndex == 17 && b.PeopleInConversation.Size == 2 && b.Conversation.Size == 5 && b.CurrentLine == 3 && b.FramesLeftBeforeNewLine == 45 && b.SoundIn2D && !b.StartedCurrentLineSpeech && b.PlayDuringCutScenes)) {
        std::printf("006e6b60_TEST FAIL\n");
        return 1;
    }
    std::printf("006e6b60_TEST PASS\n");
    return 0;
}