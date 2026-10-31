// Grade: DIFFER(347v336) 22 of 119 lines (was 361v336, 64): the created creature is assigned to the returned local and copied into one uninitialised function-scope pointer whose address the push_back takes -- that pointer is homed in the dead init argument slot ([esp+28h]) and the creature stays in esi, no push ecx frame -- residue: retail caches the people def's Object in edi from the GetSubDef return through MaleChance and the release (ours re-reads the slot; the shared engine CDefPointer with the test ENullDef ctor gives the CDefClassBase release shape, 25), the def homed one slot higher ([esp+3Ch]), and the 0x15C bit-set scheduled after the 0x1C9 store
// CreateRelevantPersonAndUpdateStats(float ratioA, float ratioB, long defA, long defB, long& countA, long& countB, const C3DVector& position, const CThingAICreatureInit& init, CTCVillage* village, vector<CThingAICreature*>& people, vector<CThingAICreature*>& created, bool listOnly) -- spawn one villager of whichever of two creature defs is under-represented against its target ratio (falling back to the other def), count it, optionally list it, flag it and add it to the village; unlisted ones get their sexuality set up from the village's people def
// Leaf: ?CreateRelevantPersonAndUpdateStats@@
class C3DVector { public: float X, Y, Z; };
class CThingAICreatureInit;
class CThingAICreature;

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void push_back(const _Tp& __x);                    // <CThingAICreature*>: 0x00517A10
    void _M_insert_overflow(_Tp* __pos, const _Tp& __x, unsigned int __n);
};
// same-TU stand-in for the out-of-line push_back COMDAT (retail's TU held its template body): it reads x, never keeps &x
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    if (_M_finish != _M_end_of_storage) {
        *_M_finish = __x;
        ++_M_finish;
    } else
        _M_insert_overflow(_M_finish, __x, 1);
}
#pragma auto_inline(on)
}

class CVillagePeopleDef {                              // the village's people sub-def (landed 0x00783380 model)
public:
    virtual void v00();
    virtual void Destroy();                            // slot 1
    long          RefCount;                            // +0x04
    unsigned char _pad08[0x34 - 0x08];
    float         MaleChance;                          // +0x34
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
class CDefClassBase {
public:
    bool GetSubDef(CDefPointer<CVillagePeopleDef>* out);   // 0x00512FE0 (GetSubDef<T>)
};
class CThing {
public:
    unsigned char  _pad00[0x70];
    CDefClassBase* m_pDef;                             // +0x70 (retail; PDB +0x80)
    unsigned char  _pad74[0x15c - 0x74];
    unsigned char  _flag15c_0 : 6;
    unsigned char  IsCrowdMember : 1;                  // +0x15c bit 6 (landed 0x00783380 name)
    unsigned char  _flag15c_7 : 1;
    unsigned char  _pad15d[0x1c9 - 0x15d];
    unsigned char  NoLOD : 1;                          // +0x1c9 bit 0 (landed 0x00783380 name)
    unsigned char  _flag1c9_1 : 7;
    CDefClassBase* GetDef() const { return m_pDef; }
};
class CThingAICreature : public CThing {
public:
    void AutoSetupSexuality(float maleChance);         // 0x008330D0
    static CThingAICreature* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                               const CThingAICreatureInit& init);   // 0x00833800
};
class CTCVillage {
public:
    void*   __vftable;
    CThing* ParentThing;                               // +0x04 (CTCBase)
    void AddThingToVillage(CThing* thing);             // 0x0065B854
};

template void std::vector<CThingAICreature*>::push_back(CThingAICreature* const& __x);   // compiled ahead of the caller

CThingAICreature* __fastcall CreateRelevantPersonAndUpdateStats(float ratioA, float ratioB, long defA, long defB,
                                                                long& countA, long& countB, const C3DVector& position,
                                                                const CThingAICreatureInit& init, CTCVillage* village,
                                                                std::vector<CThingAICreature*>& people,
                                                                std::vector<CThingAICreature*>& created, bool listOnly)
{
    CThingAICreature* creature = 0;
    CThingAICreature* person;
    if ((float)countA * ratioB < (float)countB * ratioA) {
        if (defA) {
            creature = CThingAICreature::Create(defA, position, 4, init);
            person = creature;
            if (listOnly)
                created.push_back(person);
            ++countA;
        } else if (defB) {
            creature = CThingAICreature::Create(defB, position, 4, init);
            person = creature;
            if (listOnly)
                created.push_back(person);
            ++countB;
        }
    } else {
        if (defB) {
            creature = CThingAICreature::Create(defB, position, 4, init);
            person = creature;
            if (listOnly)
                created.push_back(person);
            ++countB;
        } else if (defA) {
            creature = CThingAICreature::Create(defA, position, 4, init);
            person = creature;
            if (listOnly)
                created.push_back(person);
            ++countA;
        }
    }
    if (creature) {
        creature->NoLOD = true;
        creature->IsCrowdMember = true;
        village->AddThingToVillage(creature);
        if (!listOnly) {
            CDefPointer<CVillagePeopleDef> def;
            if (village->ParentThing->GetDef()->GetSubDef(&def))
                creature->AutoSetupSexuality(def->MaleChance);
        }
    }
    return creature;
}
