// CDefinitionManager::ValidateDefs(CValidateDefinitionInitBase* init) -- for every def class, validate each of its instantiated defs except the first (the class default) unless the def is flagged as already validated, passing the def's instantiation name
// Grade: DIFFER(137v162) -- retail spills the class iterator to a frame word and strength-reduces `&it->GlobalIndices` into edi (`lea edi,[eax+0Ch]`, both stepped by 0x18); the lane keeps the iterator in edi and indexes +0xc/+0x10
// Leaf: ValidateDefs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009ACD00, length 162 bytes, `ret 4`.
// Callees: CDefStringTable::GetString(long) @ 0x009D49B0 (by-value CCharString) on the global def string table
// @ 0x013CA828, ~CCharString @ 0x0099EAE0, CDefClassBase virtual slot 23 (+0x5c) Validate(init, name).
// Layout: retail CDefinitionManager (engine/CDefinitionManager.h carries the PDB offsets; retail containers are 4 bytes
// narrower): DefClassInfo CVectorMap<ulong, CDefClassInfo> element array first/last at +0x78/+0x7c (PDB +0x84), the
// InstantiatedDefs CArray data at +0xa4 (PDB +0xbc). Element pair (0x18 bytes): key +0, CDefClassInfo {Name +4,
// AllocFunc +8, GlobalIndices CArray<ulong> first +0xc / last +0x10 / end +0x14}. CInstantiatedDefInfo (PDB): ClassName
// +0, InstantiationName (a CDefString table index) +4, ClassIndex +8, PDef +0xc. The def's "validated" flag is bit 1
// of the byte at +0x24 (CDefClassBase bit fields).
// Shape: the class iterator lives in a frame word with the indices array pointer strength-reduced into edi; the inner
// index (ebp) starts at 1 with a top-tested unsigned `size()` (`sar 2 ; jbe`); the name is a named local destroyed
// after the virtual call.
struct CCharString {
    void* PStringData;
    ~CCharString();                                // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long index);             // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;           // 0x013CA828

struct CValidateDefinitionInitBase;
struct CDefClassBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void Validate(CValidateDefinitionInitBase* init, const CCharString& name);   // slot 23 (+0x5c)
    unsigned char _pad[0x20];
    unsigned char Flags;                           // +0x24 (bit 1: validated)
};
struct CInstantiatedDefInfo {
    long ClassName;                                // +0x00 CDefString
    long InstantiationName;                        // +0x04 CDefString
    unsigned long ClassIndex;                      // +0x08
    CDefClassBase* PDef;                           // +0x0c CDefPointer<CDefClassBase>
};
template <class T> struct CArray {                 // retail 12 bytes
    T* First;
    T* Last;
    T* End;
    unsigned long size() const { return Last - First; }
    T& operator[](unsigned long i) { return First[i]; }
};
struct CDefClassInfo {
    void* Name;                                    // +0x00 CCharString
    void* AllocFunc;                               // +0x04
    CArray<unsigned long> GlobalIndices;           // +0x08
};
struct SDefClassEntry {                            // pair<unsigned long, CDefClassInfo>
    unsigned long Key;
    CDefClassInfo Info;
};

class CDefinitionManager {
public:
    unsigned char _pad0[0x78];
    SDefClassEntry* DefClassFirst;                 // +0x78 DefClassInfo vector
    SDefClassEntry* DefClassLast;                  // +0x7c
    unsigned char _pad1[0x24];
    CInstantiatedDefInfo** InstantiatedDefs;       // +0xa4 CArray data
    void ValidateDefs(CValidateDefinitionInitBase* init);
};

void CDefinitionManager::ValidateDefs(CValidateDefinitionInitBase* init)
{
    for (SDefClassEntry* it = DefClassFirst; it != DefClassLast; ++it) {
        for (unsigned long i = 1; i < it->Info.GlobalIndices.size(); ++i) {
            CInstantiatedDefInfo* info = InstantiatedDefs[it->Info.GlobalIndices[i]];
            if (!(info->PDef->Flags & 2)) {
                CCharString name = g_DefStringTable.GetString(info->InstantiationName);
                info->PDef->Validate(init, name);
            }
        }
    }
}
