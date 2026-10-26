// CCreatureAction_GetItemOutOfPocket::CCreatureAction_GetItemOutOfPocket(CThingCreatureBase*, long) -- creature action playing a random loaded "ST_GET_ITEM_OUT_OF_POCKET" animation (speeds -1.0/0.5, silent sound pair) for an item def index
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00945410, length 109 bytes, `ret 8`. Vtable 0x01292BC4.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetRandomLoadedAnim (__fastcall: owner in ecx,
//          name in edx) @ 0x006924B0, CCreatureActionBase::CCreatureActionBase(owner, float, float, CAnimationEntry*,
//          const CSoundPair&, long, long) @ 0x00693B30, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the implicit literal temporary is built first and re-addressed with `lea`; one eax zero serves the two long
//   pushes and the CSoundPair constructor; after the string dies the vtable store and the ItemDefIndex initialiser
//   (second argument slot).
// Layout: Ego_r.pdb puts ItemDefIndex at +0xb4 (PDB CCreatureActionBase 0xb4 bytes, retail 0xa8): +0xa8 here; no
//   engine header exists for this class.
class CThingCreatureBase;
class CAnimationEntry;

class CCharString { public:                        // one pointer; the defaulted length makes a literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CAnimationEntry* __fastcall GetRandomLoadedAnim(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation,
                        const CSoundPair& sound, long c, long d);                 // 0x00693B30
};

class CCreatureAction_GetItemOutOfPocket : public CCreatureActionBase { public:   // vtable 0x01292BC4
    long ItemDefIndex;                      // +0xa8 (PDB +0xb4)
    virtual void v00();
    CCreatureAction_GetItemOutOfPocket(CThingCreatureBase* owner, long itemDefIndex);
};

CCreatureAction_GetItemOutOfPocket::CCreatureAction_GetItemOutOfPocket(CThingCreatureBase* owner, long itemDefIndex)
    : CCreatureActionBase(owner, -1.0f, 0.5f, GetRandomLoadedAnim(owner, "ST_GET_ITEM_OUT_OF_POCKET"), CSoundPair(), 0, 0)
    , ItemDefIndex(itemDefIndex)
{
}