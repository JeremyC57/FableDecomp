// CTCHeroMorph::AddTattoo(long tattooDefIndex) -- look the tattoo item def up by global index and apply its tattoo sub-def: one that removes all tattoos clears the list; otherwise every existing tattoo covering an overlapping body area is erased and the def appended; then the morph is marked changed at the current world frame
// Leaf: ?AddTattoo@CTCHeroMorph@@QAEXJ@Z
//
// Retail 0x0071DA70, 340 bytes (+12 int3 padding, `ret 4`). Ego_r.pdb `?AddTattoo@CTCHeroMorph@@QAEXJ@Z` (the third AddTattoo
// overload; 0x0071D7C0 is AddTattoo(const CThing&)).
// Callees: CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef>(long, CDefPointer*) @ 0x0042AF3C,
// CDefClassBase::GetSubDef<const CTattooDef>(CDefPointer*) const @ 0x005B1EFC, vector<CDefPointer>::clear() @ 0x00721600 and
// push_back(const CDefPointer&) @ 0x005B3325 (out of line here, inlined in 0x0071D7C0), erase(iterator) @ 0x00720810,
// CTCBase::GetWorld() const @ 0x00686A80 + static CWorld::GetWorldFrame @ 0x0049D870, the defs' virtual slot 1 on release.
// Shape: the AddTattoo(const CThing&) body with the item def fetched by index into a CDefPointer local (released last); the
// tattoo sub-def out pointer lives in the dead index slot; `this` is spilled to the frame for the loop (esi carries the copy).
// Layout: as 0x0071D7C0 (Tattoos +0x1c, FrameTattoosChanged +0x34, Changed +0x3d; CTattooDef RemovesAllTattoos +0x3c,
// CoversBodyAreaFlags +0x40, SpecificCoversBodyAreaFlags +0x48).

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
    mutable long m_refCount;                                    // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer() { if (Object && --Object->m_refCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
class CTattooDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x3c - 0x08];
    bool  RemovesAllTattoos;                            // +0x3c
    bool  Permanent;                                    // +0x3d
    unsigned char _pad3e[2];
    long  CoversBodyAreaFlags;                          // +0x40
    float VisibilityMultiplier;                         // +0x44
    long  SpecificCoversBodyAreaFlags;                  // +0x48
};
class CThingBaseDef : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<const CTattooDef>* out) const;   // 0x005B1EFC
};
typedef CDefPointer<const CThingBaseDef> CThingDefPointer;

void* FindSubDef(const CThingBaseDef* def);
void ReleaseDef(void* def);
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef (retail's body is out of line): it reads the old out pointer and stores the new one like
// CDefPointer::operator=, so VC7.1 knows the out pointer's address does not escape the call.
bool CThingBaseDef::GetSubDef(CDefPointer<const CTattooDef>* out) const
{
    void* def = FindSubDef(this);
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)

struct CThingDefVector {                                // STLport vector<CDefPointer<const CThingBaseDef> > (12 bytes)
    CThingDefPointer* _M_start;
    CThingDefPointer* _M_finish;
    CThingDefPointer* _M_end_of_storage;
    CThingDefPointer* begin() const { return _M_start; }
    CThingDefPointer* end() const { return _M_finish; }
    CThingDefPointer* erase(CThingDefPointer* pos);     // 0x00720810
    void clear();                                       // 0x00721600
    void push_back(const CThingDefPointer& x);          // 0x005B3325
};


void* GrowVectorStorage(void* vector, unsigned long count);
void DestroyRange(void* first, void* last);
#pragma auto_inline(off)
// Same-TU stand-ins for the vector members retail calls out of line (STLport COMDATs instantiated in this TU): push_back only
// reads its value through the const reference, so VC7.1 keeps the def pointer in a register across the call.
void CThingDefVector::push_back(const CThingDefPointer& x)
{
    CThingDefPointer* p = _M_finish != _M_end_of_storage ? _M_finish : (CThingDefPointer*)GrowVectorStorage(this, 1);
    *(void**)p = *(void* const*)&x;
    _M_finish = p + 1;
}
void CThingDefVector::clear()
{
    DestroyRange(_M_start, _M_finish);
    _M_finish = _M_start;
}
CThingDefPointer* CThingDefVector::erase(CThingDefPointer* pos)
{
    DestroyRange(pos, pos + 1);
    --_M_finish;
    return pos;
}
#pragma auto_inline(on)

class CThing {
public:
    unsigned char _pad00[0x70];
    CThingDefPointer PDef;                              // +0x70
};
class CWorld { public:
    static long __fastcall GetWorldFrame();             // 0x0049D870
};
class CDefinitionManager { public:
    bool GetDef(long index, CThingDefPointer* out);     // 0x0042AF3C (CDefinitionManager::GetDef<CThingBaseDef>)
};
void* FindDef(CDefinitionManager* manager, long index);
#pragma auto_inline(off)
// Same-TU stand-in for GetDef (operator=-shaped like the GetSubDef one): VC7.1 then keeps the def in a register.
bool CDefinitionManager::GetDef(long index, CThingDefPointer* out)
{
    void* def = FindDef(this, index);
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    const CWorld* GetWorld() const;                     // 0x00686A80
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0
};
class CTCHeroMorph : public CTCBase {
public:
    void* PMeshMorphSet[2];                             // +0x0c CCountedPointer<CMeshMorphSet>
    void* ParticleEmitters;                             // +0x14 (retail 4-byte list)
    void* IdleParticleEmitters;                         // +0x18
    CThingDefVector Tattoos;                            // +0x1c
    unsigned char CustomToBankIndex[0xc];               // +0x28 map<CCharString, pair<long,CDateAndTime> >
    long  FrameTattoosChanged;                          // +0x34
    long  FrameHeroBecameIdle;                          // +0x38
    bool  IdleParticlesAdded;                           // +0x3c
    bool  Changed;                                      // +0x3d
    void AddTattoo(long tattooDefIndex);
};

static inline bool TattoosOverlap(const CTattooDef* a, const CTattooDef* b)
{
    long overlap = a->CoversBodyAreaFlags & b->CoversBodyAreaFlags;
    if (overlap == 0)
        return false;
    if (overlap == 8 && (a->SpecificCoversBodyAreaFlags & b->SpecificCoversBodyAreaFlags) == 0 &&
        a->SpecificCoversBodyAreaFlags != 0 && b->SpecificCoversBodyAreaFlags != 0)
        return false;
    return true;
}

void CTCHeroMorph::AddTattoo(long tattooDefIndex)
{
    CThingDefPointer def;
    if (GetDefinitionManager()->GetDef(tattooDefIndex, &def)) {
        CDefPointer<const CTattooDef> tattoo;
        if (def->GetSubDef(&tattoo)) {
            if (tattoo->RemovesAllTattoos) {
                Tattoos.clear();
            } else {
                for (CThingDefPointer* it = Tattoos.begin(); it != Tattoos.end(); ) {
                    CDefPointer<const CTattooDef> other;
                    CThingDefPointer element = *it;
                    if (element->GetSubDef(&other) && TattoosOverlap(other.Object, tattoo.Object))
                        it = Tattoos.erase(it);
                    else
                        ++it;
                }
                Tattoos.push_back(def);
            }
            Changed = true;
            FrameTattoosChanged = GetWorld()->GetWorldFrame();
        }
    }
}