// CTCInventory::IsButtonHeldForExpression(const CCharString&) -- true when the hero owns the expression's dummy object and either expression action button is held
// Retry 10b (getsw sweep, FIXES B-15): the dummy-object flag is read through an inline getter `GetDummyObject()`; the plain member read gives the parked register/scheduling difference.
// Leaf: IsButtonHeldForExpression
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00582A16, 184 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callees: CTCBase::GetDefinitionManager() const @ 0x006869D0, CDefinitionManager::GetDef<CExpressionDef>(const
//          CCharString&, CDefPointer<const CExpressionDef>*) const @ 0x00593666 (label says CHeroPostcardGeneratorDef;
//          propagated), NInventory::CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) const @ 0x005BDF08, CThing::PeekPlayer()
//          @ 0x004C7A10 (x2 per test), CPlayer::GetJoystickDeviceNumber() @ 0x00487510, CGamePlayerInterface::
//          IsGameActionEventInQueue(long, long, long) @ 0x00446B10 (actions 0x37 / 0x38, button event 2 = held).
// Shape: the def pointer local is zeroed, filled by GetDef and then read once into edi; the def's inline counted
// Release (`dec [edi+4] ; jne ; call [vtable+4]`) is duplicated on both returns, the true path without a null test
// (the pointer was already dereferenced). Each button test is an inline helper that takes the player interface
// into a named local before the player number is fetched (the interface parked in the dead name slot, then ebx).
// Layout: CExpressionDef::DummyObject +0x70 (PDB +0x74; retail defs sit 4 bytes below the PDB); CPlayer's game
// player interface +0x0C.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharString;

class CDefClassBase { public:
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};

class CExpressionDef : public CDefClassBase { public:
    unsigned char _pad_0x08[0x68];
    long          DummyObject;                       // +0x70 CDefIndex (PDB +0x74)
    long GetDummyObject() const { return DummyObject; }
};

template <class T> class CDefPointer { public:     // the pointee is held as its non-const def base (engine/CDefPointer.h)
    CDefClassBase* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return static_cast<T*>(Object); }
};

class CDefinitionManager { public:
    CDefPointer<const CDefClassBase> GetPDefFromInstantiationName(const CCharString& name) const;   // 0x009ADA40 (hidden result pointer)
    template <class T> bool GetDef(const CCharString& name, CDefPointer<const T>* out) const;     // 0x00593666 GetDef<CExpressionDef> (FableWin.pdb template)
};
class CGameDefinitionManager : public CDefinitionManager { public: };   // RTTI CGameDefinitionManager : CDefinitionManager

// Stand-in body of GetDef<CExpressionDef> (0x00593666, same TU in retail): compiled above the caller with
// auto_inline off so VC7.1 sees that the out pointer's address does not escape (the caller keeps the def in edi).
// Only IsButtonHeldForExpression is graded from this file.
#pragma auto_inline(off)
template <class T> bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer<const T>* out) const
{
    CDefPointer<const CDefClassBase> found = GetPDefFromInstantiationName(name);
    if (found.Object) {
        if (out->Object != found.Object) {
            if (out->Object) {
                out->Object->Release();
                out->Object = 0;
            }
            out->Object = found.Object;
            if (out->Object)
                ++out->Object->RefCount;
        }
        return true;
    }
    return false;
}
#pragma auto_inline(on)

class CGamePlayerInterface { public:
    bool IsGameActionEventInQueue(long action, long buttonEventType, long playerNumber);   // 0x00446B10
};

class CPlayer { public:
    unsigned char         _pad_0x00[0x0C];
    CGamePlayerInterface* PGamePlayerInterface;      // +0x0C
public:
    long GetJoystickDeviceNumber() const;   // 0x00487510
};

class CThing { public:
public:
    const CPlayer& PeekPlayer() const;   // 0x004C7A10
};

class CTCBase { public:                              // the component base (members this row reads)
    virtual ~CTCBase();
    CThing* ParentThing;                             // +0x04
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb / FableWin.pdb IBE)
};
namespace NInventory {
class CTCInventoryBase : public CTCBase { public:
    long GetNumberOfItemsOfTypeInInventory(long defIndex) const;   // 0x005BDF08 (Ego_r.pdb QBE)
};
}

class CTCInventory : public NInventory::CTCInventoryBase { public:
    FABLE_FORCEINLINE bool IsActionButtonHeld(long action) const
    {
        CThing* thing = ParentThing;
        CGamePlayerInterface* input = thing->PeekPlayer().PGamePlayerInterface;
        return input->IsGameActionEventInQueue(action, 2, thing->PeekPlayer().GetJoystickDeviceNumber());
    }
    bool IsButtonHeldForExpression(const CCharString& expressionName) const;   // Ego_r.pdb QBE
};

bool CTCInventory::IsButtonHeldForExpression(const CCharString& expressionName) const
{
    CDefPointer<const CExpressionDef> def;
    if (GetDefinitionManager().GetDef(expressionName, &def) &&
        GetNumberOfItemsOfTypeInInventory(def->GetDummyObject()) > 0 &&
        (IsActionButtonHeld(0x37) || IsActionButtonHeld(0x38)))
        return true;
    return false;
}