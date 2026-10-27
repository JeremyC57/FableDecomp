// Test for 0x009EAAE0 CConsole::SetConsoleHelpWindowColour: the help-window colour takes R, G, B from the first three
// console arguments (narrowed to bytes) and alpha 255; the other colours are untouched.
#include <stdio.h>
class CGenericVar { public: unsigned int ValULONG; };
class CRGBColour {
public:
    unsigned char B, G, R, A;
    void SetChannels(int r, int g, int b, int a) { R = (unsigned char)r; G = (unsigned char)g; B = (unsigned char)b; A = (unsigned char)a; }
};
class CConsole {
public:
    static void SetConsoleHelpWindowColour(CConsole& console, const CGenericVar* args)
    { console.HelpWindowColour.SetChannels(args[0].ValULONG, args[1].ValULONG, args[2].ValULONG, 255); }
    CRGBColour HistoryWindowColour, ClientWindowColour, HelpWindowColour, ScrollBarColour, TextColour;
};
int main()
{
    CConsole c; c.HelpWindowColour.R = c.HelpWindowColour.G = c.HelpWindowColour.B = c.HelpWindowColour.A = 0;
    c.ClientWindowColour.R = 9;
    CGenericVar args[3]; args[0].ValULONG = 10; args[1].ValULONG = 0x120; args[2].ValULONG = 30;
    CConsole::SetConsoleHelpWindowColour(c, args);
    bool ok = c.HelpWindowColour.R == 10 && c.HelpWindowColour.G == 0x20 && c.HelpWindowColour.B == 30 &&
              c.HelpWindowColour.A == 255 && c.ClientWindowColour.R == 9;
    puts(ok ? "009eaae0_TEST PASS" : "009eaae0_TEST FAIL");
    return ok ? 0 : 1;
}