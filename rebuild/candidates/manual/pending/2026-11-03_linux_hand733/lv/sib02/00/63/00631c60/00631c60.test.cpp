// Behaviour test for 0x00631C60 NUISystem::CUIDef::Transfer: every transferred member is handed to the context once,
// at its retail offset and in retail call order, and the default CWideString lives exactly around the TextValue transfer.
#include <cstdio>
#include <cstddef>
static const void* g_seen[128]; static int g_count = 0; static int g_wideAlive = 0; static int g_wideAtText = -1; static int g_wideCtors = 0;
class CPersistContext { public:
    template <class T> void Transfer(T& value) { g_seen[g_count++] = &value; }
    template <class M> void TransferMap(M& value) { g_seen[g_count++] = &value; }
    template <class V> void TransferVector(V& value) { g_seen[g_count++] = &value; }
    template <class V> void TransferVectorOfSubComponents(V& value) { g_seen[g_count++] = &value; }
};
class CWideString { public: void* PStringData; CWideString() : PStringData(0) { ++g_wideAlive; ++g_wideCtors; } ~CWideString() { --g_wideAlive; } };
class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; };
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class C2DBoxI { public: int TLX, TLY, BRX, BRY; };
struct WideField { unsigned int PStringData; };   // a member CWideString (its ctor is not under test)
struct Blob12 { unsigned char b[12]; };
#pragma pack(push, 1)
namespace NUISystem {
class CUIDef { public:
    unsigned char _base[0x3c];
    int Type; // +0x3c
    Blob12 States; // +0x40
    unsigned int MeshIndex; // +0x4c
    unsigned int Font; // +0x50
    WideField TextValue; // +0x54
    float Height; // +0x58
    float Width; // +0x5c
    int ExpansionType; // +0x60
    Blob12 Sprites; // +0x64
    Blob12 Children; // +0x70
    Blob12 HorizontalSeparations; // +0x7c
    Blob12 VerticalSeparations; // +0x88
    Blob12 NonScrollingChildren; // +0x94
    int MeshType; // +0xa0
    C2DBoxF TextWindow; // +0xa4
    int Layer; // +0xb4
    float Angle; // +0xb8
    bool PositionIsCenter; // +0xbc
    bool TextLineBreak; // +0xbd
    bool ScaleText; // +0xbe
    bool Independant; // +0xbf
    float ScrollingSpeed; // +0xc0
    int Action; // +0xc4
    int ActionOnBack; // +0xc8
    int ActionOnSelected; // +0xcc
    int ActionOnUnselected; // +0xd0
    int ActionOnDestruction; // +0xd4
    int PreAction; // +0xd8
    int ActionOnDroppedNowhere; // +0xdc
    int ActionOnLeftClicked; // +0xe0
    int ActionOnLeftUnclicked; // +0xe4
    int ActionOnLeftHeld; // +0xe8
    int ActionOnRightClicked; // +0xec
    int ActionOnDropped; // +0xf0
    int ActionOnDraggedUp; // +0xf4
    int ActionOnDraggedDown; // +0xf8
    int ActionOnLeftClickedUnder; // +0xfc
    int ActionOnLeftClickedAbove; // +0x100
    C3DVector Up; // +0x104
    C3DVector Forward; // +0x110
    C3DVector RotationAxis; // +0x11c
    float RotationSpeed; // +0x128
    unsigned int AnimationIndex; // +0x12c
    int DownArrow; // +0x130
    int UpArrow; // +0x134
    int UpLimit; // +0x138
    int DownLimit; // +0x13c
    bool Wrapping; // +0x140
    bool Inverted; // +0x141
    C2DVector PositionOffset; // +0x142
    unsigned char AlphaOffset; // +0x14a
    bool Scrolling; // +0x14b
    C2DBoxI ViewArea; // +0x14c
    bool ComputeOffsetsOnActivate; // +0x15c
    unsigned char _pad_15d[3];
    int ParentStatePrerequisite; // +0x160
    C2DVector Dimensions; // +0x164
    C2DVector Min; // +0x16c
    C2DVector Max; // +0x174
    C2DVector Step; // +0x17c
    float InputDelay; // +0x184
    bool DrawFromViewport; // +0x188
    unsigned char _pad_189[3];
    int SliderLeft; // +0x18c
    int SliderRight; // +0x190
    unsigned int TextBankIndex; // +0x194
    int ActionText; // +0x198
    int KeyText; // +0x19c
    int UndefinedWarning; // +0x1a0
    int Redefiner; // +0x1a4
    Blob12 ActionMap; // +0x1a8
    Blob12 ActionMapAliases; // +0x1b4
    Blob12 ActionOrder; // +0x1c0
    bool EditBoxParentIsButton; // +0x1cc
    bool PasswordBox; // +0x1cd
    unsigned char _pad_1ce[2];
    int EditBoxCharLimit; // +0x1d0
    bool EditBoxUsesIME; // +0x1d4
    bool DisallowSpaceAsFirstChar; // +0x1d5
    unsigned char _pad_1d6[2];
    WideField MovieFilename; // +0x1d8
    bool LayerIndependant; // +0x1dc
    unsigned char _pad_1dd[3];
    Blob12 SwappingStates; // +0x1e0
    Blob12 SwappingTimes; // +0x1ec
    bool BastardChild; // +0x1f8
    unsigned char _pad_1f9[3];
    int Alignement; // +0x1fc
    bool RandomSwap; // +0x200
    unsigned char _pad_201[3];
    int HoveredState; // +0x204
    bool UseRelativeZoom; // +0x208
    bool UseRelativePosition; // +0x209
    bool PartOfListTree; // +0x20a
    unsigned char _pad_20b[1];
    int LeftClickedState; // +0x20c
    int RightClickedState; // +0x210
    Blob12 ShapeChildren; // +0x214
    bool UseViewArea; // +0x220
    bool PCStyle; // +0x221
    unsigned char _pad_222[2];
    int Sprite2DFlag; // +0x224
    void Transfer(CPersistContext& persist);
};
void CUIDef::Transfer(CPersistContext& persist)
{
    persist.Transfer(Type);
    persist.TransferVector(Children);
    persist.Transfer(MeshIndex);
    { CWideString defaultTextValue; persist.Transfer(TextValue); g_wideAtText = g_wideAlive; }
    persist.Transfer(Font);
    persist.Transfer(Height);
    persist.Transfer(Width);
    persist.Transfer(ExpansionType);
    persist.TransferMap(Sprites);
    persist.TransferVector(HorizontalSeparations);
    persist.TransferVector(VerticalSeparations);
    persist.TransferVectorOfSubComponents(States);
    persist.Transfer(TextLineBreak);
    persist.Transfer(ScaleText);
    persist.Transfer(Independant);
    persist.Transfer(MeshType);
    persist.TransferVector(NonScrollingChildren);
    persist.Transfer(TextWindow.TLX);
    persist.Transfer(TextWindow.TLY);
    persist.Transfer(TextWindow.BRX);
    persist.Transfer(TextWindow.BRY);
    persist.Transfer(Layer);
    persist.Transfer(Angle);
    persist.Transfer(PositionIsCenter);
    persist.Transfer(ScrollingSpeed);
    persist.Transfer(Wrapping);
    persist.Transfer(Inverted);
    persist.Transfer(PositionOffset.X);
    persist.Transfer(PositionOffset.Y);
    persist.Transfer(AlphaOffset);
    persist.Transfer(Up.X);
    persist.Transfer(Up.Y);
    persist.Transfer(Up.Z);
    persist.Transfer(Forward.X);
    persist.Transfer(Forward.Y);
    persist.Transfer(Forward.Z);
    persist.Transfer(RotationAxis.X);
    persist.Transfer(RotationAxis.Y);
    persist.Transfer(RotationAxis.Z);
    persist.Transfer(RotationSpeed);
    persist.Transfer(AnimationIndex);
    persist.Transfer(DownArrow);
    persist.Transfer(UpArrow);
    persist.Transfer(UpLimit);
    persist.Transfer(DownLimit);
    persist.Transfer(Scrolling);
    persist.Transfer(ComputeOffsetsOnActivate);
    persist.Transfer(Min.X);
    persist.Transfer(Min.Y);
    persist.Transfer(Max.X);
    persist.Transfer(Max.Y);
    persist.Transfer(Step.X);
    persist.Transfer(Step.Y);
    persist.Transfer(Dimensions.X);
    persist.Transfer(Dimensions.Y);
    persist.Transfer(SliderLeft);
    persist.Transfer(SliderRight);
    persist.Transfer(Action);
    persist.Transfer(ActionOnBack);
    persist.Transfer(ActionOnSelected);
    persist.Transfer(ActionOnUnselected);
    persist.Transfer(ActionOnDestruction);
    persist.Transfer(ActionOnLeftClicked);
    persist.Transfer(ActionOnLeftUnclicked);
    persist.Transfer(ActionOnLeftHeld);
    persist.Transfer(ActionOnRightClicked);
    persist.Transfer(ActionOnDropped);
    persist.Transfer(ActionOnDroppedNowhere);
    persist.Transfer(PreAction);
    persist.Transfer(ActionOnDraggedUp);
    persist.Transfer(ActionOnDraggedDown);
    persist.Transfer(ActionOnLeftClickedAbove);
    persist.Transfer(ActionOnLeftClickedUnder);
    persist.Transfer(InputDelay);
    persist.Transfer(DrawFromViewport);
    persist.Transfer(TextBankIndex);
    persist.Transfer(ActionText);
    persist.Transfer(KeyText);
    persist.Transfer(Redefiner);
    persist.Transfer(UndefinedWarning);
    persist.TransferMap(ActionMap);
    persist.TransferMap(ActionMapAliases);
    persist.TransferVector(ActionOrder);
    persist.Transfer(EditBoxParentIsButton);
    persist.Transfer(PasswordBox);
    persist.Transfer(EditBoxCharLimit);
    persist.Transfer(EditBoxUsesIME);
    persist.Transfer(MovieFilename);
    persist.Transfer(DisallowSpaceAsFirstChar);
    persist.Transfer(LayerIndependant);
    persist.TransferVector(SwappingStates);
    persist.TransferVector(SwappingTimes);
    persist.Transfer(BastardChild);
    persist.Transfer(Alignement);
    persist.Transfer(RandomSwap);
    persist.Transfer(UseRelativeZoom);
    persist.Transfer(UseRelativePosition);
    persist.Transfer(HoveredState);
    persist.Transfer(LeftClickedState);
    persist.Transfer(RightClickedState);
    persist.TransferVector(ShapeChildren);
    persist.Transfer(ViewArea.TLX);
    persist.Transfer(ViewArea.TLY);
    persist.Transfer(ViewArea.BRX);
    persist.Transfer(ViewArea.BRY);
    persist.Transfer(UseViewArea);
    persist.Transfer(PartOfListTree);
    persist.Transfer(PCStyle);
    persist.Transfer(Sprite2DFlag);
}
}
#pragma pack(pop)
static const unsigned long kRetail[] = { 0x3c, 0x70, 0x4c, 0x54, 0x50, 0x58, 0x5c, 0x60, 0x64, 0x7c, 0x88, 0x40, 0xbd, 0xbe, 0xbf, 0xa0, 0x94, 0xa4, 0xa8, 0xac, 0xb0, 0xb4, 0xb8, 0xbc, 0xc0, 0x140, 0x141, 0x142, 0x146, 0x14a, 0x104, 0x108, 0x10c, 0x110, 0x114, 0x118, 0x11c, 0x120, 0x124, 0x128, 0x12c, 0x130, 0x134, 0x138, 0x13c, 0x14b, 0x15c, 0x16c, 0x170, 0x174, 0x178, 0x17c, 0x180, 0x164, 0x168, 0x18c, 0x190, 0xc4, 0xc8, 0xcc, 0xd0, 0xd4, 0xe0, 0xe4, 0xe8, 0xec, 0xf0, 0xdc, 0xd8, 0xf4, 0xf8, 0x100, 0xfc, 0x184, 0x188, 0x194, 0x198, 0x19c, 0x1a4, 0x1a0, 0x1a8, 0x1b4, 0x1c0, 0x1cc, 0x1cd, 0x1d0, 0x1d4, 0x1d8, 0x1d5, 0x1dc, 0x1e0, 0x1ec, 0x1f8, 0x1fc, 0x200, 0x208, 0x209, 0x204, 0x20c, 0x210, 0x214, 0x14c, 0x150, 0x154, 0x158, 0x220, 0x20a, 0x221, 0x224 };
int main()
{
    static NUISystem::CUIDef def;
    def.Transfer(*(CPersistContext*)0);
    int bad = 0;
    if (g_count != (int)(sizeof(kRetail) / sizeof(kRetail[0]))) { printf("count %d\n", g_count); bad = 1; }
    for (int i = 0; i < g_count && !bad; ++i) {
        unsigned long off = (unsigned long)((const char*)g_seen[i] - (const char*)&def);
        if (off != kRetail[i]) { printf("call %d: +0x%lx, retail +0x%lx\n", i, off, kRetail[i]); bad = 1; }
    }
    if (g_wideCtors != 1 || g_wideAtText != 1 || g_wideAlive != 0) { printf("default CWideString not scoped to TextValue\n"); bad = 1; }
    if (sizeof(NUISystem::CUIDef) != 0x228) { printf("sizeof 0x%x\n", (unsigned)sizeof(NUISystem::CUIDef)); bad = 1; }
    if (bad) return 1;
    printf("00631c60_TEST PASS\n");
    return 0;
}