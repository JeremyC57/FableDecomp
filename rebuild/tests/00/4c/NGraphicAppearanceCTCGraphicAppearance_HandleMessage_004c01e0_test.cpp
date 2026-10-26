#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; std::printf("check failed line %d\n", __LINE__); } } while (0)
// ---- the candidate model (copied from src/004c01e0.cpp) ----
// NGraphicAppearance::CTCGraphicAppearance::HandleMessage(const NTCMessages::CMessageBase& message) -- vtable slot 27: dispatch the two caught messages: clear-primitives (type 1) marks the appearance changed with reason 8, set-thing-owner (type 4) goes to its CatchMessage
// Leaf: ?HandleMessage@CTCGraphicAppearance@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C01E0, 84 bytes (+12 int3 padding).
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

// ---- stubs for the external callees ----

static int g_order = 0;
static int g_render = 0, g_cleared = 0, g_owner = 0;
static const NTCMessages::CMessageOnSetThingOwner* g_ownerMessage = 0;
void CThing::AddToRenderList() { g_render = ++g_order; }
void NTCMessages::CMessageBase::v00() {}
long NTCMessages::CMessageBase::GetType() const { return 0; }
void NGraphicAppearance::CTCGraphicAppearance::ClearAllPrimitives() { g_cleared = ++g_order; }
void NGraphicAppearance::CTCGraphicAppearance::CatchMessage(const NTCMessages::CMessageOnSetThingOwner& message) { g_owner = ++g_order; g_ownerMessage = &message; }
struct STestClear : NTCMessages::CMessageClearPrimitives { virtual long GetType() const { return 1; } };
struct STestOwner : NTCMessages::CMessageOnSetThingOwner { virtual long GetType() const { return 4; } };
struct STestOther : NTCMessages::CMessageBase { virtual long GetType() const { return 2; } };

static void run()
{

    CThing thing;
    CPrimitiveListNode head, node;
    head.m_next = &node; head.m_prev = &node; node.m_next = &head; node.m_prev = &head;
    NGraphicAppearance::CTCGraphicAppearance a;
    a.ParentThing = &thing;
    a.Primitives.m_head = &head;
    a.UpdatePrimitiveReasons = 1;
    STestClear clear;
    a.HandleMessage(clear);                    // clear request with primitives: re-render, clear, reason 8 added
    CHECK(g_render == 1 && g_cleared == 2 && a.UpdatePrimitiveReasons == 9 && g_owner == 0);
    a.HandleMessage(clear);                    // already pending: re-render only
    CHECK(g_render == 3 && g_cleared == 2 && a.UpdatePrimitiveReasons == 9);
    a.UpdatePrimitiveReasons = 0;
    head.m_next = &head; head.m_prev = &head;
    a.HandleMessage(clear);                    // no primitives: nothing to clear
    CHECK(g_render == 4 && g_cleared == 2 && a.UpdatePrimitiveReasons == 8);
    STestOwner owner;
    a.HandleMessage(owner);
    CHECK(g_owner == 5 && g_ownerMessage == &owner && g_render == 4);
    STestOther other;
    a.HandleMessage(other);                    // not caught
    CHECK(g_order == 5 && a.UpdatePrimitiveReasons == 8);
}

int main()
{
    run();
    if (!g_ok) { std::printf("004c01e0_TEST FAIL\n"); return 1; }
    std::printf("004c01e0_TEST PASS\n");
    return 0;
}