// CDefinitionManager::GetDef(const CCharString& type_name, long index, CDefPointer<const CEnvironmentThemeDaySetDef>* out) const -- the def of a type by global index
// Leaf: ??$GetDef@VCEnvironmentThemeDaySetDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCEnvironmentThemeDaySetDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00888E60, 113 bytes, thiscall, `ret 0Ch`.
// Identity (proven): its caller 0x00888A60 passes CCharString("ENVIRONMENT_THEME_DAY"); Ego_r.pdb has this overload for CEnvironmentThemeDaySetDef (??$GetDef@VCEnvironmentThemeDaySetDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCEnvironmentThemeDaySetDef@@@@@Z) and none for CEnvironmentThemeDef.
// Callees: CDefinitionManager::GetPDefFromGlobalIndex(const CCharString&, long) const 0x009ADE10 (CDefPointer by hidden
//   pointer), the def's virtual slot 1 (destroy on the last release).
// Shape (family M026, representative 0x0043FEB0): `if (index <= 0) return false; CDefPointer<const CDefClassBase> def =
//   GetPDefFromGlobalIndex(type_name, index); if (def.Object) { *out = def; return true; } return false;` -- the temporary
//   is homed in the dead `index` slot; operator= releases with `dec [ecx+4] ; jne`, the temporary's destructor through the
//   DecRef/GetRefCount accessors (`dec esi ; mov edx,esi ; test edx,edx`).
// Header debt: the def ref-count / destroy interface is modelled on CDefClassBase (vptr, count at +4).
class CCharString { public: void* PStringData; };
class CDefClassBase {
public:
    virtual void VirtualSlot00();
    virtual void Destroy() const;                         // slot 1
    void DecRef() const { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release() const { if (--RefCount == 0) Destroy(); }
    void ReleaseTemporary() const { DecRef(); if (GetRefCount() == 0) Destroy(); }
    mutable long RefCount;                                // +0x04
};
class CEnvironmentThemeDaySetDef : public CDefClassBase { };
template <class T> class CDefPointer {
public:
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    template <class U> CDefPointer& operator=(const CDefPointer<U>& other)
    {
        if (Object != other.Object) {
            if (Object) { Object->Release(); Object = 0; }
            Object = static_cast<T*>(other.Object);
            if (Object) ++Object->RefCount;
        }
        return *this;
    }
    T* Object;
};
class CDefinitionManager {
public:
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(const CCharString& type_name, long index) const;   // 0x009ADE10
    template <class T> bool GetDef(const CCharString& type_name, long index, CDefPointer<const T>* out) const
    {
        if (index <= 0)
            return false;
        CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(type_name, index);
        if (def.Object) {
            *out = def;
            return true;
        }
        return false;
    }
};
template bool CDefinitionManager::GetDef<CEnvironmentThemeDaySetDef>(const CCharString& type_name, long index, CDefPointer<const CEnvironmentThemeDaySetDef>* out) const;
template bool CDefinitionManager::GetDef<CEnvironmentThemeDaySetDef>(const CCharString& type_name, long index, CDefPointer<const CEnvironmentThemeDaySetDef>* out) const;