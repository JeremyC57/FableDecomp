// CThingCreatureBase::IsPickPocketable() -- true when the creature def allows pick-pocketing, the hero is alive, the creature is not watching the hero, and the hero is within 2 units
// Leaf: ?IsPickPocketable@CThingCreatureBase
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00665E90, 241 bytes (+15 int3), `ret`, bool in al.
// Callees: CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>*) @ 0x00485815 (manifest names a propagated
//   CAbilityDef instantiation), CThing::GetPlayerManager @ 0x004C79F0 (`[g_0x013B8A1C]+0x30`), CPlayerManager::GetMainPlayer
//   @ 0x00449960, CPlayer::GetPControlledCreature @ 0x00487DC0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0
//   (the inlined GetInterface for CTCPerceiveThing, type 0x34 = bit 20 of the interface bitset word at +0x24),
//   CTCPerceiveThing::GetPerceivedThing @ 0x007446A0, CTCPerceiveThing::CanSeeOrSmellPerceivedThing @ 0x00661600 (landed),
//   CThing::GetPos @ 0x004C73D0, GFWorld3DDistanceSmallerThan(const C3DVector&, const C3DVector&, float) @ 0x006614F0
//   (`__fastcall`), the def's virtual slot 1 (+4) at a zero count.
// Layout: retail CThing PBaseDef +0x70, dead flag +0x91 bit 0, interface bitset +0x20, interface map +0x44; CCreatureDef
//   PickPocketable +0xCA (PDB +0xDA).
// Shape: the sub-def out pointer is the zeroed `push ecx` word (a same-TU stand-in GetSubDef keeps its address from escaping,
//   so the def lives in ebp and the lookup key reuses the word); a null def skips to the single shared `return false` exit (no release); the success
//   path releases inline and returns `mov al,1`, every failure joins one release + `xor al,al`.
#include "fable_compiler.h"
#include "engine/C3DVector.h"
bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance);   // 0x006614F0
class CDefPointeeBase { public:                        // a counted definition object
    virtual void v000();
    virtual void DestroyDef();                  // slot 1 (+4): called when the count drops to zero
    long m_refCount;                            // +4
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};
// agentNF15: the pointee is held non-const whatever T is (engine/CDefPointer.h stores CDefPointeeBase*), so the
//   CDefPointer<const T> out slots of the const def lookups keep the landed member code.
template <class T> struct CDefNonConst { typedef T Type; };
template <class T> struct CDefNonConst<const T> { typedef T Type; };
template<class T> class CDefPointer { public:          // CDefPointer<const T>: one counted pointer to a definition
    typename CDefNonConst<T>::Type* m_pObject;
    CDefPointer() : m_pObject(0) { }
};
class CCreatureDef : public CDefPointeeBase { public:
    unsigned char _pad08[0xca - 0x08];
    bool PickPocketable;                        // +0xca
};
class CDefClassBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x00485815 GetSubDef<CCreatureDef> (Ego_r.pdb member template)
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>* out) const
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing;
class CThingPlayerCreature;
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;            // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CThing { public:
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;            // +0x44
    unsigned char  _pad54[0x1c];
    CDefClassBase* PBaseDef;                // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_statusFlags;           // +0x91 (bit 0: dead)
    const C3DVector& GetPos() const;   // 0x004C73D0
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x004C79F0
};
class CThingPlayerCreature : public CThing { };
class CTCPerceiveThing { public:
    const CThing* GetPerceivedThing() const;          // 0x007446A0 Ego_r/FableWin ?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ (agentNF16)
    bool CanSeeOrSmellPerceivedThing() const;   // 0x00661600
};
class CThingCreatureBase : public CThing { public:
    bool IsPickPocketable();
};

bool CThingCreatureBase::IsPickPocketable()
{
    CDefPointer<const CCreatureDef> out;
    PBaseDef->GetSubDef(&out);
    CCreatureDef* def = out.m_pObject;
    if (def) {
        if (def->PickPocketable) {
            CThing* hero = PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCPerceiveThing* perceive;
            if (hero && !(hero->m_statusFlags & 1) && GetInterface(&perceive, 0x34)) {
                if (perceive->GetPerceivedThing() != hero || !perceive->CanSeeOrSmellPerceivedThing()) {
                    if (GFWorld3DDistanceSmallerThan(GetPos(), hero->GetPos(), 2.0f)) {
                        def->ReleaseDef();
                        return true;
                    }
                }
            }
        }
        def->ReleaseDef();
    }
    return false;
}