// CCombatManager::DeregisterAttackedCombatant(CThingCreatureBase* attacked) -- look the creature's combatant record up; while it still has attackers, deregister each (the record goes once the last attacker leaves, so the function returns then); with none, clear its last attacker, attack pattern and index, drop its being-attacked bit and, when it is no longer a registered combatant, erase its record
// Leaf: ?DeregisterAttackedCombatant@CCombatManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006ED8C0, length 242 bytes (`ret 4`).
// Callees: Combatants._M_find 0x006F0250 / erase(key) 0x006F50B0 (STLport map<unsigned __int64, CCombatantInfo>),
//          DeregisterAttackingCombatant 0x006ED510, CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor (PLastAttacker = 0
//          through a CIntelligentPointer<CThing> temporary, vtable 0x01238C6C), the attack-pattern def's slot 1,
//          CThingCreatureBase::IsInMeleeAsAttacker 0x00662410.
// Layout: the landed CCombatManager_Retail model (Combatants +0x10, node key +0x10, info +0x18); retail CCombatantInfo has a
//          0x30-byte attacker part, so DefenderInfo sits at +0x34 (attackers map, PLastAttacker +0x28, PAttackPattern +0x30,
//          AttackPatternIndex +0x34); the being-attacked flag is bit 3 of creature +0x15c.
#include "fable_compiler.h"
class CThing;
class CThingCreatureBase { public:
    unsigned char    _pad[0x18];
    unsigned __int64 UniqueID;                         // +0x18
    unsigned char    _pad20[0x15c - 0x20];
    unsigned char    _bits15c_0 : 3;
    unsigned char    IsBeingAttacked : 1;              // +0x15c bit 3
    unsigned char    _bits15c_4 : 4;
    unsigned __int64 GetUID() const { return UniqueID; }
public:
    bool IsInMeleeAsAttacker() const;   // 0x00662410
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { T* item = other.GetPItem(); SetPItem(item); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct CDefVirtual {
    virtual ~CDefVirtual();                            // slot 0
    virtual void Destroy();                            // slot 1
};
class CAttackPatternDef : public CDefVirtual { public:
    long RefCount;                                     // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
FABLE_FORCEINLINE bool DecRefToZero(long& count) { return --count == 0; }
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer& operator=(T* p)
    {
        if (m_p) {
            m_p->Release();
            m_p = 0;
        }
        m_p = p;
        return *this;
    }
};
struct SAttackerNode {                                 // map<unsigned __int64, CThingCreatureBase*> node
    char             _M_color;
    SAttackerNode*   _M_parent;
    SAttackerNode*   _M_left;
    SAttackerNode*   _M_right;
    unsigned __int64 first;                            // +0x10
    CThingCreatureBase* second;                        // +0x18
};
struct CAttackerMap {                                  // STLport map (12 bytes)
    SAttackerNode* _M_header;
    unsigned long  _M_node_count;
    unsigned long  _M_key_compare;
    unsigned long size() const { return _M_node_count; }
    SAttackerNode* begin() const { return _M_header->_M_left; }
};
class CDefendingCombatantInfo { public:                       // retail 0x40 bytes
    CAttackerMap  Attackers;                           // +0x00
    unsigned char CombatWheel[0x0c];                   // +0x0c
    float         LastVictimWheelUpdatePos[3];         // +0x18
    long          LastWheelUpdateWorldFrame;           // +0x24
    CIntelligentPointer<CThing> PLastAttacker;            // +0x28
    CDefPointer<CAttackPatternDef> PAttackPattern;     // +0x30
    long          AttackPatternIndex;                  // +0x34
    long          FrameForNextInstruction;             // +0x38
};
struct CCombatantInfo {
    CThingCreatureBase* Parent;                        // +0x00
    unsigned char       AttackerInfo[0x30];            // +0x04 CAttackingCombatantInfo (retail 0x30)
    CDefendingCombatantInfo DefenderInfo;              // +0x34
};
namespace std {
template <class K, class V> struct pair { K first; V second; };
template <class P> struct _Select1st { };
template <class K> struct less { };
template <class T> class allocator { };
template <class V> struct _Rb_tree_node {
    char           _M_color;
    _Rb_tree_node* _M_parent;
    _Rb_tree_node* _M_left;
    _Rb_tree_node* _M_right;
    V              _M_value_field;                     // first +0x10, second +0x18
};
}
long TmplSink_s7_1(const void* self, long seen);   // stand-in: the work erase does (outside the bytes graded here)
long TmplSink_s7_0(const void* self, long seen);   // stand-in: the work _M_find does (outside the bytes graded here)
namespace std {
// STLport 4.6 map<unsigned __int64, CCombatantInfo> tree (Combatants). The two members used here are same-TU stand-ins: template
// bodies are compiled in the caller's TU; each reads the key, stores nothing, lets no argument escape.
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    _Rb_tree_node<V>* _M_header;
    unsigned long     _M_node_count;
    unsigned long     _M_key_compare;
    struct iterator { _Rb_tree_node<V>* _M_node; iterator(_Rb_tree_node<V>* n) : _M_node(n) { } V& operator*() const { return _M_node->_M_value_field; } };
    template <class _KT> iterator find(const _KT& key) { return iterator(_M_find(key)); }
private:
    template <class _KT> __declspec(noinline) _Rb_tree_node<V>* _M_find(const _KT& key) const      // 0x006F0250 (private in STLport 4.6 _tree.h)
    {
        long r = TmplSink_s7_0(this, 0 + *(const long*)&key);
        return (_Rb_tree_node<V>*)r;
    }
public:
    __declspec(noinline) unsigned int erase(const K& key)                   // 0x006F50B0
    {
        long r = TmplSink_s7_1(this, 0 + *(const long*)&key);
        return (unsigned int)r;
    }
};
}
typedef std::pair<const unsigned __int64, CCombatantInfo> SCombatantPair;
typedef std::_Rb_tree<unsigned __int64, SCombatantPair, std::_Select1st<SCombatantPair>, std::less<unsigned __int64>,
                      std::allocator<SCombatantPair> > CCombatantMap;
class CCombatManager {
public:
    virtual void v00();
    unsigned char _sequences[0x0c];                    // +0x04 RegisteredCombatSequences
    CCombatantMap Combatants;                          // +0x10
    void DeregisterAttackingCombatant(CThingCreatureBase* attacker);   // 0x006ED510
    void DeregisterAttackedCombatant(CThingCreatureBase* attacked);
};
long NearSink_s7_0(const void* self, long seen);   // stand-in: the work DeregisterAttackingCombatant does outside this TU
#pragma auto_inline(off)
// Same-TU stand-in for CCombatManager::DeregisterAttackingCombatant @ 0x006ED510 (compiled in retail's TU): reads its const reference / pointer-to-const
// arguments, updates its out-parameters, and lets no argument escape.
__declspec(noinline) void CCombatManager::DeregisterAttackingCombatant(CThingCreatureBase* attacker)
{
    NearSink_s7_0(this, (long)attacker);
}
#pragma auto_inline(on)

void CCombatManager::DeregisterAttackedCombatant(CThingCreatureBase* attacked)
{
    CCombatantInfo* info = &(*Combatants.find(attacked->GetUID())).second;
    long count = info->DefenderInfo.Attackers.size();
    while (count > 0) {
        DeregisterAttackingCombatant(info->DefenderInfo.Attackers.begin()->second);
        --count;
        if (count == 0)
            return;
    }
    info->DefenderInfo.PLastAttacker = 0;
    info->DefenderInfo.PAttackPattern = 0;
    info->DefenderInfo.AttackPatternIndex = -1;
    attacked->IsBeingAttacked = 0;
    if (!attacked->IsInMeleeAsAttacker())
        Combatants.erase(attacked->GetUID());
}