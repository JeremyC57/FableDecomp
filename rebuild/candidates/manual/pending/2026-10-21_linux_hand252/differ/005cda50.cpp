// Grade: DIFFER(359v366) -- this/entry in ebx/edi (retail edi/ebx), the thing-offset C2DVector copy elided (retail goes through a [ebp-10h] temporary, so the frame is 0x48 not 0x44), and the final def release re-tests the count (add eax,4; dec; cmp) where retail uses dec/jne on ecx
// NInventory::CTCInventoryStats::GlobalUpdate(bool paused) -- base update; while the screen is open and a mannequin exists: look up the mannequin's DEFAULT animation, start it on the mannequin's animate thing when another animation is playing (keeping the thing's current offset), and step the animation by one frame
// Leaf: GlobalUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CDA50, 366 bytes, `ret 4`. Frame-pointer size TU:
//   `#pragma optimize("y", off)` + `("s", on)` (`push 5Ah ; pop edi`).
// Callees: NInventory::CTCInventoryBase::GlobalUpdate(bool) @ 0x005BDD39, CDefClassBase::GetSubDef<T> @ 0x0042B0A2
//   (label "GetSubDef<CAbilityDef>"), the animation list lookup @ 0x005DC340 (name, index, predicate), the component
//   bitset test @ 0x00410DE0 and map LowerBound @ 0x0040F020 (interface 0x5A), CTCAnimate::GetAnimateThing @ 0x0070B460
//   (`mov eax,[ecx+0Ch]`), CAnimationEntry::GetAnimateThingFlags @ 0x005DC390 and its transition time @ 0x005DC1D0,
//   CAnimInit ctor @ 0x0070C050 (label "CJoystick::CInitRumble" is propagated), CAnimateThing::SetMainAnim
//   @ 0x0070D580, the animate thing's frame update @ 0x0070E8B0; CCharString @ 0x0099EBF0 / 0x0099EAE0; the def's
//   virtual slot 1 at the last release. Data: g_FramesPerSecond @ 0x01375550, 1.0f @ 0x0122DED8, "DEFAULT"
//   @ 0x01230554; predicate vtables 0x0122FFDC (derived) / 0x0122FFCC (base).
// Layout: retail CTCInventoryStats as the landed ctor 0x005CD2A6 (pMannequin +0x140); CThing def pointer +0x70,
//   component bitset +0x20 and map +0x44.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct C2DVector {
    float X, Y;
    C2DVector() : X(0.0f), Y(0.0f) { }
};

template <class T> struct CDefPointerC;
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    mutable long RefCount;                              // +0x04
    template <class T> bool GetSubDef(CDefPointerC<T>* out) const;   // 0x0042B0A2
};
template <class T> struct CDefPointerC {
    const T* Object;
    CDefPointerC() : Object(0) { }
    CDefPointerC(const CDefPointerC& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointerC()
    {
        if (Object && --Object->RefCount == 0)
            const_cast<T*>(Object)->Destroy();
    }
    const T* operator->() const { return Object; }
};
struct CThingDef : CDefClassBase { };

struct CAnimationEntry {
    long Id;                                            // +0x00
    unsigned long GetAnimateThingFlags() const;         // 0x005DC390
    long GetTransitionTime() const;                     // 0x005DC1D0
};
struct CAnimationPredicate {                            // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry& entry) const;
    virtual ~CAnimationPredicate() { }
};
struct CAnimationPredicateAny : CAnimationPredicate {   // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry& entry) const;
};
struct CAnimationList {
    const CAnimationEntry* FindNthValid(const CCharString& name, long n, const CAnimationPredicate& predicate) const;   // 0x005DC340
};
struct CAnimationDef : CDefClassBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationList Animations;                          // +0x34
};

struct CAnimInit {
    unsigned char _pad00[0x18];
    C2DVector Offset;                                   // +0x18
    unsigned char _pad20[0x2c - 0x20];
    CAnimInit(char type, long a, long animId, long b, char c, unsigned long flags, long weight)
    {
        Set(type, a, animId, b, c, flags, weight);
    }
    void Set(char type, long a, long animId, long b, char c, unsigned long flags, long weight);   // 0x0070C050
};
struct SAnimState { short Id; };
struct CAnimateThing {
    unsigned char _pad00[0x04];
    SAnimState* PCurrent;                               // +0x04
    unsigned char _pad08[0x1c - 0x08];
    C2DVector Offset;                                   // +0x1C
    C2DVector GetOffset() const { return Offset; }
    void SetMainAnim(const CAnimInit& init, long transitionTime, long flags);   // 0x0070D580
    void UpdateFrameLogic(const struct CFrameStep& step);                       // 0x0070E8B0
};
struct CFrameStep {
    float Time, Blend, Extra;
    CFrameStep(float time) : Time(time), Blend(0.0f), Extra(0.0f) { }
};
struct CTCAnimate {
    CAnimateThing* GetAnimateThing() const;             // 0x0070B460
};
extern long g_FramesPerSecond;                          // 0x01375550

struct CTCBitset { bool Test(long bit) const; };        // 0x00410DE0
struct STCEntry { long m_kind; CTCAnimate* m_pTC; };
struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const long& kind);             // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    CTCBitset     m_tcMask;                             // +0x20
    unsigned char _pad24[0x44 - 0x24];
    CTCMap        m_tcs;                                // +0x44
    unsigned char _pad50[0x70 - 0x50];
    CDefPointerC<CThingDef> PDef;                       // +0x70
    CDefPointerC<CThingDef> GetDef() const { return PDef; }
    template <class T> __forceinline bool GetInterface(T** out, long type)
    {
        if (m_tcMask.Test(type)) {
            long key = type;
            STCEntry* it = m_tcs.LowerBound(key);
            if (it == m_tcs.m_pLast || type < it->m_kind)
                it = m_tcs.m_pLast;
            *out = it->m_pTC;
            return true;
        }
        return false;
    }
};

namespace NInventory {
struct CTCInventoryBase {
    void*         __vftable;                            // +0x00
    unsigned char _pad04[0x58 - 0x04];
    bool          InventoryOpen;                        // +0x58
    void GlobalUpdate(bool paused);                     // 0x005BDD39
};
struct CTCInventoryStats : CTCInventoryBase {
    unsigned char _pad5c[0x140 - 0x5c];
    CThing*       pMannequin;                           // +0x140
    void GlobalUpdate(bool paused);
};

void CTCInventoryStats::GlobalUpdate(bool paused)
{
    CTCInventoryBase::GlobalUpdate(paused);
    if (!InventoryOpen)
        return;
    CDefPointerC<CAnimationDef> animDef;
    if (!pMannequin)
        return;
    pMannequin->GetDef()->GetSubDef(&animDef);
    const CAnimationEntry* entry = animDef.Object->Animations.FindNthValid("DEFAULT", 0, CAnimationPredicateAny());
    CTCAnimate* animate;
    pMannequin->GetInterface(&animate, 0x5a);
    CAnimateThing* thing = animate->GetAnimateThing();
    if (thing->PCurrent->Id != entry->Id) {
        CAnimInit init(6, 0, entry->Id, 1, 1, entry->GetAnimateThingFlags(), 100);
        init.Offset = thing->GetOffset();
        thing->SetMainAnim(init, entry->GetTransitionTime(), 0);
    }
    thing->UpdateFrameLogic(CFrameStep(1.0f / g_FramesPerSecond));
}
}
