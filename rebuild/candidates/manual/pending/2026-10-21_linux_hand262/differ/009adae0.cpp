// Grade: DIFFER(402v417) -- frame + register allocation: retail spills the class info into the dead className parameter slot (frame 8: the CDefString temp + the spilled this) and hands the __true_type tag the parentIndex slot; ours gives the class info its own local (frame 0x10), pushes ebx/edi after the null test, folds the flag clear into `and byte ptr [esi+0x24],0xfd` (retail: load/and/store interleaved with `push 0x10`) and emits the null-path copy+release as `inc [ecx+4] ; dec [ecx+4]` (retail: through edx/eax).
// CDefinitionManager::InstantiateSubDef(const CCharString& className, unsigned long parentIndex) -- create a new def of the named class:
// give it the next global index, clear its flag bit 1, record a CInstantiatedDefInfo for it, add the index to the class's instance list,
// load the parent def and tell it about the new sub-def (slot 12), and return a counted pointer to the new def (null for an unknown class,
// after loading the parent).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009ADAE0, length 417 bytes, `ret 0xc` (hidden result + 2 args).
// Callees: 0x009AD580 class-info lookup (manifest "CVectorMap_FindByKey"); class info +4 factory function pointer; operator new 0x00BFEA1A;
// 0x009D5060 CDefString set-from-CCharString (manifest "CRegion::SetMiniMapRegionExitTextOffsetX" is wrong: it stores the string-table
// index from 0x009D4F90 and returns it, not `this`); STLport _M_insert_overflow 0x009B4BD0 (CInstantiatedDefInfo*) / 0x00464ACE (unsigned
// long); CDefinitionManager::LoadDef 0x009AD6E0; def slot 1 (+4) release, slot 12 (+0x30) sub-def notification.
// Layout: local model after the landed 0x009ADA90 (retail InstantiatedDefs CArray at +0xa4, CInstantiatedDefInfo {ClassName, Instantiation
// -1, ClassIndex, PDef}); class info: name CCharString +0, factory +4, instance index vector +8; def: refcount +4, manager +0x1c, global
// index +0x20, flag byte +0x24.
// Shape: global index is a `long` (the push_back of it then copies into a conversion temp, as retail does: unsigned long kept it in memory);
// CDefString built by an inline ctor around the out-of-line setter and passed by const reference.
// Tried: explicit __true_type() temp (byte store), empty user ctor, bool vs unsigned char bit-fields, by-value CDefString, the dtor through
// a named count (414: fixes nothing on the null path).
// Leaf: InstantiateSubDef
inline void* operator new(unsigned int, void* p) throw() { return p; }
struct CCharString;
struct CDefString {
    long Index;
    CDefString(const CCharString& name) { Set(name); }
    void Set(const CCharString& name);             // 0x009D5060
};
struct __true_type { __true_type() {} };
template <class T> struct CArray {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    void _M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int fillLen, bool atEnd);   // 0x009B4BD0 / 0x00464ACE
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
    T& operator[](unsigned long i) { return _M_start[i]; }
};
struct CDefinitionManager;
struct CDefClassBase {
    virtual void v00();
    virtual void Release();                        // slot 1
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void AddSubDef(const CCharString& className, unsigned long globalIndex, unsigned long parentIndex);   // slot 12 (+0x30)
    long RefCount;                                 // +0x04
    unsigned char _pad08[0x1c - 8];
    CDefinitionManager* PManager;                  // +0x1c
    unsigned long GlobalIndex;                     // +0x20
    bool FlagA : 1;                                // +0x24 bit 0
    bool Recompiled : 1;                           // +0x24 bit 1
};
struct CDefPointer {
    CDefClassBase* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(CDefClassBase* object, int) : Object(object) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            Object->RefCount = Object->RefCount + 1;
    }
    ~CDefPointer()
    {
        if (--Object->RefCount == 0)
            Object->Release();
    }
    CDefClassBase* operator->() const { return Object; }
};
struct CInstantiatedDefInfo {
    CDefString  ClassName;                         // +0x00
    long        InstantiationName;                 // +0x04
    unsigned long ClassIndex;                      // +0x08
    CDefClassBase* PDef;                           // +0x0c CDefPointer
    CInstantiatedDefInfo(const CDefString& className, unsigned long classIndex, CDefClassBase* def)
        : ClassName(className), InstantiationName(-1), ClassIndex(classIndex), PDef(def)
    {
        ++def->RefCount;
    }
};
struct CDefClassInfo {
    const CCharString& Name() const { return *(const CCharString*)this; }   // +0x00 CCharString
    unsigned char _pad00[4];
    CDefClassBase* (*Create)();                    // +0x04
    CArray<unsigned long> Instances;               // +0x08
};
struct CDefinitionManager {
    unsigned char _pad[0xa4];
    CArray<CInstantiatedDefInfo*> InstantiatedDefs;   // +0xa4
    CDefClassInfo* FindClassInfo(const CCharString& className);   // 0x009AD580
    void LoadDef(unsigned long globalIndex);       // 0x009AD6E0
    CDefPointer GetPDefFromGlobalIndex(unsigned long globalIndex)
    {
        LoadDef(globalIndex);
        return *(CDefPointer*)&InstantiatedDefs[globalIndex]->PDef;
    }
    CDefPointer InstantiateSubDef(const CCharString& className, unsigned long parentIndex);
};

CDefPointer CDefinitionManager::InstantiateSubDef(const CCharString& className, unsigned long parentIndex)
{
    CDefClassInfo* info = FindClassInfo(className);
    if (info) {
        long globalIndex = InstantiatedDefs.size();
        unsigned long classIndex = info->Instances.size();
        CDefPointer def(info->Create(), 0);
        def->Recompiled = false;
        def->PManager = this;
        def->GlobalIndex = globalIndex;
        InstantiatedDefs.push_back(new CInstantiatedDefInfo(CDefString(info->Name()), classIndex, def.Object));
        info->Instances.push_back(globalIndex);
        CDefPointer parent = GetPDefFromGlobalIndex(parentIndex);
        parent->AddSubDef(info->Name(), globalIndex, parentIndex);
        return def;
    }
    GetPDefFromGlobalIndex(parentIndex);
    return CDefPointer();
}
