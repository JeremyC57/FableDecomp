// CQR_EscortTraderScript::GetTraderRandomSpeechEnabled() -- the trader may chatter only when it is not yet in the last region of its route and the quest is in the escorting stages (2 or 3) with the trader not at its destination
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E24260, length 97 bytes.
// Callees: the inlined CCharString equality around Compare(const char*, const char*) @ 0x00411570 (`__fastcall`,
// the landed CScriptBase::OnRegionLoaded model).
// Layout: Ego_r.pdb CQR_EscortTraderScript - 0x14 with retail 12-byte vectors: TraderAtDestination +0x4b, TradeRoute
// +0xc4, RouteDynamic[3] +0xc8, RouteStatic[3] +0xec, PreviousLoadedRegion +0x110, CurrentLoadedRegion +0x114,
// QuestStage +0x11c.
// Shape: `RouteStatic[route].back()` is STLport `*(end() - 1)` (`mov eax,[esi+eax*4] ; sub eax,4 ; mov eax,[eax]`);
// the equality has `this` = CurrentLoadedRegion on the left (`cmp ecx,eax`); the stage test is `== 2 || == 3`. The
// member operator== re-reads PStringData at every use (no cached block locals): VC7.1 then keeps the back() address
// as its own CSE temporary (the unfolded `sub eax,4 ; mov eax,[eax]`), as found by the shared retry advisor.
struct CCharStringData {                       // the shared string block
    const char*   m_pData;                     // +0x00
    unsigned long m_length;                    // +0x04
};
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570

struct CCharString {
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (!PStringData || !other.PStringData)
            return false;
        if (PStringData->m_length != other.PStringData->m_length)
            return false;
        return Compare_00411570(PStringData->m_pData, other.PStringData->m_pData) == 0;
    }
};

struct CCharStringVector {                     // STLport vector<CCharString> (12 bytes)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharString& back() { return *(_M_finish - 1); }
};

struct CScriptBase {                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                              // +0x04 .. +0x40
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    void* GSI;                                              // +0x40
    void* Owner;                                            // +0x44
};

struct CQR_EscortTraderScript : CQuestScriptBase {
    bool              MissionSucceeded;                     // +0x48
    bool              MissionFailed;                        // +0x49
    bool              HiredByTrader;                        // +0x4a
    bool              TraderAtDestination;                  // +0x4b
    unsigned char     _pad4c[0xc4 - 0x4c];                  // +0x4c flags, counters, timers, random speech
    long              TradeRoute;                           // +0xc4
    CCharStringVector RouteDynamic[3];                      // +0xc8
    CCharStringVector RouteStatic[3];                       // +0xec
    CCharString       PreviousLoadedRegion;                 // +0x110
    CCharString       CurrentLoadedRegion;                  // +0x114
    bool              RegionChangeDetected;                 // +0x118
    long              QuestStage;                           // +0x11c
    bool GetTraderRandomSpeechEnabled();
};

bool CQR_EscortTraderScript::GetTraderRandomSpeechEnabled()
{
    if (CurrentLoadedRegion == RouteStatic[TradeRoute].back())
        return false;
    if ((QuestStage == 2 || QuestStage == 3) && !TraderAtDestination)
        return true;
    return false;
}