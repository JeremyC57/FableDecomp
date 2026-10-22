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