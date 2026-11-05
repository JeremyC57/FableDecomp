// CDefinitionManager::~CDefinitionManager() -- the definition manager destructor: set the vtable, ClearInstantiatedDefs, then destroy the members in reverse order (the recompiled-defs tree, the two strings, the three arrays / POD vector maps, the CDefClassInfo vector map, the two counted symbol maps, the two CWideString path vectors) and run the CResourceList base destructor
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B02E0, length 387 bytes (+ int3 padding), `ret`. Vtable 0x0129B28C (CDefinitionManager).
// Label: the manifest calls this `CDefinitionManager::Cleanup`; the body stores the vtable first, destroys every member in reverse
// declaration order and chains to the CResourceList destructor @ 0x009FC5C0, i.e. it is the (non-deleting) destructor.
// Callees: CDefinitionManager::ClearInstantiatedDefs @ 0x009AD950, CRetailTree::Erase @ 0x009B40F0 (the recompiled-defs map),
// free @ 0x00BFEA14, ~CWideString @ 0x0099B510, ~CCharString @ 0x0099EAE0, STLport __destroy_range<CDefClassInfo> @ 0x009B8070
// (`__fastcall` first/last + the __false_type tag by reference), operator delete @ 0x00BFE9BC (count blocks), ~CResourceList @ 0x009FC5C0.
// Layout (retail = engine/CDefinitionManager.h minus 4 per 16-byte PDB vector): SymbolPathList +0x40, CompilePathList +0x4c, PSymbolMap
// +0x58, PDefinitionSymbolMap +0x60, NameCRCToIndexMap +0x68, DefClassInfo +0x78, CompressedDefData +0x88, CompressedOffsets +0x94,
// InstantiatedDefs +0xa4, CurrentFilename +0xb4, CompiledFileName +0xb8, RecompiledDefs +0xc4.
// Shape: all member destructors inline with one ebp zero register: the tree dtor (erase root, reset head, zero count, free head), the
// CArray dtor (free Data when set), the POD vector dtor (free start when set), the non-POD vector dtor (destroy range, free), the
// CCountedPointer dtor (decrement the block, destroy + delete on zero, zero both words).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
struct __false_type {};
class CCharString { public:
    void* PStringData;
    ~CCharString();                                           // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    ~CWideString();                                           // 0x0099B510
};
struct CDefClassInfo {                                        // 24 bytes: a CCharString at +4 and a freed block at +0xc
    unsigned char _body[0x18];
};
void __fastcall DestroyRange_CDefClassInfo(CDefClassInfo* first, CDefClassInfo* last, const __false_type&);   // 0x009B8070
template <class T> struct CRetailVector {                     // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CRetailVector()
    {
        T* last = _M_finish;
        for (T* p = _M_start; p != last; ++p)
            p->~T();
        if (_M_start)
            free(_M_start);
    }
};
template <> struct CRetailVector<unsigned long> {             // the POD instantiation: nothing to destroy
    unsigned long* _M_start;
    unsigned long* _M_finish;
    unsigned long* _M_end_of_storage;
    ~CRetailVector()
    {
        if (_M_start)
            free(_M_start);
    }
};
template <> struct CRetailVector<CDefClassInfo> {             // the non-POD instantiation goes through the out-of-line destroy range
    CDefClassInfo* _M_start;
    CDefClassInfo* _M_finish;
    CDefClassInfo* _M_end_of_storage;
    ~CRetailVector()
    {
        __false_type tag;
        DestroyRange_CDefClassInfo(_M_start, _M_finish, tag);
        if (_M_start)
            free(_M_start);
    }
};
template <class T> class CVectorMap { public:                        // 16 bytes
    CRetailVector<T> Entries;
    bool Locked;
    bool Sorted;
};
template <class T> class CArray { public:                            // 12 bytes
    T* Data;
    long Count;
    long Capacity;
    ~CArray()
    {
        if (Data)
            free(Data);
    }
};
struct CRecompiledDefNode {
    unsigned char _M_color;
    CRecompiledDefNode* _M_parent;
    CRecompiledDefNode* _M_left;
    CRecompiledDefNode* _M_right;
    unsigned long _M_key;
    void* _M_value;
};
template <class Node> struct CRetailMap {                     // STLport _Rb_tree (12 bytes: header, node count, compare)
    Node* _M_header;
    unsigned long _M_node_count;
    unsigned char _M_compare[4];
    void Erase(Node* root);                                   // 0x009B40F0
    ~CRetailMap()
    {
        if (_M_node_count) {
            Erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
        if (_M_header)
            free(_M_header);
    }
};
class CSymbolMap;
#include "engine/CCountedPointer.h"
class CResourceList { public:                                        // the manager's base (0x40 bytes with the vptr)
    virtual void v00();
    unsigned char _body[0x40 - 4];
    ~CResourceList();                                         // 0x009FC5C0
};
struct CInstantiatedDefInfo;
class CDefinitionManager : public CResourceList {             // vtable 0x0129B28C
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
    void ClearInstantiatedDefs();                                     // 0x009AD950
    ~CDefinitionManager();
};

CDefinitionManager::~CDefinitionManager()
{
    ClearInstantiatedDefs();
}