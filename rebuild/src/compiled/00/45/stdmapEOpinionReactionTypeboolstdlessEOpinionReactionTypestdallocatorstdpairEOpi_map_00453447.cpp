// std::map<T_00453447>::map() -- STLport map default constructor (Compare() and allocator() temporaries passed to _Rb_tree(comp, alloc) @ 0x00451957); kept as the landed by-value method shape returning this
// Leaf: ?map@map_T_00453447@std@@
// Renamed (fistp2, 2026-10-24) from CDisplayManager::CopyBackBufferToTexture @ 00/45/CDisplayManager_CopyBackBufferToTexture_00453447.cpp: map/set default constructor: two empty temporaries (the Compare and the allocator) are built in one reused [ebp-1] slot and passed by reference to the two-argument tree constructor @ 0x00451957 (whose body is _Rb_tree(comp, alloc): _Rb_tree_base(alloc) then size = 0, head colour/parent cleared, left/right self-linked); the CDisplayManager::CopyBackBufferToTexture label is propagated nonsense (no display manager, no texture); key/value types not recoverable from the bytes
#pragma optimize("s",on)
// CDisplayManager::CopyBackBufferToTexture @ 0x004df7ff
// __fastcall CTexture* (CDisplayManager*)
//
// The retail body forms two 1-byte stack temporaries in disjoint lifetimes
// (so both reuse the same [ebp-1] slot), takes the address of each, passes
// the pair to a __fastcall member helper (call 0x004dd12f), keeps `this` in a
// callee-saved register (esi) across the call, and returns `this` cast to
// CTexture*. VC7.1 emits two separate `lea eax,[ebp-1]` because the two temps
// are distinct source objects that happen to share the reused slot.


namespace std {
struct map_T_00453447 {
    void _Rb_tree_00451957(char* a, char* b);
    map_T_00453447* map();
};
}

std::map_T_00453447* std::map_T_00453447::map()
{
    char* p;
    char* q;
    { char a; p = &a; }
    { char b; q = &b; }
    _Rb_tree_00451957(p, q);
    return (map_T_00453447*)this;
}