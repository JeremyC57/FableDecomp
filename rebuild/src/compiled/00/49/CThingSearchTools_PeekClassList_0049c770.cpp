// CThingSearchTools::PeekClassList() const -- the thing manager's list of every live class object (CClassFactory::ClassList, retail +0x18)
// Leaf: ?PeekClassList@CThingSearchTools@@IBEABV?$list@PAVCClass@@V?$allocator@PAVCClass@@@std@@@std@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049C770, 7 bytes: mov eax,[ecx+8] ; add eax,18h ; ret (thiscall, no arguments).
// Identity: landed as CVertexBufferWin32::DoSizeof, a label propagated from a byte-identical body; retail has no ICF and all
//   85 callers of this address are CThingSearchTools search templates that walk the returned list<CClass*> as every thing.
//   Ego_r names exactly two list getters on CThingSearchTools: ?PeekClassList@CThingSearchTools@@IBEABV?$list@PAVCClass@@...@XZ
//   (no argument) and ?PeekTypeList@...@J@Z, landed at the next address 0x0049C780 (reads ThingManager->ClassTypeInfo).
// Layout: CThingSearchTools::ThingManager +0x08 (Ego_r, engine/CThingSearchTools.h). CThingManager derives from CClassFactory
//   (RTTI 0x01245C44: CThingManager, CClassFactory, CBaseClassNonCopyable, CBaseClass); Ego_r CClassFactory has the vptr,
//   ClassTypeInfo vector +0x04, MaxClassTypeID +0x14, NoAllocatedClasses +0x18, ClassList +0x1C. The retail STLport vector is
//   12 bytes, so ClassList sits at +0x18 (the same 0xC shift as CThingManager::World, Ego_r +0x30 -> retail +0x24).
// Model: the generated engine/CThingSearchTools.h declares no member functions, so the class is modelled locally with the
//   header's first three members (as the 0x00686A80 CTCBase::PeekWorld row does).
class CClass;

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;                                    // STLport 4.6: the header node pointer
};
}

class CWorldMap;
class CThingMapManager;

class CThingManager {                                 // : CClassFactory (retail layout)
public:
    unsigned char _pad_0x00[0x18];                    // vptr, ClassTypeInfo, MaxClassTypeID, NoAllocatedClasses
    std::list<CClass*> ClassList;                     // +0x18 CClassFactory::ClassList
};

class CThingSearchTools {
public:
    CWorldMap*        WorldMap;                       // +0x00
    CThingMapManager* PThingMapManager;               // +0x04
    CThingManager*    ThingManager;                   // +0x08
protected:
    const std::list<CClass*>& PeekClassList() const;
};

const std::list<CClass*>& CThingSearchTools::PeekClassList() const
{
    return ThingManager->ClassList;
}
