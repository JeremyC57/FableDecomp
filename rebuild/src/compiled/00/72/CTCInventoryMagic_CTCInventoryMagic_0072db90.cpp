// CTCInventoryMagic::CTCInventoryMagic(CThing* thing) -- build both bases, store both vtables, empty the primitive-handle vector
// Retail 0x0072DB90, 66 bytes (+14 int3), ret 4. Called from the factory 0x004D2F46 (CTCInventoryMagic_Create).
// Shape (as the landed sibling CTCInventoryStats::CTCInventoryStats 0x005CD2A6): CTCInventoryBase(thing)
// 0x005BFF07, then NHeroInformationScreens::CBase::CBase 0x0066E734 on the second base at +0x130 (its
// address kept in edi), then the CTCInventoryMagic vtables 0x012668EC (primary) and 0x012668D8 (at
// +0x130), then InventoryMagicPrimitiveHandles (STLport vector<CEnginePrimitiveHandle>, retail +0x134,
// Ego_r.pdb +0x14C) is emptied from one `xor eax,eax` (an inline member constructor: its stores follow
// the vtable stores). This module is speed-optimised (aligned bodies, the argument loaded into eax
// and pushed rather than a pass-through `push [esp+n]`).
// Leaf: ??0CTCInventoryMagic

struct CThing;

struct CTCInventoryBase {
    virtual void v00();
    unsigned char _base[0x12C];                    // +0x04 .. +0x130
    CTCInventoryBase(CThing* thing);               // 0x005BFF07
};

namespace NHeroInformationScreens {
struct CBase {                                     // vtable 0x0123A3E0
    virtual void v00();
    CBase();                                       // 0x0066E734
};
}

struct CEnginePrimitiveHandle;
struct SPrimitiveHandleVector {                    // STLport vector<CEnginePrimitiveHandle>
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
    SPrimitiveHandleVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct CTCInventoryMagic : CTCInventoryBase, NHeroInformationScreens::CBase {
    SPrimitiveHandleVector InventoryMagicPrimitiveHandles;  // +0x134
    virtual void v00();
    CTCInventoryMagic(CThing* thing);
};

CTCInventoryMagic::CTCInventoryMagic(CThing* thing)
    : CTCInventoryBase(thing)
{
}