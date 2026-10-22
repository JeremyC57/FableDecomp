// Grade: DIFFER(552v552) -- 56 masked lines: `this` lives in ebp in retail (edi here, emitter member pointer in ebp/edi swapped), and retail builds BOTH SVectorParam pairs before the CParticleUpdateParameters ctor (start at esp+30h, end at esp+40h) with the second inline push_back copying from the CRC in esi and the `end` vector (copy-propagated); here the second pair is built after the ctor. Tried 20 variants (named/temporary/const-ref pairs, key locals, Add(key,value), declaration orders)
// Leaf: FrameUpdate
// NCreatureMode::CModeScreamerSuperAttack::FrameUpdate()
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
inline void* operator new(unsigned int, void* place) { return place; }
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC
struct CBaseClass {                                     // vtable 0x01231710
    virtual void v00();
    CBaseClass();                                       // 0x0099A2D0
};
struct __true_type { __true_type() {} };
namespace NParticleEngine {
struct SVectorParam {                                   // pair<unsigned long, C3DVector>
    unsigned long m_key;
    C3DVector     m_value;
    SVectorParam(const unsigned long& key, const C3DVector& value) : m_key(key), m_value(value) {}
};
struct SUpdateParam;
struct CVectorParamVector {                             // retail STLport vector (12 bytes)
    SVectorParam* _M_start;
    SVectorParam* _M_finish;
    SVectorParam* _M_end_of_storage;
    __forceinline CVectorParamVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    void _M_insert_overflow(SVectorParam* position, const SVectorParam& x, const __true_type&, unsigned int count, bool atEnd);   // 0x0076ADA0
    __forceinline void push_back(const SVectorParam& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) SVectorParam(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
struct CUpdateParamVector {
    SUpdateParam* _M_start;
    SUpdateParam* _M_finish;
    SUpdateParam* _M_end_of_storage;
    __forceinline CUpdateParamVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CCompare {};
struct CVectorParamMap {
    CVectorParamVector Vector;                          // +0x00
    CCompare           Compare;                         // +0x0c
    bool               Dirty;                           // +0x0d
    __forceinline CVectorParamMap() : Dirty(false) {}
    __forceinline void Add(const SVectorParam& x) { Vector.push_back(x); Dirty = true; }
};
struct CUpdateParamMap {
    CUpdateParamVector Vector;                          // +0x00
    CCompare           Compare;                         // +0x0c
    bool               Dirty;                           // +0x0d
    __forceinline CUpdateParamMap() : Dirty(false) {}
};
struct CParticleUpdateParameters : CBaseClass {         // <unsigned long, unsigned long>, vtable 0x0125FFDC
    CVectorParamMap VectorParams;                       // +0x04
    CUpdateParamMap UpdateParams;                       // +0x14
    __forceinline CParticleUpdateParameters() {}
    virtual ~CParticleUpdateParameters();               // 0x006E2500
    void SortIfDirty();                                 // 0x0076C750
};
}
struct CTCDParticleEmitter {
    void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters& parameters);   // 0x006E11A0
};
struct CTCGraphicAppearance {
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* position, float scale);   // 0x004C0560
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* Graphic;              // +0x64
    unsigned char  _pad68[0x91 - 0x68];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    void Kill(bool immediately);                // 0x004C9B80
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CThing* GetPItem() const;                         // 0x00A01B50
};
struct CIntelligentPointerThing : CBaseIntelligentPointer { };

struct CModeBase {
    virtual ~CModeBase();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x14];
    void FrameUpdate();                         // 0x0073AC50
};
namespace NCreatureMode {
struct CModeScreamerSuperAttack : CModeBase {
    unsigned char       _pad1c[0x68 - 0x1c];
    unsigned char       Filter[0xc];                        // +0x68
    CIntelligentPointerThing PTarget;                       // +0x74
    CIntelligentPointerThing PLightningBeamParticleEmitter; // +0x7c
    void FrameUpdate();
};

void CModeScreamerSuperAttack::FrameUpdate()
{
    CModeBase::FrameUpdate();
    if (!PLightningBeamParticleEmitter.GetPItem() || (PLightningBeamParticleEmitter.GetPItem()->m_flags91 & 1))
        return;
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1)) {
        C3DVector start;
        ParentThing->Graphic->GetHelperPoint("focus_eye_01", 0, 0, &start, 1.0f);
        const C3DVector& targetPos = PTarget.GetPItem()->GetPosition();
        C3DVector end(targetPos.X, targetPos.Y, targetPos.Z + 1.75f);
        CTCDParticleEmitter* emitter = 0;
        CThing* beam = PLightningBeamParticleEmitter.GetPItem();
        if (beam->HasTC(0x4b))
            emitter = beam->GetTC<CTCDParticleEmitter>(0x4b);
        NParticleEngine::SVectorParam startParam(Calc_004014a0(0, "LIGHTNING_START", 15), start);
        unsigned long endKey = Calc_004014a0(0, "LIGHTNING_END", 13);
        NParticleEngine::CParticleUpdateParameters parameters;
        parameters.VectorParams.Add(startParam);
        parameters.VectorParams.Add(NParticleEngine::SVectorParam(endKey, end));
        parameters.SortIfDirty();
        emitter->SetEmitterUpdateParameters(parameters);
    } else
        PLightningBeamParticleEmitter.GetPItem()->Kill(true);
}
}
