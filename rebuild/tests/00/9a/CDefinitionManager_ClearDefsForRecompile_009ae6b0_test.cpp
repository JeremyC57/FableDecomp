// Behaviour test for 0x009AE6B0 CDefinitionManager::ClearDefsForRecompile (model copy).
#include <cstdio>
#include <cstring>
#include <cctype>

static int my_stricmp(const char* a, const char* b)
{
    for (;; ++a, ++b) {
        int ca = std::tolower((unsigned char)*a), cb = std::tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        if (!ca) return 0;
    }
}
struct CStringData { char* Data; int Length; };
struct CCharString {
    CStringData* PData;
    bool IsEmpty() const { return PData == 0 || PData->Length == 0; }
    const char* c_str() const { return PData->Data; }
};
static int g_Destroyed = 0;
struct CCountedObject {
    long Ref; int KeyValue;
    virtual ~CCountedObject() {}
    virtual void Destroy() { ++g_Destroyed; }
    virtual void GetKey(int* out) { *out = KeyValue; }
};
struct CCountedPointer {
    CCountedObject* P;
    CCountedPointer() : P(0) {}
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
struct CDefKey { unsigned int Index; int Data; };
struct CDefEntry {
    long Type;
    CCountedPointer Obj;
    static CCountedPointer Pass(CCountedPointer p) { return p; }
};
static const char* g_TypeNames[] = { "CCreatureDef", "CObjectDef" };
struct CDefMap {
    CDefKey Keys[8]; CCountedPointer Slots[8]; int Count;
    CCountedPointer* operator[](const CDefKey& key) {
        for (int i = 0; i < Count; ++i) if (Keys[i].Index == key.Index && Keys[i].Data == key.Data) return &Slots[i];
        Keys[Count] = key; return &Slots[Count++];
    }
};
struct CDefinitionManager {
    CDefEntry** DefsBegin; CDefEntry** DefsEnd; CDefMap Map; int Cleared[8];
    void ClearDef(unsigned int index) { Cleared[index] = 1; }
    void ClearDefsForRecompile(const CCharString& name)
    {
        CDefKey key;
        unsigned int count = DefsEnd - DefsBegin;
        for (unsigned int i = 0; i < count; ++i) {
            CDefEntry* def = DefsBegin[i];
            if (!name.IsEmpty() && my_stricmp(g_TypeNames[def->Type], name.c_str()) != 0)
                continue;
            ClearDef(i);
            CCountedPointer obj = CDefEntry::Pass(DefsBegin[i]->Obj);
            def->Obj.P->GetKey(&key.Data);
            key.Index = i;
            *Map[key] = obj;
        }
    }
};

int main()
{
    CCountedObject o0, o1, o2; o0.Ref = 1; o0.KeyValue = 10; o1.Ref = 1; o1.KeyValue = 11; o2.Ref = 1; o2.KeyValue = 12;
    CDefEntry e0, e1, e2; e0.Type = 0; e0.Obj.P = &o0; e1.Type = 1; e1.Obj.P = &o1; e2.Type = 0; e2.Obj.P = &o2;
    CDefEntry* defs[3] = { &e0, &e1, &e2 };
    CDefinitionManager m; m.DefsBegin = defs; m.DefsEnd = defs + 3; m.Map.Count = 0;
    for (int i = 0; i < 8; ++i) m.Cleared[i] = 0;
    char nm[] = "ccreaturedef"; CStringData sd = { nm, 12 }; CCharString name; name.PData = &sd;
    m.ClearDefsForRecompile(name);
    bool ok = m.Cleared[0] == 1 && m.Cleared[1] == 0 && m.Cleared[2] == 1 && m.Map.Count == 2 &&
              m.Map.Keys[0].Index == 0 && m.Map.Keys[0].Data == 10 && m.Map.Keys[1].Index == 2 && m.Map.Keys[1].Data == 12 &&
              m.Map.Slots[0].P == &o0 && m.Map.Slots[1].P == &o2 && o0.Ref == 2 && o1.Ref == 1 && o2.Ref == 2 && g_Destroyed == 0;
    CCharString empty; empty.PData = 0;
    m.ClearDefsForRecompile(empty);
    ok = ok && m.Cleared[1] == 1 && m.Map.Count == 3 && m.Map.Slots[2].P == &o1 && o0.Ref == 2 && o1.Ref == 2 && g_Destroyed == 0;
    for (int i = 0; i < 3; ++i) m.Map.Slots[i].P = 0;
    e0.Obj.P = 0; e1.Obj.P = 0; e2.Obj.P = 0;
    if (!ok) { std::printf("009ae6b0_TEST FAIL\n"); return 1; }
    std::printf("009ae6b0_TEST PASS\n");
    return 0;
}