// CTCHeroOnlineScoreboard::UpdateScoreForActivity(EOnlineScoreboardActivity, float, bool) -- records the hero's local best score for an activity (first score, or a better one; `lowerIsBetter` picks the direction)
// Leaf: UpdateScoreForActivity
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055F3B0, 206 bytes, `ret 0Ch`.
// Callees: map<EOnlineScoreboardActivity,float>::find(const key&) @ 0x00566F50 (iterator by hidden pointer),
//          map::operator[](const key&) @ 0x005692B0 (float&), GFIntToCharString(long) @ 0x0099F830 (__fastcall,
//          hidden result in ecx), CCharString::CCharString(const char*, long) @ 0x0099EBF0, operator+(const CCharString&,
//          const CCharString&) @ 0x0099F570 (__fastcall), ~CCharString @ 0x0099EAE0 (x3); "New best score in activity ".
// Shape: the find result (a block-scoped iterator sharing its frame word with the later number temporary) is compared with the map's header word directly (`cmp edx,[esi]`); a first score is stored
// and returns at once; otherwise `!(score < best)` (`test ah,5 ; jp`) or `!(score > best)` (`test ah,41h ; jne`)
// leaves, and the improvement builds a "New best score" message (its log call compiled out of the retail build:
// the number temporary first, the literal in the dead flag slot, both destroyed before the store) that dies after
// the new best is stored.
// Layout: retail CTCHeroOnlineScoreboard LocalScoreboard map at +0x1C (PDB +0x20; 12-byte STLport maps).
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
};
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570

enum EOnlineScoreboardActivity { ONLINE_SCOREBOARD_ACTIVITY_0 };

struct SScoreNode;
struct CScoreMap {                                   // std::map<EOnlineScoreboardActivity, float> (12 bytes)
    struct iterator {
        SScoreNode* _M_node;
        iterator() {}
    };
    SScoreNode*   _M_header;
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    iterator find(const EOnlineScoreboardActivity& key);           // 0x00566F50
    float& operator[](const EOnlineScoreboardActivity& key);       // 0x005692B0
};

struct CTCHeroOnlineScoreboard {
    void*         __vftable;                         // +0x00
    unsigned char _pad_0x04[0x18];
    CScoreMap     LocalScoreboard;                   // +0x1C (PDB +0x20)
    void UpdateScoreForActivity(EOnlineScoreboardActivity activity, float score, bool lowerIsBetter);
};

void CTCHeroOnlineScoreboard::UpdateScoreForActivity(EOnlineScoreboardActivity activity, float score, bool lowerIsBetter)
{
    {
        CScoreMap::iterator it = LocalScoreboard.find(activity);
        if (it._M_node == LocalScoreboard._M_header) {
            LocalScoreboard[activity] = score;
            return;
        }
    }
    if (lowerIsBetter) {
        if (!(score < LocalScoreboard[activity]))
            return;
    } else if (!(score > LocalScoreboard[activity])) {
        return;
    }
    CCharString message = CCharString("New best score in activity ", -1) + GFIntToCharString(activity);
    LocalScoreboard[activity] = score;
}
