// CAIStateGroup_ThrowLogsOnFire::Update() -- collect the knothole logs, walk to and throw each onto the fire
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x00917130.
#include <stdlib.h>
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        bool has = (unsigned char)(m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1;
        if (has) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {        // CIntelligentPointer<CThing> (vtable 0x01238C6C)
    virtual void v00();
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
extern CCharString g_CCharString_013bb674;      // "OBJECT_KNOTHOLE_LOG_MOVEABLE"
extern CCharString g_CCharString_013bb678;      // "ST_WORK_GET_CRATE"
extern CCharString g_CCharString_013bb67c;      // "ST_HOLDING_CRATE"
extern CCharString g_CCharString_013bb680;      // "ST_WORK_PUT_CRATE"

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
    __forceinline void push_back_inline(const T& x)   // the same STLport body, expanded at the second site
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
    CThing* GetThingInPrimarySlot();            // 0x006A1440
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
};
struct CThingCreature : CThing {
    void* GetAnimFromName(const CCharString& name, long index); // 0x00662A00
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreature* PCreature;                  // +0x18
};
class CAIStateGroupBase { public:
    class CAIFilterBase { public:                      // vtable 0x0128FD20
        virtual bool operator()(CThing* thing) const;   // 0x00916A50
    };
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CDefinitionManager* GetPDefinitionManager() const;  // 0x008FCFB0
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_MoveGoodsBase : CAIStateGroupBase {
    bool FetchGoods(CIntelligentPointerThing source, float distance, vector<const CCharString*> anims,
        const CAIFilterBase& filter, CThing* goods);   // 0x00915890
    bool PlaceGoods(CIntelligentPointerThing target, float distance, vector<const CCharString*> anims,
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
    CTCCarrying* carrying;
    PBrain->PCreature->GetInterface(&carrying, 0x46);
    if (carrying->GetThingInPrimarySlot() == 0) {
        GetPDefinitionManager()->GetDefGlobalIndexFromName(g_CCharString_013bb674);
        PBrain->PCreature->GetAnimFromName(g_CCharString_013bb67c, 0);
        vector<const CCharString*> getAnims;
        getAnims.push_back(&g_CCharString_013bb678);
        bool failed = !FetchGoods(CIntelligentPointerThing(pile), 3.5f, getAnims, CAIFilterBase(), 0);
        if (failed) {
            Terminate();
            return;
        }
    }
    {
        vector<const CCharString*> putAnims;
        putAnims.push_back_inline(&g_CCharString_013bb680);
        PlaceGoods(CIntelligentPointerThing(fire), 4.0f, putAnims, CAIFilterBase(), 0);
        Terminate();
    }
}