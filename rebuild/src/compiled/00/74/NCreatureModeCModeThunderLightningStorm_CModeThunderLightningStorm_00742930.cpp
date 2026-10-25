// NCreatureMode::CModeThunderLightningStorm::~CModeThunderLightningStorm() -- release the hit-thing list, the storm def, the three effect pointers and both lightning-beam emitter lists, destroy the filter, then the stand base
// Leaf: ??1CModeThunderLightningStorm
// Retail: Fable.exe (TLC, VC7.1) @ 0x00742930, 234 bytes, `ret` by tail jump to ~CModeStandBase @ 0x0073D860.
// Callees: the element destructors through vtable slot 0 (`push 0 ; call [eax]`: the virtual destructor of each
//   CIntelligentPointer<CThing>), free @ 0x00BFEA14 (each vector's storage), the def's vtable slot 1 (DeleteSelf when the
//   count reaches zero), ~CBaseIntelligentPointer @ 0x00A01C10 (x3), ~CCharString @ 0x0099EAE0 (the filter's group), the
//   CAnimFilterBase vtable store 0x0122FFCC, ~CModeStandBase @ 0x0073D860.
// Caller: the landed scalar deleting destructor 0x00742910 (slot 0 of vtable 0x01267DF4 =
//   NCreatureMode::CModeThunderLightningStorm, rtti-vtables.tsv).
// Label: the manifest's "CModeStandBase_Destructor" names only the tail-called base.
// Layout: Ego_r.pdb CModeThunderLightningStorm (0xD4 with Dinkumware 16-byte vectors; retail STLport vectors are 12):
//   Filter +0x68, LightningBeamParticleEmitters +0x74, LightningBeamEndParticleEmitters +0x80, PSkyBeam +0x8C,
//   PCreatorEffect +0x94, PHandGlow +0x9C, PLightningStormDef +0xA4, HitThings +0xA8 (then MainBeamsCreated, Timer,
//   StormRotation, StormRadius, LoopingSound).
// Shape: an empty user destructor on a `__declspec(novtable)` class (no own vtable store; the landed CModeStandNamed
//   0x008CC280 model): members die in reverse order. Each STLport vector runs `_Destroy(start, finish)` (`p->~T()`, a
//   VIRTUAL call since ~CBaseIntelligentPointer is virtual) then frees its storage; the CDefPointer destructor calls the
//   def's inline `Release()` (`dec [ecx+4] ; jne`: the def pointer stays in ecx -- written as an inline member so the
//   count decrement does not force a reload of PDef) and nulls the pointer inside the non-null arm; the filter's
//   implicit destructor stores only CAnimFilterBase's vtable.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CThing;

class CBaseIntelligentPointer {                             // 8 bytes: vptr + the pointee link
public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10 (slot 0: the scalar deleting dtor)
    void* PData;                                            // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};

class CAnimFilterBase {                                     // vtable 0x0122FFCC
public:
    virtual void v00();
    ~CAnimFilterBase() { }
};

class CAnimFilterOfGroup : public CAnimFilterBase {         // 12 bytes; its own vtable is not re-stored
public:
    CCharString   Group;                                    // +0x04
    unsigned long GroupCRC;                                 // +0x08
};

class CDefPointeeBase {
public:
    virtual void v00();
    virtual void DeleteSelf() const;                        // slot 1
    mutable long RefCount;                                  // +0x04
    void Release() const
    {
        if (--RefCount == 0)
            DeleteSelf();
    }
};

template <class T> class CDefPointer {
public:
    T* PDef;
    ~CDefPointer()
    {
        if (PDef) {
            PDef->Release();
            PDef = 0;
        }
    }
};

class CSpecialAbilitiesThunderLightningStormDef : public CDefPointeeBase { };

namespace std {
template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class ForwardIter>
inline void _Destroy(ForwardIter first, ForwardIter last)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}

template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
}

class CModeStandBase {
public:
    virtual void v00();
    ~CModeStandBase();                                      // 0x0073D860
    unsigned char _pad04[0x68 - 0x04];
};

namespace NCreatureMode {

class __declspec(novtable) CModeThunderLightningStorm : public CModeStandBase {   // vtable 0x01267DF4
public:
    CAnimFilterOfGroup                                       Filter;                            // +0x68
    std::vector<CIntelligentPointer<CThing> >               LightningBeamParticleEmitters;     // +0x74
    std::vector<CIntelligentPointer<CThing> >               LightningBeamEndParticleEmitters;  // +0x80
    CIntelligentPointer<CThing>                              PSkyBeam;                          // +0x8C
    CIntelligentPointer<CThing>                              PCreatorEffect;                    // +0x94
    CIntelligentPointer<CThing>                              PHandGlow;                         // +0x9C
    CDefPointer<const CSpecialAbilitiesThunderLightningStormDef> PLightningStormDef;            // +0xA4
    std::vector<CIntelligentPointer<CThing> >               HitThings;                         // +0xA8
    bool                                                     MainBeamsCreated;                  // +0xB4
    long                                                     Timer;                             // +0xB8
    float                                                    StormRotation;                     // +0xBC
    float                                                    StormRadius;                       // +0xC0
    unsigned long                                            LoopingSound;                      // +0xC4
    ~CModeThunderLightningStorm();
};

CModeThunderLightningStorm::~CModeThunderLightningStorm()
{
}

}