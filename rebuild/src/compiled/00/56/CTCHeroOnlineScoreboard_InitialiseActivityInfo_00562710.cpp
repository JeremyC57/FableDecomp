// CTCHeroOnlineScoreboard::InitialiseActivityInfo() -- sizes ActivityInfo to the 39 scoreboard activities and fills each one's Name / Description / UnitOfMeasurement text-bank symbols, then the decimal-place counts and the lower-is-better flags
// Leaf: InitialiseActivityInfo
// Retail: Fable.exe @ 0x00562710, 1785 bytes, `ret`. Ego_r.pdb `?InitialiseActivityInfo@CTCHeroOnlineScoreboard@@AAEXXZ`.
// Callees: vector<CActivity>::resize(size_type) 0x0056A230 (the out-of-line resize(n, CActivity()) wrapper),
// CCharString::operator=(const char*) 0x0099EFE0.
// Layout: retail ActivityInfo +0x28 (12-byte STLport vector; the PDB view is 16 bytes at +0x30 because the two maps ahead
// of it are 16 there), CActivity 16 bytes (Name, Description, UnitOfMeasurement, NumDecPlacesToShow +0x0c, LowerIsBetter
// +0x0d), as the landed CreateStringFromMyScores 0x0055F480.
// Shape: one statement per field in retail order; operator[] is the inline _M_start[n], so every store reloads _M_start
// through the cached &ActivityInfo (esi). The literals reused across activities are pooled by the linker.
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString& operator=(const char* text);        // 0x0099EFE0
};
struct CActivity {                                   // CTCHeroOnlineScoreboard::CActivity (16 bytes)
    CCharString   Name;                              // +0x00
    CCharString   Description;                       // +0x04
    CCharString   UnitOfMeasurement;                 // +0x08
    unsigned char NumDecPlacesToShow;                // +0x0c
    bool          LowerIsBetter;                     // +0x0d
};
struct CActivityVector {
    CActivity* _M_start;
    CActivity* _M_finish;
    CActivity* _M_end_of_storage;
    void resize(unsigned int n);                     // 0x0056A230
    CActivity& operator[](unsigned int n) { return _M_start[n]; }
};

class CTCHeroOnlineScoreboard {
public:
    void*           __vftable;                       // +0x00
    unsigned char   _pad04[0x28 - 0x04];
    CActivityVector ActivityInfo;                    // +0x28
    void InitialiseActivityInfo();
};

void CTCHeroOnlineScoreboard::InitialiseActivityInfo()
{
    ActivityInfo.resize(39);
    ActivityInfo[0].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_MURDERS";   // 0x0124DADC
    ActivityInfo[1].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_DECAPITATIONS";   // 0x0124DAAC
    ActivityInfo[2].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_COMBATMULT";   // 0x0124DA7C
    ActivityInfo[3].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_EXPERIENCE";   // 0x0124DA4C
    ActivityInfo[4].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_CHICKEN_KICK_DIST";   // 0x0124DA18
    ActivityInfo[5].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_WEALTH";   // 0x0124D9EC
    ActivityInfo[6].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SEX";   // 0x0124D9C4
    ActivityInfo[7].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_BIGGEST_FISH";   // 0x0124D994
    ActivityInfo[8].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_ARCHERY_SCORE";   // 0x0124D964
    ActivityInfo[9].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SPOUSES";   // 0x0124D938
    ActivityInfo[10].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_TEMPLE_CONTRIBUTION";   // 0x0124D900
    ActivityInfo[11].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_ADMIRERS";   // 0x0124D8D4
    ActivityInfo[12].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_WEDDINGS";   // 0x0124D8A8
    ActivityInfo[13].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_DIVORCES";   // 0x0124D87C
    ActivityInfo[14].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_TIMES_WIDOWED";   // 0x0124D84C
    ActivityInfo[15].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_PUKINGS";   // 0x0124D820
    ActivityInfo[16].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_RENT_COLLECTED";   // 0x0124D7EC
    ActivityInfo[17].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_CHICKEN_KICK";   // 0x0124D7B0
    ActivityInfo[18].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_PAIRS";   // 0x0124D77C
    ActivityInfo[19].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_CARD_SORTING";   // 0x0124D740
    ActivityInfo[20].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_GUESS_ADDITION";   // 0x0124D704
    ActivityInfo[21].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_ORACLE_MINIGAME";   // 0x0124D6C4
    ActivityInfo[22].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_COIN_GOLF_OV";   // 0x0124D688
    ActivityInfo[23].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_COIN_GOLF_SS";   // 0x0124D64C
    ActivityInfo[24].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_BLACKJACK";   // 0x0124D614
    ActivityInfo[25].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_HIGH_SCORE_HAPENNY";   // 0x0124D5DC
    ActivityInfo[26].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SUCCESSIVE_WINS_PAIRS";   // 0x0124D5A4
    ActivityInfo[27].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SUCCESSIVE_WINS_CARD_SORTING";   // 0x0124D564
    ActivityInfo[28].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SUCCESSIVE_WINS_GUESS_ADDITION";   // 0x0124D520
    ActivityInfo[29].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SUCCESSIVE_WINS_ORACLE_MINIGAME";   // 0x0124D4D8
    ActivityInfo[30].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SUCCESSIVE_WINS_COIN_GOLF_OV";   // 0x0124D494
    ActivityInfo[31].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SUCCESSIVE_WINS_COIN_GOLF_SS";   // 0x0124D454
    ActivityInfo[32].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SUCCESSIVE_WINS_BLACKJACK";   // 0x0124D418
    ActivityInfo[33].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_SUCCESSIVE_WINS_HAPENNY";   // 0x0124D3DC
    ActivityInfo[34].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_CRIMES_REPORTED";   // 0x0124D3A8
    ActivityInfo[35].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_TOTAL_FINES";   // 0x0124D378
    ActivityInfo[36].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_TOTAL_FINES_PAID";   // 0x0124D344
    ActivityInfo[37].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_BRIBES";   // 0x0124D318
    ActivityInfo[38].Name = "TEXT_GUI_SCOREBOARD_ACTIVITY_NAME_BIGGEST_FISH_IN_COMPETITION";   // 0x0124D2D8
    ActivityInfo[0].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_MURDERS";   // 0x0124D2A4
    ActivityInfo[1].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_DECAPITATIONS";   // 0x0124D26C
    ActivityInfo[2].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_COMBATMULT";   // 0x0124D238
    ActivityInfo[3].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_EXPERIENCE";   // 0x0124D204
    ActivityInfo[4].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_CHICKEN_KICK_DIST";   // 0x0124D1C8
    ActivityInfo[5].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_WEALTH";   // 0x0124D198
    ActivityInfo[6].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SEX";   // 0x0124D168
    ActivityInfo[7].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_BIGGEST_FISH";   // 0x0124D130
    ActivityInfo[8].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_ARCHERY_SCORE";   // 0x0124D0F8
    ActivityInfo[9].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SPOUSES";   // 0x0124D0C4
    ActivityInfo[10].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_TEMPLE_CONTRIBUTION";   // 0x0124D084
    ActivityInfo[11].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_ADMIRERS";   // 0x0124D050
    ActivityInfo[12].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_WEDDINGS";   // 0x0124D01C
    ActivityInfo[13].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_DIVORCES";   // 0x0124CFE8
    ActivityInfo[14].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_TIMES_WIDOWED";   // 0x0124CFB0
    ActivityInfo[15].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_PUKINGS";   // 0x0124CF7C
    ActivityInfo[16].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_RENT_COLLECTED";   // 0x0124CF44
    ActivityInfo[17].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_CHICKEN_KICK";   // 0x0124CF00
    ActivityInfo[18].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_PAIRS";   // 0x0124CEC4
    ActivityInfo[19].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_CARD_SORTING";   // 0x0124CE80
    ActivityInfo[20].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_GUESS_THE_ADDITION";   // 0x0124CE38
    ActivityInfo[21].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_ORACLE_MINIGAME";   // 0x0124CDF0
    ActivityInfo[22].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_COIN_GOLF_OV";   // 0x0124CDA8
    ActivityInfo[23].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_COIN_GOLF_SS";   // 0x0124CD60
    ActivityInfo[24].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_BLACKJACK";   // 0x0124CD1C
    ActivityInfo[25].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_HIGH_SCORE_HAPENNY";   // 0x0124CCE0
    ActivityInfo[26].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SUCCESSIVE_WINS_PAIRS";   // 0x0124CCA0
    ActivityInfo[27].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SUCCESSIVE_WINS_CARD_SORTING";   // 0x0124CC58
    ActivityInfo[28].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SUCCESSIVE_WINS_GUESS_THE_ADDITION";   // 0x0124CC08
    ActivityInfo[29].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SUCCESSIVE_WINS_ORACLE_MINIGAME";   // 0x0124CBB8
    ActivityInfo[30].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SUCCESSIVE_WINS_COIN_GOLF_OV";   // 0x0124CB70
    ActivityInfo[31].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SUCCESSIVE_WINS_COIN_GOLF_SS";   // 0x0124CB28
    ActivityInfo[32].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SUCCESSIVE_WINS_BLACKJACK";   // 0x0124CAE0
    ActivityInfo[33].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_SUCCESSIVE_WINS_HAPENNY";   // 0x0124CA98
    ActivityInfo[34].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_CRIMES_REPORTED";   // 0x0124CA5C
    ActivityInfo[35].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_TOTAL_FINES";   // 0x0124CA24
    ActivityInfo[36].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_TOTAL_FINES_PAID";   // 0x0124C9E8
    ActivityInfo[37].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_BRIBES";   // 0x0124C9B8
    ActivityInfo[38].Description = "TEXT_GUI_SCOREBOARD_ACTIVITY_DESCRIPTION_BIGGEST_FISH_IN_COMPETITION";   // 0x0124C970
    ActivityInfo[4].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_METRES";   // 0x0124C948
    ActivityInfo[5].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GOLD";   // 0x0124C928
    ActivityInfo[10].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GOLD";   // 0x0124C928
    ActivityInfo[16].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GOLD";   // 0x0124C928
    ActivityInfo[35].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GOLD";   // 0x0124C928
    ActivityInfo[36].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GOLD";   // 0x0124C928
    ActivityInfo[37].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GOLD";   // 0x0124C928
    ActivityInfo[7].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GRAMS";   // 0x0124C908
    ActivityInfo[38].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GRAMS";   // 0x0124C908
    ActivityInfo[18].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_SECONDS";   // 0x0124C8E4
    ActivityInfo[19].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_SECONDS";   // 0x0124C8E4
    ActivityInfo[20].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_SECONDS";   // 0x0124C8E4
    ActivityInfo[25].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_POINTS";   // 0x0124C8C0
    ActivityInfo[22].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_STROKES";   // 0x0124C89C
    ActivityInfo[23].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_STROKES";   // 0x0124C89C
    ActivityInfo[24].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_GOLD";   // 0x0124C928
    ActivityInfo[17].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_POINTS";   // 0x0124C8C0
    ActivityInfo[8].UnitOfMeasurement = "TEXT_GUI_SCOREBOARD_UNITS_POINTS";   // 0x0124C8C0
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