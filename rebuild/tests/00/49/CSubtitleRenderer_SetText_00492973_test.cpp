// Behaviour test for CSubtitleRenderer::SetText (0x00492973): a copy of the function's logic on a small model.
#include <cstdio>
#include <string>
#include <vector>
#include <list>

struct Info { std::string text, b1, b2, b3; unsigned long flags; float time; bool uwp, cpm; };
struct Message { long type; };
struct Model {
    bool displaying, allShown, updateTimer, updateWhilePaused, paused, heroAlive, heroHasTC, heroDisplayingItems, modeInit;
    long groupIndex, charsShowing; bool groupRemaining; std::string groupTag, text, name, button1, button2, button3, over1, over2;
    unsigned char alpha1, alpha2; unsigned long flags; float maxTime, timer; int preloads;
    std::vector<Info> queue; std::list<Message> messages;
    Model() : displaying(false), allShown(false), updateTimer(false), updateWhilePaused(false), paused(false), heroAlive(false),
              heroHasTC(false), heroDisplayingItems(false), modeInit(false), groupIndex(0), charsShowing(5), groupRemaining(true),
              alpha1(0), alpha2(0), flags(0), maxTime(0.0f), timer(1.0f), preloads(0) { }
};
static std::string Resolve(const std::string& tag) { return tag.empty() ? tag : "[" + tag + "]"; }
static void SetText(Model& m, const std::string& text, const std::string& name, const std::string& a4, const std::string& a5,
                    const std::string& a6, unsigned long flags, float time, bool uwp, bool cpm)
{
    if (m.displaying) { Info i = { text, a4, a5, a6, flags, time, uwp, cpm }; m.queue.push_back(i); return; }
    m.groupIndex = -1; m.groupRemaining = false; m.groupTag = "";
    m.text = Resolve(text); m.name = Resolve(name); m.button2 = Resolve(a4); m.button1 = Resolve(a5); m.button3 = Resolve(a6);
    m.over1 = ""; m.over2 = "";
    m.timer = 0.0f; m.maxTime = time; m.charsShowing = 0; m.alpha1 = m.alpha2 = 0xff; m.displaying = true; m.flags = flags;
    m.updateTimer = true; m.allShown = true;
    if (flags & 0x10) m.allShown = false;
    ++m.preloads;
    if (!(m.flags & 0x4c) && !(m.flags & 0x100)) m.updateWhilePaused = uwp;
    else {
        if ((m.flags & 0x40) && m.heroAlive && m.heroHasTC) m.heroDisplayingItems = true;
        m.paused = true;
    }
    if (cpm) m.modeInit = true;
    std::list<const Message*> found;
    for (std::list<Message>::const_iterator it = m.messages.begin(); it != m.messages.end(); ++it)
        if (it->type == 0x11) found.push_back(&*it);
    if ((long)found.size() > 0)
        for (std::list<const Message*>::iterator f = found.begin(); f != found.end(); ++f)
            for (std::list<Message>::iterator it = m.messages.begin(); it != m.messages.end(); ++it)
                if (&*it == *f) { m.messages.erase(it); break; }
}
int main()
{
    Model m;
    m.heroAlive = true; m.heroHasTC = true;
    Message a = { 0x11 }, b = { 3 }, c = { 0x11 };
    m.messages.push_back(a); m.messages.push_back(b); m.messages.push_back(c);
    SetText(m, "t", "n", "x", "y", "z", 0x50, 2.5f, true, true);
    bool ok = m.displaying && m.text == "[t]" && m.name == "[n]" && m.button2 == "[x]" && m.button1 == "[y]" && m.button3 == "[z]"
        && m.groupIndex == -1 && !m.allShown && m.paused && m.heroDisplayingItems && m.modeInit && m.maxTime == 2.5f
        && m.messages.size() == 1 && m.messages.front().type == 3 && m.alpha1 == 0xff && m.preloads == 1 && !m.updateWhilePaused;
    SetText(m, "q", "", "1", "2", "3", 1, 1.0f, false, false);
    ok = ok && m.queue.size() == 1 && m.queue[0].text == "q" && m.queue[0].b1 == "1" && m.text == "[t]";
    Model n;
    SetText(n, "s", "", "", "", "", 2, 0.5f, true, false);
    ok = ok && n.allShown && !n.paused && n.updateWhilePaused && !n.modeInit && n.flags == 2;
    if (!ok) { std::printf("00492973_TEST FAIL\n"); return 1; }
    std::printf("00492973_TEST PASS\n");
    return 0;
}