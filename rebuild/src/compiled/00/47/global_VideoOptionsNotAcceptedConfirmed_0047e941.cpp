// _global::VideoOptionsNotAcceptedConfirmed() -- message-box callback: tell the controlled character's in-game menu (interface 0xC6) that the new video options were declined (revert)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047E941, 109 bytes, plain `ret`.
// Callees: CPlayerManager::GetMainPlayer() const @ 0x00449960, CPlayer::GetPlayerCharacter @ 0x00487B70,
// IS_THING_ALIVE<CThing*> @ 0x0040DCE0, std::bitset<>::test @ 0x00410DE0, CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020, CTCInGameMenu::ConfirmedVideoOptionsNotAccepted @ 0x0047DA50.
// Global: g_pMainGameComponent @ 0x013B86A0 (player manager at +0x1C).
// Shape: Ego_r.pdb `?VideoOptionsNotAcceptedConfirmed@@YIXXZ` (a `__fastcall` free function, registered by address
// at 0x0047F374); byte-identical to VideoOptionsChangedConfirmed 0x0047D9E3 except the final callee.
// Label correction: the catalog calls this row "VideoOptionsChangedConfirmed" too (propagated); the callee
// 0x0047DA50 (landed CTCInGameMenu::ConfirmedVideoOptionsNotAccepted) makes it the not-accepted twin. The player character goes through the frame word for IS_THING_ALIVE's `CThing* const&`; the
// inlined GetInterface(0xC6) keeps the type in edi, its result in ebx (zeroed before the presence test, so an
// absent menu calls through null) and homes the LowerBound key in the same frame word; the lookup is the
// `(it == last || type < it->type) ? last : it` select. Size TU with a frame pointer, as the landed
// sibling SavedConfirmed 0x0047DB73.
// Leaf: VideoOptionsNotAcceptedConfirmed
#pragma optimize("s", on)

class CTCBase;
struct CPresenceBits { bool test(long kind); };            // std::bitset<>::test 0x00410DE0
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20 std::bitset of interface types
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
bool __fastcall IS_THING_ALIVE(CThing* const& thing);      // 0x0040DCE0

// Same-TU stand-ins (as the landed 0x005BE119): with these bodies visible VC7.1 sees that neither the thing's
// address nor the lookup key escapes, keeps the thing in esi and lets the key share the thing's frame word.
bool IsAlive(const CThing* thing);
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* lo = first;
    SEntry* hi = last;
    while (lo < hi) {
        SEntry* mid = lo + (hi - lo) / 2;
        if (mid->type < *type)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}
bool __fastcall IS_THING_ALIVE(CThing* const& thing) { return thing != 0 && IsAlive(thing); }
#pragma auto_inline(on)

class CTCInGameMenu {
public:
    void ConfirmedVideoOptionsNotAccepted();                // 0x0047DA50
};
class CPlayer { public: CThing* GetPlayerCharacter(); };           // 0x00487B70
class CPlayerManager { public: CPlayer* GetMainPlayer() const; };  // 0x00449960
class CMainGameComponent { public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* m_pPlayerManager;                       // +0x1C
};
extern CMainGameComponent* g_pMainGameComponent;           // 0x013B86A0

void __fastcall VideoOptionsNotAcceptedConfirmed()
{
    CThing* thing = g_pMainGameComponent->m_pPlayerManager->GetMainPlayer()->GetPlayerCharacter();
    if (IS_THING_ALIVE(thing)) {
        CTCInGameMenu* menu = 0;
        thing->GetInterface(&menu, 0xC6);
        menu->ConfirmedVideoOptionsNotAccepted();
    }
}