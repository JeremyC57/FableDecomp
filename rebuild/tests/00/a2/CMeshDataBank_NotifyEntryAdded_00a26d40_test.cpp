#include <cstddef>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
// CMeshDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data) -- probe
// Leaf: NotifyEntryAdded
inline void* operator new(std::size_t, void* where) { return where; }

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };

template <class T> class CArray { public:                  // retail 12 bytes: STLport _Vector_base layout
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return *(_M_start + i); }
    T& back() { return *(_M_finish - 1); }
};

class CMeshDataBank;
class CCPPointerInfo;

class CMeshDataBankEntry { public:
    virtual void Release();
    CMeshDataBank* Bank;                            // +0x04
    unsigned long Index;                            // +0x08
    CMeshDataBankEntry(CMeshDataBank* bank, unsigned long index) : Bank(bank), Index(index) { }
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class C3DBoundingSphere { public:
    C3DVector Center;
    float Radius;
    C3DBoundingSphere() : Center(0.0f, 0.0f, 0.0f), Radius(0.0f) { }
};

class C3DMeshInfo : public CMeshDataBankEntry { public:           // 0x60 bytes, vtable 0x0129CD3C
    void* MeshData;                                 // +0x0c
    unsigned char BoundingBox[0x18];                // +0x10
    C3DBoundingSphere BoundingSphere;               // +0x28
    float SafeBoundingRadius;                       // +0x38
    long PhysicsMeshIndex;                          // +0x3c
    float LODErrors[4];                             // +0x40
    unsigned long LODDataSizes[4];                  // +0x50
    C3DMeshInfo(unsigned long type, unsigned long index, CMeshDataBank* bank)
        : CMeshDataBankEntry(bank, index), MeshData(0)
    {
        BoundingSphere.Radius = -1.0f;
        PhysicsMeshIndex = -1;
    }
    C3DMeshInfo(const C3DMeshInfo& other);          // 0x00A2A400
    ~C3DMeshInfo();                                 // 0x00A24930
};

class CPhysicsMeshInfo : public CMeshDataBankEntry { public:      // 0x10 bytes, vtable 0x0129CD50
    void* MeshData;                                 // +0x0c
    CPhysicsMeshInfo(unsigned long type, unsigned long index, CMeshDataBank* bank)
        : CMeshDataBankEntry(bank, index), MeshData(0)
    {
    }
};

class C3DAnimationInfo : public CMeshDataBankEntry { public:      // 0x28 bytes, vtable 0x0129CD64
    void* Data;                                     // +0x0c
    float Duration;                                 // +0x10
    float NonLoopingDuration;                       // +0x14
    float MovementVector[3];                        // +0x18
    float Rotation;                                 // +0x24
    C3DAnimationInfo(unsigned long type, unsigned long index, CMeshDataBank* bank)
        : CMeshDataBankEntry(bank, index), Data(0)
    {
    }
    ~C3DAnimationInfo();                            // 0x00A25970
};

struct CEntryPointerArray : CArray<CMeshDataBankEntry*> {
    void _M_insert_overflow(CMeshDataBankEntry** position, CMeshDataBankEntry* const& x, const __true_type& tag,
                            unsigned long fillLength, bool atEnd);   // 0x00A2BE00
    void push_back(CMeshDataBankEntry* const& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CMeshDataBankEntry*(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

#pragma auto_inline(off)
__declspec(noinline) void CEntryPointerArray::_M_insert_overflow(CMeshDataBankEntry** position, CMeshDataBankEntry* const& x,
                                                                  const __true_type&, unsigned long fillLength, bool atEnd)
{
    if (x == 0 && atEnd)
        _M_finish = position + fillLength;
}
#pragma auto_inline(on)
struct CMeshInfoArray : CArray<C3DMeshInfo> {
    void _M_insert_overflow(C3DMeshInfo* position, const C3DMeshInfo& x, const __false_type& tag,
                            unsigned long fillLength, bool atEnd);   // 0x00A2C7F0
    void push_back(const C3DMeshInfo& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) C3DMeshInfo(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

struct CPhysicsMeshInfoArray : CArray<CPhysicsMeshInfo> {
    void push_back(const CPhysicsMeshInfo& x);      // 0x00A2D190
};

struct CAnimationInfoArray : CArray<C3DAnimationInfo> {
    void push_back(const C3DAnimationInfo& x);      // 0x00A2D200
};

struct CEntryCountedPointer {
    CMeshDataBankEntry* Data;
    CCPPointerInfo* Info;
    CEntryCountedPointer() : Data(0), Info(0) { }
    void Reset(CMeshDataBankEntry* item);           // 0x00A29D20
};

struct CDynamicInfoArray : CArray<CEntryCountedPointer> {
    void resize(unsigned long newSize, const CEntryCountedPointer& value);   // 0x00A2DBC0
};

inline bool IsMeshType(unsigned long type) { return type == 1 || type == 2 || type == 4 || type == 5; }
inline bool IsPhysicsMeshType(unsigned long type) { return type == 3; }
inline bool IsAnimationType(unsigned long type) { return type == 6 || type == 10 || type == 7 || type == 8 || type == 9; }

template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }

class CBankFile { public:
    virtual void v00();
    unsigned char _pad_0x04[0x88];
    bool RetailMode;                                // +0x8c
    bool ReadOnly;                                  // +0x8d
    unsigned char _pad_0x8e[0xd6];
    virtual void NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data);   // 0x009CBC20
    bool IsReadOnly() const { return ReadOnly; }
};

class CMeshDataBank : public CBankFile { public:
    unsigned char _pad_0x164[0x21c];
    CEntryPointerArray MeshesAndAnimations;         // +0x380
    CMeshInfoArray MeshInfoBuffer;                  // +0x38c
    CPhysicsMeshInfoArray PhysicsMeshInfoBuffer;    // +0x398
    CAnimationInfoArray AnimationInfoBuffer;        // +0x3a4
    CDynamicInfoArray DynamicInfoBuffer;            // +0x3b0
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20();
    virtual void InitialiseEntry(unsigned long type, unsigned long index, const CArray<char>& data);   // slot 21
    void NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data);
};

void CMeshDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data)
{
    if (index >= MeshesAndAnimations.size()) {
        do
            MeshesAndAnimations.push_back(0);
        while (index >= MeshesAndAnimations.size());
    }
    CMeshDataBankEntry* entry = 0;
    if (IsReadOnly()) {
        if (IsMeshType(type)) {
            MeshInfoBuffer.push_back(C3DMeshInfo(type, index, this));
            entry = &MeshInfoBuffer.back();
        } else if (IsPhysicsMeshType(type)) {
            PhysicsMeshInfoBuffer.push_back(CPhysicsMeshInfo(type, index, this));
            entry = &PhysicsMeshInfoBuffer.back();
        } else if (IsAnimationType(type)) {
            AnimationInfoBuffer.push_back(C3DAnimationInfo(type, index, this));
            entry = &AnimationInfoBuffer.back();
        }
    } else {
        DynamicInfoBuffer.resize(DynamicInfoBuffer.size() > index + 1 ? DynamicInfoBuffer.size() : index + 1, CEntryCountedPointer());
        CEntryCountedPointer& slot = DynamicInfoBuffer[index];
        if (IsMeshType(type))
            slot.Reset(new C3DMeshInfo(type, index, this));
        else if (IsPhysicsMeshType(type))
            slot.Reset(new CPhysicsMeshInfo(type, index, this));
        else if (IsAnimationType(type))
            slot.Reset(new C3DAnimationInfo(type, index, this));
        entry = slot.Data;
    }
    MeshesAndAnimations[index] = entry;
    InitialiseEntry(type, index, data);
    CBankFile::NotifyEntryAdded(type, index, data);
}

// ---- test harness ----
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void CMeshDataBankEntry::Release() {}
C3DMeshInfo::C3DMeshInfo(const C3DMeshInfo& o) : CMeshDataBankEntry(o.Bank, o.Index), MeshData(o.MeshData) { BoundingSphere = o.BoundingSphere; PhysicsMeshIndex = o.PhysicsMeshIndex; }
C3DMeshInfo::~C3DMeshInfo() {}
C3DAnimationInfo::~C3DAnimationInfo() {}
void CMeshInfoArray::_M_insert_overflow(C3DMeshInfo* position, const C3DMeshInfo& x, const __false_type&, unsigned long, bool) { new (position) C3DMeshInfo(x); ++_M_finish; }
void CPhysicsMeshInfoArray::push_back(const CPhysicsMeshInfo& x) { new (_M_finish) CPhysicsMeshInfo(x); ++_M_finish; }
void CAnimationInfoArray::push_back(const C3DAnimationInfo& x) { *_M_finish = x; ++_M_finish; }
void CEntryCountedPointer::Reset(CMeshDataBankEntry* item) { Data = item; }
void CDynamicInfoArray::resize(unsigned long newSize, const CEntryCountedPointer& value) { while (size() < newSize) { *_M_finish = value; ++_M_finish; } }
void CBankFile::v00() {}
static int g_baseNotify = 0; void CBankFile::NotifyEntryAdded(unsigned long, unsigned long, const CArray<char>&) { ++g_baseNotify; }
static int g_init = 0; void CMeshDataBank::InitialiseEntry(unsigned long type, unsigned long index, const CArray<char>& data) { ++g_init; }
void CMeshDataBank::v02() {  }
void CMeshDataBank::v03() {  }
void CMeshDataBank::v04() {  }
void CMeshDataBank::v05() {  }
void CMeshDataBank::v06() {  }
void CMeshDataBank::v07() {  }
void CMeshDataBank::v08() {  }
void CMeshDataBank::v09() {  }
void CMeshDataBank::v10() {  }
void CMeshDataBank::v11() {  }
void CMeshDataBank::v12() {  }
void CMeshDataBank::v13() {  }
void CMeshDataBank::v14() {  }
void CMeshDataBank::v15() {  }
void CMeshDataBank::v16() {  }
void CMeshDataBank::v17() {  }
void CMeshDataBank::v18() {  }
void CMeshDataBank::v19() {  }
void CMeshDataBank::v20() {  }
int main()
{
    static CMeshDataBank bank;
    // static storage: zero-initialised, vptr set by the implicit constructor (a memset would wipe it)
    CMeshDataBankEntry* slots[16]; memset(slots, 0, sizeof(slots));
    bank.MeshesAndAnimations._M_start = slots; bank.MeshesAndAnimations._M_finish = slots; bank.MeshesAndAnimations._M_end_of_storage = slots + 16;
    static unsigned char meshBuf[sizeof(C3DMeshInfo) * 4]; C3DMeshInfo* mb = (C3DMeshInfo*)meshBuf;
    bank.MeshInfoBuffer._M_start = mb; bank.MeshInfoBuffer._M_finish = mb; bank.MeshInfoBuffer._M_end_of_storage = mb + 4;
    static unsigned char physBuf[sizeof(CPhysicsMeshInfo) * 4]; CPhysicsMeshInfo* pb = (CPhysicsMeshInfo*)physBuf;
    bank.PhysicsMeshInfoBuffer._M_start = pb; bank.PhysicsMeshInfoBuffer._M_finish = pb; bank.PhysicsMeshInfoBuffer._M_end_of_storage = pb + 4;
    CArray<char> data; data._M_start = data._M_finish = data._M_end_of_storage = 0;
    bank.ReadOnly = true;
    bank.NotifyEntryAdded(1, 5, data);       // mesh type, index 5: grows 6 slots through the placement-new path
    int ok = bank.MeshesAndAnimations.size() == 6 && slots[5] == (CMeshDataBankEntry*)mb && slots[0] == 0 && slots[4] == 0;
    ok = ok && mb[0].Index == 5 && mb[0].Bank == &bank && mb[0].PhysicsMeshIndex == -1 && mb[0].BoundingSphere.Radius == -1.0f;
    bank.NotifyEntryAdded(3, 2, data);       // physics type, index 2: no growth
    ok = ok && bank.MeshesAndAnimations.size() == 6 && slots[2] == (CMeshDataBankEntry*)pb && pb[0].Index == 2;
    ok = ok && g_baseNotify == 2;
    printf(ok ? "00a26d40_TEST PASS\n" : "00a26d40_TEST FAIL size=%lu\n", (unsigned long)bank.MeshesAndAnimations.size());
    return ok ? 0 : 1;
}