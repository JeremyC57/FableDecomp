// CThingObject::OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* other) -- after the game-object handler, a breakable object hit hard enough (or by a flagged thing) is killed and spawns its def's debris thing
// Leaf: OnCollision
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007037D0, length 247 bytes (+ int3 padding), `ret 10h`.
// Callees: CThingGameObject::OnCollision @ 0x00839080 (landed), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const
// @ 0x004365B0 (the inlined GetTC of kind 0x47 = bit 7 of CThing+0x28), CThing virtual slot 54 (+0xd8, bool),
// CThing::Kill(bool) @ 0x004C9B80, CDefClassBase::GetSubDef<T> @ 0x005CC10E on the base def (+0x70),
// CThing::GetPosition() const @ 0x004C73D0, the thing factory @ 0x006E0880 (`__fastcall`, (long, const C3DVector*, bool)
// as the landed CTCResurrectionItem::OnItemUsed 0x007F04C0), the sub-def's virtual slot 1 (destroy) on the last release.
// Layout (retail CThing, local model): component bits +0x20, interface map +0x44, the other thing's component +0x60
// (flag bit 0 of +0x3c), PBaseDef +0x70, Dead bit +0x91; the kind-0x47 component's +0x0c byte and +0x10 word.
// Shape: the component is preloaded with zero (`T* tc = 0; if (HasTC) tc = GetTC;`, the lookup key homed in the dead
// `normal` slot) and the ternary entry pick; the sub-def is a zeroed CDefPointer homed in the dead `other` slot and
// released through the short DecRefToZero form.
class CTCBase;
class CThing;
struct C3DVector { float X, Y, Z; };
enum ECollideType { ECT_NONE = 0, ECT_PHYSICS = 1, ECT_WEAPON = 3 };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long& type) const;          // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound const 0x004365B0 (its retail body: STLport lower_bound over the 8-byte
// {type, component} entries): VC7.1 then knows the key's address does not escape (retail's LTCG knowledge).
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long& type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CBreakableComponent {                                   // the kind-0x47 component
    unsigned char _pad00[0x0c];
    bool          BreakOnAnyCollision;                         // +0x0c
    unsigned char _pad0d[0x10 - 0x0d];
    void*         PBreakBlocker;                               // +0x10
};
struct COtherComponent {
    unsigned char _pad00[0x3c];
    unsigned char DoesNotBreakObjects : 1;                     // +0x3c bit 0
};

struct CDefObject {
    virtual void v00();
    virtual void Destroy();                                    // slot 1 (+0x04)
    long m_refCount;                                           // +0x04
};
struct CDebrisDef : CDefObject {
    unsigned char _pad08[0x34 - 0x08];
    long          DebrisThingDef;                              // +0x34
};
__forceinline bool DecRefToZero(long& count) { return --count == 0; }
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && DecRefToZero(Object->m_refCount)) Object->Destroy(); }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->m_refCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_refCount;
        }
        return *this;
    }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
struct CDefinitionManager {
    CDefPointer<CDebrisDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
struct CDefClassBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);     // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* m_pManager;                            // +0x1c
    bool GetSubDef(CDefPointer<CDebrisDef>* out);              // 0x005CC10E
};
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<T> 0x005CC10E (the landed GetSubDef body shape, as in 0x00848730): VC7.1 then knows
// the def pointer's address does not escape (retail's LTCG knowledge), so the def stays in edi across the calls.
bool CDefClassBase::GetSubDef(CDefPointer<CDebrisDef>* out)
{
    long* pIndex;
    {
        CCharString name("CDebrisDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CDebrisDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)

CThing* __fastcall CreateThing(long defIndex, const C3DVector* position, bool flag);   // 0x006E0880

class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual bool IsBreakable();                                // slot 54 (+0xd8)
    unsigned char    _pad04[0x20 - 0x04];
    unsigned long    m_presentBits[9];                         // +0x20
    CInterfaceMap    m_interfaces;                             // +0x44
    unsigned char    _pad4c[0x60 - 0x4c];
    COtherComponent* PComponent60;                             // +0x60
    unsigned char    _pad64[0x70 - 0x64];
    CDefClassBase*   PBaseDef;                                 // +0x70
    unsigned char    _pad74[0x91 - 0x74];
    unsigned char    Dead : 1;                                 // +0x91 bit 0
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;                      // 0x004C73D0
    void Kill(bool immediately);                               // 0x004C9B80
};
class CThingGameObject : public CThing {
public:
    void OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* other);   // 0x00839080
};
class CThingObject : public CThingGameObject {
public:
    void OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* other);
};

void CThingObject::OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* other)
{
    if (Dead)
        return;
    CThingGameObject::OnCollision(type, position, normal, other);
    CBreakableComponent* breakable = 0;
    bool blocked;
    if (GetInterface(&breakable, 0x47) && breakable->PBreakBlocker)
        blocked = true;
    else
        blocked = false;
    if (!IsBreakable() && !(breakable && breakable->BreakOnAnyCollision))
        return;
    if (blocked)
        return;
    if (type == ECT_PHYSICS) {
        if (other->PComponent60->DoesNotBreakObjects)
            return;
        Kill(true);
    } else if (type == ECT_WEAPON)
        Kill(true);
    else
        return;
    CDefPointer<CDebrisDef> def;
    if (PBaseDef->GetSubDef(&def) && def->DebrisThingDef)
        CreateThing(def->DebrisThingDef, &GetPosition(), false);
}