// NScript::CQ_PrisonWardenGameScript::CSecretBook::Init() -- read the book's colour from its data string ("RED" 0, "GREEN" 1, "BLUE" 2) into BookType, then overwrite the data string with "NULL"
// Leaf: ?Init@CSecretBook@CQ_PrisonWardenGameScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDE7F0, 330 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): the row is referenced once, from slot 2 (Init) of the RTTI-less entity vtable 0x012DCC6C (0x012DCC74),
//   which the landed NScript::CQ_PrisonWardenGameScript::CSecretBook::Alloc 0x00DDE720 stores; Ego_r.pdb
//   ?Init@CSecretBook@CQ_PrisonWardenGameScript@NScript@@EAEXXZ (0x00E17800) right after that Alloc (0x00E17750) and before
//   Main (0x00E179D0) -- retail Main is slot 1, 0x00DDE940. Entity slot order (landed CTyler::Init 0x00E836F0): 0 dtor,
//   1 Main, 2 Init, 3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted. Catalog label
//   Script_Set_Color_Green is invented.
// Callees: CScriptThing slot 3 GetDataString (a CCharString by value) / slot 4 SetDataString; CBasicString<char>::Compare
//   0x00411570 (the inline CCharString::operator==; strcmp intrinsic on the null arm); CCharString(const char*, long)
//   0x0099EBF0, ~CCharString 0x0099EAE0.
// Literals: "RED" 0x012DCC98, "GREEN" 0x012DCC90, "BLUE" 0x012DCC88, "NULL" 0x012393E4, "" 0x0122D70E.
// Layout: Ego_r.pdb CSecretBook (32 bytes = the landed Alloc's new size): Me +0x08, ParentClass +0x14, PMasterData +0x18,
//   BookType +0x1c.
// Shape: an if / else-if chain, each test on a fresh GetDataString() temporary destroyed before the branch (the result
//   kept in bl); the landed CQ_TraderConflictGoodScript::UpdateLiveEnemies 0x00DFC320 operator== form.
#include <string.h>
template <class T> class CBasicString { public:        // 13 bytes, pack(1)
    T*            PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    __forceinline bool operator==(const T* text) const { return Compare(PData, text) ? false : true; }
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570 for <char>
};
#pragma pack(push, 1)
class CCharStringData { public:                         // 17 bytes
    CBasicString<char> Data;                            // +0x00
    long               NoRefs;                          // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CBaseClass {                                      // vtable 0x01231710
public:
    virtual ~CBaseClass();
};
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();                            // slot 0
    virtual void v01(); virtual void v02();
    virtual CCharString GetDataString() const;          // slot 3 (+0x0c)
    virtual void SetDataString(const CCharString& data);   // slot 4 (+0x10)
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
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
class CQ_PrisonWardenGameScript {
public:
    class CSecretBook;
};
class CQ_PrisonWardenGameScript::CSecretBook : public CEntityScriptBase {
public:
    CQ_PrisonWardenGameScript* ParentClass;             // +0x14
    CQ_SunnyvaleMasterData*    PMasterData;             // +0x18
    long                       BookType;                // +0x1c
private:
    virtual void Init();
};

void CQ_PrisonWardenGameScript::CSecretBook::Init()
{
    if (Me.GetDataString() == "RED")
        BookType = 0;
    else if (Me.GetDataString() == "GREEN")
        BookType = 1;
    else if (Me.GetDataString() == "BLUE")
        BookType = 2;
    Me.SetDataString("NULL");
}
}   // namespace NScript