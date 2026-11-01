// CTCTavernGame::CameraUseCameraPoint(const CCharString& name) -- find the world things carrying this script name; if the first is a live camera point, hand the camera manager a tavern-game script module from the point's start position along its look direction
// Grade: DIFFER(321v329) -- the end-of-function vector free: retail frees the shared buffer with no null test while the vector stays memory-resident; VC7.1 either keeps the allocator::deallocate null test (vector escapes to the out-of-line GetAllThings) or scalar-replaces the vector into ebp (frame 0x18)
// Next: model GetAllThings with its real same-TU template body (CThingSearchTools 0x005ED860) so the vector neither escapes nor is scalar-replaced; otherwise lane gap: STLport vector escape analysis.
// Leaf: ?CameraUseCameraPoint@CTCTavernGame@@QAEXABVCCharString@@@Z
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

class CCharString { public:
    void* PData;
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet;
class CTCBase;

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp& reference;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    bool empty() const { return this->_M_start == this->_M_finish; }
    reference operator[](size_type __n) { return *(begin() + __n); }
};
template <class T1, class T2> struct pair { T1 first; T2 second; };
}   // namespace std

enum ETCInterfaceType { TC_CAMERA_POINT = 0x3f };       // CTCCameraPointDefinitionBase interface kind
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20 bitset of present thing components
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  Dead : 1;                            // +0x91 bit 0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->first ? end : it)->second;
    }
};

class CThingFilter_HasScriptName { public:
    CCharString ScriptName;
    CThingFilter_HasScriptName(const CCharString& name) : ScriptName(name) { }
};
class CThingSearchTools { public:
    template <class TThing, class TContainer, class TFilter> void GetAllThings(const TFilter& filter, TContainer& result) const;   // 0x005ED860
};

namespace NCameraModes {
class CScriptModuleBase { public:
    virtual ~CScriptModuleBase();
};
class CScriptModuleTavernGame : public CScriptModuleBase { public:   // 0x64 bytes, vtable 0x01255FC4
    unsigned char _body[0x60];
    CScriptModuleTavernGame(const C3DVector& position, const C3DVector& direction);   // 0x005ED240
};
}
template <class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
    CCountedPointer(T* data);                           // 0x005ED270
    CCountedPointer(const CCountedPointer& other);
};
class CGameCameraManager { public:
    void SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> module);   // 0x00699E60
};
class CAIGameCameraBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** manager);   // slot 9 (+0x24)
};
class CWorld { public:
    unsigned char      _pad00[0x20];
    CThingSearchTools* PThingSearchTools;               // +0x20 (scoped_ptr<CThingSearchTools>)
    unsigned char      _pad24[0x34 - 0x24];
    CAIGameCameraBase* PCurrentGameCamera;              // +0x34
    CAIGameCameraBase* GetGameCamera() { return PCurrentGameCamera; }
};

enum ECameraPointType { CAMERA_POINT_SCRIPTED = 6 };
class CTCCameraPointDefinitionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual ECameraPointType GetCameraPointType() const;   // slot 28 (+0x70)
    template <class T> bool GetPointType(T** point)
    {
        if (GetCameraPointType() == T::POINT_TYPE) {
            *point = (T*)this;
            return true;
        }
        return false;
    }
};
class CTCCameraPointScripted : public CTCCameraPointDefinitionBase { public:
    enum { POINT_TYPE = CAMERA_POINT_SCRIPTED };
    C3DVector GetLookDirection(const CRightHandedSet* axes) const;                     // 0x0075CCB0
    C3DVector GetStartPos(const C3DVector* base, const CRightHandedSet* axes) const;   // 0x0075CD30
};

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
protected:
    CWorld& GetWorld() const;                           // 0x00686A70
    const CWorld& PeekWorld() const;                    // 0x00686A80
    const CThingSearchTools& PeekThingSearchTools() const { const CWorld& world = PeekWorld(); return *world.PThingSearchTools; }
};
class CTCTavernGame : public CTCBase { public:
    void CameraUseCameraPoint(const CCharString& name);
};

void CTCTavernGame::CameraUseCameraPoint(const CCharString& name)
{
    std::vector<CThing*> things;
    PeekThingSearchTools().GetAllThings<CThing>(CThingFilter_HasScriptName(name), things);
    if (things.empty())
        return;
    CThing* thing = things[0];
    if (thing && !thing->Dead && thing->HasTC(TC_CAMERA_POINT)) {
        CTCCameraPointDefinitionBase* point = (CTCCameraPointDefinitionBase*)thing->GetTC(TC_CAMERA_POINT);
        CTCCameraPointScripted* scripted;
        point->GetPointType(&scripted);
        {
            CGameCameraManager* manager;
            GetWorld().GetGameCamera()->GetCameraManager(&manager);
            manager->SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(
                new NCameraModes::CScriptModuleTavernGame(scripted->GetStartPos(0, 0), scripted->GetLookDirection(0))));
        }
        return;
    }
}
