// CTCHeroAttachableAppearanceModifiers::UpdateHiddenModifiers() -- marks every modifier visible, then hides any lower-layer modifier whose hide mask overlaps a visible higher-layer modifier
class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;
    mutable long RefCount;
    void DecRef() const { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release() const
    {
        if (--RefCount == 0)
            Destroy();
    }
    void ReleaseTemporary() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template<class T> class CDefPointer {
public:
    T* Object;
    __forceinline CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    __forceinline CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
    T* operator->() const { return Object; }
};
template<class T> class CDefTempPointer {
public:
    T* Object;
    __forceinline ~CDefTempPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
};
class CAppearanceModifierDef : public CDefClassBase {
public:
    unsigned char _pad08[0x40 - 0x08];
    unsigned long HideMask;                             // +0x40
};
class CDefinition : public CDefClassBase {
public:
    bool GetSubDef(CDefPointer<const CAppearanceModifierDef>* out) const;   // 0x0057FF92
};
class CDefinitionManager {
public:
    CDefTempPointer<const CDefinition> GetPDefFromGlobalIndex(long index) const;   // 0x009AD9E0
    __forceinline bool GetDef(long index, CDefPointer<const CDefinition>& out) const
    {
        if (index <= 0)
            return false;
        CDefTempPointer<const CDefinition> def = GetPDefFromGlobalIndex(index);
        if (def.Object) {
            out = *(CDefPointer<const CDefinition>*)&def;
            return true;
        }
        return false;
    }
};
class CGameDefinitionManager : public CDefinitionManager
{
};
template<class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
struct CModifierEntry {
    long DefIndex;
    bool Visible;                                       // +0x04
};
class CTCBase {
public:
    virtual void v00();
    void* Thing;
    unsigned char _pad08[4];
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};
class CTCHeroAttachableAppearanceModifiers : public CTCBase {
public:
    unsigned char _pad0c[0x30 - 0x0c];
    vector< vector<CModifierEntry> > AppearanceModifiers;   // +0x30
    void UpdateHiddenModifiers();
};

// R&D I-3 sweep: a same-TU auto_inline(off) GetSubDef (fills only *out) makes the per-entry def pointers non-escaping;
// retail's register roles, slots and release shape follow (684v582 -> match).
const CAppearanceModifierDef* FindAppearanceModifierSubDef(const CDefinition* def);
#pragma auto_inline(off)
bool CDefinition::GetSubDef(CDefPointer<const CAppearanceModifierDef>* out) const
{
    out->Object = FindAppearanceModifierSubDef(this);
    return out->Object != 0;
}
#pragma auto_inline(on)
void CTCHeroAttachableAppearanceModifiers::UpdateHiddenModifiers()
{
    for (int layer = 0; layer < 3; ++layer) {
        vector<CModifierEntry>& v = AppearanceModifiers._M_start[layer];
        for (CModifierEntry* e = v._M_start; e != v._M_finish; ++e)
            e->Visible = true;
    }
    for (int i = 2; i > 0; --i) {
        vector<CModifierEntry>& upper = AppearanceModifiers._M_start[i];
        for (CModifierEntry* e = upper._M_start; e != upper._M_finish; ++e) {
            CDefPointer<const CDefinition> def;
            CDefPointer<const CAppearanceModifierDef> mod;
            if (!e->Visible)
                continue;
            if (!GetDefinitionManager().GetDef(e->DefIndex, def))
                continue;
            if (def->GetSubDef(&mod)) {
                unsigned long mask = mod->HideMask;
                for (int j = i - 1; j >= 0; --j) {
                    vector<CModifierEntry>& lower = AppearanceModifiers._M_start[j];
                    for (CModifierEntry* l = lower._M_start; l != lower._M_finish; ++l) {
                        CDefPointer<const CDefinition> ldef;
                        CDefPointer<const CAppearanceModifierDef> lmod;
                        if (!GetDefinitionManager().GetDef(l->DefIndex, ldef))
                            continue;
                        if (ldef->GetSubDef(&lmod) && (lmod->HideMask & mask))
                            l->Visible = false;
                    }
                }
            }
        }
    }
}