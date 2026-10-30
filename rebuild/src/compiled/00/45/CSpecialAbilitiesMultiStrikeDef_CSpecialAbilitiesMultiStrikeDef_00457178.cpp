// CSpecialAbilitiesMultiStrikeDef::CSpecialAbilitiesMultiStrikeDef() -- default ctor: def base, own vtable, the Sounds/Animations CDefString arrays (each -1 through the vector constructor iterator) and five empty vectors
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00457178, length 112 bytes (reached by a `jmp` from 0x0045F7BA).
// Callees: CParentDefClassBase::CParentDefClassBase @ 0x00430370, `vector constructor iterator' @ 0x004038C0 (x2, 4 x 4 bytes)
// with CDefString::CDefString @ 0x0042D9D0 (catalog "Field_SetToMinusOne"). Vtable 0x0123412C = CSpecialAbilitiesMultiStrikeDef.
// Layout: Ego_r.pdb CSpecialAbilitiesMultiStrikeDef (0xb4) minus 4 with 12-byte retail vectors: Sounds +0x3c, Animations +0x4c,
// HighlightWidth +0x5c (not initialised), HighlightColor +0x60, DelayTimeSecs +0x6c, NumHits +0x78, PerHitDamagerMultiplier +0x84,
// AttachmentPoint +0x90. TU `optimize("s", on)` (the def module), base as the landed COpinionPersonalityDef ctor 0x00456F71.
// Shape: an empty ctor body; the vector members' inline zeroing ctors share one zero register (VC addresses the later
// three through `lea eax`), the out-of-line CDefString ctor pushed once into edi for both iterator calls.
// Leaf: ??0CSpecialAbilitiesMultiStrikeDef
#pragma optimize("s", on)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"

class CDefString { public:
    long TablePos;
    CDefString();                                  // 0x0042D9D0 (TablePos = -1)
};

template <class T> struct CRetailVector {          // retail std::vector<T> (12 bytes)
    T* m_first; T* m_last; T* m_end;
    CRetailVector() : m_first(0), m_last(0), m_end(0) {}
};
class CCharString { public: void* PStringData; };

class CSpecialAbilitiesMultiStrikeDef : public CParentDefClassBase {   // vtable 0x0123412C
public:
    virtual void v000();
    CDefString                 Sounds[4];               // +0x3c
    CDefString                 Animations[4];           // +0x4c
    float                      HighlightWidth;          // +0x5c
    CRetailVector<long>        HighlightColor;          // +0x60
    CRetailVector<float>       DelayTimeSecs;           // +0x6c
    CRetailVector<long>        NumHits;                 // +0x78
    CRetailVector<float>       PerHitDamagerMultiplier; // +0x84
    CRetailVector<CCharString> AttachmentPoint;         // +0x90
    CSpecialAbilitiesMultiStrikeDef();
};

CSpecialAbilitiesMultiStrikeDef::CSpecialAbilitiesMultiStrikeDef()
{
}