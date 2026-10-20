// CTCVillage::GetNextGuardTypeAndIncrement() -- picks the def index of the next respawned guard: the respawn counter (post-incremented) walks the people def's weighted guard respawn table, falling back to the plain guard def
// Grade: DIFFER(236v227) -- register allocation: retail holds the people def in ecx (loop index in eax) so the last release is `dec [ecx+4] ; jne`; ours holds it in eax and releases through `mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],0`
// Leaf: GetNextGuardTypeAndIncrement
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00658FA2, 227 bytes (plain `ret`, `leave`): frame-pointer size TU,
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (`and [ebp-4],0` zero store). Ego_r.pdb
// `?GetNextGuardTypeAndIncrement@CTCVillage@@QAEJXZ`.
// Callees: CDefClassBase::GetSubDef<CVillagePeopleDef> 0x00512FE0 (landed as GetSubDef_00512fe0) on a copied counted
//          pointer to the parent thing's def (+0x70); the defs' slot 1 on the last release.
// Layout: retail CTCVillage (every list 4 bytes, see the landed IsHeroCriminal 0x0065BFCE): ParentThing +0x04,
//         NextGuardRespawn +0xa4 (PDB +0x130); retail CVillagePeopleDef: Guard +0x94, GuardRespawnType +0xa8,
//         GuardRespawnQuantity +0xb4 (PDB +0xb8; one 12-byte vector before it).
// Shape: the thing's def pointer is copied into a local counted pointer (add-ref, then the null-tested short release
//        after the lookup; the out pointer's zero store comes first); the quantity walk subtracts each entry from the
//        respawn index and returns the matching type when it goes negative or at the last entry (`js` / `je`, the loop
//        bound re-read through size()); the fallback `def->Guard` dereferences unconditionally, so the final release
//        has no null test and both returns share it.
#pragma optimize("y", off)
#pragma optimize("s", on)
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    unsigned int size() const { return end() - begin(); }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_count;                               // +0x04
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CVillagePeopleDef : CDefBase {
    unsigned char _pad_0x08[0x8c];
    long Guard;                                 // +0x94
    long MaleGhost;                             // +0x98
    long FemaleGhost;                           // +0x9c
    long NumGuards;                             // +0xa0
    long NumGhosts;                             // +0xa4
    CRetailVector<long> GuardRespawnType;       // +0xa8
    CRetailVector<long> GuardRespawnQuantity;   // +0xb4
};
struct CDefPointer {                            // CDefPointer<const CVillagePeopleDef>
    CVillagePeopleDef* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p) m_p->Release(); }
    CVillagePeopleDef* operator->() const { return m_p; }
    operator bool() const { return m_p != 0; }
};
struct CDefClassBase : CDefBase {
    bool GetSubDef_00512fe0(CDefPointer& out);  // 0x00512FE0 (GetSubDef<CVillagePeopleDef>)
};
struct CThingDefPointer {                       // CDefPointer<const CThingDef> (the thing's def at +0x70)
    CDefClassBase* m_p;
    CThingDefPointer(const CThingDefPointer& other) : m_p(other.m_p) { if (m_p) ++m_p->m_count; }
    ~CThingDefPointer() { if (m_p) m_p->Release(); }
    CDefClassBase* operator->() const { return m_p; }
};
struct CThing {
    unsigned char    _pad_0x00[0x70];
    CThingDefPointer PDef;                      // +0x70
};

struct CTCVillage {
    void*         __vftable;                    // +0x00
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x9c];
    long          NextGuardRespawn;             // +0xa4
    long GetNextGuardTypeAndIncrement();
};

long CTCVillage::GetNextGuardTypeAndIncrement()
{
    long respawn = NextGuardRespawn++;
    CDefPointer def;
    {
        CThingDefPointer thingDef(ParentThing->PDef);
        thingDef->GetSubDef_00512fe0(def);
    }
    if (def && def->GuardRespawnType.size() != 0 && def->GuardRespawnType.size() == def->GuardRespawnQuantity.size()) {
        for (unsigned int i = 0; i < def->GuardRespawnQuantity.size(); ++i) {
            respawn -= def->GuardRespawnQuantity[i];
            if (respawn < 0 || i == def->GuardRespawnQuantity.size() - 1)
                return def->GuardRespawnType[i];
        }
    }
    return def->Guard;
}
