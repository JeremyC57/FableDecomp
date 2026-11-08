// NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar::Update() -- while the bar is updatable, stop updating it once the living hero lets go of the button held for its mini-game's expression (pick-lock or pick-pocket); always reports success
// Naming wave 18: the interface-presence stand-in CTCBitset is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064A568, length 209 bytes, plain `ret` (bool true).
// Callees: CPlayerManager::GetMainPlayer 0x00449970 (CMainGameComponent::Get() 0x013B86A0 +0x1c), CPlayer::GetPControlledCreature
// 0x00487DC0, IS_THING_ALIVE 0x0043DBF0 (`__fastcall`, by reference; a separate instantiation from the CThing* one at
// 0x0043DBD0; its T is unproven -- CThingPlayerCreature* is assumed, the hero pointer reaches it through a converted
// temporary in the frame so the thing stays in esi), std::bitset::test 0x00410DE0 (TC type 0x11,
// the inventory), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0, CCharString(const char*, long)
// 0x0099EBF0 ("EXPRESSION_PICKLOCK" 0x0124F520, "EXPRESSION_PICKPOCKET" 0x0125A47C), CTCInventory::
// IsButtonHeldForExpression(const CCharString&) 0x00582A16, ~CCharString 0x0099EAE0.
// Layout: retail CQuestInfoElementMiniGameBar (PDB -4): Type +0x1c (EMiniGameType: 3 pick-pocket, 4 pick-lock),
// Updatable +0x2c.
// Shape: a size-optimised frame-pointer TU (`optimize("y", off)` + `("s", on)`: `push 11h ; pop ebx`, `leave`); the TC
// lookup is the out-parameter GetInterface template (out-of-line bitset test, the type held in ebx for the test, the key
// word and the compare); each case tests `IsButtonHeld("...") ? false : true` on an implicit CCharString conversion
// temporary (re-addressed with `lea`) and VC7.1 cross-jumps the identical tails (the 8-bit `neg/sbb/inc`, the string
// dtor, the flag clear); case 4 is laid out first.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> CTCEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                          // CThing::Components: a sorted vector of pairs (12 bytes)
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    const CTCEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;
namespace std {                                                       // STLport 4.6 <bitset>: _Base_bitset<9> words + bitset<274>
template <unsigned int _Nb> class bitset { public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(unsigned int __pos) const;                            // 0x00410DE0 ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
};
}
class CThing { public:                                          // retail CThing (local model)
    unsigned char _pad00[0x20];
    std::bitset<274>     UsingComponents;                       // +0x20
    CTCMap        Components;                            // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long type) const
    {
        if (UsingComponents.test(type)) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            const CTCMap* map = &Components;
            const CTCEntry* it = map->LowerBound(key);
            if (it == map->_M_finish || type < it->first)
                it = map->_M_finish;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CThingPlayerCreature;
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)   // 0x0043DBF0 (T = const CThingPlayerCreature*)
{
    return thing != 0 && !(reinterpret_cast<const unsigned char*>(thing)[0x91] & 1);   // CThing dead bit +0x91
}

class CTCInventory { public:                                    // TC type 0x11
    bool IsButtonHeldForExpression(const CCharString& expression);  // 0x00582A16
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                        // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                       // +0x1c
};

struct CQuestInfoElementMiniGameBar {
    unsigned char _base[0x1c];                           // CQuestInfoElementBase (vptr ...)
    long          Type;                                  // +0x1c EMiniGameType
    float         Percentage;                            // +0x20
    unsigned long Colour;                                // +0x24 CRGBColour
    unsigned long FadeColour;                            // +0x28 CRGBColour
    bool          Updatable;                             // +0x2c
    bool Update();
};

bool CQuestInfoElementMiniGameBar::Update()
{
    if (Updatable) {
        const CThingPlayerCreature* hero = CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature();
        CTCInventory* inventory;
        if (IS_THING_ALIVE(hero) && hero->GetInterface(&inventory, 0x11)) {
            switch (Type) {
            case 3:
                if (inventory->IsButtonHeldForExpression("EXPRESSION_PICKPOCKET") ? false : true)
                    Updatable = false;
                break;
            case 4:
                if (inventory->IsButtonHeldForExpression("EXPRESSION_PICKLOCK") ? false : true)
                    Updatable = false;
                break;
            }
        }
    }
    return true;
}