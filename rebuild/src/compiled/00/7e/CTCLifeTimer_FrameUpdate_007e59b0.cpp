// CTCLifeTimer::FrameUpdate() -- count the life frames down; at zero either start a 2 s fade-out (adding a CTCFadeOutAndIn component when the thing has none) or kill the thing; once expired, kill the thing as soon as its fade component is neither fading in nor out
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E59B0, length 283 bytes (+ int3 padding). Vtable 0x012706A4 (CTCLifeTimer)
// slot 9 (catalog "CTCLifeTimer::vfunc_9"); FableWin.pdb ?FrameUpdate@CTCLifeTimer@@UAEXXZ.
// Callees: the component-map LowerBound @ 0x0040F020 / its const twin @ 0x004365B0, CCharString(const char*, long) @
// 0x0099EBF0 ("CTCFadeOutAndIn" @ 0x01238FE4) / ~CCharString @ 0x0099EAE0, CThing::AddTC @ 0x004C9D60,
// CTCFadeOutAndIn::Fade(float, bool) @ 0x006C3B70 (2.0f, false), CThing::Kill(bool) @ 0x004C9B80.
// Shape: the landed EntityFadeOut (0x008A0720) component-map model: HasTC(0xf9) is bit 25 of UsingComponents[7] (+0x3c);
// the fade-out arm picks the component (AddTC with a block-scoped name and a named `CThing* thing = ParentThing` taken after
// the name: retail reloads the parent into the dead `this` register before the pushes) and each arm of the
// countdown keeps its own epilogue; the expired arm reads the parent once into a local and preloads the const component
// lookup with 0 (`xor eax,eax`), then tests the two fade-state bytes +0x0d / +0x0c.
// Layout: PDB CTCLifeTimer (0x14, as the landed ctor 0x007E5970): ParentThing +0x04, FramesLeft +0x0c, FadeOutBeforeKill
// +0x10, PerformOnDie +0x11. No engine header.
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);         // 0x0040F020
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase_Retail* Find(long kind)
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
struct CCharString {                               // fable_string.h layout: one data-block pointer
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
struct CTCParamsBase;
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    CTCBase_Retail* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    void Kill(bool performOnDie);                  // 0x004C9B80
};
struct CTCFadeOutAndIn {
    unsigned char _pad00[0x0c];
    bool FadingIn;                                 // +0x0c
    bool FadingOut;                                // +0x0d
    void Fade(float time, bool fadeIn);            // 0x006C3B70
};

struct CTCLifeTimer {
    void*   __vftable;                             // +0x00
    CThing* ParentThing;                           // +0x04 (CTCBase)
    long    m_baseWord;                            // +0x08
    long    FramesLeft;                            // +0x0c
    bool    FadeOutBeforeKill;                     // +0x10
    bool    PerformOnDie;                          // +0x11
    void FrameUpdate();
};

void CTCLifeTimer::FrameUpdate()
{
    if (FramesLeft > 0) {
        if (--FramesLeft == 0) {
            if (FadeOutBeforeKill) {
                CTCFadeOutAndIn* fade;
                if (ParentThing->HasTC(0xf9)) {
                    fade = (CTCFadeOutAndIn*)ParentThing->m_tcs.Find(0xf9);
                } else {
                    CCharString name("CTCFadeOutAndIn", -1);
                    CThing* thing = ParentThing;
                    fade = (CTCFadeOutAndIn*)thing->AddTC(name, false, 0);
                }
                fade->Fade(2.0f, false);
                return;
            }
            ParentThing->Kill(PerformOnDie);
        }
    } else if (FramesLeft == 0) {
        CThing* thing = ParentThing;
        const CTCFadeOutAndIn* fade = 0;
        if (thing->HasTC(0xf9))
            fade = (const CTCFadeOutAndIn*)((const CThing*)thing)->m_tcs.Find(0xf9);
        if (!fade->FadingOut && !fade->FadingIn)
            thing->Kill(PerformOnDie);
    }
}