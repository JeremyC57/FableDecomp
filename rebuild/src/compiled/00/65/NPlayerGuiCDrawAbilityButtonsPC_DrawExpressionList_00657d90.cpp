// NPlayerGui::CDrawAbilityButtonsPC::DrawExpressionList(CEnginePrimitivePrepareInfo*, float) -- draw the hero's expression grid: for a live hero with the expression component, scale by 0.75, fetch the expression list, and starting at the row after the scroll row draw every row that holds at least one expression, then wrap round to the rows before the scroll row, numbering the drawn rows consecutively
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00657D90, length 653 bytes (`ret 8`).
// Callees: GetPlayer 0x00643D7C, CPlayer::PeekPControlledCreature 0x00487DD0, slot-offset fill 0x00657BA0 (same-TU stand-in body below),
//   LowerBound 0x004365B0 (const), CTCInventoryAbilities::GetPrioritisedExpressionDefs 0x005EED8C, DrawExpressionButton (virtual slot 9, 0x00658020),
//   CRT free 0x00BFEA14.
// Layout: as DrawExpressionButton 0x00658020 (Columns +0x28; hero component bits +0x20, map +0x44, life flags +0x91); the
//   expressions component scroll index +0x1b8. Modelled locally.
// Grade note: matched by advisor A3. The slot-offset fill is a same-TU noinline stand-in that reads Start/Finish/End before writing,
//   so the vector stops escaping and _M_start stays in ebp (rematerialised from the vector) as in retail; `long row = firstRow;` is
//   declared before `long drawn = firstRow - 1;`; the wrap-round scan runs on its own counter.
// Leaf: DrawExpressionList
// Naming wave 31 (family dgp): GetPlayer is the non-const NPlayerGui::CDrawBase member ?GetPlayer@CDrawBase@NPlayerGui@@QAEAAVCPlayer@@XZ 0x00643D7C
//   (Ego_r.pdb / FableWin.pdb; returns CPlayer&), on a CDrawBase base of the local CDrawAbilityButtonsPC; was the invented
//   ?GetPlayer@CDrawAbilityButtonsPC@NPlayerGui@@QAEPAVCPlayer@@XZ
#include <stdlib.h>
class C2DVector { public: float X, Y; };
void __fastcall GFAPositionToScreen(C2DVector* out, const C2DVector* in);   // 0x00643D0C
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> ComponentEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    ComponentEntry *Start, *Finish;
    ComponentEntry* LowerBound(const ETCInterfaceType& key);                 // 0x0040F020
    const ComponentEntry* LowerBound(const ETCInterfaceType& key) const;     // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap;
struct CThingNative {
    void* VTable; unsigned char Base[0x1c]; unsigned int UsingComponents[9]; ComponentMap Components;
    unsigned char State[0x45]; unsigned char LifeFlags;
};
class CThingPlayerCreature : public CThingNative { };
class CPlayer { public: const CThingPlayerCreature* PeekPControlledCreature() const; };          // 0x00487DD0
class CPlayerManager;
namespace std {                                 // STLport 4.6 vector (12 bytes)
template <class _Tp> class allocator;
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp *_M_start, *_M_finish, *_M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { if (_M_start) free(_M_start); }
    unsigned int size() const { return _M_finish - _M_start; }
    _Tp& operator[](unsigned int i) { return _M_start[i]; }
};
}
class CEnginePrimitivePrepareInfo;
enum EEngine2DLayer { LAYER_44 = 0x44, LAYER_45 = 0x45 };

class CTCInventoryAbilities { public:          // the hero's abilities TC (interface type 0x6F)
    unsigned char _pad00[0x1b8];
    long          ScrollIndex;                  // +0x1b8
    void GetPrioritisedExpressionDefs(std::vector<long>& defs) const;   // 0x005EED8C (Ego_r/FableWin; engine/CTCInventoryAbilities.h PRIORITISED)
};

namespace NPlayerGui {
class CDrawBase {                           // RTTI .?AVCDrawBase@NPlayerGui@@ (layout as 0x00643D7C / 0x00657200)
public:
    CPlayer&        GetPlayer();            // 0x00643D7C ?GetPlayer@CDrawBase@NPlayerGui@@QAEAAVCPlayer@@XZ (Ego_r.pdb / FableWin.pdb)
    virtual void v00();
    void*           PlayerGui;              // +0x04
    CPlayerManager* PlayerManager;          // +0x08
    void*           World;                  // +0x0c
};
struct CDrawAbilityButtonsPC : CDrawBase {
    unsigned char   _pad10[0x28 - 0x10];
    long            Columns;                // +0x28
    void GetExpressionSlotOffsets(std::vector<long>& offsets);   // 0x00657BA0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void DrawExpressionButton(CEnginePrimitivePrepareInfo* info, std::vector<long>* expressions, long slot, long index,
                                      long row, float scale);   // slot 9
    void DrawExpressionList(CEnginePrimitivePrepareInfo* info, float scale);
};

__declspec(noinline) void CDrawAbilityButtonsPC::GetExpressionSlotOffsets(std::vector<long>& offsets)
{
    for (long i = 0; i < Columns; ++i) {
        if (offsets._M_finish == offsets._M_end_of_storage) {
            long n = (long)(offsets._M_finish - offsets._M_start);
            long* grown = (long*)malloc((n + 4) * sizeof(long));
            for (long j = 0; j < n; ++j)
                grown[j] = offsets._M_start[j];
            if (offsets._M_start)
                free(offsets._M_start);
            offsets._M_start = grown;
            offsets._M_finish = grown + n;
            offsets._M_end_of_storage = grown + n + 4;
        }
        *offsets._M_finish++ = i * Columns + 1;
    }
}
void CDrawAbilityButtonsPC::DrawExpressionList(CEnginePrimitivePrepareInfo* info, float scale)
{
    const CThingNative* hero = GetPlayer().PeekPControlledCreature();
    if (hero == 0 || (hero->LifeFlags & 1))
        return;
    std::vector<long> offsets;
    GetExpressionSlotOffsets(offsets);
    if (hero->UsingComponents[3] & 0x8000) {
        const ComponentMap& map = hero->Components;
        const ComponentEntry* it = map.LowerBound((ETCInterfaceType)0x6f);
        const ComponentEntry* last = map.Finish;
        CTCInventoryAbilities* tc = (CTCInventoryAbilities*)((void*)(it == last || 0x6f < it->first ? last : it)->second);
        if (tc) {
            scale *= 0.75f;
            std::vector<long> expressions;
            tc->GetPrioritisedExpressionDefs(expressions);
            long firstRow = tc->ScrollIndex / Columns + 1;
            long rows = (long)expressions.size() / Columns * Columns / Columns;
            long row = firstRow;
            long drawn = firstRow - 1;
            for (; row < rows; ++row) {
                bool any = false;
                long col;
                for (col = 0; col < Columns; ++col) {
                    unsigned int index = offsets._M_start[col] + row * Columns - 1;
                    if (index < expressions.size() && expressions[index] > 0) {
                        any = true;
                        break;
                    }
                }
                if (any) {
                    ++drawn;
                    for (col = 0; col < Columns; ++col)
                        DrawExpressionButton(info, &expressions, col, offsets._M_start[col] + row * Columns - 1, drawn - firstRow, scale);
                }
            }
            for (long row2 = 0; row2 < firstRow - 1; ++row2) {
                bool any = false;
                long col;
                for (col = 0; col < 3; ++col) {
                    unsigned int index = offsets._M_start[col] + row2 * Columns - 1;
                    if (index < expressions.size() && expressions[index] > 0) {
                        any = true;
                        break;
                    }
                }
                if (any) {
                    ++drawn;
                    for (col = 0; col < Columns; ++col)
                        DrawExpressionButton(info, &expressions, col, offsets._M_start[col] + row2 * Columns - 1, drawn - firstRow, scale);
                }
            }
        }
    }
}
}