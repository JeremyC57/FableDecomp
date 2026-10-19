// CDefinitionManager::RemoveUnusedMemory() -- shrink every container the manager owns to its used size: the instantiated-def table, the definition symbol map, each def class's global index list and each instantiated def's sub-def table
// Leaf: RemoveUnusedMemory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AE5F0, length 182 bytes (+ int3 padding), `ret`.
// Callees: CArray<T>::DeallocateUnusedMemory instantiations -- CArray<CInstantiatedDefInfo*> @ 0x009B6EB0, CArray<ulong>
// @ 0x009B6E00, CArray<pair<ulong, CDefClassBase::CSubDefInfo>> @ 0x009B79B0 (catalog labels name them after other
// pointer-sized instantiations); CSymbolMap::RemoveUnusedMemory @ 0x009E5350 (`add ecx,4 ; jmp`); CDefClassBase virtual
// slot 24 (+0x60) returning the def's sub-def info table (may be null).
// Layout: retail CDefinitionManager (engine/CDefinitionManager.h carries PDB offsets; retail vectors/CArrays are 12
// bytes, CVectorMaps 16): PDefinitionSymbolMap.Data +0x60 (PDB +0x68), DefClassInfo element array first/last
// +0x78/+0x7c (PDB +0x84; 0x18-byte pairs with GlobalIndices at +0xc), InstantiatedDefs CArray +0xa4 (PDB +0xbc).
// CInstantiatedDefInfo: PDef +0xc.
// Shape: two index loops over `size()` re-read every pass (the 24-byte element count by reciprocal multiply, signed
// difference compared unsigned); the class element address is formed from the re-read first pointer and a 0x18 stride
// register; the sub-def table getter is called twice (test, then the call's `this`).
template <class T> struct CArray {                 // retail 12 bytes
    T* First;
    T* Last;
    T* End;
    unsigned long size() const { return Last - First; }
    T& operator[](unsigned long i) { return First[i]; }
    void DeallocateUnusedMemory();
};
struct CSymbolMap {
    void RemoveUnusedMemory();                     // 0x009E5350
};
struct CSubDefInfo { unsigned long a, b; };        // CDefClassBase::CSubDefInfo
struct SSubDefEntry { unsigned long Key; CSubDefInfo Info; };   // pair<unsigned long, CSubDefInfo>, 12 bytes
struct CDefClassBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual CArray<SSubDefEntry>* GetSubDefInfo();   // slot 24 (+0x60)
};
struct CInstantiatedDefInfo {
    long ClassName;                                // +0x00
    long InstantiationName;                        // +0x04
    unsigned long ClassIndex;                      // +0x08
    CDefClassBase* PDef;                           // +0x0c
};
struct CDefClassInfo {
    void* Name;                                    // +0x00
    void* AllocFunc;                               // +0x04
    CArray<unsigned long> GlobalIndices;           // +0x08
};
struct SDefClassEntry { unsigned long Key; CDefClassInfo Info; };   // 0x18 bytes
struct SDefClassVector {
    SDefClassEntry* First;
    SDefClassEntry* Last;
    unsigned long size() const { return Last - First; }
    SDefClassEntry& operator[](unsigned long i) { return First[i]; }
};

class CDefinitionManager {
public:
    unsigned char _pad0[0x60];
    CSymbolMap* PDefinitionSymbolMap;              // +0x60 CCountedPointer<CSymbolMap>::Data
    unsigned char _pad1[0x14];
    SDefClassVector DefClassInfo;                  // +0x78
    unsigned char _pad2[0x24];
    CArray<CInstantiatedDefInfo*> InstantiatedDefs;   // +0xa4
    void RemoveUnusedMemory();
};

void CDefinitionManager::RemoveUnusedMemory()
{
    InstantiatedDefs.DeallocateUnusedMemory();
    PDefinitionSymbolMap->RemoveUnusedMemory();
    for (unsigned long i = 0; i < DefClassInfo.size(); ++i)
        DefClassInfo[i].Info.GlobalIndices.DeallocateUnusedMemory();
    for (unsigned long j = 0; j < InstantiatedDefs.size(); ++j)
        if (InstantiatedDefs[j]->PDef->GetSubDefInfo())
            InstantiatedDefs[j]->PDef->GetSubDefInfo()->DeallocateUnusedMemory();
}
