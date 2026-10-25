// PKCTRRedblackMap::PKCTRRedblackMap(CMapNodeAllocator* allocator) -- constructor: keep the node allocator, make the sentinel's children point at itself with a null parent and black colour, park the root and the last-found cache on the sentinel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1AE20, length 33 bytes (+15 int3 padding).
// Leaf: ??0PKCTRRedblackMap@@QAE@PAVCMapNodeAllocator@@@Z
// Retail bytes:  mov eax,ecx ; mov ecx,[esp+4] ; mov [eax],ecx ; lea ecx,[eax+4] ; mov [eax+1Ch],ecx ; mov [eax+20h],ecx ;
//                mov [ecx],ecx ; mov [eax+8],ecx ; xor ecx,ecx ; mov [eax+0Ch],ecx ; mov [eax+10h],ecx ; ret 4
// Identity (sweep1, 2026-10-25; FINDINGS chunk 433 "+0x20 dispatcher = PKCTRRedblackMap ctor"): the 0x24-byte map
//   whose Find 0x00C1AE50 / Insert 0x00C1B720 / Remove 0x00C1B7D0 / RotateLeft 0x00C1B200 / RotateRight 0x00C1B250
//   agent433 matched (layout: allocator +0, sentinel node +4 (Left, Right, Parent, Colour, Key, Value), Root +0x1c,
//   LastFound +0x20; "PKCTRRedblackTree.cpp" is the library's tree source). The only caller is the PKCDefaultFileManager
//   constructor 0x00C1AC40 (call 0x00C1AC88: its +0x20 open-file map over the +0x1c node allocator, vtable 0x012B30EC,
//   whose slots free (0x00C1B320) and allocate (0x00C1B330, operator new(0x18)) one node). Key and Value of the sentinel
//   are left alone. No PDB covers libpk: member names follow agent433's PKCTRRedblackMap rows.
// Label correction: the landed row was CDispatcher_00c1ac40 over a "sink" and a two-pointer "node list"; the manifest
//   label "CResourceList::CResourceList" is propagated.
// Shape: the allocator is stored first (initialiser), then the body in source order: root and cache, the sentinel's
//   children, its parent and colour.
class CMapNodeAllocator {                       // vtable 0x012B30EC
public:
    virtual void Free(void* node);              // slot 0 (0x00C1B320)
    virtual void* Allocate();                   // slot 1 (0x00C1B330)
};

struct PKCTRRedblackNode {                      // 0x18 bytes
    PKCTRRedblackNode* Left;                    // +0x00
    PKCTRRedblackNode* Right;                   // +0x04
    PKCTRRedblackNode* Parent;                  // +0x08
    long               Colour;                  // +0x0c (0 = black)
    unsigned long      Key;                     // +0x10
    void*              Value;                   // +0x14
};

class PKCTRRedblackMap {                        // 0x24 bytes
public:
    CMapNodeAllocator* PAllocator;              // +0x00
    PKCTRRedblackNode  Sentinel;                // +0x04
    PKCTRRedblackNode* Root;                    // +0x1c
    PKCTRRedblackNode* LastFound;               // +0x20
    PKCTRRedblackMap(CMapNodeAllocator* allocator);
};

PKCTRRedblackMap::PKCTRRedblackMap(CMapNodeAllocator* allocator)
    : PAllocator(allocator)
{
    Root = &Sentinel;
    LastFound = &Sentinel;
    Sentinel.Left = &Sentinel;
    Sentinel.Right = &Sentinel;
    Sentinel.Parent = 0;
    Sentinel.Colour = 0;
}
