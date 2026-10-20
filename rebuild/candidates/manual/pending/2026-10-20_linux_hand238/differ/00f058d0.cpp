// Grade: DIFFER(280v308) -- retail materialises each block compare (neg/sbb/inc) into a frame byte [esp+0xf] (8-byte frame) before the jne; ours branches on test eax,eax with a 4-byte frame
// CQ_AmbushTradersScript::WatchForRegionChange() -- script thread: wait for each region load; on Gibbet Woods, the Deserted Farm or Bowerstone Jail refresh the spies' allegiances
// Leaf: ?WatchForRegionChange@CQ_AmbushTradersScript@@
#include <string.h>
#pragma auto_inline(off)
long __fastcall Compare(const char* a, const char* b)       // 0x00411570 (the landed body, stand-in)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const { return Compare(PData, text) == 0; }   // 0x004115A0 (inlined here)
};
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    ~CCharString();                                         // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
};
struct CGameScriptInterface {                   // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) CGameScriptInterface::NewScriptFrame @ 0x006E7410
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual bool MsgOnRegionLoaded(CCharString& region) const;   // slot 22 (+0x58) CGameScriptInterface::MsgOnRegionLoaded @ 0x008933F0
};
struct CScriptBase {                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
struct CQ_AmbushTradersScript : CQuestScriptBase {
    void RefreshSpyAllegiences();                           // 0x00F05A10
    void WatchForRegionChange();
};
void CQ_AmbushTradersScript::WatchForRegionChange()
{
    CCharString region;
    while (!IsActiveThreadTerminating()) {
        while (!Game->MsgOnRegionLoaded(region)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        CCharStringData* data = region.PStringData;
        bool spyRegion;
        if (data == 0)
            spyRegion = strcmp("GibbetWoods", "") == 0 || strcmp("DesertedFarm", "") == 0 || strcmp("BowerstoneJail", "") == 0;
        else
            spyRegion = data->Data == "GibbetWoods" || data->Data == "DesertedFarm" || data->Data == "BowerstoneJail";
        if (spyRegion) {
            if (IsActiveThreadTerminating())
                return;
            RefreshSpyAllegiences();
        }
        Game->NewScriptFrame();
    }
}
