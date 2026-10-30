// CDefinitionManager::CompileSubDefinition(CCharString text, const CCharString& subName, long parentIndex) -- compile the text of sub def `subName` of parent def `parentIndex`: reuse the parent's existing sub def, move it over from another parent (remove it there, instantiate it here, hand it the old parent), or instantiate a new one; then persist the text into it through a string parser + persist context and mark it compiled
// Leaf: CompileSubDefinition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AE150, length 568 bytes, `ret 0Ch` (text by value, destroyed here).
// Callees: CDefinitionManager::LoadDef @ 0x009AD6E0 (GetPDefFromGlobalIndex inlined), InstantiateSubDef @ 0x009ADAE0
//          (CDefPointer by value), parent def vslots 13 (+0x34 RemoveSubDef) / 15 (+0x3c GetPSubDefInfo), sub def vslots
//          18 (+0x48 Persist) / 19 (+0x4c SetParent), CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, CStringParser
//          @ 0x00404720, list dtor @ 0x004046B0, ~CBaseClass @ 0x0099A2E0, CPersistContext @ 0x009BADD0, ~CPersistContext
//          @ 0x009BBBF0.
// Layout: retail CDefinitionManager (vectors 12 bytes): PSymbolMap +0x58, InstantiatedDefs +0xa4, ErrorCallback +0xb0
//   (PDB +0x60/+0xbc/+0xcc); retail CStringParser 0x28 bytes (STLport list 4 bytes; PDB 0x30).
// Shape: two counted-pointer flavours. InstantiateSubDef's by-value result (address escapes) releases through the
//   ref-count base's DecRef/GetRefCount pair (`add eax,4` + forwarded count) and nulls itself; the named parent /
//   old-parent / sub-def pointers come from an inline GetPDefFromGlobalIndex returning the parent-def pointer whose
//   dtor is the short `dec [r+4] ; jne` Release and whose raw-pointer ctor addrefs through AddRef() (that keeps the
//   return copy's `inc [ecx+4]` apart from the assignment's increment). The "" file name is passed as const char*
//   so its CCharString temporary is built before the first push.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
typedef void (*TErrorCallback)(CCharString*);

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CStringParserCommentList {
    void* _M_node;
    ~CStringParserCommentList();                             // 0x004046B0
};
class CStringParser : public CBaseClass { public:
    const char*              String;                         // +0x04
    unsigned long            BaseLineOffset;                 // +0x08
    CCharString              Filename;                       // +0x0c
    CCharString              TempCharString;                 // +0x10
    CStringParserCommentList Comments;                       // +0x14
    TErrorCallback           ErrorCallback;                  // +0x18
    long                     CursorPos[3];                   // +0x1c
    CStringParser(const CCharString& text, TErrorCallback callback, const CCharString& filename, long baseLine);   // 0x00404720
};
class CSymbolMap;
class CDefinitionManager;
class CPersistContext { public:
    unsigned char _body[0x2c];
    CPersistContext(CStringParser* parser, TErrorCallback callback, bool isNew, CDefinitionManager* manager,
                    CSymbolMap* symbols, CCharString* text);  // 0x009BADD0
    ~CPersistContext();                                       // 0x009BBBF0
};

struct CRefCounted {                                         // second base: the count sits at +4 behind the vptr
    long IVRefCount;
    void AddRef() { ++IVRefCount; }
    void DecRef() { --IVRefCount; }
    long GetRefCount() const { return IVRefCount; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                                  // slot 1
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual void RemoveSubDef(const CCharString& name);      // slot 13 (+0x34)
    virtual void v14();
    virtual struct CSubDefInfo* GetPSubDefInfo(const CCharString& name);   // slot 15 (+0x3c)
    virtual void v16(); virtual void v17();
    virtual void Persist(CPersistContext& context);          // slot 18 (+0x48)
    virtual void SetParent(class CDefClassBase* parent);    // slot 19 (+0x4c)
};
class CDefClassBase : public CDefVirtual, public CRefCounted { public:
    unsigned char _pad08[0x24 - 8];
    unsigned char Flags;                                     // +0x24
    void Release() { if (--IVRefCount == 0) Destroy(); }
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CSubDefInfo {
    long GlobalIndex;                                        // +0
    long ParentIndex;                                        // +4
};
struct CDefPointer {
    CDefClassBase* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(CDefClassBase* p) : Object(p) { if (Object) ++Object->IVRefCount; }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }
    ~CDefPointer() { if (Object) { Object->ReleaseTemporary(); Object = 0; } }
    CDefPointer& operator=(const CDefPointer& o)
    {
        if (Object != o.Object) {
            if (Object) { Object->Release(); Object = 0; }
            Object = o.Object;
            if (Object) ++Object->IVRefCount;
        }
        return *this;
    }
    CDefClassBase* operator->() const { return Object; }
};
struct CParentDefPointer {                                   // CDefPointer<CParentDefClassBase>
    CDefClassBase* Object;
    CParentDefPointer() : Object(0) { }
    CParentDefPointer(CDefClassBase* p) : Object(p) { if (Object) Object->AddRef(); }
    ~CParentDefPointer() { if (Object) Object->Release(); }
    CParentDefPointer& operator=(const CDefPointer& o)
    {
        if (Object != o.Object) {
            if (Object) { Object->Release(); Object = 0; }
            Object = o.Object;
            if (Object) ++Object->IVRefCount;
        }
        return *this;
    }
    CParentDefPointer& operator=(const CParentDefPointer& o)
    {
        if (Object != o.Object) {
            if (Object) { Object->Release(); Object = 0; }
            Object = o.Object;
            if (Object) ++Object->IVRefCount;
        }
        return *this;
    }
    CDefClassBase* operator->() const { return Object; }
};
struct CInstantiatedDefInfo {
    long          ClassName;                                 // +0x00
    long          InstantiationName;                         // +0x04
    unsigned long ClassIndex;                                // +0x08
    CDefPointer   PDef;                                      // +0x0c
};

class CDefinitionManager { public:
    unsigned char          _pad00[0x58];
    CSymbolMap*            PSymbolMap;                       // +0x58
    unsigned char          _pad5c[0xa4 - 0x5c];
    CInstantiatedDefInfo** InstantiatedDefs;                 // +0xa4
    unsigned char          _pada8[0xb0 - 0xa8];
    TErrorCallback         ErrorCallback;                    // +0xb0
    void LoadDef(unsigned long globalIndex);                 // 0x009AD6E0
    CDefPointer InstantiateSubDef(const CCharString& name, unsigned long parentIndex);   // 0x009ADAE0
    CDefPointer GetPDefFromGlobalIndex(long globalIndex)
    {
        LoadDef(globalIndex);
        return InstantiatedDefs[globalIndex]->PDef;
    }
    CParentDefPointer GetPParentDefFromGlobalIndex(long globalIndex)
    {
        LoadDef(globalIndex);
        return InstantiatedDefs[globalIndex]->PDef.Object;
    }
    void CompileSubDefinition(CCharString text, const CCharString& subName, long parentIndex);
};

void CDefinitionManager::CompileSubDefinition(CCharString text, const CCharString& subName, long parentIndex)
{
    CParentDefPointer parent = GetPParentDefFromGlobalIndex(parentIndex);
    CSubDefInfo* info = parent->GetPSubDefInfo(subName);
    CParentDefPointer subDef;
    bool isNew;
    if (info) {
        if (info->ParentIndex != parentIndex) {
            CParentDefPointer oldParent = GetPParentDefFromGlobalIndex(info->GlobalIndex);
            parent->RemoveSubDef(subName);
            subDef = InstantiateSubDef(subName, parentIndex);
            subDef->SetParent(oldParent.Object);
            isNew = false;
        } else {
            subDef = GetPParentDefFromGlobalIndex(info->GlobalIndex);
            isNew = false;
        }
    } else {
        subDef = InstantiateSubDef(subName, parentIndex);
        isNew = true;
    }
    if (subDef.Object) {
        CStringParser parser(text, ErrorCallback, "", 0);
        CPersistContext context(&parser, ErrorCallback, isNew, this, PSymbolMap, &text);
        subDef->Persist(context);
        subDef->Flags |= 5;
    }
}