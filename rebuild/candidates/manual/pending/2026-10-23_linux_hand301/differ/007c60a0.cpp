// Grade: DIFFER(989v960) -- first pass: per-sword IsAlive gate, EMITTER_BASE/EMITTER_TIP dummy lookups (named CCharString built before the pushes, as retail), inline CRC32 of both literals, CEmitterUpdateParameters with two sorted vectors (first push_back folds to the overflow call), sort-if-dirty, TC 0x4b emitter hand-off and the out-of-line params dtor all match in shape; ours keeps 0 in ebx across the body and spills this, so the CRC loops use ebp and the loop is rotated with a jump-in; frame 0xa8 vs 0xa0 (each name string gets its own slot)
// CTCGhostSword::UpdateGhostSwordParticleEffects() -- for each live ghost sword, reads its EMITTER_BASE / EMITTER_TIP dummy positions, packs them (keyed by name CRC) into emitter update parameters and hands them to the paired live particle emitter
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
extern const unsigned long g_crcTable_0129a168[256];    // 0x0129A168
__forceinline unsigned long CalcCRC(unsigned long seed, const char* data, unsigned long length)
{
    if (length == 0)
        return seed;
    for (unsigned long index = 0; index < length; ++index)
        seed = g_crcTable_0129a168[((const unsigned char*)data)[index] ^ (seed & 0xff)] ^ (seed >> 8);
    return seed;
}
class CBaseObject;
class CBaseIntelligentPointer {
public:
    void* _vfptr;
    CBaseObject* PItem;
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
class CTCGraphicAppearance {
public:
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* pos, CRightHandedSet* orientation, float time);   // 0x004C08C0
};
struct CTCPair { long Type; void* TC; };
struct CTCMap {
    CTCPair* _M_start;
    CTCPair* _M_finish;
    CTCPair* LowerBound(const long& key);               // 0x0040F020
    __forceinline void* Find(long type)
    {
        long key = type;
        CTCPair* it = LowerBound(key);
        if (it == _M_finish || it->Type > type)
            it = _M_finish;
        return it->TC;
    }
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long PresentBits[9];                       // +0x20
    CTCMap TCs;                                         // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* Graphics;                     // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char Flags91;                              // +0x91
    __forceinline unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
__forceinline bool IsAlive(const CBaseIntelligentPointer& p)
{
    return p.GetPItem() && !(((CThing*)p.GetPItem())->Flags91 & 1);
}
struct CEmitterPosition {
    unsigned long NameCRC;
    C3DVector Position;
    CEmitterPosition(unsigned long crc, const C3DVector& pos) : NameCRC(crc), Position(pos) {}
};
struct CEmitterPositionLess {};
struct CEmitterOther { long A, B, C, D; };
struct CEmitterOtherLess {};
struct __true_type {};
void __fastcall SortPositions(CEmitterPosition* first, CEmitterPosition* last, CEmitterPositionLess comp);   // 0x0076C000
void __fastcall SortOthers(CEmitterOther* first, CEmitterOther* last, CEmitterOtherLess comp);             // 0x0076C1A0
struct CPositionVector {
    CEmitterPosition* _M_start;
    CEmitterPosition* _M_finish;
    CEmitterPosition* _M_end_of_storage;
    CEmitterPositionLess Comp;
    bool Unsorted;
    void _M_insert_overflow(CEmitterPosition* pos, const CEmitterPosition& x, const __true_type&, unsigned int fill, bool atEnd);   // 0x0076ADA0
    __forceinline void push_back(const CEmitterPosition& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else {
            __true_type tag;
            _M_insert_overflow(_M_finish, x, tag, 1, true);
        }
        Unsorted = true;
    }
    __forceinline void Sort()
    {
        if (Unsorted) {
            SortPositions(_M_start, _M_finish, Comp);
            Unsorted = false;
        }
    }
};
struct COtherVector {
    CEmitterOther* _M_start;
    CEmitterOther* _M_finish;
    CEmitterOther* _M_end_of_storage;
    CEmitterOtherLess Comp;
    bool Unsorted;
    __forceinline void Sort()
    {
        if (Unsorted) {
            SortOthers(_M_start, _M_finish, Comp);
            Unsorted = false;
        }
    }
};
class CBase {
public:
    CBase();                                            // 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
class CEmitterUpdateParameters : public CBase {
public:
    CPositionVector Positions;                          // +0x04
    COtherVector Others;                                // +0x14
    __forceinline CEmitterUpdateParameters()
    {
        Positions._M_start = 0; Positions._M_finish = 0; Positions._M_end_of_storage = 0; Positions.Unsorted = false;
        Others._M_start = 0; Others._M_finish = 0; Others._M_end_of_storage = 0; Others.Unsorted = false;
    }
    ~CEmitterUpdateParameters();                        // 0x006E2500
};
class CTCDParticleEmitter {
public:
    void SetEmitterUpdateParameters(const CEmitterUpdateParameters& params);   // 0x006E11A0
};
template<class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
};
class CTCGhostSword {
public:
    virtual void v00();
    unsigned char _pad04[0x10 - 0x04];
    vector<CBaseIntelligentPointer> Swords;             // +0x10
    vector<CBaseIntelligentPointer> Emitters;           // +0x1C
    void UpdateGhostSwordParticleEffects();
};

void CTCGhostSword::UpdateGhostSwordParticleEffects()
{
    for (unsigned int i = 0; i < Swords.size(); ++i) {
        if (!IsAlive(Swords._M_start[i]))
            continue;
        C3DVector basePos = ((CThing*)Swords._M_start[i].GetPItem())->GetPosition();
        CTCGraphicAppearance* graphics = ((CThing*)Swords._M_start[i].GetPItem())->Graphics;
        CRightHandedSet baseOrientation;
        if (graphics) {
            CCharString name("EMITTER_BASE");
            graphics->GetDummyObject(name, 0, 0, &basePos, &baseOrientation, 1.0f);
        }
        CEmitterPosition base(CalcCRC(0, "EMITTER_BASE", 12), basePos);
        C3DVector tipPos = ((CThing*)Swords._M_start[i].GetPItem())->GetPosition();
        CRightHandedSet tipOrientation;
        if (graphics) {
            CCharString name("EMITTER_TIP");
            graphics->GetDummyObject(name, 0, 0, &tipPos, &tipOrientation, 1.0f);
        }
        CEmitterPosition tip(CalcCRC(0, "EMITTER_TIP", 11), tipPos);
        CEmitterUpdateParameters params;
        params.Positions.push_back(base);
        params.Positions.push_back(tip);
        params.Positions.Sort();
        params.Others.Sort();
        if (IsAlive(Emitters._M_start[i])) {
            CTCDParticleEmitter* emitter = 0;
            CThing* thing = (CThing*)Emitters._M_start[i].GetPItem();
            if (thing->HasTC(0x4b))
                emitter = (CTCDParticleEmitter*)thing->TCs.Find(0x4b);
            emitter->SetEmitterUpdateParameters(params);
        }
    }
}
