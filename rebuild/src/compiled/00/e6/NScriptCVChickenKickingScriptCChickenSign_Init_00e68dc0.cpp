// NScript::CV_ChickenKickingScript::CChickenSign::Init() -- the chicken-kicking prize sign: eight text keys, one per combination of prizes won (nothing / expression / key / hat)
// Leaf: ?Init@CChickenSign@CV_ChickenKickingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E68DC0, 153 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012E7648, which the landed
//   NScript::CV_ChickenKickingScript::CChickenSign constructor 0x00E64D90 (manifest label CACompressor::CACompressor is wrong;
//   slot 1 of the same vtable is the landed CChickenSign::Main 0x00E64DF0) stores (site 0x00E64DBA); Ego_r.pdb has ?Init@CChickenSign@CV_ChickenKickingScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CChickenSign (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Flags: /Zc:wchar_t
// Callees: std::vector<CWideString>::resize(size_type, const CWideString&) 0x00412B20 through STLport's inline
//   resize(n) (the CWideString() 0x0099AED0 temporary, ~CWideString 0x0099B510), CWideString::operator=(const wchar_t*)
//   0x0099B800 eight times (wide literals TEXT_QST_B17_SIGN_WON_*). The manifest label of 0x00412B20
//   (vector<pair<CCharString,long> >::resize) is wrong: its body strides 4-byte elements (`sar edi,2`).
//   (/Zc:wchar_t keeps wchar_t native, as FableWin.pdb spells the operator: PB_W).
class CWideString {                                     // 4 bytes (Ego_r.pdb)
public:
    void* PData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const wchar_t* text);  // 0x0099B800 (FableWin.pdb ??4CWideString@@QAEABV0@PB_W@Z)
};
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {   // STLport 4.6: start, finish, end of storage (0xC)
public:
    typedef unsigned int size_type;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](size_type n) { return *(_M_start + n); }
    void resize(size_type newSize, const T& x);         // 0x00412B20 for vector<CWideString>
    void resize(size_type newSize) { resize(newSize, T()); }
};
}
class CBaseClass {                                      // vtable 0x01231710
public:
    CBaseClass() { }
    virtual ~CBaseClass();
};
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();                            // slot 0
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
class CGameScriptInterface;
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
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ChickenKickingScript {
public:
    class CChickenSign;
};
class CV_ChickenKickingScript::CChickenSign : public CEntityScriptBase {
public:
    CV_ChickenKickingScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    std::vector<CWideString> TextKeys;                  // +0x1c
private:
    virtual void Init();
};

void CV_ChickenKickingScript::CChickenSign::Init()
{
    TextKeys.resize(8);
    TextKeys[0] = L"TEXT_QST_B17_SIGN_WON_NOTHING";
    TextKeys[1] = L"TEXT_QST_B17_SIGN_WON_EXPR";
    TextKeys[2] = L"TEXT_QST_B17_SIGN_WON_KEY";
    TextKeys[3] = L"TEXT_QST_B17_SIGN_WON_KEY_EXPR";
    TextKeys[4] = L"TEXT_QST_B17_SIGN_WON_HAT";
    TextKeys[5] = L"TEXT_QST_B17_SIGN_WON_HAT_EXPR";
    TextKeys[6] = L"TEXT_QST_B17_SIGN_WON_HAT_KEY";
    TextKeys[7] = L"TEXT_QST_B17_SIGN_WON_HAT_KEY_EXPR";
}
}