// CTCCreatureHitNotification::DrawHitNotification(const CEnginePrimitivePrepareInfo& info) const -- hand every hit flash's geometry to the engine as a weapon-trail primitive (global weapon-trail graphic, current world time in seconds)
// Grade: DIFFER(227v227) -- IMPROVED (48 -> 9 difflines): the real STLport list/vector templates, PeekWorld/GetFrame and the class model put this in edi as retail; left: the loop guard folds end() into `cmp ebx,[edi+10h]` (retail `mov eax,[edi+10h] ; cmp ebx,eax`) and the info pointer is then read through eax (retail edx), with a 1-byte vs 2-byte alignment pad. Tried: reversed end() compare, info.Engine without the engine local.
// Leaf: ?DrawHitNotification@CTCCreatureHitNotification@@QBEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail 0x00864FF0, 0xE3 bytes, __thiscall `ret 4`. PDB (Ego_r / FableWin): ?DrawHitNotification@CTCCreatureHitNotification@@QBEXABVCEnginePrimitivePrepareInfo@@@Z.
// Caller: the creature's primitive preparation @ 0x00663D75, behind HasTC(0xCD) (CTCCreatureHitNotification's
// GetInterfaceType returns 0xCD, vtable 0x012436CC slot 21).
// Callees: CTCBase::GetDefinitionManager() const @ 0x006869D0 (`mov ecx,[ecx+4] ; jmp`), CTCBase::PeekWorld() const
// @ 0x00686A80 (Ego_r donor order GetDefinitionManager, GetWorld, PeekWorld = retail 0x006869D0 / 0x00686A70 / 0x00686A80,
// the last two identical bodies), the static __fastcall CWorld::GetFrame() @ 0x0049D870, CEnginePrimitiveWeaponTrail ctor
// @ 0x007703B0 (landed), CIEngine::AddPrimitive (vtable slot 20, +0x50; CEngine/CIEngine slot 20 in rtti-vtables), free
// @ 0x00BFEA14 (the temporary's inline STLport list destructor).
// Globals: g_framesPerSecond @ 0x01375550.
// Layout (retail): CTCBase (engine/CTCBase.h) ParentThing +0x04; Flashes (STLport vector<NCreatureHitNotification::CFlash>,
// 8-byte elements: the 4-byte list FlashGeometry + CreationFrame) +0x0C; CThing UniqueID (unsigned __int64) +0x18; the
// definition manager's CDefPointer<CGlobalDef> +0xD0 (inline PeekGlobalDef), CGlobalDef GraphicWeaponTrail +0x5C (Ego_r
// +0x60); CEnginePrimitivePrepareInfo Engine +0x34.
// Shape: the engine pointer and the thing's UniqueID are loop-body locals; the frames-per-second global is a named local
// read before GetFrame (`fild ; fidiv`); the temporary's inline STLport list destructor is clear() + free(sentinel).
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;                                 // +0x00
    _List_node_base* _M_prev;                                 // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                              // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {   // STLport list, 4 bytes
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _Tp, allocator<_List_node<_Tp> > > _M_node;   // +0x00
    void clear()
    {
        _List_node<_Tp>* __cur = (_List_node<_Tp>*)_M_node._M_data->_M_next;
        while (__cur != _M_node._M_data) {
            _List_node<_Tp>* __tmp = __cur;
            __cur = (_List_node<_Tp>*)__cur->_M_next;
            free(__tmp);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
    ~list()
    {
        clear();
        free(_M_node._M_data);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // retail STLport vector (12 bytes)
public:
    typedef const _Tp* const_iterator;
    _Tp* _M_start;                                            // +0x00
    _Tp* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
};
}   // namespace std

enum EEngineInterpolationSet { };
class CEnginePrimitiveThemeChannelDesc { public:              // 4 bytes (PDB)
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
};
class CEnginePrimitive { public:                              // 12 bytes (PDB)
    long                             Type;               // +0x00 EEnginePrimitiveType
    EEngineInterpolationSet          InterpolationSet;   // +0x04
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;   // +0x08
};
class CEnginePrimitiveWeaponTrail : public CEnginePrimitive {
public:
    class CWeaponTrailEntry { public: unsigned long m_data[7]; };   // 28 bytes
    enum EWeaponTrailType { };
    CEnginePrimitiveWeaponTrail(unsigned __int64 handle, const std::list<CWeaponTrailEntry>& trail, unsigned long graphicIndex,
                                unsigned long graphicFrame, EWeaponTrailType trailType, unsigned char alpha, float skipPoints,
                                float currentTime, float fallOffTime, float fadeStart,
                                EEngineInterpolationSet interpolationSet);   // 0x007703B0
    std::list<CWeaponTrailEntry> Trail;                  // +0x0c
    unsigned long    GraphicIndex;                       // +0x10
    unsigned long    GraphicFrame;                       // +0x14
    EWeaponTrailType TrailType;                          // +0x18
    float            SkipPoints;                         // +0x1c
    float            FadeStart;                          // +0x20
    unsigned char    Alpha;                              // +0x24
    float            CurrentTime;                        // +0x28
    float            FallOffTime;                        // +0x2c
};

#define VF(n) virtual void v##n();
class CIEngine { public:
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09)
    VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19)
    virtual void AddPrimitive(const CEnginePrimitive& primitive);   // slot 20 (+0x50)
};
#undef VF

class CEnginePrimitivePrepareInfo { public:
    unsigned char _pad00[0x34];                          // CInterpolationInfo Interpolate
    CIEngine*     Engine;                                // +0x34
};

class CGlobalDef { public:
    unsigned char _pad00[0x5c];
    long          GraphicWeaponTrail;                    // +0x5c (retail; Ego_r +0x60)
};
class CDefPointeeBase;
class CGameDefinitionManager { public:
    unsigned char _pad00[0xd0];
    CGlobalDef*   PGlobalDef_Object;                     // +0xd0 CDefPointer<const CGlobalDef>::Object
    const CGlobalDef& PeekGlobalDef() const { return *PGlobalDef_Object; }
};

class CWorld { public:
    static long __fastcall GetFrame();                   // 0x0049D870
};
extern long g_framesPerSecond;                           // 0x01375550

class CThing { public:
    unsigned char    _pad00[0x18];
    unsigned __int64 UniqueID;                           // +0x18
};

namespace NCreatureHitNotification
{
    class CFlash { public:
        std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> FlashGeometry;   // +0x00
        long             CreationFrame;                  // +0x04
    };
}

class CTCBase : public CTCBaseLayout {
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
    const CWorld& PeekWorld() const;                              // 0x00686A80
};

class CTCCreatureHitNotification : public CTCBase {
public:
    void DrawHitNotification(const CEnginePrimitivePrepareInfo& info) const;
private:
    std::vector<NCreatureHitNotification::CFlash> Flashes;   // +0x0c
};

void CTCCreatureHitNotification::DrawHitNotification(const CEnginePrimitivePrepareInfo& info) const
{
    long graphic = GetDefinitionManager().PeekGlobalDef().GraphicWeaponTrail;
    const CWorld& world = PeekWorld();
    long framesPerSecond = g_framesPerSecond;
    float time = (float)world.GetFrame() / framesPerSecond;
    for (std::vector<NCreatureHitNotification::CFlash>::const_iterator it = Flashes.begin(); it != Flashes.end(); ++it) {
        CIEngine* engine = info.Engine;
        unsigned __int64 id = ParentThing->UniqueID;
        engine->AddPrimitive(CEnginePrimitiveWeaponTrail(id, it->FlashGeometry, graphic, 0,
                                                         (CEnginePrimitiveWeaponTrail::EWeaponTrailType)2, 1,
                                                         0.0f, time, 100.0f, 0.0f, (EEngineInterpolationSet)0));
    }
}
