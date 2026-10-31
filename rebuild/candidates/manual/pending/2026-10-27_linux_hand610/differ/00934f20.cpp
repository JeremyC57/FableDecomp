// CThingCompare_NearestGroupByMember::operator()<CThing>(CThing* a, CThing* b) const -- of two creature groups pick the one whose nearest member to Pos is closer (a null or group-less side loses)
// Grade: DIFFER(281v281) -- wave 25: names PDB-exact (class key V, CTCCreatureGroup::GetNearestMember const returning const CThingAICreature*, CThing::GetPos); residue unchanged: retail's final compare `jne 934FA2h` jumps back to the earlier return-b block, ours if-converts the last return-a/return-b pair into `mov eax,[esp+24h] ; jne ; mov eax,ebx`. Tried this wave: goto to a trailing label (the return-b block moves to the end, 18 lines), goto to a label inside the nearestA block (same), flattened `!nearestB -> return a` + ternary (283v281). Lane gap: tail-merge/if-conversion choice.
// Leaf: ??$?RVCThing@@@CThingCompare_NearestGroupByMember@@QBEPAVCThing@@PAV1@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00934F20, 279 bytes (+9 int3), `ret 8`.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (the inlined GetInterface for the creature-group
//   component, type 0xA3 = bit 3 of the interface bitset word at +0x34), CTCCreatureGroup::GetNearestMember(const C3DVector&)
//   @ 0x008128F0 (landed), CThing::GetPosition() @ 0x004C73D0.
// Layout: PDB CThingCompare_NearestGroupByMember { C3DVector Pos; } (12 bytes); CThing interface bitset +0x20, interface map
//   +0x44 (the landed 0x006994F0 model).
// Shape: null tests return the other side; the group component of `a` is fetched (end/hit pick) before `b`'s HasTC test;
//   retail asks a's group for its nearest member twice (b's group is never fetched -- kept as found); the closer member
//   wins by squared distance (first offset spilled to a frame local, the second on the x87 stack).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }

struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x004365B0
};
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    const C3DVector& GetPos() const;        // 0x004C73D0
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
class CThingAICreature : public CThing { };
class CTCCreatureGroup {
public:
    const CThingAICreature* GetNearestMember(const C3DVector& pos) const;   // 0x008128F0
};

class CThingCompare_NearestGroupByMember {
public:
    C3DVector Pos;                          // +0x00
    template <class T> T* operator()(T* a, T* b) const
    {
        if (!a)
            return b;
        if (!b)
            return a;
        if (!a->HasTC(0xa3))
            return b;
        CTCCreatureGroup* groupA = (CTCCreatureGroup*)a->GetTC(0xa3);
        if (b->HasTC(0xa3)) {
            const CThingAICreature* nearestA = groupA->GetNearestMember(Pos);
            const CThingAICreature* nearestB = groupA->GetNearestMember(Pos);
            if (!nearestA)
                return b;
            if (nearestB) {
                C3DVector offsetA = nearestA->GetPos() - Pos;
                if (!(offsetA.GetLengthSquared() < (nearestB->GetPos() - Pos).GetLengthSquared()))
                    return b;
            }
        }
        return a;
    }
};
template CThing* CThingCompare_NearestGroupByMember::operator()<CThing>(CThing* a, CThing* b) const;
