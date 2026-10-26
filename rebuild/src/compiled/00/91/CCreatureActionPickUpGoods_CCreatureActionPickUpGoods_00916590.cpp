// CCreatureAction_PickUpGoods::CCreatureAction_PickUpGoods(CThingCreatureBase*, CThing*, const CCharString&) -- creature action playing a random loaded animation of the given name (speeds -1.0/0.0, silent sound pair) on the goods it picks up
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00916590, length 110 bytes, `ret 0Ch`. Vtable 0x0128FBF4.
// Identity: the stored vtable 0x0128FBF4 is CCreatureAction_PickUpGoods (rtti-vtables.tsv); the catalog label
//   "CCreatureAction_GetOutOfBed" is propagated. Ego_r.pdb: PThingToPickUp CIntelligentPointer<CThing> at +0xb4 (PDB
//   CCreatureActionBase 0xb4 bytes, retail 0xa8), so it sits at +0xa8 here; the engine header is not used.
// Callees: GetRandomLoadedAnim (__fastcall: owner in ecx, name in edx; `GetRandomLoadedAnimFromName(name, true)`)
//          @ 0x006924B0, CCreatureActionBase::CCreatureActionBase(owner, float, float, CAnimationEntry*,
//          const CSoundPair&, long, long) @ 0x00693B30, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
//          CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (CIntelligentPointer<CThing> vtable 0x01238C6C).
// Shape: with the 0.0f speed among the arguments every zero stays an immediate (`push 0` x3 and the two CSoundPair
//   stores; the same source with 1.0f shares one eax zero, see 0x007EE7E0); the name reference argument is loaded into edx first; after the base call and the vtable store the
//   PThingToPickUp member runs its out-of-line base constructor, its vtable store and SetPItem(goods) (inline converting ctor).
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

class CCreatureAction_PickUpGoods : public CCreatureActionBase { public:   // vtable 0x0128FBF4
    CIntelligentPointer<CThing> PThingToPickUp;     // +0xa8 (PDB +0xb4)
    virtual void v00();
    CCreatureAction_PickUpGoods(CThingCreatureBase* owner, CThing* goods, const CCharString& animation);
};

CCreatureAction_PickUpGoods::CCreatureAction_PickUpGoods(CThingCreatureBase* owner, CThing* goods, const CCharString& animation)
    : CCreatureActionBase(owner, -1.0f, 0.0f, GetRandomLoadedAnim(owner, animation), CSoundPair(), 0, 0)
    , PThingToPickUp(goods)
{
}