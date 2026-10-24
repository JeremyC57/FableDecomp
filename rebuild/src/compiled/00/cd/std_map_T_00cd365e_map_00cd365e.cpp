// std::map<T_00cd365e>::map() -- STLport map default constructor (Compare() and allocator() temporaries passed to _Rb_tree(comp, alloc) @ 0x00CD2D61); kept as the landed by-value method shape returning this
// Leaf: ?map@map_T_00cd365e@std@@
// Renamed (fistp2, 2026-10-24) from CDisplayManager::CopyBackBufferToTexture @ 00/cd/CDisplayManager_CopyBackBufferToTexture_00cd365e.cpp: map/set default constructor: two empty temporaries (the Compare and the allocator) are built in one reused [ebp-1] slot and passed by reference to the two-argument tree constructor @ 0x00CD2D61 (whose body is _Rb_tree(comp, alloc): _Rb_tree_base(alloc) then size = 0, head colour/parent cleared, left/right self-linked); the CDisplayManager::CopyBackBufferToTexture label is propagated nonsense (no display manager, no texture); key/value types not recoverable from the bytes

namespace std {
struct map_T_00cd365e {
    int dummy;
    void Helper_00cd2d61(char* a, char* b);
    map_T_00cd365e* map();
};
}

#pragma optimize("s", on)
#pragma optimize("y", off)
std::map_T_00cd365e* std::map_T_00cd365e::map()
{
    char local;
    this->Helper_00cd2d61(&local, &local + 1 - 1);
    return (map_T_00cd365e*)this;
}