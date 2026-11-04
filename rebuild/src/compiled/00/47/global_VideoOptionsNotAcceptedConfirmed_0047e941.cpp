// _global::VideoOptionsNotAcceptedConfirmed() -- message-box callback: tell the controlled character's in-game menu (interface 0xC6) that the new video options were declined (revert)
// Naming wave 35c (queue land0411j 0x00487B70): calls 0x00487B70 as CPlayer::GetPPlayerCharacterThing() const (Ego_r.pdb ?GetPPlayerCharacterThing@CPlayer@@QBEPAVCThing@@XZ; was GetPlayerCharacter).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047E941, 109 bytes, plain `ret`.
// Callees: CPlayerManager::GetMainPlayer() const @ 0x00449960, CPlayer::GetPPlayerCharacterThing() const @ 0x00487B70,
// IS_THING_ALIVE<CThing*> @ 0x0040DCE0, std::bitset<>::test @ 0x00410DE0, CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020, CTCInGameMenu::ConfirmedVideoOptionsNotAccepted @ 0x0047DA50.
// Global: CMainGameComponent::Get() @ 0x013B86A0 (player manager at +0x1C).
// Shape: Ego_r.pdb `?VideoOptionsNotAcceptedConfirmed@@YIXXZ` (a `__fastcall` free function, registered by address
// at 0x0047F374); byte-identical to VideoOptionsChangedConfirmed 0x0047D9E3 except the final callee.
// Label correction: the catalog calls this row "VideoOptionsChangedConfirmed" too (propagated); the callee
// 0x0047DA50 (landed CTCInGameMenu::ConfirmedVideoOptionsNotAccepted) makes it the not-accepted twin. The player character goes through the frame word for IS_THING_ALIVE's `CThing* const&`; the
// inlined GetInterface(0xC6) keeps the type in edi, its result in ebx (zeroed before the presence test, so an
// absent menu calls through null) and homes the LowerBound key in the same frame word; the lookup is the
// `(it == last || type < it->type) ? last : it` select. Size TU with a frame pointer, as the landed
// sibling SavedConfirmed 0x0047DB73.
// Leaf: VideoOptionsNotAcceptedConfirmed
// Plan phase 3 (2026-11-04), CVectorMap wave (stand-in map): the local stand-in map class (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad21[0x44 - 0x21] removed. Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
#pragma optimize("s", on)

class CTCBase;
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    std::bitset<274> present;                                  // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
bool __fastcall IS_THING_ALIVE(CThing* const& thing);      // 0x0040DCE0

// Same-TU stand-ins (as the landed 0x005BE119): with these bodies visible VC7.1 sees that neither the thing's
// address nor the lookup key escapes, keeps the thing in esi and lets the key share the thing's frame word.
bool IsAlive(const CThing* thing);
bool __fastcall IS_THING_ALIVE(CThing* const& thing) { return thing != 0 && IsAlive(thing); }
#pragma auto_inline(on)

class CTCInGameMenu {
public:
    void ConfirmedVideoOptionsNotAccepted();                // 0x0047DA50
};
class CPlayer { public: CThing* GetPPlayerCharacterThing() const; };           // 0x00487B70
class CPlayerManager { public: CPlayer* GetMainPlayer() const; };  // 0x00449960
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* m_pPlayerManager;                       // +0x1C
};

void __fastcall VideoOptionsNotAcceptedConfirmed()
{
    CThing* thing = CMainGameComponent::Get()->m_pPlayerManager->GetMainPlayer()->GetPPlayerCharacterThing();
    if (IS_THING_ALIVE(thing)) {
        CTCInGameMenu* menu = 0;
        thing->GetInterface(&menu, 0xC6);
        menu->ConfirmedVideoOptionsNotAccepted();
    }
}