// CThingCreatureBase::GetAnimsWithNameStartingWith(const CCharString& prefix, std::vector<const CAnimationEntry*>& anims) const -- collect the appearance definition's animations whose names start with the prefix, with no group filtering
// Leaf: GetAnimsWithNameStartingWith
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662C10, length 83 bytes (+ int3 padding).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>&) @ 0x0042B0A2 (landed),
// CAnimationSet::GetAnimsWithNameStartingWith(const CCharString&, vector&, const CAnimFilterBase&) const @ 0x005DCB00,
// the def's virtual slot 1 (+4) when the count drops to zero; the CAnimFilterNoGroup vtable 0x0122FFDC.
// Identity: Ego_r.pdb `?GetAnimsWithNameStartingWith@CThingCreatureBase@@QBEXABVCCharString@@AAV?$vector@PBVCAnimationEntry@@...@Z`
// (the manifest label "HasAnim" is propagated from the neighbouring 0x00662C70, which is HasAnim).
// Layout (retail, local model): the thing's base definition PBaseDef at +0x70 (landed CTCLook::OnCreate 0x00753040);
// the appearance definition's CAnimationSet at +0x34; a counted definition holds its count at +4.
// Shape: two frame words (the def pointer, then the 4-byte CAnimFilterNoGroup temporary whose vtable store sits just
// before the call); retail loads the filled def pointer ONCE into esi before the query and releases through it
// (`dec [esi+4] ; jne`), so the release is written by hand on a named pointer local (a CDefPointer destructor re-reads
// the address-taken slot in the lane).
class CCharString;
class CAnimationEntry;

class CDefPointeeBase { public:                        // a counted definition object
    virtual void v000();
    virtual void DestroyDef();                  // slot 1 (+4): called when the count drops to zero
    long m_refCount;                            // +4
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};

template<class T> class CDefPointer { public:          // CDefPointer<const T>: one counted pointer to a definition
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
};

struct CAnimationEntryVector;                   // std::vector<const CAnimationEntry*>

struct __declspec(novtable) CAnimFilterBase {   // an animation filter: three virtual slots
    virtual void v000();
    virtual void v001();
    virtual void v002();
};

struct CAnimFilterNoGroup : CAnimFilterBase {   // vtable 0x0122FFDC
    CAnimFilterNoGroup() { }
    virtual void v000();
    virtual void v001();
    virtual void v002();
};

class CAnimationSet { public:
    void GetAnimsWithNameStartingWith(const CCharString& prefix, CAnimationEntryVector& anims,
                                      const CAnimFilterBase& filter) const;     // 0x005DCB00
};

class CAppearanceDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x2c];
    CAnimationSet AnimationSet;                 // +0x34
};

class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CAppearanceDef>& out);       // 0x0042B0A2
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAppearanceDef>& out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char  _pad00[0x70];
    CDefClassBase* PBaseDef;                    // +0x70
};

class CThingCreatureBase : public CThing { public:
    void GetAnimsWithNameStartingWith(const CCharString& prefix, CAnimationEntryVector& anims) const;
};

void CThingCreatureBase::GetAnimsWithNameStartingWith(const CCharString& prefix, CAnimationEntryVector& anims) const
{
    CDefPointer<CAppearanceDef> def;
    PBaseDef->GetSubDef(def);
    CAppearanceDef* appearance = def.m_pObject;
    appearance->AnimationSet.GetAnimsWithNameStartingWith(prefix, anims, CAnimFilterNoGroup());
    if (appearance)
        appearance->ReleaseDef();
}