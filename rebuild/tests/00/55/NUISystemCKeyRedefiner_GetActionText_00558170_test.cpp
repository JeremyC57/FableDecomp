// Behaviour test for NUISystem::CKeyRedefiner::GetActionText (0x00558170): for the movement action the sub-type picks
// the direction's game text (the tag itself when the bank has none, "Invalid Movement!!" for other sub-types); for
// other actions the symbol's game text (or the symbol itself) gets " <sub-type>" appended when the sub-type is
// positive -- through a restatement of the body (one tag per case) over a stand-in text bank (narrow strings).
#include <cstdio>
#include <map>
#include <string>

struct DataBank {
    std::map<std::string, std::string> texts;
    std::string GetTextBySymbol(const std::string& symbol) const
    {
        std::map<std::string, std::string>::const_iterator it = texts.find(symbol);
        return it == texts.end() ? std::string() : it->second;
    }
};

static std::string IntToString(long value)
{
    char buffer[32];
    std::sprintf(buffer, "%ld", value);
    return buffer;
}

static void GetActionText(const DataBank* bank, int action, int subType, std::string& text, const std::string& symbol)
{
    if (action == 0x3c) {
        const char* tag;
        switch (subType) {
        case 10: tag = "TEXT_GUI_ACTION_MOVE_FORWARD"; break;
        case 11: tag = "TEXT_GUI_ACTION_MOVE_BACK"; break;
        case 12: tag = "TEXT_GUI_ACTION_MOVE_LEFT"; break;
        case 13: tag = "TEXT_GUI_ACTION_MOVE_RIGHT"; break;
        default:
            text = "Invalid Movement!!";
            return;
        }
        text = bank->GetTextBySymbol(tag);
        if (text == "")
            text = tag;
        return;
    }
    text = bank->GetTextBySymbol(symbol);
    if (text == "")
        text = symbol;
    if (subType >= 1)
        text += " " + IntToString(subType);
}

int main()
{
    DataBank bank;
    bank.texts["TEXT_GUI_ACTION_MOVE_FORWARD"] = "Forward";
    bank.texts["TEXT_GUI_ACTION_ATTACK"] = "Attack";
    std::string text;
    GetActionText(&bank, 0x3c, 10, text, "IGNORED");
    if (text != "Forward") { std::printf("00558170_TEST FAIL\n"); return 1; }
    GetActionText(&bank, 0x3c, 13, text, "IGNORED");
    if (text != "TEXT_GUI_ACTION_MOVE_RIGHT") { std::printf("00558170_TEST FAIL\n"); return 1; }
    GetActionText(&bank, 0x3c, 3, text, "IGNORED");
    if (text != "Invalid Movement!!") { std::printf("00558170_TEST FAIL\n"); return 1; }
    GetActionText(&bank, 0x10, 2, text, "TEXT_GUI_ACTION_ATTACK");
    if (text != "Attack 2") { std::printf("00558170_TEST FAIL\n"); return 1; }
    GetActionText(&bank, 0x10, 0, text, "TEXT_GUI_ACTION_BLOCK");
    if (text != "TEXT_GUI_ACTION_BLOCK") { std::printf("00558170_TEST FAIL\n"); return 1; }
    std::printf("00558170_TEST PASS\n");
    return 0;
}