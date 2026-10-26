// CTCPhysicsControlled::AdjustThingListForDebugHelpers(vector<CThing*>& things) -- for the main player's body, unless the debug-helper flag is set, empty the list (erase(begin(), end()))
// Retail: Fable.exe (TLC, VC7.1) @ 0x007254B0, 78 bytes.
// Callees: 0x00686D40 (this-ignoring member returning g_pMainGameComponent->+0x30, the player manager; manifest
//   "CViewBase::GetDisplayEngine", propagated), CPlayerManager::IsThingMainPlayer @ 0x004497C0, memmove @ 0x00BFEAE6.
//   Global byte 0x013756E9 = the debug-helper switch.
// Layout: engine/CTCPhysicsStandard.h (owner +0x04 = the parent thing); CTCPhysicsControlled adds members from +0x88 (unused here).
//   The list is an STLport vector<CThing*> (start, finish, end of storage).
// Shape: IsThingMainPlayer's argument is pushed before the player-manager call (object expression evaluated after the
//   arguments). erase(first, last) inlines the trivial copy written as an early-out (`if (first == last) return result;`
//   then memmove + byte count): with last == finish the test is `cmp eax,eax ; jne` and esi is saved only on the memmove
//   path; the copy's end becomes the new finish (the `n ? ... : result` form tests the byte count instead). Both helpers are
//   __forceinline: a plain inline erase is called out of line and the lane then folds the self-compare away.
// Leaf: AdjustThingListForDebugHelpers
#include <string.h>
#define CTCPhysicsStandard CTCPhysicsStandardLayout
#include "engine/CTCPhysicsStandard.h"
#undef CTCPhysicsStandard
class CPlayerManager;

class CTCBase
{
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};
class CTCPhysicsBase : public CTCBase
{
public:
};
class CTCPhysicsStandard : public CTCPhysicsBase, public CTCPhysicsStandardLayout
{
};

class CThing;

class CPlayerManager { public:
    bool IsThingMainPlayer(const CThing* thing) const;   // 0x004497C0
};

extern bool g_bDebugHelpers;                // 0x013756E9

__forceinline void* CopyTrivial(const void* first, const void* last, void* result)
{
    if (first == last)
        return result;
    size_t n = (const char*)last - (const char*)first;
    return (char*)memmove(result, first, n) + n;
}

struct CThingVector {                       // STLport vector<CThing*>
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThing** begin() { return _M_start; }
    CThing** end()   { return _M_finish; }
    __forceinline CThing** erase(CThing** first, CThing** last)
    {
        CThing** i = (CThing**)CopyTrivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
};

struct CTCPhysicsControlled : CTCPhysicsStandard {
    CThing* GetThing() { return reinterpret_cast<CThing*>(owner); }
    void AdjustThingListForDebugHelpers(CThingVector& things);
};

void CTCPhysicsControlled::AdjustThingListForDebugHelpers(CThingVector& things)
{
    if (GetPlayerManager().IsThingMainPlayer(GetThing()) && !g_bDebugHelpers)
        things.erase(things.begin(), things.end());
}