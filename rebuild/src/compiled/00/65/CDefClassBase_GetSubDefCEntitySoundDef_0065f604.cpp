// CDefClassBase::GetSubDef<CEntitySoundDef>(CDefPointer<const CEntitySoundDef>* out) const -- the thing def's
// CEntitySoundDef sub-def: look the sub-def index up by class name, fetch the def from the definition manager into *out;
// false when the def has no such sub-def
// Leaf: ??$GetSubDef@VCEntitySoundDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCEntitySoundDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065F604, 162 bytes, thiscall `ret 4`, frame-pointer size TU.
// Identity (FINDINGS chunk 384; was the address-named GetSubDef_0065f604, manifest GetSubDef<CAbilityDef>): Ego_r.pdb
//   `??$GetSubDef@VCEntitySoundDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCEntitySoundDef@@@@@Z` (name set) and the
//   class-name literal the body builds, 'CEntitySoundDef'; its 17 callers are sound paths (CTCSoundPlayer::
//   PlayAttachedSoundEvent 0x0066B1D0, CTCDoor::PlaySound, CTCTalk::PlaySpeech, CThingCreatureBase::
//   PlayCreatureContinuousSounds, ...) that read CEntitySoundDef::SoundIdentifier from the result. The manager call is
//   Ego_r.pdb `?GetPDefFromGlobalIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@J@Z` (the const
//   overload, as `this` is const) @ 0x009ADA10; the index lookup is the def's virtual slot 14 (+0x38, descriptive name).
// Body/shape (unchanged from the landed model): a block-scoped CCharString("CEntitySoundDef", -1) for the virtual lookup;
//   the manager's CDefPointer<const CDefClassBase> temporary (hidden result homed in the dead string slot) is
//   converting-copied into the typed local (addref), then released through the ref-count base's two inline accessors and
//   nulled; `*out = result` is the inlined assignment (release old, store, addref); the local's destructor releases
//   (`dec [esi+4] ; jne`). The reference count is `mutable` (the pointers hold const defs).
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
    void ReleaseTemporary() const          // CDefPointer<const CDefClassBase>: the manager's temporary
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

class CDefClassBase;
class CEntitySoundDef : public CDefBase { };

template <class T> class CDefPointer;

template <> class CDefPointer<const CDefClassBase> {   // the manager's return type
public:
    const CDefBase* m_p;
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
    CDefPointer(const CDefPointer<const CDefClassBase>& other) : m_p((T*)other.m_p) { if (m_p) ++m_p->m_count; }
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
public:
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const;   // 0x009ADA10 (hidden result pointer)
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
    CDefinitionManager* m_pManager;        // +0x1C
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
        CDefPointer<const CEntitySoundDef> result(m_pManager->GetPDefFromGlobalIndex(*pIndex));
        *out = result;
        return true;
    }
    return false;
}
