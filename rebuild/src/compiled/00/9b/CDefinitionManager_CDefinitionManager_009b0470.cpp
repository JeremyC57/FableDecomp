// CDefinitionManager::CDefinitionManager(const CWideString& compiledFileName, void (*errorCallback)(CCharString*)) -- the definition manager: CResourceList root, empty path lists / symbol-map pointers / vector maps / arrays, the error callback, an empty current filename, the compiled file name = the compiled-defs directory + the argument, the empty recompiled-defs map, two fresh CSymbolMaps (the symbol map seeded with the "NULL" symbol and finalised), and the +0x34 word cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B0470, length 368 bytes (+ int3 padding), `ret 8`. Vtable 0x0129B28C (CDefinitionManager).
// Callees: CResourceList::CResourceList @ 0x009FC570 (the base), CCharString::CCharString() @ 0x0099E4B0, operator+(const CWideString&,
// const CWideString&) @ 0x0099BE70 (`__fastcall`, hidden result pointer in ecx, left operand in edx: the global 0x013CA7DC), malloc
// @ 0x00BFEA0E (the STLport map header node, 24 bytes), operator new @ 0x00BFEA1A (0x24-byte CSymbolMap), CSymbolMap::CSymbolMap
// @ 0x009E5200, CCountedPointer<CSymbolMap>::Reset(CSymbolMap*) @ 0x009B1F50, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString
// @ 0x0099EAE0, CSymbolMap::AddSymbolDirty(const CCharString&, long) @ 0x009E5420, CSymbolMap::Finalise @ 0x009E5C30.
// Layout (retail = engine/CDefinitionManager.h minus 4 per 16-byte PDB vector): SymbolPathList +0x40, CompilePathList +0x4c, PSymbolMap
// +0x58, PDefinitionSymbolMap +0x60, NameCRCToIndexMap +0x68, DefClassInfo +0x78, CompressedDefData +0x88, CompressedOffsets +0x94,
// InstantiatedDefs +0xa4, ErrorCallback +0xb0, CurrentFilename +0xb4, CompiledFileName +0xb8, DependencyCheckCRC +0xbc (untouched),
// UseSafeBinaryFormat +0xc0, RecompiledDefs +0xc4; the CResourceList part holds the word at +0x34 cleared last.
// Shape: members in declaration order with one ebx zero register. The vectors are modelled as STLport's _Vector_base
// (start, finish, and _M_end_of_storage as an _STLP_alloc_proxy deriving from the empty allocator, built from an
// `allocator_type()` temporary), and CArray<T> as a thin class deriving from that vector (the PDB sizes CArray<unsigned char>
// like a vector: 16 bytes, 12 in retail). With plain three-pointer vectors and a three-field CArray the scheduler hoists the
// ErrorCallback argument load and the lea edi/ebp of the two counted pointers too early. The CVectorMaps are
// {12-byte vector, pad byte, bool sorted at +0xd}; the map ctor is the inlined STLport _Rb_tree() (null start, malloc'd
// header, red, no parent, self left/right, zero count); each Reset takes a null-checked `new CSymbolMap`; the "NULL" symbol
// is a scoped named string.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
class CCharString { public:
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CWideString { public:                                   // non-POD (user ctor): the by-value operator+ result travels through a hidden pointer in ecx
    void* PStringData;
    CWideString();                                     // 0x0099B4D0 (not called here)
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
extern CWideString g_CompiledDefsDirectory;           // 0x013CA7DC
template <class T> struct CAllocator { CAllocator() {} CAllocator(const CAllocator&) {} };
template <class T> struct CAllocProxy : CAllocator<T> {
    T* _M_data;
    CAllocProxy(const CAllocator<T>& a, T* p) : CAllocator<T>(a), _M_data(p) {}
};
template <class T> struct CVectorBase {
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) {}
};
template <class T> struct CRetailVector : CVectorBase<T> {   // STLport vector (12 bytes)
    CRetailVector() : CVectorBase<T>(CAllocator<T>()) {}
};
template <class T> class CVectorMap { public:                 // 16 bytes: the pair vector and two flags (only the second is reset)
    CRetailVector<T> Entries;
    bool Locked;                                       // +0x0c (left uninitialised)
    bool Sorted;                                       // +0x0d
    CVectorMap() : Sorted(false) {}
};
template <class T> class CArray : public CRetailVector<T> { public:  // Lionhead's vector wrapper (12 bytes retail)
};
struct CRecompiledDefNode {                            // 24-byte node: pair<unsigned long, CDefPointer<CDefClassBase>>
    unsigned char _M_color;
    CRecompiledDefNode* _M_parent;
    CRecompiledDefNode* _M_left;
    CRecompiledDefNode* _M_right;
    unsigned long _M_key;
    void* _M_value;
};
template <class Node> struct CRetailMap {              // STLport _Rb_tree (12 bytes: header, node count, compare)
    Node* _M_header;
    unsigned long _M_node_count;
    unsigned char _M_compare[4];
    CRetailMap() : _M_header(0)
    {
        _M_header = (Node*)malloc(sizeof(Node));
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};
class CSymbolMap { public:                                    // 0x24 bytes
    unsigned char _body[0x24];
    CSymbolMap();                                      // 0x009E5200
    void AddSymbolDirty(const CCharString& name, long value);   // 0x009E5420
    void Finalise();                                   // 0x009E5C30
};
struct SCountBlock;
template <class T> class CCountedPointer { public:
    T*           m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
    void Reset(T* object);                             // 0x009B1F50
    T* operator->() const { return m_pObject; }
};
class CResourceList { public:                                 // the manager's base (0x40 bytes with the vptr)
    virtual void v00();
    unsigned char _body[0x34 - 4];
    long m_resourceCount;                              // +0x34 (name unverified)
    unsigned char _pad38[0x40 - 0x38];
    CResourceList();                                   // 0x009FC570
};
struct CDefClassInfo; struct CInstantiatedDefInfo;
class CDefinitionManager : public CResourceList {      // vtable 0x0129B28C
public:
    CRetailVector<CWideString>                 SymbolPathList;        // +0x40
    CRetailVector<CWideString>                 CompilePathList;       // +0x4c
    CCountedPointer<CSymbolMap>                PSymbolMap;            // +0x58
    CCountedPointer<CSymbolMap>                PDefinitionSymbolMap;  // +0x60
    CVectorMap<unsigned long>                  NameCRCToIndexMap;     // +0x68
    CVectorMap<CDefClassInfo>                  DefClassInfo;          // +0x78
    CArray<unsigned char>                      CompressedDefData;     // +0x88
    CVectorMap<unsigned long>                  CompressedOffsets;     // +0x94
    CArray<CInstantiatedDefInfo>               InstantiatedDefs;      // +0xa4
    void (__cdecl* ErrorCallback)(CCharString*);                      // +0xb0
    CCharString                                CurrentFilename;       // +0xb4
    CWideString                                CompiledFileName;      // +0xb8
    unsigned long                              DependencyCheckCRC;    // +0xbc
    bool                                       UseSafeBinaryFormat;   // +0xc0
    CRetailMap<CRecompiledDefNode>             RecompiledDefs;        // +0xc4
    CDefinitionManager(const CWideString& compiledFileName, void (__cdecl* errorCallback)(CCharString*));
};

CDefinitionManager::CDefinitionManager(const CWideString& compiledFileName, void (__cdecl* errorCallback)(CCharString*))
    : CResourceList(), ErrorCallback(errorCallback), CurrentFilename(), CompiledFileName(g_CompiledDefsDirectory + compiledFileName),
      RecompiledDefs()
{
    UseSafeBinaryFormat = false;
    PDefinitionSymbolMap.Reset(new CSymbolMap());
    PSymbolMap.Reset(new CSymbolMap());
    {
        CCharString nullName("NULL");
        PSymbolMap->AddSymbolDirty(nullName, 0);
    }
    PSymbolMap->Finalise();
    m_resourceCount = 0;
}