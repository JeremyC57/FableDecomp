// CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>* out) -- loads the card-game sub-def of the parent thing's def into a local def pointer and assigns it to *out
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

class CTavernGameCardBaseDef : public CDefPointee { public:
};

template<class T> class CDefPointer { public:
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

class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CTavernGameCardBaseDef>* out);   // 0x008F27A0
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CTavernGameCardBaseDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThing { public:
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