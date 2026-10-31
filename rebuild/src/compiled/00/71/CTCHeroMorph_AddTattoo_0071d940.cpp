// CTCHeroMorph::AddTattoo(CDefPointer<const CThingBaseDef>& def) -- add a tattoo item to the hero: a "removes all tattoos" def clears the list, otherwise every worn tattoo whose body area clashes with the new one is taken off and the new def appended; either way the morph is marked changed at the current world frame
// Grade: RELOCATION_MATCH (header ctor landed 2026-10-31; was PENDING HEADER) (retry wave 2 sec2b): on engine/CDefPointer.h with the real GetSubDef<CTattooDef> body (landed 0x005B1EFC) as a noinline explicit specialization and the Tattoos vector taken through a named reference for the clash loop (this in ebx, iterator in ebp; was 299v300 ebx/ebp swap). Graded RELOCATION_MATCH with the header text inlined plus `CDefPointer(ENullDef) : Object(0) { }` (hdr/0071d940.verify.cpp); this file compiles once the header declares that ctor. Test: hdr/0071d940.test.cpp PASS
// Leaf: ?AddTattoo@CTCHeroMorph@@QAEXAAV?$CDefPointer
//
// Retail 0x0071D940, 304 bytes incl. padding (`ret 4`). Ego_r.pdb
// `?AddTattoo@CTCHeroMorph@@QAEXAAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z` (of the three overloads, the CDefPointer one).
// Callees: CDefClassBase::GetSubDef<CTattooDef>(CDefPointer<const CTattooDef>*) const @ 0x005B1EFC (catalog label
// "GetSubDef<CAbilityDef>" is a byte-twin propagation), SGI `__copy(first, last, result, const random_access_iterator_tag&,
// long*)` @ 0x0071FA60 and `_Destroy_range(first, last, const allocator&)` @ 0x005B26E9 (both /Gr `__fastcall`; the
// inlined clear(), tag and allocator temporaries homed in the dead `def` slot; labels "_Move<CMeleeCombatAbilityDef>" and
// "_Destroy_range<CFlash>" are propagated), vector<CDefPointer<const CThingBaseDef>>::erase(iterator) @ 0x00720810 (landed),
// ::push_back(const CDefPointer&) @ 0x005B3325 (out of line), CTCBase::GetWorld() const @ 0x00686A80, CWorld::GetFrame()
// @ 0x0049D870 (a static: the GetWorld call is still emitted), the defs' slot 1 on release.
// Layout: retail CTCHeroMorph Tattoos +0x1C (12-byte vector), FrameTattoosChanged +0x34, Changed +0x3D; CTattooDef
// RemovesAllTattoos +0x3C, CoversBodyAreaFlags +0x40, SpecificCoversBodyAreaFlags +0x48.
// Shape: two tattoos clash when their CoversBodyAreaFlags overlap, unless the overlap is exactly 8 and both carry
// disjoint, non-zero specific flags; each worn def is copied into a counted local (add-ref) before its own GetSubDef.
#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"                             // CDefPointer(ENullDef) landed in the header 2026-10-31

class CTattooDef : public CSubDefClassBase {                // RTTI .?AVCTattooDef@@ : CSubDefClassBase
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CTattooDef", -1); }   // inline
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    unsigned char _pad25[0x3c - 0x25];
    bool RemovesAllTattoos;                                 // +0x3c
    bool Permanent;                                         // +0x3d
    long CoversBodyAreaFlags;                               // +0x40
    float VisibilityMultiplier;                             // +0x44
    long SpecificCoversBodyAreaFlags;                       // +0x48
};
class CThingBaseDef : public CDefClassBase { };

class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (protected non-const overload; hidden result pointer)
};

// The retail GetSubDef<CTattooDef> body (landed 0x005B1EFC), noinline: only AddTattoo is graded.
template <>
__declspec(noinline) bool CDefClassBase::GetSubDef<CTattooDef>(CDefPointer<const CTattooDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CTattooDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);       // 0x0071FA60
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x005B26E9
}
typedef CDefPointer<const CThingBaseDef> CThingDefPointer;
struct CThingDefVector {                                    // STLport vector<CDefPointer<const CThingBaseDef> > (12 bytes)
    CThingDefPointer* _M_start;
    CThingDefPointer* _M_finish;
    CThingDefPointer* _M_end_of_storage;
    CThingDefPointer* begin() { return _M_start; }
    CThingDefPointer* end() { return _M_finish; }
    CThingDefPointer* erase(CThingDefPointer* pos);                 // 0x00720810
    void push_back(const CThingDefPointer& value);                  // 0x005B3325
    CThingDefPointer* erase(CThingDefPointer* first, CThingDefPointer* last)
    {
        CThingDefPointer* i = std::__copy(last, _M_finish, first, std::random_access_iterator_tag(), (int*)0);
        std::__destroy_aux(i, _M_finish, std::__false_type());
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
class CWorld {
public:
    static long GetFrame();                                 // 0x0049D870 (returns the global @ 0x013B89BC)
};
class CTCBase {
public:
    virtual void v00();
protected:
    const CWorld& PeekWorld() const;                        // 0x00686A80 (Ego_r.pdb CTCBase, protected)
};
class CTCHeroMorph : public CTCBase {
public:
    unsigned char _pad04[0x1c - 0x04];
    CThingDefVector Tattoos;                                // +0x1c
    unsigned char _pad28[0x34 - 0x28];
    long FrameTattoosChanged;                               // +0x34
    long FrameHeroBecameIdle;                               // +0x38
    bool IdleParticlesAdded;                                // +0x3c
    bool Changed;                                           // +0x3d
    void AddTattoo(CThingDefPointer& def);
};

void CTCHeroMorph::AddTattoo(CThingDefPointer& def)
{
    CDefPointer<const CTattooDef> tattoo(NULL_DEF);
    if (def->GetSubDef(&tattoo)) {
        if (tattoo->RemovesAllTattoos) {
            Tattoos.clear();
        } else {
            CThingDefVector& tattoos = Tattoos;
            for (CThingDefPointer* it = tattoos.begin(); it != tattoos.end(); ) {
                CDefPointer<const CTattooDef> worn(NULL_DEF);
                CThingDefPointer wornDef = *it;
                long overlap;
                if (wornDef->GetSubDef(&worn) &&
                    (overlap = worn->CoversBodyAreaFlags & tattoo->CoversBodyAreaFlags) != 0 &&
                    (overlap != 8 ||
                     (worn->SpecificCoversBodyAreaFlags & tattoo->SpecificCoversBodyAreaFlags) != 0 ||
                     worn->SpecificCoversBodyAreaFlags == 0 || tattoo->SpecificCoversBodyAreaFlags == 0))
                    it = tattoos.erase(it);
                else
                    ++it;
            }
            Tattoos.push_back(def);
        }
        Changed = true;
        FrameTattoosChanged = PeekWorld().GetFrame();
    }
}