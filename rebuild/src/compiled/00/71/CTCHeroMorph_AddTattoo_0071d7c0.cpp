// CTCHeroMorph::AddTattoo(const CThing& tattooThing) -- apply a tattoo item: a tattoo def that removes all tattoos clears the list; otherwise every existing tattoo covering an overlapping body area is erased and the item's def appended; then the morph is marked changed at the current world frame
// Leaf: ?AddTattoo@CTCHeroMorph@@QAEXABVCThing@@@Z
//
// Retail 0x0071D7C0, 372 bytes (+12 int3 padding, `ret 4`). Ego_r.pdb `?AddTattoo@CTCHeroMorph@@QAEXABVCThing@@@Z` (the first of the
// three AddTattoo overloads; 0x0071DA70 is AddTattoo(long)).
// Callees: CDefClassBase::GetSubDef<const CTattooDef>(CDefPointer*) const @ 0x005B1EFC, STLport `__copy(first, last, result,
// random_access_iterator_tag, long*)` @ 0x0071FA60 and `_Destroy(first, last, tag)` @ 0x005B26E9 (both `__fastcall`, the inlined
// erase(begin(), end()) of clear()), vector<CDefPointer>::erase(iterator) @ 0x00720810, `_M_insert_overflow` @ 0x005B3275 (the
// inlined push_back's growth path), CTCBase::PeekWorld() const @ 0x00686A80 + static CWorld::GetWorldFrame @ 0x0049D870, the defs'
// virtual slot 1 on release.
// Shape: clear() and push_back are inlined here (AddTattoo(long) calls their out-of-line copies); the copied CDefPointer for
// push_back is built in the dead `tattooThing` slot, which also homes the empty tag temporaries -- VC7.1 overlaps them only with
// same-TU `auto_inline(off)` bodies of the growth/copy/destroy templates that never read the tag. Inside the loop the sub-def out
// pointer is declared before the element copy, so the copy is released first.
// Layout: retail CTCHeroMorph (two 4-byte lists): Tattoos +0x1c, FrameTattoosChanged +0x34, Changed +0x3d (PDB +0x2c/+0x4c/+0x55);
// CTattooDef RemovesAllTattoos +0x3c, CoversBodyAreaFlags +0x40, SpecificCoversBodyAreaFlags +0x48 (Ego_r.pdb); CThing PDef +0x70.
#include <new>

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __true_type { __true_type() { } };

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

CThingDefPointer* __fastcall __copy(CThingDefPointer* first, CThingDefPointer* last, CThingDefPointer* result,
                                    const random_access_iterator_tag& tag, long* distance);   // 0x0071FA60
void __fastcall _Destroy(CThingDefPointer* first, CThingDefPointer* last, const __true_type& tag);   // 0x005B26E9

struct CThingDefVector {                                // STLport vector<CDefPointer<const CThingBaseDef> > (12 bytes)
    CThingDefPointer* _M_start;
    CThingDefPointer* _M_finish;
    CThingDefPointer* _M_end_of_storage;
    CThingDefPointer* begin() const { return _M_start; }
    CThingDefPointer* end() const { return _M_finish; }
    CThingDefPointer* erase(CThingDefPointer* pos);     // 0x00720810
    CThingDefPointer* erase(CThingDefPointer* first, CThingDefPointer* last)
    {
        CThingDefPointer* i = __copy(last, _M_finish, first, random_access_iterator_tag(), (long*)0);
        _Destroy(i, _M_finish, __true_type());
        _M_finish = i;
        return first;
    }
    __forceinline void clear() { erase(begin(), end()); }
    void _M_insert_overflow(CThingDefPointer* pos, const CThingDefPointer& x, const __true_type& tag,
                            unsigned long fillLength, bool atEnd);   // 0x005B3275
    __forceinline void push_back(const CThingDefPointer& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) CThingDefPointer(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);
    }
};

void* GrowVectorStorage(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the growth template (instantiated in retail's TU): reads the value only through its const reference and
// never touches the empty tag.
void CThingDefVector::_M_insert_overflow(CThingDefPointer* pos, const CThingDefPointer& x, const __true_type& tag,
                                         unsigned long fillLength, bool atEnd)
{
    CThingDefPointer* p = (CThingDefPointer*)GrowVectorStorage(this, fillLength);
    if (p)
        *(void**)p = *(void* const*)&x;
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
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
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
    void AddTattoo(const CThing& tattooThing);
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

void CTCHeroMorph::AddTattoo(const CThing& tattooThing)
{
    CDefPointer<const CTattooDef> tattoo;
    if (tattooThing.PDef->GetSubDef(&tattoo)) {
        if (tattoo->RemovesAllTattoos) {
            Tattoos.clear();
        } else {
            for (CThingDefPointer* it = Tattoos.begin(); it != Tattoos.end(); ) {
                CDefPointer<const CTattooDef> other;
                CThingDefPointer def = *it;
                if (def->GetSubDef(&other) && TattoosOverlap(other.Object, tattoo.Object))
                    it = Tattoos.erase(it);
                else
                    ++it;
            }
            Tattoos.push_back(CThingDefPointer(tattooThing.PDef));
        }
        Changed = true;
        FrameTattoosChanged = PeekWorld().GetWorldFrame();
    }
}