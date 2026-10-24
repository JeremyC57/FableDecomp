// CTCCreatureGenerator::DoNavCheck(const CThingCreatureBase& creature) -- drop every open generation position the creature cannot navigate to from the generator (swap with the last, pop)
// Grade: DIFFER(233v234) -- register/scheduling only: retail loads the creature into ebx and turns it into the interface map in place (`mov ebx,[esp+20h] ; mov al,[ebx+20h] ; add ebx,44h`), tests the loop count with `test edi,edi` and zeroes the index offset (`xor ebx,ebx`) after the branch, and in the remove arm decrements the last-entry offset before the copy (`sub edi,0Ch ; lea edx,[edi+eax]`) with the pop_back through eax; ours reads the creature into eax (`lea ebx,[eax+44h]`), compares against a zeroed ebx and folds the -0Ch into the lea; tried early return vs block, if/else order (fixed the arm layout), a separate loop count (fixed the ebp copy), a GetTC with the HasTC test inside
// Leaf: ?DoNavCheck@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074A4E0, 234 bytes (+6 int3), __thiscall `ret 4`.
// Callees: CThing::GetPosition @ 0x004C73D0 (the generator's parent), CVectorMap::LowerBound const @ 0x004365B0 (the
//   creature's interface map, type 7 = CTCCreatureNavigation), CTCCreatureNavigation::IsAbleToNavigateFromPosToPos @
//   0x006CB620 (tolerance 0).
// Label: the manifest's "VertexBuffer_FilterNavigableVertices" is descriptive; Ego_r.pdb names
//   `?DoNavCheck@CTCCreatureGenerator@@IAEXABVCThingCreatureBase@@@Z`. Layout: OpenPositions (vector<C3DVector>) at +0x60
//   (retail; PDB +0x70 with the larger Dinkumware containers), as the landed GetUndeadGenerationPoint 0x0074A260.
// Shape: `n = OpenPositions.size(); if (n > 0) { pos = GetPosition(); nav = creature's navigation TC (left unset when the
//   bit is clear: retail never stores it on that path); for (i = 0; i < count;) if (!nav->IsAble...(pos, OpenPositions[i],
//   0)) { OpenPositions[i] = OpenPositions[count - 1]; pop_back(); --count; } else ++i; }` -- the index lives in the dead
//   creature argument slot, the key 7 for LowerBound too.
class C3DVector {
public:
    float X, Y, Z;
};

class CTCBase;
class CInterfaceMap {
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;              // 0x004365B0
};

class CThing {
public:
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];                        // +0x20
    CInterfaceMap   m_interfaces;                            // +0x44
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTCConst(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};

class CThingCreatureBase : public CThing {
};

class CTCCreatureNavigation {
public:
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float tolerance);   // 0x006CB620
};

namespace _STL {
template <class T> class vector {
public:
    T* _M_start;                                             // +0x00
    T* _M_finish;                                            // +0x04
    T* _M_end_of_storage;                                    // +0x08
    long size() const { return _M_finish - _M_start; }
    T& operator[](long n) { return *(_M_start + n); }
    void pop_back() { --_M_finish; }
};
}

class CTCCreatureGenerator {
public:
    virtual void v00();
    CThing*          ParentThing;                            // +0x04
    unsigned char    _pad08[0x60 - 0x08];
    _STL::vector<C3DVector> OpenPositions;                   // +0x60
    void DoNavCheck(const CThingCreatureBase& creature);
};

void CTCCreatureGenerator::DoNavCheck(const CThingCreatureBase& creature)
{
    long numPositions = OpenPositions.size();
    if (numPositions > 0) {
        C3DVector position = ParentThing->GetPosition();
        CTCCreatureNavigation* navigation;
        if (creature.HasTC(7))
            navigation = creature.GetTCConst<CTCCreatureNavigation>(7);
        long count = numPositions;
        for (long i = 0; i < count; ) {
            if (!navigation->IsAbleToNavigateFromPosToPos(position, OpenPositions[i], 0.0f)) {
                OpenPositions[i] = OpenPositions[count - 1];
                OpenPositions.pop_back();
                --count;
            }
            else
                ++i;
        }
    }
}
