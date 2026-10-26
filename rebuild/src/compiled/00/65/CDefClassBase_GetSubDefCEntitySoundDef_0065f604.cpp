// CDefClassBase::GetSubDef<CEntitySoundDef>(CDefPointer<const CEntitySoundDef>* out) const -- the def's CEntitySoundDef sub-def: look the sub-def
// index up by class name, fetch the def from the definition manager into *out; false when the def has no such sub-def
// Leaf: ??$GetSubDef@VCEntitySoundDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCEntitySoundDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065F604, 162 bytes, thiscall `ret 4`, frame-pointer size TU.
// Identity (FINDINGS chunk 384; manifest GetSubDef<CAbilityDef>): Ego_r.pdb / FableWin.pdb name set
//   `??$GetSubDef@VCEntitySoundDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCEntitySoundDef@@@@@Z`
//   and the class-name literal 'CEntitySoundDef' the body builds for the lookup (one retail body per T: no ICF, so each
//   instantiation is its own address); the model of the sweep3 GetSubDef family.
//   The manager call @ 0x009ADA10 is Ego_r `?GetPDefFromGlobalIndex@CDefinitionManager@@IAE?AV?$CDefPointer@VCDefClassBase@@@@J@Z`,
//   the protected non-const overload: PDefManager (+0x1C) is a plain CDefinitionManager*, and Ego_r / FableWin list
//   the public const overload (QBE, CDefPointer<const CDefClassBase>) first, 0x30 bytes before it, as retail 0x009AD9E0
//   (called by the const CDefinitionManager::GetDef<T>) precedes 0x009ADA10. The index lookup is the def's virtual
//   slot 14 (+0x38, descriptive name).
// Body/shape (unchanged from the landed model): a block-scoped CCharString("CEntitySoundDef", -1) for the virtual lookup; the
//   manager's CDefPointer<CDefClassBase> temporary (hidden result homed in the dead string slot) is
//   converting-copied into the typed local (addref), then released through the ref-count base's two inline accessors
//   and nulled; `*out = result` is the inlined assignment (release old, store, addref); the local's destructor
//   releases (`dec [esi+4] ; jne`). The reference count is `mutable` (the pointers hold const defs). Frame pointer and size optimisation from the TU's pragmas (the two forced inlines keep the size TU from calling them out of line).
#pragma optimize("y", off)
#pragma optimize("s", on)

#include "fable_string.h"

struct CRefCounted {                       // second base: the count sits at +4 behind the vptr
    mutable long m_count;
    void DecRef() const { --m_count; }
    long GetRefCount() const { return m_count; }
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy() const;          // slot 1
};

class CDefBase : public CDefVirtual, public CRefCounted {
public:
    void Release() const                   // CDefPointer<T>: destructor and assignment
    {
        if (--m_count == 0)
            Destroy();
    }
    void ReleaseTemporary() const          // CDefPointer<CDefClassBase>: the manager's temporary
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

class CDefClassBase;
class CEntitySoundDef : public CDefBase { };

template <class T> class CDefPointer;

template <> class CDefPointer<CDefClassBase> {         // the manager's (protected overload's) return type
public:
    CDefBase* m_p;
    CDefPointer() { }
    __forceinline ~CDefPointer()                       // forced: the size TU would call it out of line
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};

template <class T> class CDefPointer {
public:
    T* m_p;
    CDefPointer() { }
    CDefPointer(const CDefPointer<CDefClassBase>& other) : m_p((T*)other.m_p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer()
    {
        if (m_p)
            m_p->Release();
    }
    __forceinline CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) {
                m_p->Release();
                m_p = 0;
            }
            m_p = other.m_p;
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
};

class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (protected non-const overload; hidden result pointer)
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual const long* GetSubDefIndex(const CCharString& name) const;   // slot 14 (+0x38)
    unsigned char _pad[0x1c - 4];
    CDefinitionManager* PDefManager;       // +0x1C (Ego_r CDefClassBase::PDefManager)
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;
};

template <>
bool CDefClassBase::GetSubDef<CEntitySoundDef>(CDefPointer<const CEntitySoundDef>* out) const
{
    const long* pIndex;
    {
        CCharString name("CEntitySoundDef", -1);
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<const CEntitySoundDef> result(PDefManager->GetPDefFromGlobalIndex(*pIndex));
        *out = result;
        return true;
    }
    return false;
}
