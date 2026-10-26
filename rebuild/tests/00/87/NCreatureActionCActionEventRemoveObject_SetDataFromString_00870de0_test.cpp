#include <cstdio>
// Behaviour test for NCreatureAction::CActionEventRemoveObject::SetDataFromString (retail 0x00870DE0): field 0 of the
// event string becomes the Hand name; the result is the parser's success flag normalised to bool.
class CCharString { public:
    char Text[32];
};
static int g_parseResult;
static int g_lastIndex;
int GetDataFromEventString(const CCharString& data, long index, CCharString* out)
{
    g_lastIndex = (int)index;
    if (g_parseResult) {
        int i = 0;
        while (data.Text[i] && data.Text[i] != ',' && i < 31) { out->Text[i] = data.Text[i]; ++i; }
        out->Text[i] = 0;
    }
    return g_parseResult;
}
struct CActionEventRemoveObject {
    CCharString Hand;
    bool SetDataFromString(const CCharString& data)
    {
        return GetDataFromEventString(data, 0, &Hand) ? true : false;
    }
};

int main()
{
    CActionEventRemoveObject e;
    CCharString data;
    std::sprintf(data.Text, "RIGHT_HAND,X");
    e.Hand.Text[0] = 0;
    g_lastIndex = -1;
    g_parseResult = 7;
    bool ok = e.SetDataFromString(data);
    int pass = ok == true && g_lastIndex == 0 && e.Hand.Text[0] == 'R' && e.Hand.Text[10] == 0;
    g_parseResult = 0;
    bool bad = e.SetDataFromString(data);
    pass = pass && bad == false;
    if (!pass) { std::printf("00870de0_TEST FAIL\n"); return 1; }
    std::printf("00870de0_TEST PASS\n");
    return 0;
}