// CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* out) -- loads the card-game sub-def of the parent thing's def into a local def pointer and assigns it to *out
// Grade: DIFFER(112v92) -- retail keeps the local def's object in esi from before the assignment through the local's destructor, even across the old object's virtual Destroy() call (no reload of the address-taken local); the lane reloads `[esp+8]` after every call and also schedules the local's zero store and the `[thing+70h]` load differently. Tried: member-form operator= and destructor, operator= reading both objects into locals plus a local-copy destructor. Next: lane gap candidate (address-taken local CSE'd across a call by retail); maybe a GetSubDef out-parameter of raw `T**` type.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F0B20, length 92 bytes (ret 4). Name and signature: Ego_r.pdb
// `?GetCardDef@CTCTavernGameCardBase@@QAEXPAV?$CDefPointer@$$CBVCTavernGameCardBaseDef@@@@@Z`.
// Callee: CDefClassBase::GetSubDef<CTavernGameCardBaseDef>(CDefPointer<const CTavernGameCardBaseDef>*) @ 0x008F27A0 (bool,
// result unused), called on the def object the parent thing holds at +0x70; the def's virtual slot 1 destroys it.
// Layout: CTCBase ParentThing +0x04; a def's reference count at +0x04 (after its vptr).
// Shape: the local def pointer is null-constructed in the `push ecx` frame slot after its address is pushed; then the
// inline CDefPointer::operator= (`if (Object != other.Object) { if (Object) { release; Object = 0; } Object =
// other.Object; if (Object) ++count; }` -- the add-ref is `inc dword ptr [esi+4]`) and the local's inline destructor
// (null test, `dec [esi+4] ; jne`, slot-1 destroy; its dead zero store is dropped). The equal case jumps straight to the
// destructor's null test.
struct CDefPointee {                         // counted def: count at +4, slot 1 destroys
    virtual void v00();
    virtual void Destroy();                  // slot 1 (+4)
    long m_refCount;                         // +0x04
};

struct CTavernGameCardBaseDef : CDefPointee {
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        T* object = Object;
        if (object) {
            if (--object->m_refCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        T* old = Object;
        T* object = other.Object;
        if (old != object) {
            if (old) {
                if (--old->m_refCount == 0)
                    old->Destroy();
                Object = 0;
            }
            Object = object;
            if (object)
                ++object->m_refCount;
        }
        return *this;
    }
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer<CTavernGameCardBaseDef>* out);   // 0x008F27A0
};

struct CThing {
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PDef;                     // +0x70
};

struct CTCTavernGameCardBase {
    void*   __vftable;                       // +0x00
    CThing* ParentThing;                     // +0x04
    void GetCardDef(CDefPointer<CTavernGameCardBaseDef>* out);
};

void CTCTavernGameCardBase::GetCardDef(CDefPointer<CTavernGameCardBaseDef>* out)
{
    CDefPointer<CTavernGameCardBaseDef> def;
    ParentThing->PDef->GetSubDef(&def);
    *out = def;
}
