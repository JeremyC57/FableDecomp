// NPlayerGui::CStatBase::CStatBase(CPlayerGuiDef*) -- stat-bar base constructor: stores the GUI def, clears the value/max/fade state, sets the GUI mode to 1 and constructs the two empty handle/effect vectors
// Leaf: ??0CStatBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064CC11, 84 bytes. No callees. Vtable 0x0125A4E4 (NPlayerGui::CStatBase).
// Layout: PDB CStatBase (Ego_r 0x50) with retail's 12-byte VC6 vectors (retail 0x48): GuiDef +0x4, ValueInitialised
// +0x8, CurrentValue..OldRecentMaxValue +0xC..+0x1C (floats), FrameCurrentValueLastMoved +0x20, FrameFullyOpenedOn
// +0x24, GuiMode +0x28, FadeInTimer +0x2C, PrimitiveHandles +0x30, Effects +0x3C.
// Shape: an odd-address /Os constructor (the sibling copy 0x0065431D is the one CCrimeStatBar calls): under
// #pragma optimize("s", on) every float zero is `fldz ; fstp` while the integer zeros share ecx; the vectors are member
// objects with inline constructors zeroing their three words.
#pragma optimize("s", on)
struct CPlayerGuiDef;
struct CEnginePrimitiveHandle;
struct CParticleEmitterPointer;

enum EPlayerGuiMode { PLAYER_GUI_MODE_DEFAULT = 1 };

template<class T> struct CRetailVector {               // VC6 Dinkumware vector: first/last/end
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) {}
};

namespace NPlayerGui {

class CStatBase {
public:
    CStatBase(CPlayerGuiDef* guiDef);
    virtual ~CStatBase();

    CPlayerGuiDef*                          GuiDef;                      // +0x04
    bool                                    ValueInitialised;            // +0x08
    float                                   CurrentValue;                // +0x0C
    float                                   OldValue;                    // +0x10
    float                                   LastOldValue;                // +0x14
    float                                   RecentMaxValue;              // +0x18
    float                                   OldRecentMaxValue;           // +0x1C
    unsigned long                           FrameCurrentValueLastMoved;  // +0x20
    unsigned long                           FrameFullyOpenedOn;          // +0x24
    EPlayerGuiMode                          GuiMode;                     // +0x28
    float                                   FadeInTimer;                 // +0x2C
    CRetailVector<CEnginePrimitiveHandle>   PrimitiveHandles;            // +0x30
    CRetailVector<CParticleEmitterPointer>  Effects;                     // +0x3C
};

CStatBase::CStatBase(CPlayerGuiDef* guiDef)
    : GuiDef(guiDef),
      ValueInitialised(false),
      CurrentValue(0.0f),
      OldValue(0.0f),
      LastOldValue(0.0f),
      RecentMaxValue(0.0f),
      OldRecentMaxValue(0.0f),
      FrameCurrentValueLastMoved(0),
      FrameFullyOpenedOn(0),
      GuiMode(PLAYER_GUI_MODE_DEFAULT),
      FadeInTimer(0.0f)
{
}

}
