// CCreatureAction_DragonHoverInto::Initialise() -- animation "HOVER_INTO", base Initialise, drop the owner's physics body ~30 units below its position, query its facing, and flag the owner's interface-9 component
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F3FD0, length 213 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("HOVER_INTO" @ 0x012884B0), SetAnimationName 0x008574D0,
// CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPosition 0x004C73D0,
// the physics body's (+0x60) virtual slots 31 (+0x7c, ResetPosition(const C3DVector&)) and 70 (+0x118, GetFacing: C3DVector by
// hidden pointer, result unused), CVectorMap::LowerBound 0x0040F020 (inlined GetInterface 9). Constant 30.148 @ 0x0128838C.
// Shape: the body calls go through inline creature forwarders (`mov ecx,[eax+60h] ; mov eax,[ecx]`, as 0x008BF0F0 in-chunk);
// the new position is a default-constructed C3DVector assigned member by member (X/Y as dword moves, Z - offset on the FPU; a
// 3-float ctor routes X/Y through the FPU, a copy + `Z -= k` reloads Z from the copy); the component lookup result is
// not tested, so the "missing" path writes through the uninitialised out-parameter (two tails).
class CTCBase;

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CPhysicsBody {                           // the creature's physics body at +0x60
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void ResetPosition(const C3DVector& position);   // slot 31 (+0x7c)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacing();              // slot 70 (+0x118), by value through the hidden pointer
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CTCHover {                               // the interface-9 component
    unsigned char _pad00[0x10];
    bool          m_bFlag10;                    // +0x10
};

class CThing { public:
    const C3DVector& GetPos() const;       // 0x004C73D0 (CThing)
};
class CThingCreatureBase : public CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsBody* m_pBody;                      // +0x60
    void ResetPosition(const C3DVector& position) { m_pBody->ResetPosition(position); }
    C3DVector GetFacing() const { return m_pBody->GetFacing(); }
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

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_DragonHoverInto : public CCombatActionBase {
public:
    virtual void Initialise();
};

void CCreatureAction_DragonHoverInto::Initialise()
{
    {
        CCharString name("HOVER_INTO");
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    const C3DVector& pos = PCreature.GetPItem()->GetPos();
    C3DVector lowered;
    lowered.X = pos.X;
    lowered.Y = pos.Y;
    lowered.Z = pos.Z - 30.148378f;
    PCreature.GetPItem()->ResetPosition(lowered);
    PCreature.GetPItem()->GetFacing();
    CTCHover* hover;
    PCreature.GetPItem()->GetInterface(&hover, 9);
    hover->m_bFlag10 = true;
}