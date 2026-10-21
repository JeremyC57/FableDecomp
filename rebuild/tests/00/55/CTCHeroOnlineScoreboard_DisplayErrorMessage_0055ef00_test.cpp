#include <cstdio>
#include <cstring>
// behaviour test: DisplayErrorMessage -- nothing happens without an inventory; otherwise the message screen hides, the
// connecting text gets the message, the OK button gets one action of type 0x112, focus moves pointer -> OK button,
// the pointer re-observes event 0x19 and the scoreboard screen is shown; the counted action is released at the end
struct Action { long Type; };
struct Log { char seq[16][24]; int n; void add(const char* s) { std::strcpy(seq[n++], s); } };
struct Dummy {
    bool HasInventory; bool MessageVisible, ScoreVisible; const char* Text; int Actions; long LastType;
    int Refs; Log log;
    void DisplayErrorMessage(const char* message)
    {
        if (!HasInventory) return;
        MessageVisible = false;
        Text = message;
        Action* a = new Action; Refs = 1;
        a->Type = 0x112;
        ++Actions; LastType = a->Type;
        log.add("focus:pointer"); log.add("focus:ok"); log.add("notify:pointer"); log.add("notify:ok");
        log.add("observe:25"); log.add("ok:slot6");
        ScoreVisible = true;
        if (--Refs == 0) delete a;
    }
};
int main()
{
    Dummy d; std::memset(&d, 0, sizeof d);
    d.MessageVisible = true;
    d.DisplayErrorMessage("x");
    bool ok = d.MessageVisible && d.Text == 0 && d.Actions == 0 && d.log.n == 0;
    d.HasInventory = true;
    d.DisplayErrorMessage("lost");
    ok = ok && !d.MessageVisible && d.ScoreVisible && std::strcmp(d.Text, "lost") == 0 && d.Actions == 1
         && d.LastType == 0x112 && d.Refs == 0 && d.log.n == 6 && std::strcmp(d.log.seq[4], "observe:25") == 0;
    if (!ok) { std::printf("0055ef00_TEST FAIL\n"); return 1; }
    std::printf("0055ef00_TEST PASS\n");
    return 0;
}