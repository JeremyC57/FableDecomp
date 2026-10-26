// CTCInventory::IsButtonHeldForExpression(const CCharString&) -- true when the hero owns the expression's dummy object and either expression action button is held
// Retry 10b (getsw sweep, FIXES B-15): the dummy-object flag is read through an inline getter `GetDummyObject()`; the plain member read gives the parked register/scheduling difference.
// Leaf: IsButtonHeldForExpression
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00582A16, 184 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callees: CTCBase::GetDefinitionManager() @ 0x006869D0, CDefinitionManager::GetDef<CExpressionDef>(const
//          CCharString&, CDefPointer<CExpressionDef>*) @ 0x00593666 (label says CHeroPostcardGeneratorDef;
//          propagated), CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) @ 0x005BDF08, CThing::GetPlayer()
//          @ 0x004C7A10 (x2 per test), CPlayer::GetPlayerNumber() @ 0x00487510, CGamePlayerInterface::
//          IsGameActionEventInQueue(long, long, long) @ 0x00446B10 (actions 0x37 / 0x38, button event 2 = held).
// Shape: the def pointer local is zeroed, filled by GetDef and then read once into edi; the def's inline counted
// Release (`dec [edi+4] ; jne ; call [vtable+4]`) is duplicated on both returns, the true path without a null test
// (the pointer was already dereferenced). Each button test is an inline helper that takes the player interface
// into a named local before the player number is fetched (the interface parked in the dead name slot, then ebx).
// Layout: CExpressionDef::DummyObject +0x70 (PDB +0x74; retail defs sit 4 bytes below the PDB); CPlayer's game
// player interface +0x0C.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharString;

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};

class CExpressionDef : public CDefBase { public:
    unsigned char _pad_0x08[0x68];
    long          DummyObject;                       // +0x70 CDefIndex (PDB +0x74)
    long GetDummyObject() const { return DummyObject; }
};

template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};

class CDefinitionManager { public:
    CDefPointer<CDefBase> GetPDefFromInstantiationName(const CCharString& name);   // 0x009ADA40 (hidden result pointer)
    template <class T> bool GetDef(const CCharString& name, CDefPointer<T>* out);  // 0x00593666 (CExpressionDef)
};

// Stand-in body of GetDef<CExpressionDef> (0x00593666, same TU in retail): compiled above the caller with
// auto_inline off so VC7.1 sees that the out pointer's address does not escape (the caller keeps the def in edi).
// Only IsButtonHeldForExpression is graded from this file.
#pragma auto_inline(off)
template <class T> bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer<T>* out)
{
    CDefPointer<CDefBase> found = GetPDefFromInstantiationName(name);
    if (found.Object) {
        if (out->Object != found.Object) {
            if (out->Object) {
                out->Object->Release();
                out->Object = 0;
            }
            out->Object = (T*)found.Object;
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
    long GetPlayerNumber();                          // 0x00487510
};

class CThing { public:
    CPlayer* GetPlayer();                            // 0x004C7A10
};

class CTCInventory { public:
    void*   __vftable;                               // +0x00
    CThing* ParentThing;                             // +0x04 CTCBase

    CDefinitionManager* GetDefinitionManager() const;    // 0x006869D0 (CTCBase)
    long GetNumberOfItemsOfTypeInInventory(long defIndex);   // 0x005BDF08 (CTCInventoryBase)
    __forceinline bool IsActionButtonHeld(long action)
    {
        CThing* thing = ParentThing;
        CGamePlayerInterface* input = thing->GetPlayer()->PGamePlayerInterface;
        return input->IsGameActionEventInQueue(action, 2, thing->GetPlayer()->GetPlayerNumber());
    }
    bool IsButtonHeldForExpression(const CCharString& expressionName);
};

bool CTCInventory::IsButtonHeldForExpression(const CCharString& expressionName)
{
    CDefPointer<CExpressionDef> def;
    if (GetDefinitionManager()->GetDef(expressionName, &def) &&
        GetNumberOfItemsOfTypeInInventory(def->GetDummyObject()) > 0 &&
        (IsActionButtonHeld(0x37) || IsActionButtonHeld(0x38)))
        return true;
    return false;
}