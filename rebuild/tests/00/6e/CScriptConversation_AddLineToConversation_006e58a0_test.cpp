// Behaviour test for 0x006E58A0 (CScriptConversation::AddLineToConversation).
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>

struct Thing { bool Dead; };
struct Line { long PersonSpeaking; long PersonSpokenTo; std::string LineToSay; bool ShowSubtitle; };

struct Conversation {
    std::vector<const Thing*> People;
    std::vector<Line> Lines;
    void AddLineToConversation(const Thing& speaker, const Thing* listener, const std::string& line, bool showSubtitle)
    {
        std::vector<const Thing*>::iterator it = std::find(People.begin(), People.end(), &speaker);
        if (it != People.end()) {
            long speakerIndex = (long)(it - People.begin());
            long listenerIndex = -1;
            if (listener && !listener->Dead) {
                std::vector<const Thing*>::iterator f = std::find(People.begin(), People.end(), listener);
                if (f != People.end()) listenerIndex = (long)(f - People.begin());
            }
            Line l; l.PersonSpeaking = speakerIndex; l.PersonSpokenTo = listenerIndex; l.LineToSay = line;
            l.ShowSubtitle = showSubtitle;
            Lines.push_back(l);
        }
    }
};
#define FAIL { std::printf("006e58a0_TEST FAIL\n"); return 1; }
int main()
{
    Thing a = { false }, b = { false }, c = { false }, dead = { true };
    Conversation conv; conv.People.push_back(&a); conv.People.push_back(&b); conv.People.push_back(&dead);
    conv.AddLineToConversation(c, &a, "nobody", true);
    if (!conv.Lines.empty()) FAIL;
    conv.AddLineToConversation(a, &b, "hello", true);
    if (conv.Lines.size() != 1 || conv.Lines[0].PersonSpeaking != 0 || conv.Lines[0].PersonSpokenTo != 1) FAIL;
    if (conv.Lines[0].LineToSay != "hello" || !conv.Lines[0].ShowSubtitle) FAIL;
    conv.AddLineToConversation(b, 0, "to all", false);
    if (conv.Lines.size() != 2 || conv.Lines[1].PersonSpeaking != 1 || conv.Lines[1].PersonSpokenTo != -1) FAIL;
    conv.AddLineToConversation(b, &dead, "to the dead", false);
    if (conv.Lines[2].PersonSpokenTo != -1) FAIL;
    conv.AddLineToConversation(b, &c, "outsider", false);
    if (conv.Lines[3].PersonSpokenTo != -1 || conv.Lines[3].ShowSubtitle) FAIL;
    std::printf("006e58a0_TEST PASS\n");
    return 0;
}