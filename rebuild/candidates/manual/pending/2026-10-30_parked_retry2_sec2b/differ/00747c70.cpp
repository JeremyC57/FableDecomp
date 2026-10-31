// CTCStealth::UpdateDangerInfo() -- per update: drop dead danger things; mark the attackers beyond the allowed maximum as not to attack (nearest first) and restart the danger cool-off timer; with no danger things count the cool-off down (clearing the danger status when it runs out); count the running-away timer down
// Grade: DIFFER(308v303) (33,113 vs parked 46) -- empty() test reproduces retail's eax/ecx begin/end compare, the re-read end in esi and ebx predicate; left: retail pushes esi at entry and zeroes the attackers vector through xor esi,esi (ours immediates, +5 bytes), over>0 as cmp ebp,esi (retry wave 2 sec2b)
// Was: DIFFER(303v303) -- register allocation: retail tests begin/end in eax/ecx and re-reads end into esi, keeps &DangerThings in ebp and the predicate dword in ebx and zeroes the attackers vector through esi; ours CSEs end into esi and swaps ebx/ebp (one attempt; inline STLport remove_if model)
// Leaf: UpdateDangerInfo
// Retail: Fable.exe (TLC, VC7.1) @ 0x00747C70, 303 bytes (plain `ret`).
// Callees: __find_if<SDangerThing*, CDeadDangerThing> @ 0x00748E40 and remove_copy_if @ 0x00748D80 (`__fastcall` LTCG
//   instantiations behind the inline STLport remove_if; manifest "LinearSearch_Range_WithValidityCheck" / "_Remove_if"),
//   vector<SDangerThing>::erase(first, last) @ 0x00749280, CTCStealth::GetNumberOfAttackersOverMaximum @ 0x00747890,
//   sort<const CThing**, CCompareAttackers> @ 0x00749740 (`__fastcall`), CVectorMap::LowerBound @ 0x0040F020 (interface 8 =
//   CTCTargetingAI), CTCTargetingAI::AddThingToNotAttack(CThing*) @ 0x007F61B0, __ftol2 @ 0x00BFEA70, free @ 0x00BFEA14,
//   CTCStealth::ClearDangerStatus @ 0x00747090. g_framesPerSecond @ 0x01375550, 1.0f @ 0x0122DED8.
// Layout: Ego_r.pdb CTCStealth PDef +0x1C, RunningAwayFromCombatTimer +0x30, InDangerCoolOffTimer +0x34, DangerThings +0x38;
//   CStealthDef DangerFeedbackHangtimeSeconds +0x40.
// Shape: the inline STLport remove_if (find_if through the iterator-category tag, then remove_copy_if from the next element)
//   inside erase(remove_if(...), end()); the attackers vector is a local; the over-maximum count drives a count-down loop
//   over the sorted attackers.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
class CTCBase;
class CThing;
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
};
struct SDangerThing {                                   // pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus>
    CBaseIntelligentPointer first;
    long Status;
    long Timer;
};
struct CDeadDangerThing { };                            // anonymous-namespace predicate (empty)
struct CCompareAttackers { };                           // the sort comparator (empty)
SDangerThing* __fastcall __find_if(SDangerThing* first, SDangerThing* last, CDeadDangerThing pred,
                                   const random_access_iterator_tag& tag);   // 0x00748E40
SDangerThing* __fastcall remove_copy_if(SDangerThing* first, SDangerThing* last, SDangerThing* result, CDeadDangerThing pred);   // 0x00748D80
inline SDangerThing* remove_if(SDangerThing* first, SDangerThing* last, CDeadDangerThing pred)
{
    first = __find_if(first, last, pred, random_access_iterator_tag());
    if (first == last)
        return first;
    SDangerThing* next = first;
    return remove_copy_if(++next, last, first, pred);
}
void __fastcall sort(const CThing** first, const CThing** last, CCompareAttackers compare);   // 0x00749740
template <class T> struct CRetailVector {               // STLport vector: {start, finish, end_of_storage}
    T* first;
    T* last;
    T* end;
    CRetailVector() : first(0), last(0), end(0) {}
    ~CRetailVector() { if (first) free(first); }
    T* begin_() const { return first; }
    T* end_() const { return last; }
    bool empty() const { return first == last; }
    T* erase(T* from, T* to);                           // 0x00749280 (SDangerThing)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CTCTargetingAI {
    void AddThingToNotAttack(CThing* thing);            // 0x007F61B0
};
struct CStealthDef {
    unsigned char _pad00[0x40];
    float DangerFeedbackHangtimeSeconds;                // +0x40
};
extern long g_framesPerSecond;                          // 0x01375550
struct CTCStealth {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x1c - 0x08];
    const CStealthDef* PDef;                            // +0x1c
    unsigned char _pad20[0x30 - 0x20];
    long          RunningAwayFromCombatTimer;           // +0x30
    long          InDangerCoolOffTimer;                 // +0x34
    CRetailVector<SDangerThing> DangerThings;           // +0x38
    long GetNumberOfAttackersOverMaximum(CRetailVector<CThing*>* attackers) const;   // 0x00747890
    void ClearDangerStatus();                           // 0x00747090
    void UpdateDangerInfo();
};

void CTCStealth::UpdateDangerInfo()
{
    if (!DangerThings.empty()) {
        DangerThings.erase(remove_if(DangerThings.begin_(), DangerThings.end_(), CDeadDangerThing()), DangerThings.end_());
        CRetailVector<CThing*> attackers;
        long over = GetNumberOfAttackersOverMaximum(&attackers);
        if (over > 0 && attackers.first != attackers.last) {
            sort((const CThing**)attackers.first, (const CThing**)attackers.last, CCompareAttackers());
            CThing** it = attackers.first;
            do {
                CThing* attacker = *it++;
                if (attacker->HasTC(8))
                    attacker->GetTC<CTCTargetingAI>(8)->AddThingToNotAttack(ParentThing);
            } while (--over > 0);
        }
        InDangerCoolOffTimer = (long)(PDef ? g_framesPerSecond * PDef->DangerFeedbackHangtimeSeconds : 1.0f);
    } else if (InDangerCoolOffTimer >= 0) {
        if (--InDangerCoolOffTimer < 0)
            ClearDangerStatus();
    }
    if (RunningAwayFromCombatTimer >= 0)
        --RunningAwayFromCombatTimer;
}
