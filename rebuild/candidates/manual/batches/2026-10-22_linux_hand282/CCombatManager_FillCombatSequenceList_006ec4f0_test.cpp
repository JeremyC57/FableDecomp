#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
// CCombatManager::FillCombatSequenceList(CThingCreatureBase& attacker, CThingCreatureBase& defender, vector<CCountedPointer<CCombatSequenceBase>>& list) -- build the attacker's combat sequences against the defender: one CCombatSequenceGeneric per CombatSequenceDef index (registered via slot 19), then one per registered CombatSequence name (made by the name's creator, slot 1 called on the stored pointer); stops at the first empty name
// Leaf: FillCombatSequenceList
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EC4F0, 697 bytes (`ret 0Ch`).
// Callees: CThingCreatureBase::GetCombatTypeDef 0x00666D10, CThing::GetDefName 0x004C7CC0, CDefStringTable::GetString
//          0x009D49B0 (table 0x013CA828), `__fastcall` operator+(const char*, const CCharString&) 0x0099F690 (literal
//          0x01261E60), ~CCharString 0x0099EAE0, GetDef<CCombatSequenceDef> 0x006F0EF0 (on World+0x10), operator new
//          0x00BFEA1A, CCombatSequenceBase::CCombatSequenceBase 0x008B0AB0 (generic vtable 0x01261D74), the count block's
//          destroy 0x006EE3D0, operator delete 0x00BFE9BC, vector::_M_insert_overflow 0x006F42E0, the registered-sequence
//          map's find 0x006F1360, CBasicString<char>::operator==(const char*) 0x004115A0 ("" 0x0122D70E).
// Shape: GetDef is a same-TU template instantiation in retail, so its body is visible here (noinline stand-in): with
//          it external VC7.1 treats the out CDefPointer as escaped and re-reads it after every call (the zero then
//          lives in ebx for the whole loop); visible, the def is loaded once into ebp. The warning trace is built and
//          dropped (debug output compiled out). The empty-name test `break`s (retail jumps straight to the epilogue).
//          The creator is copied to a local before the call (`mov eax,[eax+14h]; call eax`). The counted-pointer
//          temporary's release zeroes both words; STLport push_back inline with the __false_type tag.
// Layout: retail CCombatManager: RegisteredCombatSequences map +0x04 (12 bytes), Combatants +0x10, World +0x20;
//          CCombatTypeDef: CombatSequence vector<CCharString> +0x50, CombatSequenceDef vector<long> +0x5c (PDB +0x58/+0x68);
//          CCombatSequenceGeneric::Def +0x28 (retail 0x2c bytes); map node value {CCharString +0x10, creator +0x14}.
#include <string.h>
struct __false_type { __false_type() {} };
inline void* operator new(size_t, void* where) { return where; }
void __cdecl operator delete(void* block);          // 0x00BFE9BC

#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    ~CCharString();                             // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
struct CDefString { long m_index; };
struct CDefStringTable { CCharString GetString(CDefString name); };   // 0x009D49B0
extern CDefStringTable g_DefStringTable;           // 0x013CA828
inline void CombatTrace(const CCharString&) {}

template<class T> struct CRetailVector {
    T* first;
    T* last;
    T* end_of_storage;
};
struct CCombatTypeDef {
    unsigned char _pad[0x50];
    CRetailVector<CCharString> CombatSequence;      // +0x50
    CRetailVector<long> CombatSequenceDef;          // +0x5c
};
struct CThingCreatureBase {
    unsigned char _pad[0x10];
    unsigned char ThingClass;                       // +0x10
    CDefString GetDefName() const;                  // 0x004C7CC0
    const CCombatTypeDef* GetCombatTypeDef();       // 0x00666D10
};
struct CCombatSequenceDef {
    virtual void v00();
    virtual void Destroy();
    long RefCount;
};
template<class T> struct CDefPointer {
    T* Data;
    CDefPointer() : Data(0) {}
    CDefPointer(const CDefPointer& o) { T* d = o.Data; Data = d; if (d) ++d->RefCount; }
    ~CDefPointer() { if (Data) { if (--Data->RefCount == 0) Data->Destroy(); } }
};
CCombatSequenceDef* LookupSequenceDef(long index);   // stand-in: the def table lookup
struct CDefManager {
    void GetDef(long index, CDefPointer<CCombatSequenceDef>* out);   // 0x006F0EF0
};
#pragma auto_inline(off)
// Same-TU body of the GetDef<CCombatSequenceDef> instantiation 0x006F0EF0 (the out pointer is only written through).
__declspec(noinline) void CDefManager::GetDef(long index, CDefPointer<CCombatSequenceDef>* out)
{
    if (index > 0) {
        CCombatSequenceDef* def = LookupSequenceDef(index);
        if (def) {
            out->Data = def;
            ++def->RefCount;
        }
    }
}
#pragma auto_inline(on)
struct CWorld {
    unsigned char _pad[0x10];
    CDefManager* DefManager;                        // +0x10
};
struct CRegisteredSequenceMap;
struct CCombatSequenceBase {
    virtual void v00();
    virtual void OnCreate();                        // slot 1 (+0x04)
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18();
    virtual void Register(CRegisteredSequenceMap* map);   // slot 19 (+0x4c)
    unsigned char _body[0x28 - 4];
    CCombatSequenceBase(CThingCreatureBase& creature, CThingCreatureBase& target);   // 0x008B0AB0
};
struct CCombatSequenceGeneric : CCombatSequenceBase {    // vtable 0x01261D74
    virtual void v00();
    CDefPointer<CCombatSequenceDef> Def;            // +0x28
    CCombatSequenceGeneric(CThingCreatureBase& creature, CThingCreatureBase& target, const CDefPointer<CCombatSequenceDef>& def)
        : CCombatSequenceBase(creature, target), Def(def) {}
};
typedef CCombatSequenceBase* (__fastcall* CSequenceCreator)(CThingCreatureBase&, CThingCreatureBase&);
struct SSequenceNode {
    long colour; SSequenceNode* parent; SSequenceNode* left; SSequenceNode* right;
    CCharString first;                              // +0x10
    CSequenceCreator second;                        // +0x14
};
struct CRegisteredSequenceMap {
    SSequenceNode* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    SSequenceNode* find(const CCharString& key);    // 0x006F1360
    SSequenceNode* end() { return _M_header; }
};

void __fastcall DestroyCombatSequence(void* object);   // 0x006EE3D0
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(DestroyCombatSequence), m_pObject(object) {}
};
struct CSequencePointer {
    CCombatSequenceBase* Data;
    SCountBlock* Info;
    CSequencePointer(CCombatSequenceBase* object) : Data(object)
    {
        if (object)
            Info = new SCountBlock(object);
        else
            Info = 0;
    }
    CSequencePointer(const CSequencePointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->m_references; }
    ~CSequencePointer()
    {
        if (Info && --Info->m_references == 0) {
            Info->m_pfnDestroy(Info->m_pObject);
            operator delete(Info);
        }
        Data = 0;
        Info = 0;
    }
    CCombatSequenceBase* operator->() const { return Data; }
};
struct CSequenceVector {
    CSequencePointer* _M_start;
    CSequencePointer* _M_finish;
    CSequencePointer* _M_end_of_storage;
    CSequencePointer& back() { return *(_M_finish - 1); }
    void _M_insert_overflow(CSequencePointer* pos, const CSequencePointer& x, const __false_type&,
                            unsigned long fill_len, bool atend);   // 0x006F42E0
    void push_back(const CSequencePointer& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CSequencePointer(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
void* GrowSequenceVector(void* vector, unsigned long count);
#pragma auto_inline(off)
__declspec(noinline) void CSequenceVector::_M_insert_overflow(CSequencePointer* pos, const CSequencePointer& x, const __false_type& p2, unsigned long fill_len, bool atend)
{
    CSequencePointer* p = (CSequencePointer*)GrowSequenceVector(this, fill_len);
    if (p)
        new (p) CSequencePointer(x);
}
#pragma auto_inline(on)

struct CCombatManager {
    virtual void v00();
    CRegisteredSequenceMap RegisteredCombatSequences;   // +0x04
    unsigned char _combatants[0xc];                     // +0x10
    unsigned char _pad1c[4];
    CWorld* World;                                      // +0x20
    void FillCombatSequenceList(CThingCreatureBase& attacker, CThingCreatureBase& defender, CSequenceVector& list);
};

void CCombatManager::FillCombatSequenceList(CThingCreatureBase& attacker, CThingCreatureBase& defender, CSequenceVector& list)
{
    if (attacker.ThingClass == 2)
        return;
    const CCombatTypeDef* def = attacker.GetCombatTypeDef();
    if (def->CombatSequence.first == def->CombatSequence.last && def->CombatSequenceDef.first == def->CombatSequenceDef.last) {
        CCharString name = g_DefStringTable.GetString(attacker.GetDefName());
        CCharString message = "WARNING : No combat sequences registered for this combat type - in creature " + name;
        CombatTrace(message);
    }
    for (const long* it = def->CombatSequenceDef.first; it != def->CombatSequenceDef.last; ++it) {
        CDefPointer<CCombatSequenceDef> sequenceDef;
        World->DefManager->GetDef(*it, &sequenceDef);
        CCombatSequenceBase* sequence = new CCombatSequenceGeneric(attacker, defender, sequenceDef);
        sequence->Register(&RegisteredCombatSequences);
        list.push_back(CSequencePointer(sequence));
    }
    for (const CCharString* it2 = def->CombatSequence.first; it2 != def->CombatSequence.last; ++it2) {
        if (*it2 == "")
            break;
        SSequenceNode* found = RegisteredCombatSequences.find(*it2);
        if (found == RegisteredCombatSequences.end())
            continue;
        CSequenceCreator create = found->second;
        list.push_back(CSequencePointer(create(attacker, defender)));
        list.back()->OnCreate();
    }
}

// ---- behaviour test ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)

static CCharStringData g_blocks[64]; static int g_nblocks;
static int g_strings, g_strDtors, g_inStub;
struct StubGuard { StubGuard() { g_inStub++; } ~StubGuard() { g_inStub--; } };   // copies made inside the stubs are not counted
static CCharString MakeString(const char* text)
{
    StubGuard guard;
    CCharString s;
    CCharStringData* d = &g_blocks[g_nblocks++];
    d->Data.PData = (char*)text; d->Data.StringLength = (unsigned long)strlen(text); d->NoRefs = 1;
    s.PStringData = d; g_strings++;
    return s;
}
bool CBasicStringChar::operator==(const char* text) const { return strcmp(PData, text) == 0; }
CCharString::~CCharString() { if (!g_inStub) g_strDtors++; }
static const char* g_plusLeft; static const char* g_plusRight;
CCharString __fastcall operator+(const char* left, const CCharString& right)
{ StubGuard guard; g_plusLeft = left; g_plusRight = right.PStringData->Data.PData; return MakeString("joined"); }
static long g_nameIndex; static int g_getStrings;
CCharString CDefStringTable::GetString(CDefString name) { StubGuard guard; g_nameIndex = name.m_index; g_getStrings++; return MakeString("CREATURE_X"); }
CDefStringTable g_DefStringTable;

static CCombatTypeDef* g_typeDef; static int g_typeDefCalls;
CDefString CThingCreatureBase::GetDefName() const { CDefString s; s.m_index = 77; return s; }
const CCombatTypeDef* CThingCreatureBase::GetCombatTypeDef() { g_typeDefCalls++; return g_typeDef; }

static int g_defDestroys;
void CCombatSequenceDef::v00() {}
void CCombatSequenceDef::Destroy() { g_defDestroys++; }
static CCombatSequenceDef g_defs[4];
CCombatSequenceDef* LookupSequenceDef(long index) { return index <= 4 ? &g_defs[index - 1] : 0; }

static int g_registers, g_creates; static void* g_registerMap; static CCombatSequenceBase* g_order[16]; static int g_norder;
#define VF(n) void CCombatSequenceBase::v##n() {}
VF(00) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18)
void CCombatSequenceBase::OnCreate() { g_creates++; g_order[g_norder++] = this; }
void CCombatSequenceBase::Register(CRegisteredSequenceMap* map) { g_registers++; g_registerMap = map; }
CCombatSequenceBase::CCombatSequenceBase(CThingCreatureBase& creature, CThingCreatureBase& target)
{ memset(_body, 0, sizeof _body); *(CThingCreatureBase**)(_body + 0) = &creature; *(CThingCreatureBase**)(_body + 8) = &target; }
void CCombatSequenceGeneric::v00() {}
static CThingCreatureBase* CreatureOf(CCombatSequenceBase* s) { return *(CThingCreatureBase**)(s->_body + 0); }
static CThingCreatureBase* TargetOf(CCombatSequenceBase* s) { return *(CThingCreatureBase**)(s->_body + 8); }

static int g_seqDestroys;
void __fastcall DestroyCombatSequence(void* object) { (void)object; g_seqDestroys++; }

static CSequencePointer* g_grown; static int g_grows;
void* GrowSequenceVector(void* vector, unsigned long count)
{
    CSequenceVector* v = (CSequenceVector*)vector;
    long n = (long)(v->_M_finish - v->_M_start);
    CSequencePointer* storage = (CSequencePointer*)new char[(n + count + 4) * sizeof(CSequencePointer)];
    memcpy(storage, v->_M_start, n * sizeof(CSequencePointer));
    v->_M_start = storage; v->_M_finish = storage + n + count; v->_M_end_of_storage = storage + n + count + 4;
    g_grows++; g_grown = storage + n;
    return storage + n;
}

static SSequenceNode g_header, g_nodeA, g_nodeC;
static CCombatSequenceBase* g_madeA; static CCombatSequenceBase* g_madeC;
static CThingCreatureBase* g_madeBy[2]; static int g_made;
static CCombatSequenceBase* __fastcall MakeA(CThingCreatureBase& a, CThingCreatureBase& d) { g_madeBy[0] = &a; g_madeBy[1] = &d; g_made++; return g_madeA; }
static CCombatSequenceBase* __fastcall MakeC(CThingCreatureBase& a, CThingCreatureBase& d) { g_madeBy[0] = &a; g_madeBy[1] = &d; g_made++; return g_madeC; }
static int g_finds; static const char* g_findKeys[8];
SSequenceNode* CRegisteredSequenceMap::find(const CCharString& key)
{
    const char* k = key.PStringData->Data.PData; g_findKeys[g_finds++] = k;
    if (strcmp(k, "A") == 0) return &g_nodeA;
    if (strcmp(k, "C") == 0) return &g_nodeC;
    return _M_header;
}
void CCombatManager::v00() {}

int main()
{
    static CCombatManager mgr; CCombatManager* m = &mgr;
    static CWorld world; static CDefManager defs;
    world.DefManager = &defs; m->World = &world;
    m->RegisteredCombatSequences._M_header = &g_header;
    g_nodeA.second = MakeA; g_nodeC.second = MakeC;
    static CThingCreatureBase attacker, defender;
    static CCombatTypeDef typeDef; g_typeDef = &typeDef;
    static double storageBytes[8 * sizeof(CSequencePointer) / sizeof(double)];
    CSequencePointer* storage = (CSequencePointer*)storageBytes;
    CSequenceVector list; list._M_start = list._M_finish = storage; list._M_end_of_storage = storage + 8;

    // a ThingClass-2 attacker gets nothing
    attacker.ThingClass = 2;
    m->FillCombatSequenceList(attacker, defender, list);
    CHECK(g_typeDefCalls == 0 && list._M_finish == storage);

    // no sequences at all: the warning is built (name + literal) and dropped, list untouched
    attacker.ThingClass = 0;
    m->FillCombatSequenceList(attacker, defender, list);
    CHECK(g_typeDefCalls == 1 && g_getStrings == 1 && g_nameIndex == 77 && g_plusRight && strcmp(g_plusRight, "CREATURE_X") == 0);
    CHECK(strcmp(g_plusLeft, "WARNING : No combat sequences registered for this combat type - in creature ") == 0);
    CHECK(g_strings == 2 && g_strDtors == 2 && list._M_finish == storage);

    // three def indices (index 9 has no def): one generic sequence each, registered, def ref-counted
    long ids[3] = { 1, 9, 2 };
    typeDef.CombatSequenceDef.first = ids; typeDef.CombatSequenceDef.last = ids + 3;
    g_defs[0].RefCount = 5; g_defs[1].RefCount = 1;
    m->FillCombatSequenceList(attacker, defender, list);
    CHECK(g_getStrings == 1 && list._M_finish == storage + 3 && g_registers == 3 && g_registerMap == &m->RegisteredCombatSequences);
    CHECK(g_defs[0].RefCount == 6 && g_defs[1].RefCount == 2 && g_defDestroys == 0);
    CCombatSequenceGeneric* s0 = (CCombatSequenceGeneric*)storage[0].Data;
    CCombatSequenceGeneric* s1 = (CCombatSequenceGeneric*)storage[1].Data;
    CCombatSequenceGeneric* s2 = (CCombatSequenceGeneric*)storage[2].Data;
    CHECK(s0->Def.Data == &g_defs[0] && s1->Def.Data == 0 && s2->Def.Data == &g_defs[1]);
    CHECK(CreatureOf(s0) == &attacker && TargetOf(s0) == &defender);
    CHECK(storage[0].Info && storage[0].Info->m_references == 1 && storage[0].Info->m_pObject == s0 && storage[0].Info->m_pfnDestroy == DestroyCombatSequence);
    CHECK(storage[2].Info && storage[2].Info->m_references == 1 && g_seqDestroys == 0 && g_creates == 0);

    // names: "A" made and created, "B" unregistered (skipped), then a null name stops the walk ("C" never looked up)
    typeDef.CombatSequenceDef.first = typeDef.CombatSequenceDef.last = 0;
    static CCombatSequenceBase* madeA; madeA = new CCombatSequenceGeneric(attacker, defender, s0->Def); g_madeA = madeA;
    static CCombatSequenceBase* madeC; madeC = new CCombatSequenceGeneric(attacker, defender, s0->Def); g_madeC = madeC;
    CCharString names[4];
    names[0] = MakeString("A"); names[1] = MakeString("B"); names[2].PStringData = 0; names[3] = MakeString("C");
    typeDef.CombatSequence.first = names; typeDef.CombatSequence.last = names + 4;
    list._M_finish = list._M_start = storage; list._M_end_of_storage = storage + 1;   // full: push goes through the overflow
    m->FillCombatSequenceList(attacker, defender, list);
    CHECK(g_finds == 2 && strcmp(g_findKeys[0], "A") == 0 && strcmp(g_findKeys[1], "B") == 0);
    CHECK(g_made == 1 && g_madeBy[0] == &attacker && g_madeBy[1] == &defender);
    CHECK(list._M_finish - list._M_start == 1 && list._M_start[0].Data == madeA && list._M_start[0].Info->m_references == 1);
    CHECK(g_creates == 1 && g_order[0] == madeA && g_registers == 3);

    // an empty (non-null) name also stops the walk; overflow growth path
    names[0] = MakeString("C"); names[1] = MakeString(""); names[2] = MakeString("A");
    typeDef.CombatSequence.last = names + 3;
    list._M_start = list._M_finish = storage; list._M_end_of_storage = storage;
    m->FillCombatSequenceList(attacker, defender, list);
    CHECK(g_grows == 1 && list._M_start[0].Data == madeC && list._M_start[0].Info->m_references == 1);
    CHECK(g_creates == 2 && g_order[1] == madeC && g_finds == 3 && g_made == 2);
    if (fails == 0) printf("006ec4f0_TEST PASS\n"); else printf("006ec4f0_TEST FAIL\n");
    return fails;
}
