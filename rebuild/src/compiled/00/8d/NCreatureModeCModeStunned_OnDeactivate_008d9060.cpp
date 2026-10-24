// NCreatureMode::CModeStunned::OnDeactivate() -- vtable slot 6: the stunned creature gets its default targeting flags back, then the out-of-action base deactivation runs
// Leaf: ?OnDeactivate@CModeStunned@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D9060, 72 bytes (+8 int3 padding).
// Name: FableWin.pdb ?OnDeactivate@CModeStunned@NCreatureMode@@EAEXXZ; slot 6 of the vtable 0x01267C74 (rtti-vtables.tsv;
// CTCCreatureModeManager::CModeBase declares OnDeactivate; slot 5 of the sibling modes is the landed OnActivate).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound(const long*) @ 0x0040F020 (the inlined GetInterface,
// type 9 = CTCTargeted, bit 9 of the +0x20 bitset), CTCTargeted::ReturnToDefaultTargetingFlags @ 0x007EA2B0,
// CModeOutOfActionBase::OnDeactivate @ 0x008D8F30.
// Layout: CModeBase {vptr, CThing* +4}; the thing's interface bitset +0x20 and {type, component} map +0x44 (as the
// landed CModeCreatureIsProjectile::Recover 0x008E08C0).
// Shape: the out-parameter GetInterface template (the key homed in the `push ecx` slot, `it == last || 9 < it->type`
// picks last) used as the condition; the base call keeps `this` in edi. Plain `ret`.
struct CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);  // 0x0040F020
};

struct CTCTargeted {                       // interface type 9
    void ReturnToDefaultTargetingFlags();  // 0x007EA2B0
};

struct CThingCreatureBase {
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];      // +0x20 std::bitset of interface types
    CInterfaceMap   m_interfaces;          // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

namespace NCreatureMode {

struct CModeBase {                         // CTCCreatureModeManager::CModeBase
    virtual ~CModeBase();
    CThingCreatureBase* m_pThing;          // +0x04
};

struct CModeOutOfActionBase : CModeBase {
    void OnDeactivate();                   // 0x008D8F30
};

struct CModeStunned : CModeOutOfActionBase {
    virtual void OnDeactivate();
};

void CModeStunned::OnDeactivate()
{
    CTCTargeted* targeted;
    if (m_pThing->GetInterface(&targeted, 9))
        targeted->ReturnToDefaultTargetingFlags();
    CModeOutOfActionBase::OnDeactivate();
}

} // namespace NCreatureMode