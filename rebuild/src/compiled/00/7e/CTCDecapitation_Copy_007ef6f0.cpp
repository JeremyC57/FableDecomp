// CTCDecapitation::Copy(const CTCBase& other) -- copy the decapitated flag from another decapitation component
// Leaf: ?Copy@CTCDecapitation@@UAEXABVCTCBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EF6F0, 13 bytes, `ret 4`.
// Identity: CTCDecapitation vtable 0x01240534 slot 12 (RTTI .?AVCTCDecapitation@@); FableWin.pdb
//   `?Copy@CTCDecapitation@@UAEXABVCTCBase@@@Z` is in the class's name set.
// CTCBase virtual slots (Ego_r.pdb donor order, matched to the CTCBase vtable 0x0125BE0C): 2 OnRemove, 3 ConstructFromParams,
//   4 OnCreate, 5 InitialActivate, 6 OnInitialActivate, 7 RenderUpdate, 8 HasFrameUpdate, 9 FrameUpdate,
//   10 BulletTimeFrameUpdate, 11 DrawPreparePrimitivesForRendering, 12 Copy, 13 OnDie, 14 OnKill (the editor-only
//   EditFrameUpdate / EditorDrawPreparePrimitivesForRendering of the debug build are absent from retail).
// Layout: Ego_r.pdb CTCDecapitation (0x14): Decapitated +0x0c, PDef +0x10.
// Shape: one byte copy through dl; the base reference is downcast to the derived component (offset 0).
class CThing;

class CTCBase {
public:
    virtual void Copy(const CTCBase& other);
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};

class CTCDecapitation : public CTCBase {
public:
    virtual void Copy(const CTCBase& other);
    bool  Decapitated;                          // +0x0c
    void* PDef;                                 // +0x10 CDefPointer<const CDecapitationDef>
};

void CTCDecapitation::Copy(const CTCBase& other)
{
    Decapitated = static_cast<const CTCDecapitation&>(other).Decapitated;
}