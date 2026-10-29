// NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember::GetNearestCrateToMe() -- walk the raid's crate list and return (a copy of) the crate nearest to this team member (none if every crate is 1e7 or more away)
// Leaf: ?GetNearestCrateToMe@CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@AAE?AVCScriptThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCEDF0, 350 bytes, thiscall, hidden result pointer, `ret 4`.
// Identity (proven): the only caller is the CCrateTeamMember entity code at 0x00DCE549 (this = the member, a CScriptThing
//   result slot pushed; the same function tests CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30 on esi); the row sits
//   right after the landed CCrateTeamMember::GoOnPatrol 0x00DCEC70 / IsThingCarryingCrate 0x00DCED10, and it reads the
//   parent's crate vector those rows read (ParentClass +0x14, CrateList +0x54). The only CCrateTeamMember member returning a
//   CScriptThing in Ego_r.pdb / FableWin.pdb is
//   ?GetNearestCrateToMe@CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@AAE?AVCScriptThing@@XZ (private).
//   Catalog label Main_CheckThingDistance is invented.
// Callees: GetDistanceBetweenThings 0x00CBE512 (Ego_r ?GetDistanceBetweenThings@@YIMABVCScriptThing@@0@Z, __fastcall);
//   CBaseClass::CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0 (the local's base, out of line); operator delete 0x00BFE9BC
//   (the counted pointer's info block); the info block's DeleteFunc.
// Layout: Ego_r.pdb CCrateTeamMember (entity offsets equal retail): Me +0x08, ParentClass +0x14; CQ_OrchardFarmRaidScript
//   CrateList (STLport vector<CScriptThing>, 12-byte elements) at Ego_r 0x68 - 0x14 = +0x54.
// Shape: a named CScriptThing local (out-of-line CBaseClass ctor, inline vtable store) assigned with the inline
//   CScriptThing / CCountedPointer operator= (`__forceinline`), returned through the compiler-generated copy constructor
//   (only the CScriptThing vtable store: no base call), then destroyed with the `__forceinline` dtor (vtable store, release,
//   both words zeroed) and the out-of-line ~CBaseClass. 10000000.0f is 0x4B189680. Two details pick retail's registers:
//   operator[] as STLport's `*(begin() + n)` (with `_M_start + n` the element address is `mov edx,[ecx] ; add edx,ebp`),
//   and the counted-pointer assignment reading other.Info before other.Data (`lea eax,&other.PImp ; mov eax,[eax]`;
//   Data first loads [edx+ebp+4] into ecx, 351v350).
class CBaseClass {                                      // vtable 0x01231710
public:
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
void operator delete(void* p);                          // 0x00BFE9BC
class CCPPointerInfo {                                  // 12 bytes
public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                if (--Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    operator delete(Info);
                }
            }
            Data = data;
            Info = info;
            if (Info)
                ++Info->RefCount;
        }
        return *this;
    }
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    CScriptThing() { }
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    __forceinline virtual ~CScriptThing() { }
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
float __fastcall GetDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b);   // 0x00CBE512
namespace std {                                         // STLport 4.6 vector (12 bytes)
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
};
}
class CGameScriptInterfaceBase;
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing          Me;                           // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OrchardFarmRaidScript {                        // the owning level script: only CrateList is read
public:
    unsigned char _before[0x54];                        // CLevelScriptBase (0x48) + CrateCount, CommentTimer, three bools
    std::vector<CScriptThing> CrateList;                // +0x54
    class CCrateTeamMember;
};
class CQ_OrchardFarmRaidScript::CCrateTeamMember : public CEntityScriptBase {
public:
    CQ_OrchardFarmRaidScript* ParentClass;              // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;              // +0x18
private:
    CScriptThing GetNearestCrateToMe();
};

CScriptThing CQ_OrchardFarmRaidScript::CCrateTeamMember::GetNearestCrateToMe()
{
    float nearest = 10000000.0f;
    CScriptThing crate;
    for (unsigned int i = 0; i < ParentClass->CrateList.size(); ++i) {
        float distance = GetDistanceBetweenThings(Me, ParentClass->CrateList[i]);
        if (distance < nearest) {
            nearest = distance;
            crate = ParentClass->CrateList[i];
        }
    }
    return crate;
}
}   // namespace NScript