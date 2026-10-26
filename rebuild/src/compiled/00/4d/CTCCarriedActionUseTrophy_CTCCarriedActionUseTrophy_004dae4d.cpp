// CTCCarriedActionUseTrophy::CTCCarriedActionUseTrophy(CThing*) -- CTCCarriedActionUseBase(thing), own vtable, the embedded CTCCarriedActionUseWallMount built from the same thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DAE4D, length 36 bytes.
// Callees: CTCCarriedActionUseBase::CTCCarriedActionUseBase(CThing*) @ 0x004DAC61 (vtable 0x0123F894),
// CTCCarriedActionUseWallMount::CTCCarriedActionUseWallMount(CThing*) @ 0x004DAE20 (vtable 0x0123FE6C)
// -- both labelled "CEnginePrimitiveGroupManager" in the manifest (propagated). Vtable 0x0123FEF4 =
// CTCCarriedActionUseTrophy (rtti-vtables.tsv); the manifest's "CConsoleCommandBase" label is propagated.
// Layout = Ego_r.pdb: the base is 0x14 bytes, the wall-mount member PTCCarriedActionUseWallMount sits
// at +0x14 (it is an embedded object, not a pointer, despite its P prefix: its constructor runs on
// `this + 0x14`). The argument is pushed straight from its stack slot twice (`push dword ptr [esp+8]`)
// and the body has no frame: a size-optimised TU (#pragma optimize("s", on)).
#pragma optimize("s", on)
class CThing;

class CTCCarriedActionUseBase { public:
    virtual void v00();
    unsigned char _body[0x10];                                  // +0x04..+0x13
    CTCCarriedActionUseBase(CThing* thing);                     // 0x004DAC61
};

struct CTCCarriedActionUseWallMount {
    virtual void v00();
    unsigned char _body[0x10];                                  // +0x04..+0x13 (same base layout)
    CTCCarriedActionUseWallMount(CThing* thing);                // 0x004DAE20
};

struct CTCCarriedActionUseTrophy : CTCCarriedActionUseBase {
    CTCCarriedActionUseWallMount PTCCarriedActionUseWallMount;  // +0x14 (embedded)
    virtual void v00();                                         // own vtable 0x0123FEF4
    CTCCarriedActionUseTrophy(CThing* thing);
};

CTCCarriedActionUseTrophy::CTCCarriedActionUseTrophy(CThing* thing)
    : CTCCarriedActionUseBase(thing)
    , PTCCarriedActionUseWallMount(thing)
{
}