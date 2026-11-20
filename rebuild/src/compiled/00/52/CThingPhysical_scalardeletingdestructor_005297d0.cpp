// CThingPhysical::`scalar deleting destructor'
// Leaf: ?ScalarDeletingDestructor@CThingPhysical@@QAEPAXI@Z
// Retail 0x005297D0, 28 bytes (slot 0 of the CThingPhysical vtable). Standard MSVC deleting-destructor thunk:
//   push esi; mov esi,ecx; call <~CThingPhysical 0x006A5D50>; test [esp+8],1; je L;
//   mov ecx,esi; call <CThing class operator delete 0x004C73B0, __fastcall>; L: mov eax,esi; pop esi; ret 4
//
// agentNF182 (2026-11-20, in-place respelling of this row's existing local model): the destructor it calls is
//   ??1CThingPhysical@@UAE@XZ 0x006A5D50 (Ego_r.pdb 0x006850B0 + FableWin.pdb 0x01F1EFE0; its landed row and the other referencer
//   0x00838A00 bind that spelling), called by qualified name so the call is direct. The earlier comment described 0x005298C0
//   (CThingGameObject's ??_G, which calls 0x00838A00). Header debt: the self leaf and the operator delete stand-in stay (the real
//   ??_G is emitted only by a TU that compiles a CThingPhysical constructor or destructor: retail's TU-mate is the constructor
//   0x005296B0, a separate landed row); see NF182 NOTES.
// The callees are declared but NOT defined in this TU so /O2 cannot inline them away.

class CThingPhysical
{ public:
    virtual ~CThingPhysical();      // @0x6a5d50
    void FreeSelf();                // @0x4c73b0
    void *ScalarDeletingDestructor(unsigned int flags);
};

void *CThingPhysical::ScalarDeletingDestructor(unsigned int flags)
{
    this->CThingPhysical::~CThingPhysical();
    if (flags & 1)
        FreeSelf();
    return this;
}
