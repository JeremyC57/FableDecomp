// Behaviour test for 005f694d: src/005f694d.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCInventoryAbilities::ResetAssignedAbilities()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryAbilities::ResetAssignedAbilities() -- rebuilds the received-ability order from the assigned abilities (saving them), swaps each palette row's slot-1 entry with its neighbour, steps the assigned-level cursor back one row and re-cycles the palette
// Retail 0x005F694D, 290 bytes. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: GetUIMiscThingsDef() 0x005EE3B3 (CDefPointer by value), vector<EHeroAbility>::erase(first, last) 0x005F8F89
// (manifest "CRGBColour_CopyAssign"), vector<EHeroAbility>::push_back(const value_type&) 0x005F9BCD (manifest
// "Vector_PushElement_OrReallocate"), GetNumAbilitiesPerRow() const 0x005EDCF0 (x2), CycleAssignedAbilities(bool)
// 0x005F64DD, the def's slot 1 on release.
// Shape: ReceivedAbilityOrder is cleared through an inline `clear() { erase(begin(), end()); }`; the copy loop runs while
// both AssignedAbilities and AssignedAbilitiesSave have an element (`i < a.size() && i < b.size()`, rotated); the
// swap is an inline std::swap of two vector elements; the level cursor is
// `std::max(level - perRow, -perRow)` (right argument first: the negated row width is the [ebp-8] temporary).
// Fix (retry 10): GetUIMiscThingsDef (0x005EE3B3, same TU, CDefPointer by hidden pointer) has a same-TU body below; with
// only the declaration the final release loaded the def through eax (`mov ecx,eax`), with it straight into ecx as retail.
// Layout (retail; Ego_r.pdb names): CurrentAssignedAbilityLevel +0x144, ReceivedAbilityOrder +0x154,
// AssignedAbilities +0x17C, AssignedAbilitiesSave +0x188; CUIMiscThingsDef::TotalSpellsInContainer +0x14C.
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace std {
template<class T> inline const T& max(const T& a, const T& b) { return a < b ? b : a; }
}

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                            // slot 1
    long RefCount;                                     // +0x04
    void Release() { if (DecRefToZero(RefCount)) Destroy(); }
};
struct CUIMiscThingsDef : CDefPointeeBase {
    unsigned char _pad_0x08[0x14c - 0x08];
    unsigned long TotalSpellsInContainer;              // +0x14c
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& other);
    explicit CDefPointer(T* object) : Object(object) { if (object) ++object->RefCount; }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
enum EHeroAbility { ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };
struct CAbilityVector {                                // vector<EHeroAbility> (12 bytes)
    EHeroAbility* _M_start;
    EHeroAbility* _M_finish;
    EHeroAbility* _M_end_of_storage;
    EHeroAbility* begin() { return _M_start; }
    EHeroAbility* end() { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    EHeroAbility& operator[](unsigned long n) { return *(begin() + n); }
    EHeroAbility* erase(EHeroAbility* first, EHeroAbility* last);   // 0x005F8F89
    void push_back(const EHeroAbility& value);                      // 0x005F9BCD
    void clear() { erase(begin(), end()); }
};

class CTCInventoryAbilities { public:
    unsigned char _pad_0x000[0x144];
    long CurrentAssignedAbilityLevel;                  // +0x144
    unsigned char _pad_0x148[0x154 - 0x148];
    CAbilityVector ReceivedAbilityOrder;               // +0x154
    unsigned char _pad_0x160[0x17c - 0x160];
    CAbilityVector AssignedAbilities;                  // +0x17c
    CAbilityVector AssignedAbilitiesSave;              // +0x188
    CDefPointer<CUIMiscThingsDef> GetUIMiscThingsDef();   // 0x005EE3B3
    long GetNumAbilitiesPerRow() const;                // 0x005EDCF0
    void CycleAssignedAbilities(bool forward);         // 0x005F64DD
    void ResetAssignedAbilities();
};

CUIMiscThingsDef* FindMiscThingsDef_s7();   // stand-in: the "UI_MISC_THINGS_DEF" index lookup and GetDef (0x005EE3B3's work)
#pragma auto_inline(off)
// Same-TU stand-in for GetUIMiscThingsDef (0x005EE3B3, matched separately): fetch the def and return an add-ref'd counted
// pointer through the hidden result.
__declspec(noinline) CDefPointer<CUIMiscThingsDef> CTCInventoryAbilities::GetUIMiscThingsDef()
{
    return CDefPointer<CUIMiscThingsDef>(FindMiscThingsDef_s7());
}
#pragma auto_inline(on)

void CTCInventoryAbilities::ResetAssignedAbilities()
{
    CDefPointer<CUIMiscThingsDef> misc = GetUIMiscThingsDef();
    ReceivedAbilityOrder.clear();
    for (unsigned long i = 0; i < AssignedAbilities.size() && i < AssignedAbilitiesSave.size(); ++i) {
        AssignedAbilitiesSave[i] = AssignedAbilities[i];
        ReceivedAbilityOrder.push_back(AssignedAbilities[i]);
    }
    for (unsigned long j = 0; j < ReceivedAbilityOrder.size(); ++j) {
        if (j % misc->TotalSpellsInContainer == 1 && j < ReceivedAbilityOrder.size() - 1) {
            EHeroAbility swapped = ReceivedAbilityOrder[j];
            ReceivedAbilityOrder[j] = ReceivedAbilityOrder[j + 1];
            ReceivedAbilityOrder[j + 1] = swapped;
        }
    }
    CurrentAssignedAbilityLevel = std::max(CurrentAssignedAbilityLevel - GetNumAbilitiesPerRow(), -GetNumAbilitiesPerRow());
    CycleAssignedAbilities(true);
}

// ---- stubs and driver ----
void CDefPointeeBase::v00() {}
#include <stdlib.h>
static int g_destroyed = 0;
void CDefPointeeBase::Destroy() { ++g_destroyed; }
static CUIMiscThingsDef g_misc;
CUIMiscThingsDef* FindMiscThingsDef_s7() { return &g_misc; }
template <> CDefPointer<CUIMiscThingsDef>::CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
static int g_erases = 0;
EHeroAbility* CAbilityVector::erase(EHeroAbility* first, EHeroAbility* last) { ++g_erases; _M_finish = first; return first; }
void CAbilityVector::push_back(const EHeroAbility& value) { *_M_finish = value; ++_M_finish; }
long CTCInventoryAbilities::GetNumAbilitiesPerRow() const { return 3; }
static int g_cycles = 0; static bool g_forward = false;
void CTCInventoryAbilities::CycleAssignedAbilities(bool forward) { ++g_cycles; g_forward = forward; }
int main()
{
    static CTCInventoryAbilities inv; static EHeroAbility received[16], assigned[8], saved[8];
    for (int i = 0; i < 8; ++i) { assigned[i] = (EHeroAbility)(10 + i); saved[i] = ABILITY_NULL; }
    inv.ReceivedAbilityOrder._M_start = inv.ReceivedAbilityOrder._M_finish = received; inv.ReceivedAbilityOrder._M_end_of_storage = received + 16;
    inv.AssignedAbilities._M_start = assigned; inv.AssignedAbilities._M_finish = assigned + 7; inv.AssignedAbilities._M_end_of_storage = assigned + 8;
    inv.AssignedAbilitiesSave._M_start = saved; inv.AssignedAbilitiesSave._M_finish = saved + 6; inv.AssignedAbilitiesSave._M_end_of_storage = saved + 8;
    g_misc.TotalSpellsInContainer = 3; g_misc.RefCount = 0; inv.CurrentAssignedAbilityLevel = 1;
    inv.ResetAssignedAbilities();
    CHECK(g_erases == 1); CHECK(inv.ReceivedAbilityOrder._M_finish - received == 6);           // copies min(7, 6)
    CHECK(saved[0] == 10 && saved[5] == 15);
    // slot 1 of each 3-wide row swapped with its neighbour: j = 1 and j = 4
    CHECK(received[0] == 10); CHECK(received[1] == 12); CHECK(received[2] == 11); CHECK(received[3] == 13); CHECK(received[4] == 15); CHECK(received[5] == 14);
    CHECK(inv.CurrentAssignedAbilityLevel == -2); CHECK(g_cycles == 1); CHECK(g_forward == true);
    CHECK(g_misc.RefCount == 0); CHECK(g_destroyed == 1);                                         // the by-value def released at exit
    inv.CurrentAssignedAbilityLevel = 9; inv.ResetAssignedAbilities(); CHECK(inv.CurrentAssignedAbilityLevel == 6); CHECK(g_destroyed == 2);
    if (fails == 0) printf("005f694d_TEST PASS\n"); else printf("005f694d_TEST FAIL\n");
    return fails;
}