// CAIStateGroup_ThrowLogsOnFire::Update() -- collect the knothole logs, walk to and throw each onto the fire
// Leaf: Update
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x00917130.
// Name fix (agentNF7 2026-11-10): the local CBaseIntelligentPointer stand-in replaced by the shared engine/CIntelligentPointer.h (Ego_r.pdb CBaseIntelligentPointer: GetPItem returns CBaseObject*, SetPItem takes CBaseObject*, virtual dtor), so the calls bind the PDB spellings.
#include "fable_compiler.h"
// NF49 (round 7): the AI state-group base and the manager come from the shared headers (were local CAIStateGroupBase /
// CDefinitionManager models): the manager accessor 0x008FCFB0 is the protected const
// ?PeekDefinitionManager@CAIStateGroupBase@@IBEABVCGameDefinitionManager@@XZ (Ego_r.pdb / FableWin.pdb; engine/CAIStateGroupBase.h
// _MEMBERS + _PEEKDEFINITIONMANAGER + _PROTECTED), was a made-up GetPDefinitionManager returning CDefinitionManager*; the nested
// CAIFilterBase is the header's (_FILTER, which needs the _AIACTIONS forward enums); GetDefGlobalIndexFromName 0x009AD410 is
// engine/CDefinitionManager.h's const member, reached through CGameDefinitionManager.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_FILTER
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER_PROTECTED
#include <stdlib.h>
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"   // CBaseIntelligentPointer 0x00A01B10 / ~ 0x00A01C10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 (Ego_r.pdb)
// CIntelligentPointerThing = the shared CIntelligentPointer<CThing> (engine/CIntelligentPointer.h).
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CAIStateGroupBase.h"
namespace { extern CCharString LOG_DEF_NAME; }      // "OBJECT_KNOTHOLE_LOG_MOVEABLE"
namespace { extern CCharString PICK_UP_ANIM_NAME; }      // "ST_WORK_GET_CRATE"
namespace { extern CCharString HOLDING_LOG_ANIM_NAME; }      // "ST_HOLDING_CRATE"
namespace { extern CCharString PUT_LOG_IN_BONFIRE_ANIM_NAME; }      // "ST_WORK_PUT_CRATE"

struct __true_type { };
template <class T> struct vector {              // STLport vector<T> (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    vector(const vector& other);                // 0x00916CE0
    ~vector() { if (_M_start) free(_M_start); }
    template <class Tag> void _M_insert_overflow(T* position, const T& x, const Tag&, unsigned int fillLength, bool atEnd);  // 0x00916D50
    void push_back(const T& x)                  // 0x00916EF0
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish) *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
    FABLE_FORCEINLINE void push_back_inline(const T& x)   // the same STLport body, expanded at the second site
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish) *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
// Same-TU body of the STLport vector<const CCharString*>::_M_insert_overflow instantiation (tag unused).
template <class T> template <class Tag> __declspec(noinline) void vector<T>::_M_insert_overflow(T* pos, const T& x, const Tag&, unsigned int n, bool atend)
{
    unsigned int oldSize = _M_finish - _M_start;
    unsigned int len = oldSize + (oldSize < n ? n : oldSize);
    T* ns = len ? (T*)malloc(len * sizeof(T)) : 0;
    T* nf = ns;
    T* p;
    for (p = _M_start; p != pos; ++p) *nf++ = *p;
    for (; n != 0; --n) *nf++ = x;
    if (!atend)
        for (p = pos; p != _M_finish; ++p) *nf++ = *p;
    if (_M_start)
        free(_M_start);
    _M_start = ns;
    _M_finish = nf;
    _M_end_of_storage = ns + len;
}
class CTCCarrying { public:                            // interface 0x46
    CThing* GetThingInPrimarySlot() const;            // 0x006A1440
};
struct CThingCreature : CThing {
    void* GetAnimFromName(const CCharString& name, long index); // 0x00662A00
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreature* PCreature;                  // +0x18
};
struct CAIStateGroup_MoveGoodsBase : CAIStateGroupBase {
    bool FetchGoods(CIntelligentPointer<CThing> source, float distance, vector<const CCharString*> anims,
        const CAIFilterBase& filter, CThing* goods);   // 0x00915890
    bool PlaceGoods(CIntelligentPointer<CThing> target, float distance, vector<const CCharString*> anims,
        const CAIFilterBase& filter, CThing* goods);   // 0x00915B60
};
struct CAIStateGroup_ThrowLogsOnFire : CAIStateGroup_MoveGoodsBase {
    CThing* GetPLogPile();                      // 0x00917050
    CThing* GetPBonfire();                      // 0x009170B0
    virtual void Update();
};

void CAIStateGroup_ThrowLogsOnFire::Update()
{
    CThing* pile = GetPLogPile();
    if (pile == 0) {
        Terminate();
        return;
    }
    CThing* fire = GetPBonfire();
    if (fire == 0) {
        Terminate();
        return;
    }
    const CTCCarrying* carrying;
    PBrain->PCreature->PeekTC(TCI_CARRYING, &carrying);
    if (carrying->GetThingInPrimarySlot() == 0) {
        PeekDefinitionManager().GetDefGlobalIndexFromName(LOG_DEF_NAME);
        PBrain->PCreature->GetAnimFromName(HOLDING_LOG_ANIM_NAME, 0);
        vector<const CCharString*> getAnims;
        getAnims.push_back(&PICK_UP_ANIM_NAME);
        bool failed = !FetchGoods(CIntelligentPointer<CThing>(pile), 3.5f, getAnims, CAIFilterBase(), 0);
        if (failed) {
            Terminate();
            return;
        }
    }
    {
        vector<const CCharString*> putAnims;
        putAnims.push_back_inline(&PUT_LOG_IN_BONFIRE_ANIM_NAME);
        PlaceGoods(CIntelligentPointer<CThing>(fire), 4.0f, putAnims, CAIFilterBase(), 0);
        Terminate();
    }
}