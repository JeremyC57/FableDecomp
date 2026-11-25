// CTCChest::InitClosed() -- set the chest up from its def: reset the thing's animator and start the appearance's first "CHEST_OPEN" animation (stopped, update mode 1, priority 100), reset the open/close helper to closed with the animation's length as the opening time (snapped open, animation at its end, when the chest is already open), and keep the thing frame-updating while the helper is mid-transition
// Leaf: ?InitClosed@CTCChest@@AAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007502D0, 407 bytes, plain `ret`.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined lookup of CTCAnimationComplex 0x5A),
//   CTCAnimationComplex::GetAnimation 0x0070B460, CAnimateThing::Reset 0x0070EC30 (name unproven, as the landed
//   0x00835E30), CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2 (result ignored), CCharString(const char*, long)
//   0x0099EBF0 ("CHEST_OPEN" 0x01269568) / ~CCharString 0x0099EAE0, CAnimationSet::GetAnimation(const CCharString&, long,
//   const CAnimFilterBase&) 0x005DC340 (CAnimFilterNoGroup temporary, vtables 0x0122FFDC / 0x0122FFCC), CTCBase::GetWorld
//   0x00686A70, CMeshDataBank::PeekAnimation 0x00A242C0, C3DAnimationInfo::GetNonLoopingDuration 0x00A23CE0,
//   CAnimInit::Init 0x0070C050, CAnimateThing::SetMainAnim 0x0070D580, COpenCloseHelper::InitOpened 0x00A55C00,
//   CThing::SetForceFrameUpdate 0x004C7C60; the defs' slot 1.
// Layout: CTCChest (retail, Ego_r.pdb - 4): ChestOpener COpenCloseHelper +0x1c (Opened, Closed, Opening, Closing,
//   OpenCloseTransitionAmount +4, LastOpenCloseTransitionAmount +8, TotalTimeToOpen +0xc, TotalTimeToClose +0x10),
//   ChestOpen +0x30; CThing bitset +0x20, map +0x44, PBaseDef +0x70; CWorld PMainMeshBank +0x3c; CAppearanceDef
//   Animation +0x34; CAnimateThing main anim set CurrentPercentageComplete +0x1c / LastPercentageComplete +0x20 (Ego_r
//   minus the 4-byte-smaller retail vectors).
// Shape: the animation complex is `T* p = 0; if (HasTC) p = GetTC;`; the thing def is held add-referenced by a CDefPointer
//   copy for the whole function; GetSubDef has a same-TU out-only stand-in; the init is the CAnimInit Init-forwarding ctor
//   (frame zero stores after the pushes); the mesh bank is named before the PeekAnimation argument; the helper reset is an
//   inline Init in retail's store order (closed flag, both transition amounts last-first, the two times, then the
//   opening/closing flags); the transition test compares Last != Amount.
#include "fable_compiler.h"
class CTCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
class CAnimationEntry { public:
    long AnimIndex;                                            // +0x00
};
class CAnimFilterBase { public:                                       // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;   // 0x0041CCE4
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {                  // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;   // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                    // 0x00429A2F
};
// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x005DC340 ?GetAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@JABVCAnimFilterBase@@@Z (was ?GetAnimation@CAnimationSet@@QBEPBVCAnimationEntry@@ABVCCharString@@JABVCAnimFilterBase@@@Z)
class CAnimationSet { public:
 public: CAnimationEntry const& GetAnim(CCharString const&, long, CAnimFilterBase const&) const; };

class CDefPointeeBase { public:
    virtual void Destroy0();
    virtual void Destroy();                                    // slot 1 (+0x04)
    long RefCount;                                             // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* object) : Object(object) { if (Object) ++Object->RefCount; }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
class CAppearanceDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                                   // +0x34
};
class CThingBaseDef : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CAppearanceDef>* out) const;    // 0x0042B0A2
};
void* FindDef_CThingBaseDef_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real template instantiation is out of line in retail): it writes the out slot only, so VC7.1 knows
// the def pointer's address does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<CAppearanceDef>* out) const
{
    void* def = FindDef_CThingBaseDef_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

// Naming (NF184, 2026-11-20, q6391 / q6462): the local CAnimFrameInfo / CAnimInit models replaced by the shared engine/CAnimInit.h
//   (FABLE_RETAIL_CANIMINIT_CTORS: the inline argument constructor, Frame zeroed then Init 0x0070C050 = Ego_r.pdb / FableWin.pdb
//   ?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z; was the made-up Set with a long update mode). The constructor's pick-any COMDAT
//   is compiled as its owner row 0x005B33F4's size TU compiles it (NF184 scaffolding opt-in FABLE_RETAIL_CANIMINIT_CTOR_SIZE_BODY).
#define FABLE_RETAIL_CANIMINIT_CTORS
#define FABLE_RETAIL_CANIMINIT_CTOR_SIZE_BODY
#include "engine/CAnimInit.h"
class CAnimSet { public:
    long          UpdateMode;                                  // +0x00
    unsigned char _pad04[0x1c - 0x04];                         // Anims, InternalAlphas
    float         CurrentPercentageComplete;                   // +0x1c
    float         LastPercentageComplete;                      // +0x20
};
class CAnimateThing { public:
    CAnimSet MainAnimSet;                                      // +0x00 (CurrentAnim.MainAnimSet)
    void ClearAllAnims();                                      // 0x0070EC30 (Ego_r + FableWin; NF292, was Reset)
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();                             // 0x0070B460
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                           // +0x20
    CInterfaceMap  m_interfaces;                               // +0x44
    unsigned char  _pad54[0x1c];
    CThingBaseDef* PBaseDef;                                   // +0x70
    void SetForceFrameUpdate(bool force);                      // 0x004C7C60
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template <class T> FABLE_FORCEINLINE T* GetTC(long wanted)
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((it == map->end() || wanted < it->first) ? map->end() : it)->second;
    }
};

class C3DAnimationInfo { public:
    float GetNonLoopingDuration() const;                       // 0x00A23CE0
};
class CMeshDataBank { public:
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;   // 0x00A242C0
};
class CWorld { public:
    unsigned char  _pad00[0x3c];
    CMeshDataBank* PMainMeshBank;                              // +0x3c
};

struct COpenCloseHelper {
    bool  Opened;                                              // +0x00
    bool  Closed;                                              // +0x01
    bool  Opening;                                             // +0x02
    bool  Closing;                                             // +0x03
    float OpenCloseTransitionAmount;                           // +0x04
    float LastOpenCloseTransitionAmount;                       // +0x08
    float TotalTimeToOpen;                                     // +0x0c
    float TotalTimeToClose;                                    // +0x10
    void Init(float timeToOpen, float timeToClose)
    {
        Opened = false;
        Closed = true;
        LastOpenCloseTransitionAmount = 0.0f;
        OpenCloseTransitionAmount = 0.0f;
        TotalTimeToOpen = timeToOpen;
        TotalTimeToClose = timeToClose;
        Opening = false;
        Closing = false;
    }
    void InitOpened();                                         // 0x00A55C00
    bool IsTransitioning() const
    {
        return Opening || Closing || LastOpenCloseTransitionAmount != OpenCloseTransitionAmount;
    }
};

class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCContainer : public CTCBase
{
public:
};
class CTCContainerRewardHero : public CTCContainer
{
public:
};

class CTCChest : public CTCContainerRewardHero {
public:
    void*            __vftable;                                // +0x00
    CThing*          ParentThing;                              // +0x04
    unsigned char    _pad08[0x1c - 0x08];
    COpenCloseHelper ChestOpener;                              // +0x1c
    bool             ChestOpen;                                // +0x30

private:
    void InitClosed();
};

void CTCChest::InitClosed()
{
    CTCAnimationComplex* complex = 0;
    if (ParentThing->HasTC(0x5a))
        complex = ParentThing->GetTC<CTCAnimationComplex>(0x5a);
    CAnimateThing& animator = complex->GetAnimation();
    animator.ClearAllAnims();
    CDefPointer<CThingBaseDef> thingDef(ParentThing->PBaseDef);
    CDefPointer<CAppearanceDef> appearance;
    thingDef->GetSubDef(&appearance);
    const CAnimationEntry* entry = (&reinterpret_cast<const CAnimationSet*>(&(appearance->Animation))->GetAnim("CHEST_OPEN", 0, CAnimFilterNoGroup()));
    CMeshDataBank* meshBank = GetWorld().PMainMeshBank;
    float duration = meshBank->PeekAnimation(entry->AnimIndex)->GetNonLoopingDuration();
    CAnimInit init(0, 0, entry->AnimIndex, ANIM_UPDATE_FROM_SPEED, false, 0, 100);
    animator.SetMainAnim(init, 1, 0);
    ChestOpener.Init(duration, 0.0f);
    if (ChestOpen) {
        ChestOpener.InitOpened();
        animator.MainAnimSet.CurrentPercentageComplete = duration;
        animator.MainAnimSet.LastPercentageComplete = duration;
    }
    if (ChestOpener.IsTransitioning())
        ParentThing->SetForceFrameUpdate(true);
}