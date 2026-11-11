// NPlayerGui::CDrawAbilityButtons::DrawAbilityList(CEnginePrimitivePrepareInfo*, float) -- draw the hero's ability grid: for a live hero with the abilities component (0x6f), scale by 0.75, fetch the ability def list, and starting at the row after the scroll row draw every row that holds at least one ability (each column at its slot offset), then wrap round to the rows before the scroll row, numbering the drawn rows consecutively from 0
// Leaf: DrawAbilityList
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064DB2B, length 546 bytes (frame pointer /Os TU, `ret 8`).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::PeekPControlledCreature 0x00487DD0, IS_THING_ALIVE 0x0043DBF0
//   (__fastcall, by reference; same-TU stand-in body), virtual slot 5 (slot-offset fill), std::bitset::test 0x00410DE0, LowerBound
//   0x004365B0 (const; same-TU stand-in body), CTCHeroAbilities ability-list fill 0x005F4B30, the ability vector dtor 0x005F9F14,
//   DrawAbilityButton 0x0064AE80, CRT free 0x00BFEA14 (offset vector storage).
// Layout: as the landed CDrawMeleeActionButtonEffects::Update 0x0064BE45 (same TU): PlayerManager +0x08, Columns +0x28 (PDB +0x2c,
//   retail 4 smaller); hero thing presence bits +0x20, interface map +0x44; abilities component scroll index +0x144. Modelled locally.
// Shape: /Os via optimize("y",off)+("s",on); the GetInterface out-param idiom with the null component tested in the same condition;
//   the drawn-row counter is initialised as `firstRow - 1` (initialising it from the scrollRow local swaps two registers).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stdlib.h>
class CTCBase;
namespace std {                                                       // STLport 4.6 <bitset>: _Base_bitset<9> words + bitset<274>
template <unsigned int _Nb> class bitset { public:
    // the row lays the 9 words (36 bytes) out itself: the stand-in was an empty class
    bool test(unsigned int __pos) const;                            // 0x00410DE0 ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
};
}
// agentNF16: the stand-in CInterfaceMap struct is the explicit specialisation of CVectorMap<ETCInterfaceType, CTCBase*> (the landed
//   0x0074EE90 idiom); 0x0040F020 is its LowerBound (NF14 / established spelling).
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    const value_type* LowerBound(const ETCInterfaceType& key) const;   // 0x004365B0
    value_type* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
#pragma auto_inline(off)
const CInterfaceMap::value_type* CInterfaceMap::LowerBound(const ETCInterfaceType& key) const
{
    const value_type* it = _M_start;
    int len = int(_M_finish - _M_start);
    while (len > 0) {
        int half = len / 2;
        const value_type* middle = it + half;
        if (middle->first < key) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad0[0x20];
    mutable std::bitset<274> m_present;               // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    template <class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long type) const
    {
        if (m_present.test(type)) {
            ETCInterfaceType key = (ETCInterfaceType)type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(key);
            *ppOut = (T*)((it == map->_M_finish || type < it->first) ? map->_M_finish : it)->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0
bool IsThingAliveImpl(const void* thing);
#pragma auto_inline(off)
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && IsThingAliveImpl(thing);
}
#pragma auto_inline(on)
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;               // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
struct LongVector {
    long *Start, *Finish, *End;
    LongVector() : Start(0), Finish(0), End(0) {}
    ~LongVector() { if (Start) free(Start); }
    unsigned int size() const { return Finish - Start; }
};
class CHeroAbilityDef;
struct CAbilityDefPointer { CHeroAbilityDef* Object; };
struct AbilityVector {                             // vector<CDefPointer<const CHeroAbilityDef> >
    CAbilityDefPointer *Start, *Finish, *End;
    AbilityVector() : Start(0), Finish(0), End(0) {}
    ~AbilityVector();                              // 0x005F9F14
    unsigned int size() const { return Finish - Start; }
};
struct CTCHeroAbilities {
    unsigned char _pad00[0x144];
    long          ScrollIndex;                     // +0x144
    void GetAbilities(AbilityVector& out);         // 0x005F4B30
};
class CEnginePrimitivePrepareInfo;

namespace NPlayerGui {
class CDrawAbilityButtons { public:
    void*           PlayerGui;              // +0x04
    CPlayerManager* PlayerManager;          // +0x08
    void*           World;                  // +0x0c
    unsigned char   _pad10[0x28 - 0x10];
    long            Columns;                // +0x28
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void GetSlotOffsets(LongVector& offsets);   // slot 5
    void DrawAbilityButton(CEnginePrimitivePrepareInfo* info, AbilityVector* abilities, long slot, long index, long row,
                           float scale);   // 0x0064AE80
    void DrawAbilityList(CEnginePrimitivePrepareInfo* info, float scale);
};

void CDrawAbilityButtons::DrawAbilityList(CEnginePrimitivePrepareInfo* info, float scale)
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    if (!IS_THING_ALIVE(hero))
        return;
    LongVector offsets;
    GetSlotOffsets(offsets);
    CTCHeroAbilities* tc;
    if (hero->GetInterface(&tc, 0x6f) && tc) {
        scale *= 0.75f;
        AbilityVector abilities;
        tc->GetAbilities(abilities);
        long firstRow = tc->ScrollIndex / Columns + 1;
        long rows = ((long)abilities.size() + 2) / Columns * Columns / Columns;
        long scrollRow = firstRow - 1;
        long drawn = firstRow - 1;
        long row;
        for (row = firstRow; row < rows; ++row) {
            bool any = false;
            long col;
            for (col = 0; col < Columns; ++col) {
                unsigned int index = offsets.Start[col] + row * Columns - 1;
                if (index < abilities.size() && abilities.Start[index].Object != 0) {
                    any = true;
                    break;
                }
            }
            if (any) {
                ++drawn;
                for (col = 0; col < Columns; ++col)
                    DrawAbilityButton(info, &abilities, col, offsets.Start[col] + row * Columns - 1, drawn - firstRow, scale);
            }
        }
        for (row = 0; row < scrollRow; ++row) {
            bool any = false;
            long col;
            for (col = 0; col < Columns; ++col) {
                unsigned int index = offsets.Start[col] + row * Columns - 1;
                if (index < abilities.size() && abilities.Start[index].Object != 0) {
                    any = true;
                    break;
                }
            }
            if (any) {
                ++drawn;
                for (col = 0; col < Columns; ++col)
                    DrawAbilityButton(info, &abilities, col, offsets.Start[col] + row * Columns - 1, drawn - firstRow, scale);
            }
        }
    }
}
}