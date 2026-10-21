// CCombatActionBase::ProcessPossibleProneThingsStrikeEvent(float, const std::pair<C3DVector,C3DVector>&, const std::pair<C3DVector,C3DVector>&, CThingGameObject&) -- a prone thing not yet struck this event is tested against the swept weapon triangle; on a hit it joins the strike event's struck list, may trigger the action-on-strike, starts the hit pause and runs the on-hit frames
// Leaf: ?ProcessPossibleProneThingsStrikeEvent@CCombatActionBase@@IAEXMABU?$pair@VC3DVector@@V1@@std@@0AAVCThingGameObject@@@Z
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 (CRT import thunk)
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
};
namespace std {
template <class A, class B> struct pair { A first; B second; };
}
template <class T> struct CAllocator { CAllocator() { } };
template <class T> struct CAllocProxy : CAllocator<T> {
    T* _M_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), _M_data(p) { }
};
template <class T> struct CVectorBase {         // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
    ~CVectorBase() { if (_M_start) free(_M_start); }       // the CRT free import thunk 0x00BFEA14
};
struct __true_type { __true_type() { } };
template <class T> struct CVector : CVectorBase<T> {   // STLport vector<T> (12 bytes)
    CVector() : CVectorBase<T>(CAllocator<T>()) { }
    void push_back(const T& x);                 // out of line (0x0066E630 for C3DVector)
    void _M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int n, bool atend);   // 0x00788FFC
    __forceinline void push_back_inline(T x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) T(x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, x, __true_type(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);

#pragma auto_inline(off)
// Same-TU body of the vector growth member (_M_insert_overflow, instantiated in retail's TU): the new storage receives the value
// read through its const reference; the empty tag is never touched.
template <class T>
__declspec(noinline) void CVector<T>::_M_insert_overflow(T* pos, const T& x, const __true_type& p2, unsigned int n, bool atend)
{
    T* p = (T*)GrowVectorStorage_s7(this, n);
    if (p)
        new (p) T(x);
}
#pragma auto_inline(on)
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound template member (compiled in the caller's TU):
// binary search for the first entry whose type is not below the key read through the pointer.
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
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CThingGameObject : CThing { };
namespace NHitLocations { struct CHitLocation; }
struct CTCHitLocations {                        // interface type 0x7F
    NHitLocations::CHitLocation* GetPCombatFirstHitLocationIntersectionWithTriangle(const CVector<C3DVector>& triangle,
        C3DVector* hitPosition, C3DVector* hitNormal, float tolerance);   // 0x008169B0
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +4
    CThing* GetPItem() const;                   // 0x00A01B50
};
extern long g_StrikeHitPauseFrames;             // 0x013B86D0
namespace {
void __fastcall ProcessFramesRunningForOnHitWithParams(float& runningTime, float lastRunningTime,
    const std::pair<C3DVector, C3DVector>& previous, const std::pair<C3DVector, C3DVector>& current,
    const C3DVector& hitPosition);              // 0x00856040 (file-static)
}

struct CCombatActionBase {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();               // slot 3 (+0x0c)
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual bool IsToPerformActionOnStrike() const;   // slot 76 (+0x130)
    unsigned char _pad04[0x40 - 0x04];
    float RunningTimeSeconds;                   // +0x40
    float LastRunningTimeSeconds;               // +0x44
    unsigned char _pad48[0x62 - 0x48];
    bool  PerformedAction;                      // +0x62
    unsigned char _pad63[0xa8 - 0x63];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char _padb0[0xe8 - 0xb0];
    bool  HitThing;                             // +0xe8
    unsigned char _pade9[3];
    long  FrameUpdatePauseFrames;               // +0xec
    struct CStrikeEvent {
        unsigned char _pad00[0x1c];
        CVector<CThingGameObject*> StruckThingList;   // +0x1c
    };
    bool IsThingInStruckThingList(const CThingGameObject& thing) const;   // 0x00856F00
    CStrikeEvent* GetPActiveStrikeEvent();      // 0x00856E50
    bool HasPerformedAction() const;            // 0x006925A0
    void ProcessPossibleProneThingsStrikeEvent(float time, const std::pair<C3DVector, C3DVector>& previous,
        const std::pair<C3DVector, C3DVector>& current, CThingGameObject& thing);
};

void CCombatActionBase::ProcessPossibleProneThingsStrikeEvent(float time, const std::pair<C3DVector, C3DVector>& previous,
    const std::pair<C3DVector, C3DVector>& current, CThingGameObject& thing)
{
    if (IsThingInStruckThingList(thing))
        return;
    CTCHitLocations* hitLocations;
    if (!thing.GetInterface(&hitLocations, 0x7f))
        return;
    C3DVector hitPosition;
    C3DVector hitNormal;
    NHitLocations::CHitLocation* hit;
    {
        CVector<C3DVector> triangle;
        triangle.push_back(current.first);
        triangle.push_back(current.second);
        triangle.push_back(previous.first);
        hit = hitLocations->GetPCombatFirstHitLocationIntersectionWithTriangle(triangle, &hitPosition, &hitNormal, 0.25f);
    }
    if (hit) {
        GetPActiveStrikeEvent()->StruckThingList.push_back_inline(&thing);
        if (!HasPerformedAction() && IsToPerformActionOnStrike() && PTarget.GetPItem() == &thing) {
            PerformAction();
            PerformedAction = true;
        }
        if (PTarget.GetPItem() == &thing || (!(PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1)) && !HitThing))
            FrameUpdatePauseFrames = g_StrikeHitPauseFrames;
        HitThing = true;
        ProcessFramesRunningForOnHitWithParams(RunningTimeSeconds, LastRunningTimeSeconds, previous, current, hitPosition);
    }
}