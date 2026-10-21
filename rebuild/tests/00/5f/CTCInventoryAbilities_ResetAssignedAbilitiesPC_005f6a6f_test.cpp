// Behaviour test for 005f6a6f: src/005f6a6f.cpp copied verbatim; ResetAssignedAbilitiesPC clears ReceivedAbilityOrder,
// copies each assigned ability (while the index is inside both vectors) into the save vector and the received order,
// sets the row level to max(level - perRow, -perRow), cycles forward, and releases the misc-things def.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// CTCInventoryAbilities::ResetAssignedAbilitiesPC() -- copies the assigned abilities into the save slots and the received order, steps the HUD row back one page and re-cycles the assignment
// Leaf: ResetAssignedAbilitiesPC
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F6A6F, 216 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`).
// Callees: CTCInventoryAbilities::GetMiscThingsDef() const @ 0x005EE3B3 (CDefPointer by hidden pointer; manifest label
//          "CCreatureAction_CastStrengthMultiStrikeProper" is propagated), vector<EHeroAbility>::erase(first, last)
//          @ 0x005F8F89 and ::push_back(const EHeroAbility&) @ 0x005F9BCD (manifest labels generic), CTCInventoryAbilities::
//          GetNumAbilitiesPerRow() const @ 0x005EDCF0 (landed name; twice), CycleAssignedAbilities(bool) @ 0x005F64DD
//          (Ego_r.pdb ?CycleAssignedAbilities@CTCInventoryAbilities@@QAEX_N@Z).
// Shape: the misc-things def is a named local held to the end (released with a null test, `dec [ecx+4] ; jne`), returned
// by GetMiscThingsDef, a same-TU noinline stand-in body (retail's LTCG saw it keep no reference to the hidden result
// slot; with the callee extern the release is the long `add eax,4 ; dec [eax] ; cmp [eax],0` form);
// ReceivedAbilityOrder is cleared through the inline clear() = erase(begin, end); the copy loop runs while the index
// is inside BOTH vectors (rotated: the first size tested on entry and at the bottom), elements reached with the
// STLport `*(begin() + i)` form; the row level is the Dinkumware `Max(level - perRow, -perRow)` over two frame
// temporaries (the right argument evaluated first).
// Layout: retail CTCInventoryAbilities (CTCInventoryBase 0x130 + the screen base vptr, 12-byte vectors; engine/
// CTCInventoryAbilities.h carries PDB offsets): CurrentAssignedAbilityLevel +0x144, ReceivedAbilityOrder +0x154,
// AssignedAbilities +0x17C, AssignedAbilitiesSave +0x188.
#pragma optimize("y", off)
#pragma optimize("s", on)

template <class T> inline const T& Max(const T& left, const T& right)
{
    return left < right ? right : left;
}

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CUIMiscThingsDef : CDefBase {
};
struct CMiscThingsDefPointer {                       // CDefPointer<const CUIMiscThingsDef>
    CUIMiscThingsDef* Object;
    CMiscThingsDefPointer() {}
    CMiscThingsDefPointer(CUIMiscThingsDef* object) : Object(object) {}   // adopts a reference the caller already owns
    ~CMiscThingsDefPointer() { if (Object) Object->Release(); }
};

enum EHeroAbility { HERO_ABILITY_NULL };

struct CAbilityVector {                              // retail std::vector<EHeroAbility> {first, last, end}
    EHeroAbility* _M_start;
    EHeroAbility* _M_finish;
    EHeroAbility* _M_end_of_storage;
    EHeroAbility* begin() { return _M_start; }
    EHeroAbility* end() { return _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    EHeroAbility& operator[](unsigned long n) { return *(begin() + n); }
    EHeroAbility* erase(EHeroAbility* first, EHeroAbility* last);   // 0x005F8F89
    void push_back(const EHeroAbility& value);       // 0x005F9BCD
    void clear() { erase(begin(), end()); }
};

struct CTCInventoryAbilities {
    void*          __vftable;                        // +0x000
    unsigned char  _pad_0x004[0x140];
    long           CurrentAssignedAbilityLevel;      // +0x144 (PDB +0x15C)
    unsigned char  _pad_0x148[0x0C];
    CAbilityVector ReceivedAbilityOrder;             // +0x154 (PDB +0x170)
    unsigned char  _pad_0x160[0x1C];
    CAbilityVector AssignedAbilities;                // +0x17C (PDB +0x1A4)
    CAbilityVector AssignedAbilitiesSave;            // +0x188 (PDB +0x1B4)

    CMiscThingsDefPointer GetMiscThingsDef() const;  // 0x005EE3B3
    long GetNumAbilitiesPerRow() const;              // 0x005EDCF0
    void CycleAssignedAbilities(bool forward);       // 0x005F64DD
    void ResetAssignedAbilitiesPC();
};
CUIMiscThingsDef* Lookup_CTCInventoryAbilities_GetMiscThingsDef(const void* owner);   // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) CMiscThingsDefPointer CTCInventoryAbilities::GetMiscThingsDef() const
{
    return CMiscThingsDefPointer(Lookup_CTCInventoryAbilities_GetMiscThingsDef(this));
}
#pragma auto_inline(on)

void CTCInventoryAbilities::ResetAssignedAbilitiesPC()
{
    CMiscThingsDefPointer miscThings = GetMiscThingsDef();
    ReceivedAbilityOrder.clear();
    for (unsigned long i = 0; i < AssignedAbilities.size() && i < AssignedAbilitiesSave.size(); ++i) {
        AssignedAbilitiesSave[i] = AssignedAbilities[i];
        ReceivedAbilityOrder.push_back(AssignedAbilities[i]);
    }
    CurrentAssignedAbilityLevel = Max(CurrentAssignedAbilityLevel - GetNumAbilitiesPerRow(), -GetNumAbilitiesPerRow());
    CycleAssignedAbilities(true);
}
// ---- harness ----
static int g_destroyed = 0; static int g_cycles = 0; static bool g_forward = false; static long g_perRow = 3;
static CUIMiscThingsDef g_def; static int g_lookups = 0;
void CDefBase::v00() { }
void CDefBase::Destroy() { ++g_destroyed; }
CUIMiscThingsDef* Lookup_CTCInventoryAbilities_GetMiscThingsDef(const void* owner)
{ (void)owner; ++g_lookups; ++g_def.RefCount; return &g_def; }
EHeroAbility* CAbilityVector::erase(EHeroAbility* first, EHeroAbility* last)
{
    EHeroAbility* out = first;
    for (EHeroAbility* p = last; p != _M_finish; ++p) *out++ = *p;
    _M_finish = out;
    return first;
}
void CAbilityVector::push_back(const EHeroAbility& value) { if (_M_finish != _M_end_of_storage) *_M_finish++ = value; }
long CTCInventoryAbilities::GetNumAbilitiesPerRow() const { return g_perRow; }
void CTCInventoryAbilities::CycleAssignedAbilities(bool forward) { ++g_cycles; g_forward = forward; }
static void Set(CAbilityVector& v, EHeroAbility* storage, int n, int cap) { v._M_start = storage; v._M_finish = storage + n; v._M_end_of_storage = storage + cap; }
int main()
{
    static unsigned char invBuf[sizeof(CTCInventoryAbilities)]; CTCInventoryAbilities& inv = *(CTCInventoryAbilities*)invBuf;
    static EHeroAbility received[8], assigned[8], saved[8];
    g_def.RefCount = 1;

    for (int i = 0; i < 8; ++i) { received[i] = (EHeroAbility)(100 + i); assigned[i] = (EHeroAbility)(10 + i); saved[i] = (EHeroAbility)0; }
    Set(inv.ReceivedAbilityOrder, received, 2, 8); Set(inv.AssignedAbilities, assigned, 4, 8); Set(inv.AssignedAbilitiesSave, saved, 3, 8);
    inv.CurrentAssignedAbilityLevel = 7;
    inv.ResetAssignedAbilitiesPC();
    // three copied (the save vector is shorter), received order rebuilt from scratch
    CHECK(saved[0] == 10 && saved[1] == 11 && saved[2] == 12 && saved[3] == 0);
    CHECK(inv.ReceivedAbilityOrder._M_finish - inv.ReceivedAbilityOrder._M_start == 3);
    CHECK(received[0] == 10 && received[1] == 11 && received[2] == 12);
    CHECK(inv.CurrentAssignedAbilityLevel == 4 && g_cycles == 1 && g_forward);
    CHECK(g_lookups == 1 && g_def.RefCount == 1 && g_destroyed == 0);

    // level clamps at -perRow; an empty assigned list copies nothing
    Set(inv.AssignedAbilities, assigned, 0, 8);
    inv.CurrentAssignedAbilityLevel = -1;
    inv.ResetAssignedAbilitiesPC();
    CHECK(inv.CurrentAssignedAbilityLevel == -3 && inv.ReceivedAbilityOrder._M_finish == inv.ReceivedAbilityOrder._M_start);
    CHECK(g_cycles == 2 && g_def.RefCount == 1);

    // last reference: destroyed
    g_def.RefCount = 0;
    inv.ResetAssignedAbilitiesPC();
    CHECK(g_destroyed == 1);

    if (fails == 0) printf("005f6a6f_TEST PASS\n");
    return fails;
}