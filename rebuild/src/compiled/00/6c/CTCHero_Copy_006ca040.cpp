// CTCHero::Copy(const CTCBase&) -- copies the two use flags, the title and title-object def and the counted title sub-def pointer from another hero TC, then chains to CTCBase::Copy
// Leaf: ?Copy@CTCHero@@UAEXABVCTCBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CA040, length 87 bytes (+ int3 padding). Ego_r.pdb:
// ?Copy@CTCHero@@UAEXABVCTCBase@@@Z (virtual; the source is static_cast down from the CTCBase reference).
// Callees: CTCBase::Copy(const CTCBase&) @ 0x006868D0 (manifest "$E2" is propagated); the def's virtual slot 1 (destroy) on
// release.
// Shape: byte/dword member copies through al/cl/edx/eax in declaration order (WeaponsUsable +0xc, WillUsable +0xd, Title +0x10,
// HeroTitleObjectDef +0x14; the two script-override flags and the podium flag are NOT copied), then the inline counted
// CDefPointer operator= on +0x18: `if (Object != other.Object) { if (Object) { if (--count == 0) Destroy(); Object = 0; }
// Object = other.Object; if (Object) ++count; }` -- the release decrements in memory (`dec [ecx+4]`) on the member read once
// into ecx, the null store sits inside the non-null arm, the add-ref is `inc [eax+4]` after the store. Then `push edi ;
// call CTCBase::Copy` with the original reference.
// Local model: no engine header exists for CTCHero; engine/CTCBase.h is a flattened POD (vptr as a data member), so the
// polymorphic CTCBase is modelled locally.
class CThing;

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual void Copy(const CTCBase& other);                // 0x006868D0
};

class CDefPointeeBase { public:                                    // counted def: vptr, count at +4, slot 1 destroys
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long m_count;                                           // +0x04
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};

struct CDefPointer {                                        // CDefPointer<CHeroTitleDef const>
    CDefPointeeBase* Object;
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
};

#pragma pack(push, 1)
struct CIntelligentPointer {                                // CIntelligentPointer<CThing> (8 bytes, unaligned members)
    virtual void v00();
    void* m_pItem;
};
#pragma pack(pop)

class CTCHero : public CTCBase { public:                                  // vtable 0x0125EB54
    bool WeaponsUsable;                                     // +0x0c
    bool WillUsable;                                        // +0x0d
    bool WeaponUseOverriddenByScripts;                      // +0x0e
    bool WillUseOverriddenByScripts;                        // +0x0f
    long Title;                                             // +0x10 EHeroTitle
    long HeroTitleObjectDef;                                // +0x14
    CDefPointer PHeroTitleSubDef;                           // +0x18
    bool OnBoastingPodium;                                  // +0x1c
    CIntelligentPointer LastWeaponEquipped;                 // +0x1d
    unsigned __int64 LastWeaponEquippedID;                  // +0x28
    CIntelligentPointer PTeleportParticle;                  // +0x30
    virtual void Copy(const CTCBase& other);
};

void CTCHero::Copy(const CTCBase& other)
{
    const CTCHero& hero = static_cast<const CTCHero&>(other);
    WeaponsUsable = hero.WeaponsUsable;
    WillUsable = hero.WillUsable;
    Title = hero.Title;
    HeroTitleObjectDef = hero.HeroTitleObjectDef;
    PHeroTitleSubDef = hero.PHeroTitleSubDef;
    CTCBase::Copy(other);
}