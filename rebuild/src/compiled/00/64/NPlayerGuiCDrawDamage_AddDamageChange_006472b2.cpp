// NPlayerGui::CDrawDamage::AddDamageChange(float change, bool hero) -- queue a positive health change for the damage display: the change scaled by ten and rounded, shown for one second's worth of frames, tagged with whether it hit the hero
// Retail-asm: 0x006472D4 `fstp dword ptr [ebp+8]` / 0x006472D7 `fld dword ptr [ebp+8]` + 0x006472DA `fistp dword ptr [ebp-4]` -- the engine's inline-assembly ROUND helper (the scaled change stored to its by-value copy and reloaded for a bare dword fistp, then `fild` back to float). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword`: no C/C++ form gives the bare dword fistp, so it is the original's `__asm` (same helper as the landed GFGetGraphicFrame 0x00643B9F in this GUI TU).
// Leaf: ?AddDamageChange@CDrawDamage@NPlayerGui@@QAEXM_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006472B2, 86 bytes, thiscall, ebp frame, `leave ; ret 8`, /O1 TU.
// Identity (proven): Ego_r.pdb ?AddDamageChange@CDrawDamage@NPlayerGui@@QAEXM_N@Z; its push_front 0x00655EB4 is the only caller of the
//   SDamageValue list insert 0x00655AF3 (landed _M_create_node 0x0065544C header), and the landed caller
//   CGameScriptInterface::ChangeHeroHealthBy 0x00891720 calls AddDamageChange(float, bool).
// Callees: __ftol2 0x00BFEA70, std::list<SDamageValue>::push_front(const SDamageValue&) 0x00655EB4 (the landed body there is an
//   invented ListWrapper_00655eb4::ListPushFrontWrapper_00655eb4 -> Review at landing).
// Globals: 0x013BA860 a file-static float the TU's dynamic initialiser 0x0121DF1C sets to (float)FramesPerSecond (0x01375550);
//   descriptive name g_fFramesPerSecond_013ba860. Float pool 0.0f (0x0122DEDC), 10.0f (0x01237F0C).
// Layout: CDrawDamage DamageValues (list<SDamageValue>, 4 bytes in STLport) +0x28; SDamageValue {long Duration; float Change;
//   bool Hero;} (Ego_r.pdb, 12 bytes).
#pragma optimize("y", off)
#pragma optimize("s", on)
__forceinline long ROUND(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

extern float g_fFramesPerSecond_013ba860;          // 0x013BA860

namespace std {
template <class _Tp> class allocator { };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Value, class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Value _M_data; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    void push_front(const _Tp& __x);               // 0x00655EB4
};
}

namespace NPlayerGui {
class CDrawDamage {
public:
    class SDamageValue {                           // Ego_r.pdb, 12 bytes
    public:
        long  Duration;                            // +0x00
        float Change;                              // +0x04
        bool  Hero;                                // +0x08
    };
    unsigned char _base[0x28];                     // CDrawBase
    std::list<SDamageValue> DamageValues;          // +0x28
    void AddDamageChange(float change, bool hero);
};

void CDrawDamage::AddDamageChange(float change, bool hero)
{
    if (change > 0.0f) {
        change *= 10.0f;
        long rounded = ROUND(change);
        SDamageValue value;
        value.Duration = (long)g_fFramesPerSecond_013ba860;
        value.Change = (float)rounded;
        value.Hero = hero;
        DamageValues.push_front(value);
    }
}
}