// std::_Construct(std::pair<CCharString,CScriptInfo>*, const std::pair<CCharString,CScriptInfo>&) -- placement copy of a named script registration: key string, the two alloc functions, script name, id and master flag (catalog label CProfileTreeEntry is propagated; neighbours are pair<CCharString,CScriptInfo> helpers)
// Retail: Fable.exe @ 0x00CB60B0, 55 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CCharString copy ctor @ 0x0099EC30 (x2).
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N00cb60b0 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct CScriptBase; struct CScriptDataBase; struct CGameScriptInterfaceBase;
typedef CScriptBase* (__cdecl *ScriptAllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);
typedef CScriptDataBase* (__cdecl *ScriptDataAllocFunc)();
struct CScriptInfo {                             // PDB sizeof 0x14
    ScriptAllocFunc     AllocFunc;               // +0x00
    ScriptDataAllocFunc MasterScriptDataAlloc;   // +0x04
    CCharString         Name;                    // +0x08
    long                ID;                      // +0x0C
    bool                MasterScript;            // +0x10
    __forceinline CScriptInfo(const CScriptInfo& o)   // the implicit memberwise copy, spelled out so VC7.1 inlines it
        : AllocFunc(o.AllocFunc), MasterScriptDataAlloc(o.MasterScriptDataAlloc), Name(o.Name), ID(o.ID), MasterScript(o.MasterScript) { }
};
struct CNamedScriptInfo {                        // std::pair<CCharString, CScriptInfo>
    CCharString first;                           // +0x00
    CScriptInfo second;                          // +0x04
    __forceinline CNamedScriptInfo(const CNamedScriptInfo& o) : first(o.first), second(o.second) { }   // pair copy, forced inline (VC7.1 otherwise calls it)
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CNamedScriptInfo* p, const CNamedScriptInfo& value)
{
    if (p != 0)
        new (p) CNamedScriptInfo(value);
}
}