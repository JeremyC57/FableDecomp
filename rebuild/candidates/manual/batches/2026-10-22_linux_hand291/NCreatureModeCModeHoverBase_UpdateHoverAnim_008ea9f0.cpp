// NCreatureMode::CModeHoverBase::UpdateHoverAnim() -- if a hover animation is queued, start it as the main animation (mode 6
//   when both it and the current animation loop, else 2) and pop it; otherwise pick the best hover state for the creature's
//   forward/velocity and, when the state's animation differs from the current looping one or the current animation is empty or
//   finished, queue it (unless it resolves to the DEFAULT entry) and make it current
// Leaf: UpdateHoverAnim
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EA9F0, 570 bytes (`ret`).
// Callees: CVectorMap::LowerBound 0x0040F020 (inline GetTC 0x5a, unchecked miss path), CTCAnimationComplex::GetAnimation
//   0x0070B460, virtual slot 13 (+0x34), CAnimInit::Set 0x0070C050, CAnimateThing::SetMainAnim 0x0070D580, list erase
//   0x008EBF30 / push_back 0x008EBF90, operator delete 0x00BFE9BC, driver slot 70 (+0x118, GetForward), GetBestHoverState
//   0x008EA930 (manifest says GetMovementSpeed), CCharString copy 0x0099EC30 / != 0x0099E900 / = 0x0099EFB0 / dtor 0x0099EAE0,
//   GetRandomAnimFromName 0x00662D80, operator new 0x00BFEA1A, CCountedPointer<CHoverAnim>(T*) 0x008EBE90 / dtor 0x0073D630.
// Data: "DEFAULT" 0x01230554.
// Shape: the queued anim is a CCountedPointer<const CHoverAnim> copy (inline AddRef/release); the CAnimInit is a temporary in
//   the SetMainAnim call (mode computed first, Looping re-read for the argument); if/else (not an early return) lets the mode
//   share the name's frame slot; C3DVector needs a user copy ctor for retail's load/store order; the list iterator needs a
//   user copy ctor so erase's by-value argument is built in place.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator!=(const CCharString& other) const;    // 0x0099E900
};
struct CBlendPair {
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};
struct CAnimInit {                                      // 0x2c bytes
    long          Mode;                                 // +0x00
    unsigned char AnimType;                             // +0x04
    bool          Flag05;                               // +0x05
    long          AnimIndex;                            // +0x08
    float         Scales[3];                            // +0x0c
    CBlendPair    Blend;                                // +0x18
    unsigned long Flags;                                // +0x20
    long          Arg24;                                // +0x24
    long          Percent;                              // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};
struct CAnimateThing {
    void*         VTable;                               // +0x00
    void*         AnimBegin;                            // +0x04
    void*         AnimEnd;                              // +0x08
    unsigned char _pad0c[0x51 - 0x0c];
    unsigned char Flags51;                              // +0x51 bit 0: looping, bit 1: finished
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
    bool IsLooping() const { return (Flags51 & 1) != 0; }
};
struct CTCAnimationComplex {
    CAnimateThing* GetAnimation();                      // 0x0070B460
};
struct CAnimationEntry {
    long BankIndex;                                     // +0x00
};
struct CInterfaceMap {                                  // CVectorMap<long, CTCBase*>
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end_of_storage;
    SEntry* LowerBound(const long* type);               // 0x0040F020
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
struct CTCDriverBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetForward() const;               // slot 70 (+0x118)
};
struct CThingCreatureBase {
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad50[0x60 - 0x50];
    CTCDriverBase*    PTCDriver;                          // +0x60
    unsigned char  _pad64[0x10c - 0x64];
    C3DVector      Forward;                             // +0x10c
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
    CAnimationEntry* GetRandomAnimFromName(const CCharString& name, long seed);   // 0x00662D80
};
struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
};
template <class T> struct CCountedPointer {
    T* m_pObject;                          // +0
    SCountBlock* m_pCount;                 // +4
    explicit CCountedPointer(T* object);   // 0x008EBE90
    ~CCountedPointer();                    // 0x0073D630
    T* operator->() const { return m_pObject; }
};
template <class T> struct CCountedPointer<const T> {
    const T* m_pObject;
    SCountBlock* m_pCount;
    __forceinline CCountedPointer(const CCountedPointer<T>& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    __forceinline ~CCountedPointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
    const T* operator->() const { return m_pObject; }
};
struct CModeBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CCharString GetAnimPrefix();                // slot 12 (+0x30)
    virtual long GetAnimInitMode();                     // slot 13 (+0x34)
    CThingCreatureBase* ParentThing;                    // +0x04
    unsigned char _body[0x14];
};
namespace NCreatureMode {
struct CModeHoverBase : CModeBase {
    struct CRadialArea { float ThetaLow, ThetaHigh, RadHigh; };
    struct CHoverState {
        CCharString AnimName;                           // +0x00
        CRadialArea Area;                               // +0x04
        long        MovementType;                       // +0x10
        float       MovementSpeed;                      // +0x14
    };
    struct CHoverAnim {
        CAnimationEntry* AnimToUse;                     // +0x00
        CCharString      AnimName;                      // +0x04
        bool             Looping;                       // +0x08
        CHoverAnim(CAnimationEntry* anim, const CCharString& name, bool looping) : AnimToUse(anim), AnimName(name), Looping(looping) {}
    };
    struct CAnimQueue {                                 // STLport list<CCountedPointer<CHoverAnim> >
        struct SNode {
            SNode* Next;
            SNode* Prev;
            CCountedPointer<CHoverAnim> Data;
        };
        struct iterator { SNode* Node; iterator(SNode* node) : Node(node) {} iterator(const iterator& other) : Node(other.Node) {} };
        SNode* Header;
        bool empty() const { return Header->Next == Header; }
        CCountedPointer<CHoverAnim>& front() { return Header->Next->Data; }
        iterator begin() { return iterator(Header->Next); }
        iterator erase(iterator where);                 // 0x008EBF30
        void pop_front() { erase(begin()); }
        void push_back(const CCountedPointer<CHoverAnim>& value);   // 0x008EBF90
    };
    CHoverState* HoverStatesStart;                      // +0x1c
    CHoverState* HoverStatesFinish;                     // +0x20
    CHoverState* HoverStatesEnd;                        // +0x24
    CCharString  CurrentName;                           // +0x28
    const CHoverState* PCurrentState;                   // +0x2c
    long         CurrentMovementType;                   // +0x30
    CAnimQueue   AnimQueue;                             // +0x34
    const CHoverState* GetBestHoverState(const C3DVector& forward, const C3DVector& velocity) const;   // 0x008EA930
    void UpdateHoverAnim();
};

void CModeHoverBase::UpdateHoverAnim()
{
    CThingCreatureBase* creature = ParentThing;
    CTCAnimationComplex* animComplex;
    if (creature->HasTC(0x5a))
        animComplex = creature->GetTC<CTCAnimationComplex>(0x5a);
    CAnimateThing* animation = animComplex->GetAnimation();
    if (!AnimQueue.empty()) {
        CCountedPointer<const CHoverAnim> hoverAnim = AnimQueue.front();
        long mode;
        if (hoverAnim->Looping && animation->IsLooping())
            mode = 6;
        else
            mode = 2;
        animation->SetMainAnim(CAnimInit(5, 0, hoverAnim->AnimToUse->BankIndex, GetAnimInitMode(), hoverAnim->Looping, 0, 100), mode, 0);
        AnimQueue.pop_front();
    } else {
        C3DVector forward = creature->Forward;
        C3DVector velocity = creature->PTCDriver->GetForward();
        const CHoverState* state = GetBestHoverState(forward, velocity);
        if (state)
            PCurrentState = state;
        CCharString name(PCurrentState->AnimName);
        if ((CurrentName != name && animation->IsLooping()) || animation->AnimBegin == animation->AnimEnd
            || (animation->Flags51 & 2)) {
            CAnimationEntry* defaultEntry = creature->GetRandomAnimFromName("DEFAULT", -1);
            CAnimationEntry* entry = creature->GetRandomAnimFromName(name, -1);
            if (entry != defaultEntry)
                AnimQueue.push_back(CCountedPointer<CHoverAnim>(new CHoverAnim(entry, name, true)));
            CurrentName = name;
        }
    }
}
}
