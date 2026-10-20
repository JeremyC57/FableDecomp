// CTCTavernGameSpotTheAddition::UpdateHighlightedObject() -- finds the knick-knack nearest the pointer (within twice the def's knick-knack radius), records the relative distance, deselects when it is beyond the radius, and plays the selection-change sound when the selection changes
// Leaf: UpdateHighlightedObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D8290, 291 bytes (plain `ret`).
// Callees: CDefClassBase::GetSubDef<CTavernGameSpotTheAdditionDef>(CDefPointer<const CTavernGameSpotTheAdditionDef>*)
//          0x007D9A80 (on the owner's def class, owner +0x70; this TU, see the stand-in below); CThing::GetPosition()
//          const 0x004C73D0; CCharString(const char*, long) 0x0099EBF0 ("TAVERN_GAME_OBJECT_SELECTION_CHANGE" 0x0125603C);
//          CTCTavernGame::PlaySound(const CCharString&) 0x005EA950; ~CCharString 0x0099EAE0; the def's slot 1 on release.
// Layout (retail, PDB - 0x18): CTCTavernGameSpotTheAddition Pointer +0x104, DistFromObject +0x10c, KnickKnacks
//          (vector<KnickKnack>, 16-byte elements, Thing at +0x0c) +0x110, Selected +0x144;
//          CTavernGameSpotTheAdditionDef::KnickKnackRadius +0x5c (PDB +0x64, two 12-byte vectors before it).
// Shape: the radius is a named float local (copied through edx into its frame word); the best distance starts at
//        `radius + radius`, the index counter shares the def's zero register; the planar distance is
//        sqrt(dx*dx + dy*dy) with the fsqrt intrinsic (VC emits the Y term first); the sound name is a block-scoped
//        named string; the def was dereferenced, so its release has no null test.
#include <math.h>
#pragma intrinsic(sqrt)
struct C3DVector { float X, Y, Z; };

struct CCharString {
    void* m_data;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CThing {
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct KnickKnack {                             // 0x10 bytes
    C3DVector Pos;                              // +0x00
    CThing*   Thing;                            // +0x0c (CThingObject*)
};

struct CTavernGameSpotTheAdditionDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long          m_count;                      // +0x04
    unsigned char _pad_0x08[0x54];
    float         KnickKnackRadius;             // +0x5c
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
struct CSpotTheAdditionDefPointer {             // CDefPointer<const CTavernGameSpotTheAdditionDef>
    CTavernGameSpotTheAdditionDef* Object;
    CSpotTheAdditionDefPointer() : Object(0) {}
    ~CSpotTheAdditionDefPointer() { if (Object) Object->Release(); }
    CTavernGameSpotTheAdditionDef* operator->() const { return Object; }
};

struct CDefClassBase {
    bool GetSubDef(CSpotTheAdditionDefPointer* out);   // 0x007D9A80
};
CTavernGameSpotTheAdditionDef* FindRef_CDefClassBase_GetSubDef_0(CDefClassBase* defClass);   // stand-in callee of the stand-in body below
// Stand-in body (the real 0x007D9A80 is in this TU, out of line, see the landed CDefClassBase_GetSubDef007d9a80): it only
// writes the def through the out parameter, so VC7.1 knows the caller's address-taken pointer does not escape the call.
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CSpotTheAdditionDefPointer* out)
{
    CTavernGameSpotTheAdditionDef* def = FindRef_CDefClassBase_GetSubDef_0(this);
    if (!def)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)

struct CThingDefOwner {
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* DefClass;                    // +0x70
};

struct CTCTavernGameSpotTheAddition {
    virtual void v00();
    CThingDefOwner* Owner;                      // +0x04
    unsigned char   _pad_0x08[0xfc];
    CThing*         Pointer;                    // +0x104 (CThingObject*)
    float           PointerScale;               // +0x108
    float           DistFromObject;             // +0x10c
    KnickKnack*     KnickKnacksBegin;           // +0x110
    KnickKnack*     KnickKnacksEnd;             // +0x114
    KnickKnack*     KnickKnacksEndOfStorage;    // +0x118
    unsigned char   _pad_0x11c[0x28];
    long            Selected;                   // +0x144
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950 (CTCTavernGame)
    void UpdateHighlightedObject();
};

void CTCTavernGameSpotTheAddition::UpdateHighlightedObject()
{
    CSpotTheAdditionDefPointer def;
    Owner->DefClass->GetSubDef(&def);
    float radius = def->KnickKnackRadius;
    float bestDist = radius + radius;
    long selected = -1;
    DistFromObject = -1.0f;
    long index = 0;
    for (KnickKnack* knick = KnickKnacksBegin; knick != KnickKnacksEnd; ++knick, ++index) {
        const C3DVector& pointerPos = Pointer->GetPosition();
        const C3DVector& pos = knick->Thing->GetPosition();
        float dx = pos.X - pointerPos.X;
        float dy = pos.Y - pointerPos.Y;
        float dist = (float)sqrt(dx * dx + dy * dy);
        if (dist < bestDist) {
            bestDist = dist;
            selected = index;
        }
    }
    if (selected != -1)
        DistFromObject = bestDist / radius;
    if (bestDist > radius)
        selected = -1;
    if (selected != Selected) {
        Selected = selected;
        CCharString sound("TAVERN_GAME_OBJECT_SELECTION_CHANGE", -1);
        PlaySound(sound);
    }
}