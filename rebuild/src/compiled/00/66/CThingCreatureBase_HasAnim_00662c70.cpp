// CThingCreatureBase::HasAnim(const CCharString& name, const CAnimFilterBase& filter) const -- whether the animation set of the creature's appearance definition holds an animation of that name passing the filter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662C70, length 72 bytes (+ int3 padding).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<const CAppearanceDef>&) @ 0x0042B0A2 (landed),
// CAnimationSet::HasAnim(const CCharString&, const CAnimFilterBase&) const @ 0x005DC2E0 (manifest "CAnimationDef_FindAnimationByName"),
// the def's virtual slot 1 (+4) when the temporary drops the last reference.
// Layout (retail, local model): the thing's base definition PBaseDef at +0x70 (as in the landed CTCLook::OnCreate
// 0x00753040); the appearance definition's CAnimationSet at +0x34; a counted definition holds its count at +4.
// Shape: Ego_r.pdb `?HasAnim@CThingCreatureBase@@QBE_NABVCCharString@@ABVCAnimFilterBase@@@Z`. The def pointer is
// zero-initialised in the `push ecx` slot and filled by GetSubDef; retail loads it ONCE into esi before the query
// (`lea ecx,[esi+34h]`) and releases through that register (`dec [esi+4] ; jne`), the bool result riding in bl. A
// CDefPointer destructor re-reads the address-taken slot after the call in the lane (and forms `&m_refCount`), so the
// release is written by hand on a named pointer local (same behaviour as the original scope-exit release).
class CCharString;
class CAnimFilterBase;

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

class CAnimationSet { public:
    bool HasAnim(const CCharString& name, const CAnimFilterBase& filter) const;   // 0x005DC2E0
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
    bool HasAnim(const CCharString& name, const CAnimFilterBase& filter) const;
};

bool CThingCreatureBase::HasAnim(const CCharString& name, const CAnimFilterBase& filter) const
{
    CDefPointer<CAppearanceDef> def;
    PBaseDef->GetSubDef(def);
    CAppearanceDef* appearance = def.m_pObject;
    bool found = appearance->AnimationSet.HasAnim(name, filter);
    if (appearance)
        appearance->ReleaseDef();
    return found;
}