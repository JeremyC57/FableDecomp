// Test for 0x0065F604 CDefClassBase::GetSubDef<CEntitySoundDef>: with an index the def comes back in *out with its
// reference counted once for *out (the manager's and the local's references released); without one, false and *out
// untouched; an old *out def is released on reassignment.
#include <stdio.h>
#include <string.h>
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


static int g_destroyed, g_lookups;
static char g_name[32];
CCharString::CCharString(const char* text, long) { strncpy(g_name, text, sizeof g_name - 1); storage_ = 0; }
CCharString::~CCharString() { }
void CDefVirtual::v00() { }
void CDefClassBase::v00() { } void CDefClassBase::v04() { } void CDefClassBase::v08() { } void CDefClassBase::v0c() { }
void CDefClassBase::v10() { } void CDefClassBase::v14() { } void CDefClassBase::v18() { } void CDefClassBase::v1c() { }
void CDefClassBase::v20() { } void CDefClassBase::v24() { } void CDefClassBase::v28() { } void CDefClassBase::v2c() { }
void CDefClassBase::v30() { } void CDefClassBase::v34() { }
const long* CDefClassBase::GetSubDefIndex(const CCharString&) const { return 0; }
void CDefVirtual::Destroy() const { ++g_destroyed; }
static CEntitySoundDef g_def, g_old;
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index)
{
    static CDefPointer<CDefClassBase> s_r;      // copied out bitwise; no local destructor runs here
    s_r.m_p = index == 7 ? &g_def : 0;
    if (s_r.m_p) ++s_r.m_p->m_count;                  // the manager hands out a counted reference
    return s_r;
}
class CTestDef : public CDefClassBase {
public:
    long index; bool has;
    virtual void v00() { } virtual void v04() { } virtual void v08() { } virtual void v0c() { } virtual void v10() { }
    virtual void v14() { } virtual void v18() { } virtual void v1c() { } virtual void v20() { } virtual void v24() { }
    virtual void v28() { } virtual void v2c() { } virtual void v30() { } virtual void v34() { }
    virtual const long* GetSubDefIndex(const CCharString&) const { ++g_lookups; return has ? &index : 0; }
};
int main()
{
    CDefinitionManager mgr;
    CTestDef d; d.PDefManager = &mgr; d.index = 7; d.has = true;
    g_def.m_count = 1; g_old.m_count = 1;
    CDefPointer<const CEntitySoundDef> out; out.m_p = &g_old; ++g_old.m_count;
    if (!d.GetSubDef<CEntitySoundDef>(&out)) return 1;
    if (strcmp(g_name, "CEntitySoundDef") != 0 || g_lookups != 1) return 2;
    if (out.m_p != &g_def || g_def.m_count != 2) return 3;      // owner's 1 + out's 1
    if (g_old.m_count != 1 || g_destroyed != 0) return 4;
    d.has = false;
    if (d.GetSubDef<CEntitySoundDef>(&out) || out.m_p != &g_def || g_def.m_count != 2) return 5;
    out.m_p = 0;
    fflush(stdout);
    puts("0065f604_TEST PASS");
    return 0;
}
