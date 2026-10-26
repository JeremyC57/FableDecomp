// CDefinitionManager::ClearDefsForRecompile(const CCharString& name) -- for every def whose type name matches (case-insensitively; all defs when the name is empty): clear it (0x9AD6E0), then re-file its counted object in the recompile map (+0xc4, operator[] 0x9B7BC0) under {index, key from the object's vslot 0x48}
// Leaf: ClearDefsForRecompile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AE6B0, length 268 bytes (`ret 4`). Type-name table @ 0x013CA828 (GetName 0x9D49F0), _stricmp 0xBFEBA8, key-data ctor 0x9BAA70.
// Note: the +1/+1/-1 refcount triple is a by-value pass-through copy (param copy, return copy, param dtor); the map-slot assignment keeps the old pointer in a local (short release form).
extern "C" int __cdecl _stricmp(const char*, const char*);
struct CStringData { char* Data; int Length; };
class CCharString { public:
    CStringData* PData;
    bool IsEmpty() const { return PData == 0 || PData->Length == 0; }
    const char* c_str() const { return PData->Data; }
};
struct CCountedObject { virtual void f0(); virtual void Destroy(); long Ref; };
struct CCountedPointer {
    CCountedObject* P;
    CCountedPointer(const CCountedPointer& o) : P(o.P) { if (P) ++P->Ref; }
    ~CCountedPointer() { if (P && --P->Ref == 0) P->Destroy(); }
    CCountedPointer& operator=(const CCountedPointer& o) {
        if (P != o.P) {
            CCountedObject* old = P;
            if (old) { if (--old->Ref == 0) old->Destroy(); P = 0; }
            P = o.P;
            if (P) ++P->Ref;
        }
        return *this;
    }
};
struct CDefKeyData { CDefKeyData(); unsigned char _d[0x2c]; };
struct CDefKey { unsigned long Index; CDefKeyData Data; };
struct CDefSourceObj {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c(); virtual void v40(); virtual void v44();
    virtual void GetKey(CDefKeyData* out);
};
struct CDefEntry {
    long Type; long _04; long _08;
    CCountedPointer Obj;                                  // +0x0c
    static __forceinline CCountedPointer Pass(CCountedPointer p) { return p; }
};
struct CDefTypeNames { const char* GetName(long type); };
extern CDefTypeNames g_DefTypeNames;                      // 0x13CA828
struct CDefMap { CCountedPointer* operator[](const CDefKey& key); };
class CDefinitionManager {
public:
    void ClearDefsForRecompile(const CCharString& name);
    void ClearDef(unsigned long index);
    unsigned char _pad00[0xa4];
    CDefEntry** DefsBegin;                                // +0xa4
    CDefEntry** DefsEnd;                                  // +0xa8
    unsigned char _padac[0xc4 - 0xac];
    CDefMap Map;                                          // +0xc4
};
void CDefinitionManager::ClearDefsForRecompile(const CCharString& name)
{
    CDefKey key;
    unsigned long count = DefsEnd - DefsBegin;
    for (unsigned long i = 0; i < count; ++i) {
        CDefEntry* def = DefsBegin[i];
        if (!name.IsEmpty() && _stricmp(g_DefTypeNames.GetName(def->Type), name.c_str()) != 0)
            continue;
        ClearDef(i);
        CCountedPointer obj = CDefEntry::Pass(DefsBegin[i]->Obj);
        ((CDefSourceObj*)def->Obj.P)->GetKey(&key.Data);
        key.Index = i;
        *Map[key] = obj;
    }
}