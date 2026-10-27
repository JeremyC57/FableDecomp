// CDefinitionManager::GetDef<CCutsceneDef>(const CCharString& name, CDefPointer<const CCutsceneDef>* out) const -- look the def up by instantiation name; on a hit store it in *out and return true
// Leaf: ??$GetDef@VCCutsceneDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCCutsceneDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD2A71, 92 bytes, thiscall, `ret 8`, frame pointer (size TU).
// Identity (probable): FableWin.pdb names this GetDef<CCutsceneDef> instantiation; only caller RunCutsceneMacro 0x00CBFB7D (bound ?RunCutsceneMacro@@YIXABVCCharString@@AAUCCutsceneCastMap@@...), which looks the macro's cutscene def up by name.
// Callees: CDefinitionManager::GetPDefFromInstantiationName(const CCharString&) const 0x009ADA40 (hidden result pointer, homed
//   in the dead name slot); the defs' virtual Destroy (slot 1).
// Shape (family K080, representative 0x004196B2): the landed speed-TU GetDef 0x0083B000 under `optimize("y", off)` +
//   `("s", on)`: the converting operator= is __forceinline (size mode otherwise calls it), the old def released through
//   Release() (`dec [ecx+4] ; jne`), the temporary through ReleaseTemporary() on the count base (`dec [eax] ; cmp [eax],0`).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString { public: void* PStringData; };
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
};
struct CRefCounted {                                    // the count sits at +4 behind the vptr
    mutable long m_count;
    void DecRef() const { --m_count; }
    long GetRefCount() const { return m_count; }
};
class CDefClassBase : public CDefVirtual, public CRefCounted {
public:
    void Release() const { if (--m_count == 0) Destroy(); }
    void ReleaseTemporary() const { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
class CCutsceneDef : public CDefClassBase { };                  // (members not touched here)
template <class T> class CDefPointer {
public:
    T* m_p;
    CDefPointer() { }
    ~CDefPointer() { if (m_p) m_p->ReleaseTemporary(); }
    template <class U> __forceinline CDefPointer& operator=(const CDefPointer<U>& other)
    {
        if (m_p != other.m_p) {
            if (m_p) {
                m_p->Release();
                m_p = 0;
            }
            m_p = (T*)other.m_p;
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
};
class CDefinitionManager {
public:
    CDefPointer<const CDefClassBase> GetPDefFromInstantiationName(const CCharString& name) const;   // 0x009ADA40
    template <class T> bool GetDef(const CCharString& name, CDefPointer<const T>* out) const
    {
        CDefPointer<const CDefClassBase> def = GetPDefFromInstantiationName(name);
        if (def.m_p) {
            *out = def;
            return true;
        }
        return false;
    }
};
template bool CDefinitionManager::GetDef<CCutsceneDef>(const CCharString&, CDefPointer<const CCutsceneDef>*) const;