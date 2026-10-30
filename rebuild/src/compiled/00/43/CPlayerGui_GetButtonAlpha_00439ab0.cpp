// CPlayerGui::GetButtonAlpha(long button, bool applyButtonAlpha, bool special, bool contextSensitive) -- alpha of a HUD button: the normal/special swap fade (|scale| or 1 - |scale|), else the player character's context-sensitive inventory fade, optionally times the button's own ability / quick-access alpha
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00439AB0, length 242 bytes, `ret 0x10`.
// Callees: CPlayerManager::GetMainPlayer 0x00449970 (PlayerManager +0x08), CPlayer::GetPlayerCharacter 0x00487B70
// (`PlayerCharacter.GetPItem()`; propagated label "_Cons_val<...>"), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// const 0x004365B0 (TC 0x11 = word 0 bit 17, the inventory), CTCInventory::GetContextSensitiveFadeAlpha(long)
// 0x00581D2A. Float 1.0f @ 0x0122DED8.
// Layout (retail; engine/CPlayerGui.h carries the PDB offsets 8-0x14 bytes higher): AbilityButtonsAlpha
// (vector<float>) +0x260, QuickAccessButtonsAlpha +0x26c, AbilityButtonScale +0x278, NormalToSpecial +0x27c,
// SpecialToNormal +0x27d.
// Shape: the creature path's fall-back leaves through `goto done` to the shared final `return alpha` -- alpha is
// then a merged value live across the calls, so VC spills it once at entry (`fst [esp]`, the `push ecx` word) and
// reloads it there; a direct `return alpha` there is constant-folded to a fresh `fld 1.0`, and assigning the fade
// result to alpha moves the reload block. The tail multiply is shared by the fade and plain paths.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> CTCEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    const CTCEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase* Find(long type) const
    {
        const CTCEntry* it = LowerBound((ETCInterfaceType)type);
        if (it == _M_finish || type < it->first)
            it = _M_finish;
        return it->second;
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                  // +0x20
    unsigned char _pad40[4];
    CTCMap        Components;                          // +0x44
    unsigned char _pad50[0x41];
    unsigned char Dead : 1;                            // +0x91 bit 0
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) const { return Components.Find(type); }
};
class CTCInventory { public:                                  // TC type 0x11
    float GetContextSensitiveFadeAlpha(long button);   // 0x00581D2A
};
class CPlayer { public:
    CThing* GetPlayerCharacter();                      // 0x00487B70
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"

template <class T> struct vector {                     // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](long n) { return *(_M_start + n); }
};

extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)

class CPlayerGui { public:
    unsigned char   _pad000[8];
    CPlayerManager* PlayerManager;                     // +0x08
    unsigned char   _pad00c[0x254];
    vector<float>   AbilityButtonsAlpha;               // +0x260
    vector<float>   QuickAccessButtonsAlpha;           // +0x26c
    float           AbilityButtonScale;                // +0x278
    bool            NormalToSpecial;                   // +0x27c
    bool            SpecialToNormal;                   // +0x27d
    float GetButtonAlpha(long button, bool applyButtonAlpha, bool special, bool contextSensitive);
};

float CPlayerGui::GetButtonAlpha(long button, bool applyButtonAlpha, bool special, bool contextSensitive)
{
    float alpha = 1.0f;
    if (NormalToSpecial || SpecialToNormal) {
        alpha = (float)fabs(AbilityButtonScale);
        if (!special)
            alpha = 1.0f - alpha;
    } else if (contextSensitive && !applyButtonAlpha) {
        CThing* character = PlayerManager->GetMainPlayer().GetPlayerCharacter();
        if (character && !character->Dead && character->HasTC(0x11))
            return ((CTCInventory*)character->GetTC(0x11))->GetContextSensitiveFadeAlpha(button);
        goto done;
    }
    if (applyButtonAlpha) {
        if (special)
            return alpha * AbilityButtonsAlpha[button];
        return alpha * QuickAccessButtonsAlpha[button];
    }
done:
    return alpha;
}