// Behaviour test for CTCHeroOnlineScoreboard::InitialiseActivityInfo (0x00562710): the vector is sized to 39 activities,
// every activity gets its three text symbols, and only the listed activities get 2 decimal places / lower-is-better.
#include <cstdio>
#include <cstring>
#include <cstdlib>

class CCharString { public:
    const char* text;
    CCharString() : text(0) { }
    const CCharString& operator=(const char* t) { text = t; return *this; }
};
struct CActivity {
    CCharString   Name;
    CCharString   Description;
    CCharString   UnitOfMeasurement;
    unsigned char NumDecPlacesToShow;
    bool          LowerIsBetter;
    CActivity() : NumDecPlacesToShow(0), LowerIsBetter(false) { }
};
struct CActivityVector {
    CActivity* _M_start;
    CActivity* _M_finish;
    CActivity* _M_end_of_storage;
    CActivityVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CActivityVector() { delete[] _M_start; }
    void resize(unsigned int n) { delete[] _M_start; _M_start = new CActivity[n]; _M_finish = _M_end_of_storage = _M_start + n; }
    CActivity& operator[](unsigned int n) { return _M_start[n]; }
};
class CTCHeroOnlineScoreboard { public:
    CActivityVector ActivityInfo;
    void InitialiseActivityInfo();
};
void CTCHeroOnlineScoreboard::InitialiseActivityInfo()
{
    ActivityInfo.resize(39);
    ActivityInfo[0].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_MURDERS";
    ActivityInfo[1].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_DECAPITATIONS";
    ActivityInfo[2].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_COMBATMULT";
    ActivityInfo[3].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_EXPERIENCE";
    ActivityInfo[4].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_CHICKEN_KICK_DIST";
    ActivityInfo[5].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_WEALTH";
    ActivityInfo[6].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SEX";
    ActivityInfo[7].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_BIGGEST_FISH";
    ActivityInfo[8].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_ARCHERY_SCORE";
    ActivityInfo[9].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SPOUSES";
    ActivityInfo[10].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_TEMPLE_CONTRIBUTION";
    ActivityInfo[11].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_ADMIRERS";
    ActivityInfo[12].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_WEDDINGS";
    ActivityInfo[13].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_DIVORCES";
    ActivityInfo[14].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_TIMES_WIDOWED";
    ActivityInfo[15].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_PUKINGS";
    ActivityInfo[16].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_RENT_COLLECTED";
    ActivityInfo[17].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_CHICKEN_KICK";
    ActivityInfo[18].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_PAIRS";
    ActivityInfo[19].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_CARD_SORTING";
    ActivityInfo[20].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_GUESS_ADDITION";
    ActivityInfo[21].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_ORACLE_MINIGAME";
    ActivityInfo[22].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_COIN_GOLF_OV";
    ActivityInfo[23].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_COIN_GOLF_SS";
    ActivityInfo[24].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_BLACKJACK";
    ActivityInfo[25].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_HAPENNY";
    ActivityInfo[26].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SUCCESSIVE_WINS_PAIRS";
    ActivityInfo[27].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SUCCESSIVE_WINS_CARD_SORTING";
    ActivityInfo[28].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SUCCESSIVE_WINS_GUESS_ADDITION";
    ActivityInfo[29].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SUCCESSIVE_WINS_ORACLE_MINIGAME";
    ActivityInfo[30].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SUCCESSIVE_WINS_COIN_GOLF_OV";
    ActivityInfo[31].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SUCCESSIVE_WINS_COIN_GOLF_SS";
    ActivityInfo[32].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SUCCESSIVE_WINS_BLACKJACK";
    ActivityInfo[33].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SUCCESSIVE_WINS_HAPENNY";
    ActivityInfo[34].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_CRIMES_REPORTED";
    ActivityInfo[35].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_TOTAL_FINES";
    ActivityInfo[36].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_TOTAL_FINES_PAID";
    ActivityInfo[37].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_BRIBES";
    ActivityInfo[38].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_BIGGEST_FISH_IN_COMPETITION";
    ActivityInfo[0].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_MURDERS";
    ActivityInfo[1].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_DECAPITATIONS";
    ActivityInfo[2].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_COMBATMULT";
    ActivityInfo[3].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_EXPERIENCE";
    ActivityInfo[4].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_CHICKEN_KICK_DIST";
    ActivityInfo[5].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_WEALTH";
    ActivityInfo[6].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SEX";
    ActivityInfo[7].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_BIGGEST_FISH";
    ActivityInfo[8].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_ARCHERY_SCORE";
    ActivityInfo[9].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SPOUSES";
    ActivityInfo[10].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_TEMPLE_CONTRIBUTION";
    ActivityInfo[11].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_ADMIRERS";
    ActivityInfo[12].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_WEDDINGS";
    ActivityInfo[13].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_DIVORCES";
    ActivityInfo[14].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_TIMES_WIDOWED";
    ActivityInfo[15].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_PUKINGS";
    ActivityInfo[16].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_RENT_COLLECTED";
    ActivityInfo[17].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_CHICKEN_KICK";
    ActivityInfo[18].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_PAIRS";
    ActivityInfo[19].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_CARD_SORTING";
    ActivityInfo[20].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_GUESS_THE_ADDITION";
    ActivityInfo[21].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_ORACLE_MINIGAME";
    ActivityInfo[22].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_COIN_GOLF_OV";
    ActivityInfo[23].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_COIN_GOLF_SS";
    ActivityInfo[24].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_BLACKJACK";
    ActivityInfo[25].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_HAPENNY";
    ActivityInfo[26].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SUCCESSIVE_WINS_PAIRS";
    ActivityInfo[27].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SUCCESSIVE_WINS_CARD_SORTING";
    ActivityInfo[28].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SUCCESSIVE_WINS_GUESS_THE_ADDITION";
    ActivityInfo[29].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SUCCESSIVE_WINS_ORACLE_MINIGAME";
    ActivityInfo[30].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SUCCESSIVE_WINS_COIN_GOLF_OV";
    ActivityInfo[31].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SUCCESSIVE_WINS_COIN_GOLF_SS";
    ActivityInfo[32].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SUCCESSIVE_WINS_BLACKJACK";
    ActivityInfo[33].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SUCCESSIVE_WINS_HAPENNY";
    ActivityInfo[34].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_CRIMES_REPORTED";
    ActivityInfo[35].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_TOTAL_FINES";
    ActivityInfo[36].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_TOTAL_FINES_PAID";
    ActivityInfo[37].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_BRIBES";
    ActivityInfo[38].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_BIGGEST_FISH_IN_COMPETITION";
    ActivityInfo[4].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_METRES";
    ActivityInfo[5].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GOLD";
    ActivityInfo[10].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GOLD";
    ActivityInfo[16].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GOLD";
    ActivityInfo[35].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GOLD";
    ActivityInfo[36].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GOLD";
    ActivityInfo[37].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GOLD";
    ActivityInfo[7].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GRAMS";
    ActivityInfo[38].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GRAMS";
    ActivityInfo[18].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_SECONDS";
    ActivityInfo[19].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_SECONDS";
    ActivityInfo[20].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_SECONDS";
    ActivityInfo[25].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_POINTS";
    ActivityInfo[22].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_STROKES";
    ActivityInfo[23].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_STROKES";
    ActivityInfo[24].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GOLD";
    ActivityInfo[17].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_POINTS";
    ActivityInfo[8].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_POINTS";
    ActivityInfo[4].NumDecPlacesToShow = 2;
    ActivityInfo[7].NumDecPlacesToShow = 2;
    ActivityInfo[38].NumDecPlacesToShow = 2;
    ActivityInfo[18].NumDecPlacesToShow = 2;
    ActivityInfo[19].NumDecPlacesToShow = 2;
    ActivityInfo[20].NumDecPlacesToShow = 2;
    ActivityInfo[21].NumDecPlacesToShow = 2;
    ActivityInfo[18].LowerIsBetter = true;
    ActivityInfo[19].LowerIsBetter = true;
    ActivityInfo[20].LowerIsBetter = true;
    ActivityInfo[21].LowerIsBetter = true;
    ActivityInfo[22].LowerIsBetter = true;
    ActivityInfo[23].LowerIsBetter = true;
}

int main()
{
    CTCHeroOnlineScoreboard sb;
    sb.InitialiseActivityInfo();
    bool ok = (sb.ActivityInfo._M_finish - sb.ActivityInfo._M_start) == 39;
    for (int i = 0; ok && i < 39; ++i) {
        CActivity& a = sb.ActivityInfo[i];
        if (a.Name.text == 0 || a.Description.text == 0)
            ok = false;
        bool dec2 = (i == 4 || i == 7 || i == 38 || (i >= 18 && i <= 21));
        if (a.NumDecPlacesToShow != (dec2 ? 2 : 0))
            ok = false;
        if (a.LowerIsBetter != (i >= 18 && i <= 23))
            ok = false;
    }
    if (ok && std::strcmp(sb.ActivityInfo[0].Name.text, "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_MURDERS") != 0)
        ok = false;
    if (ok && std::strcmp(sb.ActivityInfo[24].UnitOfMeasurement.text, "TEXT_GUI_SCOREBOARD_UNITS_GOLD") != 0)
        ok = false;
    if (!ok) { std::printf("00562710_TEST FAIL\n"); return 1; }
    std::printf("00562710_TEST PASS\n");
    return 0;
}