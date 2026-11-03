P="rebuild/src/compiled/00/52/CThingManager_LoadHeroStateScript_00520570.cpp"
EDITS=[
(P,"""    CCharString ReadAsStringUntilString_009ba3e0(const CCharString& terminator);            // 0x009BA3E0
    CCharString ReadAsStringUntilStringInclusive(const CCharString& terminator);   // 0x009BA330""",
"""    CCharString ReadAsStringUntilPastString(const CCharString& terminator);       // 0x009BA3E0 (Ego_r / FableWin)
    CCharString ReadAsStringUntilString(const CCharString& terminator);           // 0x009BA330 (Ego_r / FableWin)"""),
(P,"""    CPersistContext(CStringParser* parser, void (*errorCallback)(CCharString*), bool reportErrors,
                    CDefinitionManager* definitions, CSymbolMap* symbols, CCharString* sourceText);   // 0x009BADD0""",
"""    CPersistContext(CStringParser& parser, void (__fastcall* errorCallback)(const CCharString&), bool reportErrors,
                    const CDefinitionManager* definitions, const CSymbolMap* symbols, CCharString* sourceText);   // 0x009BADD0"""),
(P,"""public:
    CThing* LoadSingleThing(CPersistContext& context, long mode);   // 0x0051FD80
    void LoadHeroStateScript(CStringParser& parser);""","""private:
    CThing* LoadSingleThing(CPersistContext& context, long mode);   // 0x0051FD80 (Ego_r / FableWin: private, AAE)
public:
    void LoadHeroStateScript(CStringParser& parser);"""),
(P,"    parser.ReadAsStringUntilString_009ba3e0(g_strNewThing);","    parser.ReadAsStringUntilPastString(g_strNewThing);"),
(P,"parser.ReadAsStringUntilStringInclusive(g_strEndThingLevel);","parser.ReadAsStringUntilString(g_strEndThingLevel);"),
(P,"    CPersistContext context(&heroParser, 0, true, 0, 0, 0);","    CPersistContext context(heroParser, 0, true, 0, 0, 0);"),
]
