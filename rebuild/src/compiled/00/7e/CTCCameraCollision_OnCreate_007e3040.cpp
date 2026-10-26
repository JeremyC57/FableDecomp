// CTCCameraCollision::OnCreate() -- vtable slot 4: run the base slot, look the parent thing's def up by its global index and fetch its camera-collision sub-def into CameraCollisionDef
//
// Retail 0x007E3040, 81 bytes. Slot 4 of the CTCCameraCollision vtable 0x01270394 (rtti-vtables.tsv).
// Callees: CTCBase's slot-4 body @ 0x00686850 (an empty `ret`, called non-virtually; manifest "$E2"),
// CTCBase::GetDefinitionManager() @ 0x006869D0 (`mov ecx,[ecx+4] ; jmp` to the thing's accessor
// 0x004C7990, which returns the world's +0x20 object; the manifest label is a propagated _Cons_val),
// CDefinitionManager::GetDef<T>(long, CDefPointer<T>*) @ 0x0042AF3C, CDefClassBase::GetSubDef<T>(
// CDefPointer<T>*) const @ 0x007E3290, and the thing def's virtual slot 1 (`call [eax+4]`, destroy)
// when the local pointer drops the last count.
// Layout: CThing::DefGlobalIndex is the ushort at +0x8c in retail (Ego_r.pdb +0x9c; `movzx` widens
// it to the long index); CameraCollisionDef (CDefPointer<const CCameraCollisionDef>) is +0x0c.
//        through the release (`mov esi,[esp+8] ; mov ecx,esi ; call ; test esi,esi ; dec [esi+4]`) and
//        sinks the null store below the argument pushes; the lane re-reads the frame slot after the
//        call (the 24k "CDefPointer local re-read from its slot" gap) and stores the zero first.
// Shape: a local CDefPointer (the `push ecx` frame slot) whose null store is scheduled right before
// the GetDef call; the sub-def lookup goes through the pointer's raw `operator->` (the object loaded
// into esi once, reused by the destructor); the destructor is the null-tested intrusive release on
// a local copy of the object (`dec [esi+4] ; jne ; call [eax+4]`, no trailing null store).

template <class T> class CDefPointer;
class CCameraCollisionDef;

class CDefClassBase
{ public:
    virtual void v00();
    virtual void Destroy();                         // slot 1 (+0x04)
    long m_refCount;                                // +0x04
    bool GetSubDef(CDefPointer<CCameraCollisionDef>* out) const;   // GetSubDef<T> instance @ 0x007E3290
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCameraCollisionDef>* out) const
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

template <class T>
class CDefPointer
{ public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        T* object = Object;
        if (object != 0)
        {
            if (--object->m_refCount == 0)
                object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};

struct CThingDef : CDefClassBase {};
class CCameraCollisionDef : public CDefClassBase { public:};

class CDefinitionManager
{ public:
    bool GetDef(long globalIndex, CDefPointer<CThingDef>* out) const;   // GetDef<T> instance @ 0x0042AF3C
};
class CGameDefinitionManager : public CDefinitionManager
{
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CThingDef>* out) const
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThing
{ public:
    unsigned char  _pad_0x00[0x8c];
    unsigned short DefGlobalIndex;                  // +0x8c
};

#pragma pack(push, 1)
class CTCBase
{ public:
    void*   __vftable;                              // +0x00
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
    unsigned char _pad_0x0a[0x2];                   // +0x0a
    void OnCreate();                                 // 0x00686850 (empty)
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

class CTCCameraCollision : public CTCBase
{ public:
    CDefPointer<CCameraCollisionDef> CameraCollisionDef;   // +0x0c
    void OnCreate();
};
#pragma pack(pop)

void CTCCameraCollision::OnCreate()
{
    CTCBase::OnCreate();
    CDefPointer<CThingDef> thingDef;
    GetDefinitionManager().GetDef(ParentThing->DefGlobalIndex, &thingDef);
    thingDef->GetSubDef(&CameraCollisionDef);
}