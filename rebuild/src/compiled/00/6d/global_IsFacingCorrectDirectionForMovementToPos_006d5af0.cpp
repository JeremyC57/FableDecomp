// IsFacingCorrectDirectionForMovementToPos(const CThingAICreature& creature, const CBrainOutput& output, const CTCLook& look, const C3DVector& pos) -- (anonymous namespace) true unless the creature's interface-90 component state is of kind 7 (the retail body ignores output, look and pos)
// Leaf: ?IsFacingCorrectDirectionForMovementToPos@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D5AF0, length 91 bytes (+ int3 padding), `__fastcall`, `ret 8`.
// Ego_r.pdb / FableWin.pdb: `?IsFacingCorrectDirectionForMovementToPos@?A0x35f42d16@@YI_NABVCThingAICreature@@
// ABVCBrainOutput@@ABVCTCLook@@ABVC3DVector@@@Z` -- a file-local __fastcall helper (creature in ecx, output in edx,
// look and pos on the stack = `ret 8`). Callers 0x006D5F08 / 0x006D6014 (CThingAICreature brain update code).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0 (inlined GetInterface, type 90 =
// word +0x28 bit 26), the interface-90 component's state accessor @ 0x0070B470 (`return [this+0Ch]`; manifest
// "CWorld::DrawGetEnvironment" is propagated; the landed CTCLook::IsCurrentlyTurningViaAnimation 0x00753B40 calls it
// the same way).
// Shape: the landed GetInterface(type, T** out) template on the const creature; the bool result is ignored, so the
// absent path reads the uninitialised out word from the `push ecx` frame slot (`mov eax,[esp]`) -- the out pointer
// must NOT be initialised. `m_kind != 7 ? true : false` gives `cmp byte ; setne al` (a bare `!= 7` goes through
// `xor ecx,ecx ; setne cl ; mov al,cl` and merges the two tails). The helper sits in an
// anonymous namespace; a file-scope pointer to it keeps the otherwise unreferenced definition emitted.
class CTCBase;

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};

class CThingAICreature { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template <class T> bool GetInterface(long type, T** out) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTurnState {                                 // the state object handed out by the interface-90 component
    unsigned char _pad_0x00[0x50];
    unsigned char m_kind;                           // +0x50
};

struct CTCTurnSource {                              // interface 90
    CTurnState* GetState();                         // 0x0070B470
};

class CBrainOutput;
class CTCLook;
class C3DVector;

enum { TC_INTERFACE_90 = 90 };

namespace {
bool __fastcall IsFacingCorrectDirectionForMovementToPos(const CThingAICreature& creature, const CBrainOutput& output,
                                                         const CTCLook& look, const C3DVector& pos)
{
    CTCTurnSource* source;
    creature.GetInterface(TC_INTERFACE_90, &source);
    return source->GetState()->m_kind != 7 ? true : false;
}
}

bool (__fastcall* g_pIsFacingCorrectDirectionForMovementToPos)(const CThingAICreature&, const CBrainOutput&,
                                                               const CTCLook&, const C3DVector&)
    = &IsFacingCorrectDirectionForMovementToPos;