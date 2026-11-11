// CTCStealth::UpdateViewability() -- the owner is viewable, except while sneaking in an engine theme whose def's stealth factor (+0x88) exceeds 1e-4
// Leaf: UpdateViewability
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00747480, length 119 bytes.
// Callees: CThing::GetPos @ 0x004C73D0 (returns const C3DVector&; manifest label "CVertexBufferWin32::DoSizeof" is propagated),
// CTCBase::GetWorldMap @ 0x00686D30, CWorldMap::GetStrongestEngineThemeAt(const C3DVector&, long*) @ 0x0063ADF0, CTCBase::GetDefinitionManager
// @ 0x006869D0, CDefinitionManager::GetDef<CEngineThemeDef>(long, CDefPointer*) @ 0x0063F3A0 (manifest instantiation "<CThingPhysicalSwitchDef>"
// is a byte twin), the def's virtual slot 1 (destroy). Constant 1e-4f @ 0x0129BA3C.
// Layout: CTCStealth Viewable +0x0C, Sneaking +0x0F (retail = PDB); engine theme def float at +0x88.
// Shape: the position is a named reference local (GetPos runs before the pushes); the def pointer's null-safe dtor merges with the
// `if (def.Object)` test; `value > 1e-4f` gives `fcomp ; test ah,41h ; jne`.
#include "engine/C3DVector.h"
class CThing { public:
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CWorldMap { public:
    long GetStrongestEngineThemeAt(const C3DVector& pos, long* weight);   // 0x0063ADF0
};
class CEngineThemeDef { public:
    virtual void v00();
    virtual void Destroy() const;               // slot 1
    mutable long RefCount;                      // +0x04
    unsigned char _pad08[0x88 - 0x08];
    float StealthFactor;                        // +0x88
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
};
class CDefinitionManager { public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // GetDef<CEngineThemeDef> 0x0063F3A0 (FableWin.pdb template)
};
class CGameDefinitionManager : public CDefinitionManager { public: };   // RTTI CGameDefinitionManager : CDefinitionManager
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb / FableWin.pdb IBE)
    CWorldMap& GetWorldMap();   // 0x00686D30 (Ego_r.pdb CTCBase, protected)
};
class CTCStealth : public CTCBase { public:
    bool Viewable;                              // +0x0c
    bool Smellable;                             // +0x0d
    bool Hearable;                              // +0x0e
    bool Sneaking;                              // +0x0f
    void UpdateViewability();
};

void CTCStealth::UpdateViewability()
{
    Viewable = true;
    if (Sneaking) {
        const C3DVector& position = ParentThing->GetPos();
        long weight;
        long theme = GetWorldMap().GetStrongestEngineThemeAt(position, &weight);
        CDefPointer<const CEngineThemeDef> def;
        GetDefinitionManager().GetDef(theme, &def);
        if (def.Object) {
            if (def.Object->StealthFactor > 1e-4f)
                Viewable = false;
        }
    }
}