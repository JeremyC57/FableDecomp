class CCharString
{
public:
    CCharString(const CCharString& other);
    ~CCharString();

private:
    void* storage_;
};

struct LipSyncEntry;

class LipSyncEntryArray
{
public:
    LipSyncEntry* begin;
    LipSyncEntry* end;
    void EraseRange(LipSyncEntry* first, LipSyncEntry* last);
};

class GossipMap
{
public:
    LipSyncEntryArray& operator[](const CCharString& key);
};

extern GossipMap DAT_013bae44;

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::ClearGossip(CCharString gossipName)
{
    CCharString localKey(gossipName);
    LipSyncEntryArray& entries = DAT_013bae44[localKey];
    entries.EraseRange(entries.begin, entries.end);
}