// NGraphicAppearance::CTCGraphicAppearance::SetLightingChannel(const CEnginePrimitiveThemeChannelDesc& channel, bool includeCarried, bool includeWeapons) -- store the lighting channel, optionally pass it on to the graphic appearance of every carried thing (weapons only when asked), then re-render
// Leaf: SetLightingChannel
//
// Retail 0x004BFB40, 272 bytes incl. padding (`ret 0Ch`). Ego_r.pdb `?SetLightingChannel@CTCGraphicAppearance@
// NGraphicAppearance@@QAEXABVCEnginePrimitiveThemeChannelDesc@@_N1@Z`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined out-parameter GetInterface whose end/hit choice is ONE
// ternary inside the store -- that lands the component straight in ecx for the next call (an if-assign gives
// `mov eax,[eax+4] ... mov ecx,eax`, HasTC + a by-value GetTC moves the key out of the dead argument slot): CTCCarrying type 0x46 = bit 6
// of the bitset word +0x28, and the carried thing's CTCGraphicAppearance type 0x5B = bit 27; the weapon test is type 0x48 =
// bit 8, `test dh,1`), CTCCarrying::GetAllCarriedThings(vector<CThing*>&) const @ 0x006A2220 (manifest "RemapGroups" is
// propagated), itself recursively, free @ 0x00BFEA14 (the vector buffer), CThing::AddToRenderList @ 0x004C7AC0.
// Layout: retail CTCGraphicAppearance (landed GetAllMeshes model): ParentThing +0x04, LightingChannel +0x40,
// UpdatePrimitiveReasons +0x58; retail CThing bitset +0x20, interface map +0x44.
// Shape: the channel is a one-dword record copied first; the carried-thing vector is an STLport vector declared after the
// carrying lookup (zeroed through edi), walked by index with `*(begin() + i)` against `size()`; the recursion re-reads the
// includeCarried argument from its slot; `this` is spilled to the frame because ebx carries includeWeapons in the loop.
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14

class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    void AddToRenderList();                                 // 0x004C7AC0
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

template <class T> struct CAllocator { CAllocator() { } };  // STLport allocator<T> (empty)
template <class T> struct CAllocProxy : CAllocator<T> {     // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {                     // STLport _Vector_base<T, allocator<T> >
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
    ~CVectorBase() { if (_M_start) free(_M_start); }
};
template <class T> struct CRetailVector : CVectorBase<T> {  // STLport vector<T> (12 bytes)
    CRetailVector() : CVectorBase<T>(CAllocator<T>()) { }
    T* begin() const { return this->_M_start; }
    unsigned int size() const { return this->_M_finish - this->_M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};

class CTCCarrying { public:
    void GetAllCarriedThings(CRetailVector<CThing*>& things) const;   // 0x006A2220
};

class CEnginePrimitiveThemeChannelDesc { public:                   // one dword
    unsigned long Channel;
};

namespace NGraphicAppearance {
class CTCGraphicAppearance;
}

namespace NGraphicAppearance {

class CTCGraphicAppearance { public:
    void*          __vftable;                               // +0x00 (CTCBase)
    CThing*        ParentThing;                             // +0x04
    unsigned char  _pad08[0x40 - 0x08];
    CEnginePrimitiveThemeChannelDesc LightingChannel;       // +0x40
    unsigned char  _pad44[0x58 - 0x44];
    unsigned char  UpdatePrimitiveReasons;                  // +0x58
    void SetLightingChannel(const CEnginePrimitiveThemeChannelDesc& channel, bool includeCarried, bool includeWeapons);
};

void CTCGraphicAppearance::SetLightingChannel(const CEnginePrimitiveThemeChannelDesc& channel, bool includeCarried,
                                              bool includeWeapons)
{
    LightingChannel = channel;
    CTCCarrying* carrying;
    if (includeCarried && ParentThing->GetInterface(&carrying, 0x46)) {
        CRetailVector<CThing*> carried;
        carrying->GetAllCarriedThings(carried);
        for (unsigned int i = 0; i < carried.size(); ++i) {
            CThing* thing = carried[i];
            CTCGraphicAppearance* appearance;
            if ((includeWeapons || !thing->HasTC(0x48)) && thing->GetInterface(&appearance, 0x5b))
                appearance->SetLightingChannel(channel, includeCarried, includeWeapons);
        }
    }
    ParentThing->AddToRenderList();
    UpdatePrimitiveReasons |= 4;
}

}