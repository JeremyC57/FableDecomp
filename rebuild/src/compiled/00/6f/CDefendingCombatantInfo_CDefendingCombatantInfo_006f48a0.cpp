// CDefendingCombatantInfo::~CDefendingCombatantInfo() -- compiler-generated: release the attack-pattern def, the last-attacker pointer, the combat wheel's rings, then the attackers map
// Leaf: ??1CDefendingCombatantInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F48A0, length 132 bytes, plain `ret`. 11 callers, all in the combat manager:
// CCombatManager::RegisterAttackedCombatant 0x006ED270 (x3) and RegisterAttackingCombatant 0x006EDA80 (x3) on stack
// CCombatantInfo temporaries, CCombatantMap::erase 0x006F4E40, and the CCombatantInfo destroyers 0x006F4A60.. (each calls it
// on `this + 0x34`, then destroys AttackerInfo.CombatSequences at +0x28 inline). The CCombatantInfo constructor 0x006F4A00
// builds the same member set inline at +0x34 (map head of 0x20-byte nodes, wheel vector zeroed, the CIntelligentPointer<CThing>
// with vtable 0x01238C6C, a null def). The manifest label (_Dest_val<allocator<pair<EAnimComponent,CIVCountedPointer
// <CAnimComponentBase>>>>) is propagated and wrong.
// Layout (Ego_r.pdb CDefendingCombatantInfo, 68 bytes; retail drops 4 per container): Attackers (map<unsigned __int64,
// CThingCreatureBase*>) +0x00, CombatWheel (CCombatWheel { vector<CCombatRing> CombatRings }) +0x0c, LastVictimWheelUpdatePos
// +0x18, LastWheelUpdateWorldFrame +0x24, PLastAttacker +0x28, PAttackPattern +0x30, AttackPatternIndex +0x34,
// FrameForNextInstruction +0x38.
// Callees: CDefPointeeBase slot 1 (DeleteThis) through the def, CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10,
// the ring destroy loop 0x006F44E0 (`__fastcall` first/last + the iterator tag by reference), free @ 0x00BFEA14, the map's
// _M_erase 0x006EFC70 (thiscall on the tree, root on the stack; manifest "FreeTree_CTreeOwner").
// Shape: members in reverse order; the ring vector is the landed 0x006FD040 form (tag temporary in the high byte of the
// reserved `push ecx` slot), the map is the landed CRetailTree clear-then-free form.
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14

class CThing;
class CThingCreatureBase;
class CAttackPatternDef;

class CDefPointeeBase { public:
    virtual ~CDefPointeeBase();
    virtual void DeleteThis();                               // slot 1 (+4)
    unsigned long RefCount;                                  // +0x04
};

template<class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    ~CDefPointer()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->DeleteThis();
            Object = 0;
        }
    }
};

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();                      // 0x00A01C10
    void* PItem;                                             // +0x04
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: };

class C3DVector { public: float X, Y, Z; };

struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };

struct CCombatRing;                                          // CDefendingCombatantInfo::CCombatRing
void __fastcall DestroyRange(CCombatRing* first, CCombatRing* last, const SNonscalarPtrIteratorTag& tag);   // 0x006F44E0

struct CCombatRingVector {                                   // STLport vector<CCombatRing>
    CCombatRing* _M_start;
    CCombatRing* _M_finish;
    CCombatRing* _M_end_of_storage;
    ~CCombatRingVector()
    {
        DestroyRange(_M_start, _M_finish, SNonscalarPtrIteratorTag());
        if (_M_start)
            free(_M_start);
    }
};

struct CCombatWheel {
    CCombatRingVector CombatRings;                           // +0x00
};

struct CAttackerTree {                                       // STLport _Rb_tree {head, count, compare}
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node*         head;
    unsigned long count;
    long          compare;
    void Erase(node* root);                                  // 0x006EFC70
    ~CAttackerTree()
    {
        if (count) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            count = 0;
        }
        if (head)
            free(head);
    }
};

class CDefendingCombatantInfo { public:
    CAttackerTree                         Attackers;         // +0x00
    CCombatWheel                          CombatWheel;       // +0x0c
    C3DVector                             LastVictimWheelUpdatePos;    // +0x18
    long                                  LastWheelUpdateWorldFrame;   // +0x24
    CIntelligentPointer<CThing>           PLastAttacker;     // +0x28
    CDefPointer<const CAttackPatternDef>  PAttackPattern;    // +0x30
    long                                  AttackPatternIndex;          // +0x34
    long                                  FrameForNextInstruction;     // +0x38
};

void DestroyDefendingCombatantInfo(CDefendingCombatantInfo* info)   // emits the implicit destructor; not part of the retail body
{
    info->CDefendingCombatantInfo::~CDefendingCombatantInfo();
}