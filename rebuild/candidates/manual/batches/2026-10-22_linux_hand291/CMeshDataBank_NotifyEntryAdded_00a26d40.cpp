// CMeshDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data) -- grow the
//   entry-pointer table with nulls up to `index`; in a read-only bank build the info object in place in the typed buffer
//   (C3DMeshInfo for types 1/2/4/5, CPhysicsMeshInfo for 3, C3DAnimationInfo for 6/10/7/8/9) and point at its back(),
//   otherwise grow the dynamic counted-pointer buffer to index+1 and Reset its slot to a new info object; store the entry
//   pointer, call InitialiseEntry (slot 21) and the CBankFile base NotifyEntryAdded
// Leaf: NotifyEntryAdded
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A26D40, 744 bytes (`ret 0Ch`).
// Callees: vector<CMeshDataBankEntry*>::_M_insert_overflow 0x00A2BE00 (manifest "std_vector_insert_4bytes_variant2"),
//   C3DMeshInfo copy 0x00A2A400 / ~ 0x00A24930 / out-of-line ctor 0x00A248F0 (manifest "CSoundInit::CSoundInit"; the
//   new-expression calls it, the temporary is built inline), vector<C3DMeshInfo>::_M_insert_overflow 0x00A2C7F0,
//   push_back for CPhysicsMeshInfo 0x00A2D190 and C3DAnimationInfo 0x00A2D200, ~C3DAnimationInfo 0x00A25970,
//   vector<CCountedPointer<CMeshDataBankEntry>>::resize 0x00A2DBC0, CCountedPointer::Reset 0x00A29D20, operator new
//   0x00BFEA1A, InitialiseEntry (vtable 0x0129CE94 slot 21 = 0x00A276D0), CBankFile::NotifyEntryAdded 0x009CBC20.
// Layout (retail): CBankFile::ReadOnly +0x8D; CMeshDataBank MeshesAndAnimations +0x380, MeshInfoBuffer +0x38C,
//   PhysicsMeshInfoBuffer +0x398, AnimationInfoBuffer +0x3A4, DynamicInfoBuffer +0x3B0 (12-byte STLport vectors, as the
//   landed ctor 0x00A27030); entry vtables C3DMeshInfo 0x0129CD3C (0x60 bytes), CPhysicsMeshInfo 0x0129CD50 (0x10),
//   C3DAnimationInfo 0x0129CD64 (0x28); CMeshDataBankEntry Bank +4, Index +8.
// Shape: the type tests are inline bool helpers (fixes block order); the max is a ternary `size() > index+1 ? size() :
//   index+1`; the grow loop is if + do-while, and the pointer vector's overflow insert gets a same-TU read-only noinline
//   stand-in so the push_back(0) temporary is loop-invariant (store hoisted, zero promoted to edi before the guard) --
//   found by the Fable 5.1 advisor.
inline void* operator new(unsigned int, void* where) { return where; }

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };

template <class T> struct CArray {                  // retail 12 bytes: STLport _Vector_base layout
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return *(_M_start + i); }
    T& back() { return *(_M_finish - 1); }
};

struct CMeshDataBank;
struct CCPPointerInfo;

struct CMeshDataBankEntry {
    virtual void Release();
    CMeshDataBank* Bank;                            // +0x04
    unsigned long Index;                            // +0x08
    CMeshDataBankEntry(CMeshDataBank* bank, unsigned long index) : Bank(bank), Index(index) { }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct C3DBoundingSphere {
    C3DVector Center;
    float Radius;
    C3DBoundingSphere() : Center(0.0f, 0.0f, 0.0f), Radius(0.0f) { }
};

struct C3DMeshInfo : CMeshDataBankEntry {           // 0x60 bytes, vtable 0x0129CD3C
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

struct CPhysicsMeshInfo : CMeshDataBankEntry {      // 0x10 bytes, vtable 0x0129CD50
    void* MeshData;                                 // +0x0c
    CPhysicsMeshInfo(unsigned long type, unsigned long index, CMeshDataBank* bank)
        : CMeshDataBankEntry(bank, index), MeshData(0)
    {
    }
};

struct C3DAnimationInfo : CMeshDataBankEntry {      // 0x28 bytes, vtable 0x0129CD64
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

struct CBankFile {
    virtual void v00();
    unsigned char _pad_0x04[0x88];
    bool RetailMode;                                // +0x8c
    bool ReadOnly;                                  // +0x8d
    unsigned char _pad_0x8e[0xd6];
    virtual void NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data);   // 0x009CBC20
    bool IsReadOnly() const { return ReadOnly; }
};

struct CMeshDataBank : CBankFile {
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
