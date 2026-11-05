// CMiniMapDisplay::~CMiniMapDisplay() -- forget the region minimap texture and region, then destroy the RegionMinimap, the MinimapList entries, the Markers list and the two primitive-handle vectors, then the CBaseClassNonCopyable base
// Leaf: ??1CMiniMapDisplay
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00442610, 201 bytes. Callers: the ??_G 0x004426E0 (slot 0 of the CMiniMapDisplay vtable
//   0x01231768) and 0x00443D2A (the owner's destructor, CMiniMapDisplay embedded at its +0x28; Ego_r.pdb has
//   ?GetMiniMap@CPlayerGui@@QAEAAVCMiniMapDisplay@@XZ).
// Identity: first store = vtable 0x01231768 (RTTI .?AVCMiniMapDisplay@@ : CBaseClassNonCopyable : CBaseClass); user-declared
//   virtual dtor (the store stays; Ego_r.pdb names only ??_GCMiniMapDisplay@@UAEPAXI@Z, the dtor being inlined there).
// Callees: CRegionMinimap::~CRegionMinimap 0x0066CCF0 (Ego_r.pdb ??1CRegionMinimap@@QAE@XZ); CMinimapData::~CMinimapData
//   0x00642A60 (bound ??1CMinimapData@@QAE@XZ); std::_List_base<CMiniMapDisplay::CMarkerInfo>::clear 0x00440730 (kept out of
//   line by VC7.1 from the _list.c body, as retail); free 0x00BFEA14; CEnginePrimitiveHandle's virtual dtor (slot 0,
//   `push 0 ; call [eax]`); CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (tail jump).
// Layout (Ego_r.pdb less the Dinkumware growth): MapPrimitiveHandles +0x04, MarkerPrimitiveHandles +0x10 (vector
//   <CEnginePrimitiveHandle>, 8-byte polymorphic handles), MiniMapGraphic +0x1C, Markers list<CMarkerInfo> +0x20,
//   MarkerFlashFade (CFadeInFadeOutBase, 0x1C) +0x24, four bools +0x40, MinimapList vector<CMinimapData> (0x3C elements)
//   +0x44, RegionMinimapTexture +0x50, RegionMinimap (0x64, engine/CRegionMinimap.h) +0x54, PRegion +0xB8, UpdateRequired.
// Shape: the body is `RegionMinimapTexture = 0; PRegion = 0;`; then the members in reverse order (STLport 4.6 bodies).
// Label: manifest "_global::RenderState_Destructor" was descriptive.
// Header debt: engine/CRegionMinimap.h is a POD without the dtor (retail calls 0x0066CCF0), so CRegionMinimap is a local
//   0x64-byte model with the out-of-line dtor; CMinimapData / CEnginePrimitiveHandle / CBaseIntelligentPointer are local
//   models (no retail headers). CFadeInFadeOutBase and CBaseClassNonCopyable come from the shared headers.
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
// ---- vector (stl/_vector.h)
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
// ---- list (stl/_list.h, stl/_list.c)
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    ~_List_base()
    {
        clear();
        _M_node.deallocate((_Node*)_M_node._M_data, 1);
    }
    void clear();
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
};
} // namespace std

#include "engine/CBaseClassNonCopyable.h"
#include "engine/CFadeInFadeOutBase.h"
class CThing;
class CTexture;
class CRegion;
class CBaseObject;
#pragma pack(push, 1)
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)
class CEnginePersistentPrimitiveBase;
class CEnginePrimitiveHandle {                                           // Ego_r.pdb 8 bytes: vptr + Data
public:
    virtual ~CEnginePrimitiveHandle();
    CEnginePersistentPrimitiveBase* Data;                                // +0x04 CIVCountedPointer<CEnginePersistentPrimitiveBase>
};
class CMinimapData {                                                     // retail 0x3c bytes
public:
    ~CMinimapData();                                                     // 0x00642A60
    unsigned char _data[0x3c];
};
class CRegionMinimap {                                                   // engine/CRegionMinimap.h layout, 0x64 bytes
public:
    ~CRegionMinimap();                                                   // 0x0066CCF0
    unsigned char _data[0x64];
};
class CMiniMapDisplay : public CBaseClassNonCopyable {
public:
    class CMarkerInfo {
    public:
        CIntelligentPointer<const CThing> PThing;                        // +0x00
        long GraphicIndex;                                               // +0x08
        bool AutoMarker;                                                 // +0x0c
    };
    virtual ~CMiniMapDisplay();
    std::vector<CEnginePrimitiveHandle> MapPrimitiveHandles;             // +0x04
    std::vector<CEnginePrimitiveHandle> MarkerPrimitiveHandles;          // +0x10
    long MiniMapGraphic;                                                 // +0x1c
    std::list<CMarkerInfo> Markers;                                      // +0x20
    CFadeInFadeOutBase MarkerFlashFade;                                  // +0x24
    bool MiniMapIsPrisonCourtyard1;                                      // +0x40
    bool MiniMapIsPrisonCourtyard2;                                      // +0x41
    bool MiniMapIsHeadsmansHillNorth;                                    // +0x42
    bool MiniMapIsHeadsmansHillSouth;                                    // +0x43
    std::vector<CMinimapData> MinimapList;                               // +0x44
    CTexture* RegionMinimapTexture;                                      // +0x50
    CRegionMinimap RegionMinimap;                                        // +0x54
    CRegion* PRegion;                                                    // +0xb8
    bool UpdateRequired;                                                 // +0xbc
};
CMiniMapDisplay::~CMiniMapDisplay()
{
    RegionMinimapTexture = 0;
    PRegion = 0;
}