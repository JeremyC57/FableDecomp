// CTCSmashableBase::OnCreate() -- clear the last hit frame and the smashed flag, fetch the parent thing's definition and, when found, take its smashable sub-def into Definition
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EEC90, 93 bytes (plain `ret`). Vtable slot 4 of CTCSmashableBarrier 0x0124017C (shared by
// CTCSmashableWindow 0x01240BA4 and the base vtable 0x01271860: the inherited base method).
// Callees: CTCBase::GetDefinitionManager() @ 0x006869D0 (manifest "_Cons_val<...>", propagated), CDefinitionManager::GetDef(long,
// CDefPointer*) @ 0x0042AF3C (manifest name a byte-twin template), CDefClassBase::GetSubDef<CSmashableDef>(CDefPointer*) @ 0x007EEEA0
// (manifest "GetSubDef<CAICreatureWillPowerIndicatorDef>", propagated); the def's virtual slot 1 on the last release.
// Layout: Ego_r.pdb CTCSmashableBase (0x18): LastWorldFrameHit +0x0c, Smashed +0x10, Definition +0x14; CThing's definition index is
// the word at +0x8c. Model and stand-in out-param bodies as the landed sibling CTCDegradableBase::OnCreate 0x008EC870.
// Shape: the GetDef result is tested BEFORE the def object is read into edi (`test al,al ; mov edi,[esp+8] ; je`): the bool is a named
// local and the object is read after it; the release is written by hand on that local.
struct CThing {
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                                // +0x8c
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};

struct CSmashableDef;

struct CDefClassBase {
    virtual void v00();
    virtual void DestroySelf();                             // slot 1 (+4)
    long RefCount;                                          // +0x04
    bool GetSubDef(CDefPointer<const CSmashableDef>* out);  // 0x007EEEA0
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const CSmashableDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CDefClassBase>* out);   // 0x0042AF3C
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline.
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CDefClassBase>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CDefinitionManager* GetDefinitionManager();             // 0x006869D0
};

struct CTCSmashableBase : CTCBase {
    long                              LastWorldFrameHit;    // +0x0c
    bool                              Smashed;              // +0x10
    CDefPointer<const CSmashableDef>  Definition;           // +0x14
    void OnCreate();
};

void CTCSmashableBase::OnCreate()
{
    LastWorldFrameHit = 0;
    Smashed = false;
    CDefPointer<CDefClassBase> def;
    bool found = GetDefinitionManager()->GetDef(ParentThing->DefIndex, &def);
    CDefClassBase* object = def.Object;
    if (found)
        object->GetSubDef(&Definition);
    if (object != 0 && --object->RefCount == 0)
        object->DestroySelf();
}
