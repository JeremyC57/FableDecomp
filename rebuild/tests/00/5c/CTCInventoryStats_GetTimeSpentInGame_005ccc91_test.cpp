// Behaviour test for CTCInventoryStats::GetTimeSpentInGame (0x005CCC91): frames run at 15 per second; the text is
// "<time spent>: <hours> <hour label> <minutes> <mins label>" with the singular label for exactly one hour, leftover
// seconds dropped, and the main game text bank used when no bank is passed -- through a copy of the body over a
// stand-in text bank.
#include <cstdio>
#include <cstring>
#include <string>

struct DataBank {
    const char* prefix;
    std::string GetTextBySymbol(const std::string& symbol) const
    {
        if (symbol == "TEXT_GUI_STATS_LABEL_HOUR") return std::string(prefix) + "hour";
        if (symbol == "TEXT_GUI_STATS_LABEL_HOURS") return std::string(prefix) + "hours";
        if (symbol == "TEXT_GUI_STATS_LABEL_MINS") return std::string(prefix) + "mins";
        if (symbol == "TEXT_GUI_STATS_LABEL_TIMESPENT") return std::string(prefix) + "Time spent";
        return "?";
    }
};

static DataBank g_mainBank = { "" };

static std::string IntToString(long value)
{
    char buffer[32];
    std::sprintf(buffer, "%ld", value);
    return buffer;
}

static std::string GetTimeSpentInGame(unsigned long frames, const DataBank* bank)
{
    std::string space(" ");
    std::string colon(": ");
    std::string newLine("\n");
    std::string text;
    if (bank == 0)
        bank = &g_mainBank;
    std::string hoursText;
    unsigned long seconds = frames / 15;
    unsigned long hours = seconds / 3600;
    unsigned long secondsInHour = seconds % 3600;
    hoursText = IntToString((long)hours) + space;
    if (hours == 1) {
        std::string hourLabel("TEXT_GUI_STATS_LABEL_HOUR");
        hoursText += bank->GetTextBySymbol(hourLabel);
    }
    else {
        std::string hoursLabel("TEXT_GUI_STATS_LABEL_HOURS");
        hoursText += bank->GetTextBySymbol(hoursLabel);
    }
    {
        std::string minsLabel("TEXT_GUI_STATS_LABEL_MINS");
        std::string timeSpentLabel("TEXT_GUI_STATS_LABEL_TIMESPENT");
        text = bank->GetTextBySymbol(timeSpentLabel) + colon + hoursText + space +
               IntToString((long)(secondsInHour / 60)) + space + bank->GetTextBySymbol(minsLabel);
    }
    return text;
}

int main()
{
    // 1 h 2 min 59 s at 15 frames per second
    if (GetTimeSpentInGame((3600 + 120 + 59) * 15 + 14, 0) != "Time spent: 1 hour 2 mins") {
        std::printf("005ccc91_TEST FAIL\n");
        return 1;
    }
    DataBank other = { "x" };
    if (GetTimeSpentInGame((2 * 3600 + 30 * 60) * 15, &other) != "xTime spent: 2 xhours 30 xmins" ||
        GetTimeSpentInGame(59 * 15, &other) != "xTime spent: 0 xhours 0 xmins") {
        std::printf("005ccc91_TEST FAIL\n");
        return 1;
    }
    std::printf("005ccc91_TEST PASS\n");
    return 0;
}