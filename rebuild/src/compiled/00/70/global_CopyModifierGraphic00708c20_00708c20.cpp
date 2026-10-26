// _global::Copy_ModifierGraphic_00708c20(const CModifierGraphic* first, const CModifierGraphic* last, CModifierGraphic* result, const random_access_iterator_tag&, int*) -- assign [first, last) of hero appearance modifier graphics onto result (counted def pointer + two words), return the advanced result
// Leaf: Copy_ModifierGraphic_00708c20
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00708C20, length 107 bytes (`ret 0Ch`).
// Identity: `__copy<CTCHeroAttachableAppearanceModifiers::CModifierGraphic*>` -- the catalog element is right (12 bytes in
//   Ego_r.pdb: CDefPointer<const CTextureReplacementDef> +0, BankIndex +4, RenderSizeX +8), the `_Move` name is the
//   Dinkumware spelling of this SGI copy. Callers: CTCHeroAttachableAppearanceModifiers::UpdateGraphics 0x007070D0 and three
//   vector helpers of the TU.
// Shape: SGI `__copy(first, last, result, random_access_iterator_tag, Distance*)` (the counted form), `__fastcall`: first in
//   ecx, last in edx, result + the tag reference + the distance pointer on the stack (`ret 0Ch`); the signed count is
//   (last - first) / sizeof(T) (reciprocal multiply), counted down; an empty range returns the untouched result.
//   The implicit operator= runs the inlined intrusive pointer assignment (same-object skip; release through virtual
//   slot 1 when the count reaches zero and clear the slot; adopt + add a reference) and then the plain words.
struct random_access_iterator_tag { random_access_iterator_tag() { } };   // empty tag, passed by const reference

struct CDefCountedBase {                        // counted def: vtable +0 (slot 1 releases), reference count +4
    virtual void Destroy(int flags);            // slot 0
    virtual void Delete();                      // slot 1 (+0x04)
    unsigned long RefCount;                     // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Delete();
    }
};

template <class T>
class CDefPointer { public:                            // 4-byte intrusive pointer; operator= as the landed CIVCountedPointer 0x00A43720
    CDefCountedBase* Object;                    // +0x00
    CDefPointer& operator=(const CDefPointer& other)
    {
        CDefCountedBase* old = Object;
        if (old != other.Object) {
            if (old) {
                old->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};

class CTextureReplacementDef;

namespace CTCHeroAttachableAppearanceModifiers {
class CModifierGraphic { public:                       // Ego_r.pdb, 12 bytes
    CDefPointer<const CTextureReplacementDef> PTextureReplacement;   // +0x00
    long  BankIndex;                            // +0x04
    float RenderSizeX;                          // +0x08
};
}
using CTCHeroAttachableAppearanceModifiers::CModifierGraphic;

CModifierGraphic* __fastcall Copy_ModifierGraphic_00708c20(const CModifierGraphic* first, const CModifierGraphic* last, CModifierGraphic* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}