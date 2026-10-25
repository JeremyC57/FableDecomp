// CTCHeroStats::UpdateAppearanceStats(bool force) -- recomputes the hero's attractiveness, scariness, good/evil appearance and sound-radius / visibility multipliers from the worn clothing and the tattoos, unless neither changed since the last update
// Grade: DIFFER(876v869) -- two spots: (1) the clothing def pointer from CDefinitionManager::GetDef is released through the escaped-local form (`mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],0`) where retail uses `dec [ecx+4] ; jne` -- retail treats both the GetDef and the GetSubDef<CAppearanceModifierDef> out-pointers as non-escaping, but giving this file a same-TU stand-in for both (not just GetSubDef, as here) re-allocates the whole body (the sub-def pointer cached in edi, morph in a register: 225 normalised lines), which smells of an LTCG/IPA summary this lane cannot reproduce; (2) the morph lookup's key temporary lands in [ebp-10h] where retail reuses the modifiers lookup's [ebp-14h]. Tried: GetInterface vs HasTC+GetTC lookups, joint/separate early returns, Release forms (member, MI base, local count), stand-ins before/after, assignment-style stand-in bodies.
// Leaf: ?UpdateAppearanceStats@CTCHeroStats@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057D23B, 867 bytes (0x363, no padding), thiscall `ret 4`. Ego_r.pdb
//   `?UpdateAppearanceStats@CTCHeroStats@@QAEX_N@Z`. Callers: CTCHeroStats::FrameUpdate 0x0057D5D8 and two sites in
//   0x005CE2D1 / 0x005D28C7 (the inventory clothing / tattoo paths).
// Callees: std::bitset::test 0x00410DE0 and the interface map's LowerBound 0x004365B0 (the inline CThing interface
//   lookup, 0x5e CTCHeroAttachableAppearanceModifiers, 3 CTCHeroMorph), list<long>(const allocator&) 0x0042AB58 /
//   ~list 0x0042AB73, CTCHeroAttachableAppearanceModifiers::GetAppearanceModifierList(list<long>&, type) const 0x00706800
//   / IsWearingAppearanceModifierCoveringBodyArea(long, type) const 0x00706450, CTCBase::GetDefinitionManager() const
//   0x006869D0, CDefinitionManager::GetDef(long, CDefPointer<CDefClassBase>*) const 0x0042B040,
//   CDefClassBase::GetSubDef<CAppearanceModifierDef> 0x0057FF92, CAppearanceModifierDef::GetTypeBalancedAttractiveness /
//   Scariness / Goodstrength 0x0057FB86 / 0x0057FBBB / 0x0057FBF0, CTattooDef::GetTypeBalanced* 0x0057FC46 / 0x0057FC71 /
//   0x0057FC9C, CTCHeroMorph::GetTattoos(vector<CDefPointer<CTattooDef> >&) const 0x0071DD50, ~vector 0x00581353,
//   GFLimit<float, float, float> 0x0048DAE0 (__fastcall), CTCBase::GetWorld() 0x00686A80, CWorld::GetFrame() 0x0049D870
//   (static). Constant 0.0f 0x0122DEDC.
// Layout: retail CTCHeroStats (8 below Ego_r here): ParentThing +0x04, InherentAttractiveness +0x84, InherentScariness
//   +0x88, Attractiveness +0x9c, Scariness +0xa0, AppearanceGoodstrength +0xa4, SoundRadiusMultiplier +0xa8,
//   VisibilityMultiplier +0xac, FrameUpdatedClothingStats +0xb0, FrameUpdatedTattooStats +0xb4.
//   CTCHeroAttachableAppearanceModifiers::FrameLastChanged +0x10; CTCHeroMorph::FrameTattoosChanged +0x34 (STLport
//   containers); CHeroStatsDef::AppearanceModifierScaling +0x70 (the game definitions' PHeroStatsDef +0xdc);
//   CAppearanceModifierDef SoundRadiusMultiplier +0x54 / VisibilityMultiplier +0x58; CTattooDef CoversBodyAreaFlags
//   +0x40 / VisibilityMultiplier +0x44 (as Ego_r).
// Shape: size TU. Both interface lookups return early when the component is missing; the "nothing changed" test
//   compares the unsigned clothing frame and the signed tattoo frame; the clothing modifier list is filled for types 2
//   and 0; each list entry's def is fetched through the definition manager into a def pointer and its appearance
//   modifier sub-def (result unchecked) is summed in; then each tattoo not covered by worn clothing (body-area type 2);
//   the three appearance values are clamped with GFLimit and the multipliers floored at 0; both frames are stamped.
#pragma optimize("s", on)
#pragma optimize("y", off)

struct CDefPointee {                                     // a counted definition object
    virtual void v000();
    virtual void DestroyDef();                           // slot 1 (+4)
    long m_refCount;                                     // +4
    void AddRef() { ++m_refCount; }
    void Release()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            Object->AddRef();
    }
    __forceinline ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Tp> struct _List_node {
    _List_node* _M_next;                                 // +0x00
    _List_node* _M_prev;                                 // +0x04
    _Tp         _M_data;                                 // +0x08
};

template <class _Tp> struct _List_iterator {
    _List_node<_Tp>* _M_node;
    _List_iterator(_List_node<_Tp>* __x) : _M_node(__x) { }
    _Tp& operator*() const { return _M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _Alloc allocator_type;
    typedef _List_iterator<_Tp> iterator;
    _List_node<_Tp>* _M_node;                            // +0x00 (the header node)
    explicit list(const allocator_type& __a = allocator_type());   // 0x0042AB58 (list<long>)
    ~list();                                                        // 0x0042AB73
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;                                       // +0x00
    _Tp* _M_finish;                                      // +0x04
    _Tp* _M_end_of_storage;                              // +0x08
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector();                                           // 0x00581353 (vector<CDefPointer<CTattooDef> >)
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp& operator[](size_type __n) { return *(_M_start + __n); }
};

}

enum EHeroAttachableAppearanceModifierType { HERO_APPEARANCE_MODIFIER_TYPE_0 };

void __fastcall GFLimit(float& value, const float& minimum, const float& maximum);   // 0x0048DAE0 (GFLimit<float,float,float>)

class CAppearanceModifierScalingDef;

class CDefClassBase;
class CAppearanceModifierDef : public CDefPointee {
public:
    unsigned char _pad_0x08[0x54 - 0x08];
    float         SoundRadiusMultiplier;                 // +0x54
    float         VisibilityMultiplier;                  // +0x58
    float GetTypeBalancedAttractiveness(const std::vector<CAppearanceModifierScalingDef>& scalings) const;   // 0x0057FB86
    float GetTypeBalancedScariness(const std::vector<CAppearanceModifierScalingDef>& scalings) const;        // 0x0057FBBB
    float GetTypeBalancedGoodstrength(const std::vector<CAppearanceModifierScalingDef>& scalings) const;     // 0x0057FBF0
};
class CTattooDef : public CDefPointee {
public:
    unsigned char _pad_0x08[0x40 - 0x08];
    long          CoversBodyAreaFlags;                   // +0x40
    float         VisibilityMultiplier;                  // +0x44
    float GetTypeBalancedAttractiveness(const std::vector<CAppearanceModifierScalingDef>& scalings) const;   // 0x0057FC46
    float GetTypeBalancedScariness(const std::vector<CAppearanceModifierScalingDef>& scalings) const;        // 0x0057FC71
    float GetTypeBalancedGoodstrength(const std::vector<CAppearanceModifierScalingDef>& scalings) const;     // 0x0057FC9C
};
class CDefClassBase : public CDefPointee {
public:
    bool GetSubDef(CDefPointer<CAppearanceModifierDef>* out) const;   // 0x0057FF92
};

class CHeroStatsDef {
public:
    unsigned char _pad_0x00[0x70];
    std::vector<CAppearanceModifierScalingDef> AppearanceModifierScaling;   // +0x70
};
class CDefinitionManager {
public:
    unsigned char        _pad_0x00[0xdc];
    const CHeroStatsDef* PHeroStatsDef;                  // +0xdc
    bool GetDef(long index, CDefPointer<CDefClassBase>* out) const;   // 0x0042B040
};

void* FindSubDef_0057ff92(const void* def);              // stand-in callees of the stand-in bodies below
void* FindDef_0042b040(long index);
// Same-TU stand-ins for the GetSubDef<CAppearanceModifierDef> / GetDef<CDefClassBase> instantiations: they write the
// out pointer only, so VC7.1 knows the def pointers' addresses do not escape.
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAppearanceModifierDef>* out) const
{
    void* def = FindSubDef_0057ff92(this);
    if (!def)
        return false;
    out->Object = (CAppearanceModifierDef*)def;
    return true;
}
class CWorld {
public:
    static long GetFrame();                              // 0x0049D870
};

class CTCHeroAttachableAppearanceModifiers {             // interface 0x5e
public:
    unsigned char _pad_0x00[0x10];
    unsigned long FrameLastChanged;                      // +0x10
    void GetAppearanceModifierList(std::list<long>& modifiers, EHeroAttachableAppearanceModifierType type) const;   // 0x00706800
    bool IsWearingAppearanceModifierCoveringBodyArea(long bodyAreaFlags,
                                                     EHeroAttachableAppearanceModifierType type) const;           // 0x00706450
};
class CTCHeroMorph {                                     // interface 3
public:
    unsigned char _pad_0x00[0x34];
    long          FrameTattoosChanged;                   // +0x34
    void GetTattoos(std::vector<CDefPointer<CTattooDef> >& tattoos) const;   // 0x0071DD50
};

struct CPresenceBits {
    bool test(long bit) const;                           // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                       // +0x00
    SEntry* last;                                        // +0x04
    SEntry* LowerBound(const long* type);                // 0x004365B0
};

class CThing {
public:
    unsigned char _pad_0x00[0x20];
    CPresenceBits UsingComponents;                       // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap Components;                            // +0x44
    template <class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (UsingComponents.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* found = map->LowerBound(&type);
            CInterfaceMap::SEntry* it;
            if (found != map->last && !(wanted < found->type))
                it = found;
            else
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CTCHeroStats {
public:
    void*         __vftable;                             // +0x00
    CThing*       ParentThing;                           // +0x04
    unsigned char _pad_0x08[0x84 - 0x08];
    float         InherentAttractiveness;                // +0x84
    float         InherentScariness;                     // +0x88
    unsigned char _pad_0x8c[0x9c - 0x8c];
    float         Attractiveness;                        // +0x9c
    float         Scariness;                             // +0xa0
    float         AppearanceGoodstrength;                // +0xa4
    float         SoundRadiusMultiplier;                 // +0xa8
    float         VisibilityMultiplier;                  // +0xac
    long          FrameUpdatedClothingStats;             // +0xb0
    long          FrameUpdatedTattooStats;               // +0xb4
    const CDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    CWorld* GetWorld() const;                                 // 0x00686A80 (CTCBase)
    void UpdateAppearanceStats(bool force);
};

void CTCHeroStats::UpdateAppearanceStats(bool force)
{
    CTCHeroAttachableAppearanceModifiers* modifiers = 0;
    if (!ParentThing->GetInterface(&modifiers, 0x5e))
        return;
    CTCHeroMorph* morph = 0;
    if (!ParentThing->GetInterface(&morph, 3))
        return;
    if (!force && FrameUpdatedClothingStats != 0 && modifiers->FrameLastChanged < (unsigned long)FrameUpdatedClothingStats &&
        FrameUpdatedTattooStats != 0 && morph->FrameTattoosChanged < FrameUpdatedTattooStats)
        return;
    Attractiveness = InherentAttractiveness;
    Scariness = InherentScariness;
    AppearanceGoodstrength = 0.0f;
    SoundRadiusMultiplier = 1.0f;
    VisibilityMultiplier = 1.0f;
    std::list<long> clothing;
    modifiers->GetAppearanceModifierList(clothing, (EHeroAttachableAppearanceModifierType)2);
    modifiers->GetAppearanceModifierList(clothing, (EHeroAttachableAppearanceModifierType)0);
    const CHeroStatsDef* heroStatsDef = GetDefinitionManager().PHeroStatsDef;
    for (std::list<long>::iterator it = clothing.begin(); it != clothing.end(); ++it) {
        CDefPointer<CDefClassBase> def;
        GetDefinitionManager().GetDef(*it, &def);
        CDefPointer<CAppearanceModifierDef> modifierDef;
        def->GetSubDef(&modifierDef);
        Attractiveness += modifierDef->GetTypeBalancedAttractiveness(heroStatsDef->AppearanceModifierScaling);
        Scariness += modifierDef->GetTypeBalancedScariness(heroStatsDef->AppearanceModifierScaling);
        AppearanceGoodstrength += modifierDef->GetTypeBalancedGoodstrength(heroStatsDef->AppearanceModifierScaling);
        SoundRadiusMultiplier *= modifierDef->SoundRadiusMultiplier;
        VisibilityMultiplier *= modifierDef->VisibilityMultiplier;
    }
    std::vector<CDefPointer<CTattooDef> > tattoos;
    morph->GetTattoos(tattoos);
    for (unsigned int i = 0; i < tattoos.size(); ++i) {
        CDefPointer<CTattooDef> tattoo = tattoos[i];
        if (!modifiers->IsWearingAppearanceModifierCoveringBodyArea(tattoo->CoversBodyAreaFlags,
                                                                    (EHeroAttachableAppearanceModifierType)2)) {
            Attractiveness += tattoo->GetTypeBalancedAttractiveness(heroStatsDef->AppearanceModifierScaling);
            Scariness += tattoo->GetTypeBalancedScariness(heroStatsDef->AppearanceModifierScaling);
            AppearanceGoodstrength += tattoo->GetTypeBalancedGoodstrength(heroStatsDef->AppearanceModifierScaling);
            VisibilityMultiplier *= tattoo->VisibilityMultiplier;
        }
    }
    GFLimit(Attractiveness, -1.0f, 1.0f);
    GFLimit(Scariness, -1.0f, 1.0f);
    GFLimit(AppearanceGoodstrength, -2.0f, 2.0f);
    if (SoundRadiusMultiplier < 0.0f)
        SoundRadiusMultiplier = 0.0f;
    if (VisibilityMultiplier < 0.0f)
        VisibilityMultiplier = 0.0f;
    FrameUpdatedClothingStats = GetWorld()->GetFrame();
    FrameUpdatedTattooStats = GetWorld()->GetFrame();
}
