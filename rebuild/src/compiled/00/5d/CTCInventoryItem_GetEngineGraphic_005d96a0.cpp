// CTCInventoryItem::GetEngineGraphic(long defIndex) -- static: cache the item def for defIndex and return a copy of its CEngineGraphic (def +0x25), or a default (all-zero) graphic when the def cannot be cached
// Leaf: ?GetEngineGraphic@CTCInventoryItem@@SI
// Retail: Fable.exe @ 0x005D96A0, 72 bytes (+8 int3). Ego_r.pdb `?GetEngineGraphic@CTCInventoryItem@@SI?AVCEngineGraphic@@J@Z`
// (static __fastcall: the hidden CEngineGraphic result in ecx, defIndex in edx; `ret`). Instance twin 0x005D92D0 returns
// `const CEngineGraphic&` to the same +0x25 field.
// Callee: CTCInventoryItem::CacheDef(long) @ 0x005D94B0 (static `__fastcall` bool). Global: 0x013B8C18 the cached
// CInventoryItemDef (engine/CInventoryItemDef.h: Graphic +0x25, a CEngineGraphic kept as bytes in the header).
// CEngineGraphic: 14 bytes in the PDB (BankIndex, AnimStep, RenderSizeX, AdditiveAlpha, Type -- packed, no tail pad;
// Ego_r `??0CEngineGraphic@@QAE@XZ` is the default constructor that zeroes all five, stored in member order).
// Shape: speed TU; the field address is formed once (`add eax,25h`) and the implicit copy is dword/dword/dword/word.
#include "engine/CInventoryItemDef.h"
#pragma pack(push, 1)
class CEngineGraphic {                                  // 14 bytes (PDB size)
public:
    unsigned long BankIndex;                            // +0x00
    float         AnimStep;                             // +0x04
    float         RenderSizeX;                          // +0x08
    bool          AdditiveAlpha;                        // +0x0C
    unsigned char Type;                                 // +0x0D EEngineGraphicType
    CEngineGraphic() : BankIndex(0), AnimStep(0.0f), RenderSizeX(0.0f), AdditiveAlpha(false), Type(0) {}
};
#pragma pack(pop)
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};
class CTCInventoryItem {                                // static API only (instance layout: engine/CTCInventoryItem.h)
public:
    static bool __fastcall CacheDef(long defIndex);                     // 0x005D94B0
    static CEngineGraphic __fastcall GetEngineGraphic(long defIndex);
    static CDefPointer<const CInventoryItemDef> s_CachedItemDef;      // 0x013B8C18
};
CEngineGraphic __fastcall CTCInventoryItem::GetEngineGraphic(long defIndex)
{
    if (CacheDef(defIndex))
        return reinterpret_cast<const CEngineGraphic&>(s_CachedItemDef->Graphic);
    return CEngineGraphic();
}