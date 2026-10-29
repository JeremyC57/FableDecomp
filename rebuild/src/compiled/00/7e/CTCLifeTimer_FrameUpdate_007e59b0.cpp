// CTCLifeTimer::FrameUpdate() -- count the life frames down; at zero either start a 2 s fade-out (adding a CTCFadeOutAndIn component when the thing has none) or kill the thing; once expired, kill the thing as soon as its fade component is neither fading in nor out
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E59B0, length 283 bytes (+ int3 padding). Vtable 0x012706A4 (CTCLifeTimer)
// slot 9 (catalog "CTCLifeTimer::vfunc_9"); FableWin.pdb ?FrameUpdate@CTCLifeTimer@@UAEXXZ.
// Callees: the component-map LowerBound @ 0x0040F020 / its const twin @ 0x004365B0, CCharString(const char*, long) @
// 0x0099EBF0 ("CTCFadeOutAndIn" @ 0x01238FE4) / ~CCharString @ 0x0099EAE0, CThing::AddTC @ 0x004C9D60,
// CTCFadeOutAndIn::FadeOut(float, bool) @ 0x006C3B70 (2.0f, false), CThing::Kill(bool) @ 0x004C9B80.
// Shape: the landed EntityFadeOut (0x008A0720) component-map model: HasTC(0xf9) is bit 25 of UsingComponents[7] (+0x3c);
// the fade-out arm picks the component (AddTC with a block-scoped name and a named `CThing* thing = ParentThing` taken after
// the name: retail reloads the parent into the dead `this` register before the pushes) and each arm of the
// countdown keeps its own epilogue; the expired arm reads the parent once into a local and preloads the const component
// lookup with 0 (`xor eax,eax`), then tests the two fade-state bytes +0x0d / +0x0c.
// Layout: PDB CTCLifeTimer (0x14, as the landed ctor 0x007E5970): ParentThing +0x04, FramesLeft +0x0c, FadeOutBeforeKill
// +0x10, PerformOnDie +0x11. No engine header.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry_Retail;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const ETCInterfaceType& kind);         // 0x0040F020
    const STCEntry_Retail* LowerBound(const ETCInterfaceType& kind) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase* Find(long kind)
    {
        const STCEntry_Retail* it = LowerBound((ETCInterfaceType)kind);
        return ((it == m_pLast || kind < it->first) ? m_pLast : it)->second;
    }
    __forceinline CTCBase* Find(long kind) const
    {
        const STCEntry_Retail* it = LowerBound((ETCInterfaceType)kind);
        return ((it == m_pLast || kind < it->first) ? m_pLast : it)->second;
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap_Retail;
class CCharString { public:                               // fable_string.h layout: one data-block pointer
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
class CTCParamsBase;
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    void Kill(bool performOnDie);                  // 0x004C9B80
public:
    CTCBase* AddTC(const CCharString&, bool, const CTCParamsBase*);   // 0x004C9D60
};
class CTCFadeOutAndIn { public:
    unsigned char _pad00[0x0c];
    bool FadingIn;                                 // +0x0c
    bool FadingOut;                                // +0x0d
public:
    void FadeOut(float, bool);   // 0x006C3B70
};

class CTCLifeTimer { public:
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
                fade->FadeOut(2.0f, false);
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