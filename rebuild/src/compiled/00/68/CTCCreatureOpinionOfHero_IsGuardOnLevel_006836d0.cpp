// CTCCreatureOpinionOfHero::IsGuardOnLevel() -- true when the creature's village has a living guard member (a thing holding interface 0xB4)
// Leaf: ?IsGuardOnLevel@CTCCreatureOpinionOfHero@@QAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006836D0, length 152 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (inlined GetInterface for type 0x23 = CTCVillageMember),
// CTCVillageMember::GetPVillage @ 0x007EE540 (twice: the null test, then the list), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3).
// Layout: the retail CTCVillage keeps a single-pointer STLport list<CIntelligentPointer<CThing>> at +0x2C (its members; node
// {next, prev, pointer}); interface 0xB4 is word 5 bit 20 of the presence bitset.
// Shape: the list is a reference local (`add ebp,2Ch`, the end re-read through it each pass); inline IS_THING_ALIVE then a
// masked-word HasTC; nested ifs with one `xor al,al` exit.
// Model: engine/CTCCreatureOpinionOfHero.h is the PDB layout; retail STLport lists are 4 bytes (PDB 12), so HeroOpinionEnemy
// sits at +0x342 (as in the landed SetHeroOpinionEnemy 0x006815D0 and KnowsOfDeed 0x00681AE0), hence a local model.
// Retail CThing: interface bitset +0x20, map +0x44, Dead bit +0x91.
struct CTCBaseInterface;
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCVillage;
class CTCVillageMember { public:                       // interface 0x23
public:
    CTCVillage* GetPVillage() const;   // 0x007EE540
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
struct SThingPtrNode { SThingPtrNode* next; SThingPtrNode* prev; CIntelligentPointer<CThing> value; };
struct CThingPtrList {                          // list<CIntelligentPointer<CThing> > (retail: one sentinel pointer)
    SThingPtrNode* head;
};
class CTCVillage { public:
    unsigned char _pad00[0x2c];
    CThingPtrList Members;                      // +0x2c
    bool GetIsHeroEnemyOfGuards();              // 0x00658604
};
class CTCCreatureOpinionOfHero { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool IsGuardOnLevel();
};

bool CTCCreatureOpinionOfHero::IsGuardOnLevel()
{
    const CTCVillageMember* member;
    if (ParentThing->PeekTC(TCI_VILLAGE_MEMBER, &member)) {
        if (member->GetPVillage()) {
            CThingPtrList& members = member->GetPVillage()->Members;
            for (SThingPtrNode* node = members.head->next; node != members.head; node = node->next) {
                if (IS_THING_ALIVE(node->value) && node->value.GetPItem()->HasTC(TCI_GUARD))
                    return true;
            }
        }
    }
    return false;
}