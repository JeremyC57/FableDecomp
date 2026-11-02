// Standalone C++03: original handle/member cleanup bodies with traced dependencies.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
class CBaseClass {
public:
    CBaseClass();                                                        // 0x0099A2D0
    virtual ~CBaseClass();                                               // slot 0  0x0099A2E0
};
class CIVCountedPointeeBase {
public:
    CIVCountedPointeeBase();
    virtual ~CIVCountedPointeeBase() { }                                 // slot 0  0x004664E0 (scalar deleting; inline dtor)
    virtual void Delete() const;                                         // slot 1  0x0042D930
    void AddRef() const { ++IVRefCount; }                                // FableWin.pdb ?AddRef@CIVCountedPointeeBase@@QBEXXZ (inline)
    long Release() const { return --IVRefCount; }                        // FableWin.pdb ?Release@CIVCountedPointeeBase@@QBEJXZ (inline)
    long GetRefCount() const { return IVRefCount; }                      // FableWin.pdb ?GetRefCount@CIVCountedPointeeBase@@QBEJXZ (inline)
private:
    mutable unsigned long IVRefCount;                                    // +0x04 (mutable: the const accessors count; see above)
};
class CEnginePrimitiveHandle;
namespace std {
    template<class T> struct less;
    template<class T> class allocator;
    template<class A, class B> struct pair;
    template<class K, class V, class C, class A> class map;
}
enum EEnginePrimitiveStatIndex {
    ENGINE_PRIM_STAT_VERTICES,
    ENGINE_PRIM_STAT_TRIANGLES,
    ENGINE_PRIM_STAT_TEXTURES
};
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase {
    friend class CEnginePrimitiveHandle;
public:
    CEnginePersistentPrimitiveBase(long type);
    virtual ~CEnginePersistentPrimitiveBase();
    virtual void CollectStats(std::map<unsigned long,unsigned long,
        std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,
        EEnginePrimitiveStatIndex, unsigned long) const = 0;
protected:
    virtual void NotifyReleased(const CEnginePrimitiveHandle&);
    virtual bool WasRendered() const;
private:
    long Type;
};

template<class T> class CIVCountedPointer {
public:
    CIVCountedPointer() : Object(0) { }
    ~CIVCountedPointer() { Release(); }
    void Reset(T* object);
    T* operator->() const { return static_cast<T*>(Object); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object ? Object->GetRefCount() : 0; }
    void Release() {
        CIVCountedPointeeBase* object = Object;
        if (object && object->Release() == 0) object->Delete();
        Object = 0;
    }
private:
    CIVCountedPointeeBase* Object;
};
class CEnginePrimitiveHandle : public CBaseClass {
public:
    CEnginePrimitiveHandle() { }
    virtual ~CEnginePrimitiveHandle() {
        if (!Data.IsNull() && Data->GetRefCount() == 1) Data->NotifyReleased(*this);
    }
    void SetData(CEnginePersistentPrimitiveBase* data);
    void Clear() {
        if (!Data.IsNull() && Data->GetRefCount() == 1) Data->NotifyReleased(*this);
        Data.Release();
    }
private:
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;
};

static unsigned notifications, deletes, baseDestructions;
static unsigned callbackMode;
static int mainReached;
static CEnginePersistentPrimitiveBase* replacement;
static void auditExit();
class TestAuditRegistration { public: TestAuditRegistration() { if (atexit(auditExit)) abort(); } };
static TestAuditRegistration auditRegistration;
namespace { CEnginePrimitiveHandle hit_pos; }

static void check(bool v) { if (!v) abort(); }
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() {
    void* data = this;
    memcpy(&data, (const unsigned char*)this + sizeof(CBaseClass), sizeof(data));
    check(data == 0);
    ++baseDestructions;
}
CIVCountedPointeeBase::CIVCountedPointeeBase() : IVRefCount(0) { }
void CIVCountedPointeeBase::Delete() const { abort(); }
CEnginePersistentPrimitiveBase::CEnginePersistentPrimitiveBase(long type) : Type(type) { }
CEnginePersistentPrimitiveBase::~CEnginePersistentPrimitiveBase() { }
void CEnginePersistentPrimitiveBase::NotifyReleased(const CEnginePrimitiveHandle&) { abort(); }
bool CEnginePersistentPrimitiveBase::WasRendered() const { return false; }
// Setup boundaries expose only genuine PDB methods, not alternate cleanup logic.
template<class T> void CIVCountedPointer<T>::Reset(T* value) {
    Release();
    Object = value;
    if (Object) Object->AddRef();
}
void CEnginePrimitiveHandle::SetData(CEnginePersistentPrimitiveBase* data) { Data.Reset(data); }
class TestPrimitive : public CEnginePersistentPrimitiveBase {
public:
    TestPrimitive() : CEnginePersistentPrimitiveBase(0) { }
    virtual void Delete() const { ++deletes; }
    virtual void CollectStats(std::map<unsigned long,unsigned long,
        std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,
        EEnginePrimitiveStatIndex, unsigned long) const { abort(); }
protected:
    virtual void NotifyReleased(const CEnginePrimitiveHandle& h) {
        ++notifications;
        check(GetRefCount() == 1);
        if (callbackMode == 1) const_cast<CEnginePrimitiveHandle&>(h).SetData(0);
        if (callbackMode == 2) const_cast<CEnginePrimitiveHandle&>(h).SetData(replacement);
    }
};
static TestPrimitive* finalPrimitive;
int main() {
    mainReached = 1;
    const unsigned counts[] = {0, 1, 2, 17};
    for (unsigned ci = 0; ci != 4; ++ci) {
        TestPrimitive primitive;
        for (unsigned i = 1; i < counts[ci]; ++i) primitive.AddRef();
        unsigned beforeN = notifications, beforeD = deletes, beforeB = baseDestructions;
        {
            CEnginePrimitiveHandle h;
            if (counts[ci]) h.SetData(&primitive);
        }
        check(notifications - beforeN == (counts[ci] == 1 ? 1u : 0u));
        check(deletes - beforeD == (counts[ci] == 1 ? 1u : 0u));
        check(baseDestructions - beforeB == 1);
        check(primitive.GetRefCount() == (counts[ci] ? counts[ci] - 1 : 0));
    }
    for (unsigned mode = 1; mode != 3; ++mode) {
        TestPrimitive oldPrimitive, newPrimitive;
        newPrimitive.AddRef();
        replacement = &newPrimitive;
        callbackMode = mode;
        unsigned beforeN = notifications, beforeD = deletes;
        { CEnginePrimitiveHandle h; h.SetData(&oldPrimitive); }
        check(notifications - beforeN == 1 && deletes - beforeD == 1);
        check(oldPrimitive.GetRefCount() == 0 && newPrimitive.GetRefCount() == 1);
    }
    callbackMode = 0;
    finalPrimitive = new TestPrimitive;
    hit_pos.SetData(finalPrimitive);
    notifications = deletes = baseDestructions = 0;
    return 0;
}
static void auditExit() {
    check(mainReached && notifications == 1 && deletes == 1 && baseDestructions == 1);
    check(finalPrimitive->GetRefCount() == 0);
    delete finalPrimitive;
    puts("01229fc0_TEST PASS");
}
