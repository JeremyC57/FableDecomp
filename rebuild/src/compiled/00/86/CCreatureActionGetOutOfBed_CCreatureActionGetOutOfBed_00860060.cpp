// CCreatureAction_GetOutOfBed::CCreatureAction_GetOutOfBed(CThingCreatureBase*, CThing*, const CCharString&) -- creature action playing a random loaded animation of the given name (-1.0/0.5, silent sound pair, 100/50) out of the bed it holds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00860060, length 107 bytes, `ret 0Ch`. Vtable 0x01276B0C.
// Layout: engine/CCreatureAction_GetOutOfBed.h is the PDB build (PBed CIntelligentPointer<CThing> at +0xb4, PDB
//   CCreatureActionBase 0xb4 bytes, retail 0xa8), so PBed sits at +0xa8 here and the header is not used.
// Callees: GetRandomLoadedAnim (__fastcall: owner in ecx, name in edx; `GetRandomLoadedAnimFromName(name, true)`)
//          @ 0x006924B0, CCreatureActionBase::CCreatureActionBase(owner, float, float, CAnimationEntry*,
//          const CSoundPair&, long, long) @ 0x00693B30, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10,
//          CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (CIntelligentPointer<CThing> vtable 0x01238C6C).
// Shape: the trailing longs 100/50 are pushed first, eax zeroes the CSoundPair temporary (inline constructor); the name reference argument is loaded into edx first; after the base call and the vtable store the
//   PBed member runs its out-of-line base constructor, its vtable store and SetPItem(bed) (inline converting ctor).
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

class CCreatureAction_GetOutOfBed : public CCreatureActionBase { public:   // vtable 0x01276B0C
    CIntelligentPointer<CThing> PBed;     // +0xa8 (PDB +0xb4)
    virtual void v00();
    CCreatureAction_GetOutOfBed(CThingCreatureBase* owner, CThing* bed, const CCharString& animation);
};

CCreatureAction_GetOutOfBed::CCreatureAction_GetOutOfBed(CThingCreatureBase* owner, CThing* bed, const CCharString& animation)
    : CCreatureActionBase(owner, -1.0f, 0.5f, GetRandomLoadedAnim(owner, animation), CSoundPair(), 100, 50)
    , PBed(bed)
{
}