// NGraphicAppearance::CTCGraphicAppearance::HandleMessage(const NTCMessages::CMessageBase& message) -- vtable slot 27: dispatch the two caught messages: clear-primitives (type 1) marks the appearance changed with reason 8, set-thing-owner (type 4) goes to its CatchMessage
// Leaf: ?HandleMessage@CTCGraphicAppearance@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C01E0, 86 bytes (+10 int3 padding).
// Name: FableWin.pdb ?HandleMessage@CTCGraphicAppearance@NGraphicAppearance@@UAEXABVCMessageBase@NTCMessages@@@Z; slot 27 of the
// vtable 0x0123986C (rtti-vtables.tsv; slot 27 of the other components is their landed HandleMessage, e.g. 0x006869B0).
// Callees: the message's virtual slot 1 (its type, called once per test), CThing::AddToRenderList @ 0x004C7AC0,
// CTCGraphicAppearance::ClearAllPrimitives @ 0x004BF9A0, CatchMessage(const CMessageOnSetThingOwner&) @ 0x004BFD10 (landed).
// Layout: as the landed SetAsChanged 0x004BF9E0: ParentThing +4, Primitives (STLport list header node) +0x38,
// UpdatePrimitiveReasons +0x58.
// Shape: two `if` tests each with its own virtual GetType call; the type-1 arm is CatchMessage(CMessageClearPrimitives)
// = SetAsChanged(8) inlined with the constant folded (`test byte ptr [esi+58h],8`, `or al,8`; the landed out-of-line
// CatchMessage 0x004BFCE0 keeps the 8 in bl). `ret 4`.
class C3DVector { public: float X, Y, Z; };
class CTCAnimationBase;

class CThing {
public:
    void AddToRenderList();                             // 0x004C7AC0
};

namespace NTCMessages {
class CMessageBase {
public:
    virtual void v00();
    virtual long GetType() const;                       // slot 1 (+0x04)
};
class CMessageClearPrimitives : public CMessageBase { };   // type 1
class CMessageOnSetThingOwner : public CMessageBase { };   // type 4
}

struct CPrimitiveListNode {
    CPrimitiveListNode* m_next;                         // +0x00
    CPrimitiveListNode* m_prev;                         // +0x04
};
struct CPrimitiveList {                                 // STLport list<CPrimitive>: the header-node pointer
    CPrimitiveListNode* m_head;
    bool empty() const { return m_head->m_next == m_head; }
};

namespace NGraphicAppearance {

class CTCGraphicAppearance { public:
    // +0x00 vftable (CTCBase), from the virtual below
    CThing*        ParentThing;                         // +0x04
    bool           ToBeRemoved;                         // +0x08
    bool           IsInGlobalUpdateTCs;                 // +0x09
    unsigned char  MeshEffects[0x0c];                   // +0x0c map<void*, CMeshEffectPriorityEntry> (STLport)
    unsigned char  TintColours[0x0c];                   // +0x18 map<void*, CRGBColour> (STLport)
    CTCAnimationBase* PTCAnimation;                     // +0x24
    C3DVector      CachedStaticFocalPosOffset;          // +0x28
    float          Scale;                               // +0x34
    CPrimitiveList Primitives;                          // +0x38 list<CPrimitive> (header-node pointer)
    void*          Components;                          // +0x3c list<CAGraphicAppearanceComponent*> (header-node pointer)
    unsigned long  LightingChannel;                     // +0x40 CEnginePrimitiveThemeChannelDesc
    unsigned char  MainGraphic[0x0e];                   // +0x44 CEngineGraphic
    unsigned short ActivePersistenceFlags;              // +0x52
    long           FrameUpdatedWasRenderedFlag;         // +0x54
    unsigned char  UpdatePrimitiveReasons;              // +0x58
    void ClearAllPrimitives();                          // 0x004BF9A0
    void SetAsChanged(unsigned char reasons)            // 0x004BF9E0, inlined here
    {
        ParentThing->AddToRenderList();
        if ((reasons & 8) && !(UpdatePrimitiveReasons & 8) && !Primitives.empty())
            ClearAllPrimitives();
        UpdatePrimitiveReasons |= reasons;
    }
    void CatchMessage(const NTCMessages::CMessageClearPrimitives& message) { SetAsChanged(8); }
    void CatchMessage(const NTCMessages::CMessageOnSetThingOwner& message);   // 0x004BFD10
    virtual void HandleMessage(const NTCMessages::CMessageBase& message);
};

void CTCGraphicAppearance::HandleMessage(const NTCMessages::CMessageBase& message)
{
    if (message.GetType() == 1)
        CatchMessage(static_cast<const NTCMessages::CMessageClearPrimitives&>(message));
    else if (message.GetType() == 4)
        CatchMessage(static_cast<const NTCMessages::CMessageOnSetThingOwner&>(message));
}

} // namespace NGraphicAppearance