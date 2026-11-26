// CTCFireheartMinigame::`scalar deleting destructor'(unsigned int flags) -- ~CTCFireheartMinigame, then operator delete when bit 0 of flags is set; returns this
// Leaf: ?CTCFireheartMinigame_scalar_deleting_destructor@@YIPAXPAVCTCFireheartMinigame@@HI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E7EB0, `ret 4`; vftable 0x012709CC slot 0; calls 0x007E7ED0 (the class destructor) and operator delete 0x00BFE9BC.
// Naming (NF456, r454b unit with 0x007E7ED0): the destructor call is the real one, ??1CTCFireheartMinigame@@UAE@XZ (Ego_r + FableWin name sets; virtual),
//   written as the qualified call `minigame->CTCFireheartMinigame::~CTCFireheartMinigame()` (direct, not through the vftable); was the free stand-in
//   CTCFireheartMinigame_dtor. The self stays the free stand-in: retail's ??_GCTCFireheartMinigame@@UAEPAXI@Z (Ego_r name set) is compiler-generated
//   and only emitted beside the vftable (the ctor-owned thin-row route), which is not this row's object.
class CTCFireheartMinigame { public:
    virtual ~CTCFireheartMinigame();                 // 0x007E7ED0
};

void operator delete(void*);

void* __fastcall CTCFireheartMinigame_scalar_deleting_destructor(CTCFireheartMinigame* minigame, int edx_unused, unsigned int flags)
{
    minigame->CTCFireheartMinigame::~CTCFireheartMinigame();
    if (flags & 1)
        operator delete(minigame);
    return minigame;
}
