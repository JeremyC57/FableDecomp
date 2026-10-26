// CCreatureAction_OpenChest::CCreatureAction_OpenChest(CThingCreatureBase*, CThing*, const CCharString&) -- creature action playing a random loaded animation of the given name (speeds -1.0/1.0, silent sound pair) on the chest it holds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EE7E0, length 105 bytes, `ret 0Ch`. Vtable 0x0127173C.
// Identity: the stored vtable 0x0127173C is CCreatureAction_OpenChest (rtti-vtables.tsv); the catalog label
//   "CCreatureAction_GetOutOfBed" is propagated. Ego_r.pdb: PChest CIntelligentPointer<CThing> at +0xb4 (PDB
//   CCreatureActionBase 0xb4 bytes, retail 0xa8), so it sits at +0xa8 here; the engine header is not used.
// Callees: GetRandomLoadedAnim (__fastcall: owner in ecx, name in edx; `GetRandomLoadedAnimFromName(name, true)`)
//          @ 0x006924B0, CCreatureActionBase::CCreatureActionBase(owner, float, float, CAnimationEntry*,
//          const CSoundPair&, long, long) @ 0x00693B30, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
//          CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (CIntelligentPointer<CThing> vtable 0x01238C6C).
// Shape: one zero register (eax) serves the two trailing long zeros and the CSoundPair temporary's inline
//   constructor; the name reference argument is loaded into edx first; after the base call and the vtable store the
//   PChest member runs its out-of-line base constructor, its vtable store and SetPItem(chest) (inline converting ctor).
class CThingCreatureBase;
class CThing;
class CCharString;
class CAnimationEntry;
class CBaseObject;

class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CAnimationEntry* __fastcall GetRandomLoadedAnim(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

struct CBaseIntelligentPointer {            // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                   // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:   // CIntelligentPointer<CThing>: vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation,
                        const CSoundPair& sound, long c, long d);                 // 0x00693B30
};

class CCreatureAction_OpenChest : public CCreatureActionBase { public:   // vtable 0x0127173C
    CIntelligentPointer<CThing> PChest;     // +0xa8 (PDB +0xb4)
    virtual void v00();
    CCreatureAction_OpenChest(CThingCreatureBase* owner, CThing* chest, const CCharString& animation);
};

CCreatureAction_OpenChest::CCreatureAction_OpenChest(CThingCreatureBase* owner, CThing* chest, const CCharString& animation)
    : CCreatureActionBase(owner, -1.0f, 1.0f, GetRandomLoadedAnim(owner, animation), CSoundPair(), 0, 0)
    , PChest(chest)
{
}