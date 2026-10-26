// CTCTargetingAI::IsThingAttackable(const CThing* thing) const -- true unless the thing is on the "things to not attack" list
// Leaf: IsThingAttackable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F6170, length 53 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Callers: CTCTargetingAI::AddThingToNotAttack @ 0x007F61B0 (adds the thing only when this returns true; labelled
// "CTCSolidToThing::AddThingNotSolidTo", but CTCSolidToThing is 28 bytes and the list is read at +0x48),
// CAIStateGroup_StealthSeekEnemy::IsRunnable / IsStillRunnable. The manifest label "CTCFollowed::IsAIFollower" is
// propagated from the shape.
// Layout: PDB CTCTargetingAI (84 bytes): PBestEnemyCreatureForAttack +0x38, PScriptTargetOverride +0x40, ThingsToNotAttack
// +0x48 (list<CIntelligentPointer<CThing>>, retail one sentinel pointer); the CTCTargetingBase part is opaque here.
// Shape: as CTCFollowed::IsFollower 0x006D4D70 with the list at +0x48 and the exits swapped (`mov al,1` when the walk
// ends, `xor al,al` on a hit).
class CThing;

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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CThingPointerList {                       // retail std::list<CIntelligentPointer<CThing>>: one sentinel pointer
    struct node {
        node*                   next;
        node*                   prev;
        CIntelligentPointer<CThing> value;           // +0x08
    };
    node* head;
};

class CTCTargetingAI { public:
    unsigned char           _base[0x38];         // CTCTargetingBase
    CIntelligentPointer<CThing> PBestEnemyCreatureForAttack;   // +0x38
    CIntelligentPointer<CThing> PScriptTargetOverride;         // +0x40
    CThingPointerList       ThingsToNotAttack;   // +0x48
    bool IsThingAttackable(const CThing* thing) const;
};

bool CTCTargetingAI::IsThingAttackable(const CThing* thing) const
{
    for (CThingPointerList::node* n = ThingsToNotAttack.head->next; n != ThingsToNotAttack.head; n = n->next) {
        if (n->value.GetPItem() == thing)
            return false;
    }
    return true;
}