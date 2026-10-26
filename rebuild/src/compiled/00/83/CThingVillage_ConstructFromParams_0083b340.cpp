// CThingVillage::ConstructFromParams(long, const C3DVector&, long) -- base CThingGameObject construction with an empty script name, no creators and the final flag set
// Leaf: ?ConstructFromParams@CThingVillage
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083B340, 75 bytes, `ret 0Ch`, bool in al.
// Identity: ?ConstructFromParams@CThingVillage@@IAE_NJABVC3DVector@@J@Z (Ego_r.pdb); called only by CThingVillage::Create
//   0x0083B7F0. The manifest label "CThingGameObject_ConstructorEmpty" is descriptive.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 ("" @ 0x0122D70E, length -1), CThingGameObject::
//   ConstructFromParams(long, const C3DVector&, long, unsigned long, const CCharString&, CThingPhysical*, CThingPhysical*,
//   bool) 0x008388D0 (landed), CCharString::~CCharString() 0x0099EAE0.
// Layout: no members touched. Local model, the landed sibling CThingHolySite::ConstructFromParams 0x0083BAB0 shape.
// Shape: the empty name is a named CCharString local in its own block (homed in the `push ecx` slot); the base result goes
//   to a bool local (bl) that survives the destructor; base arguments (defIndex, pos, player, 0, name, 0, 0, true).
class C3DVector;
class CThingPhysical;

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CThingGameObject {
public:
    virtual void v00();
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& pos, long player, unsigned long flags, const CCharString& scriptName,
                             CThingPhysical* creator, CThingPhysical* originalCreator, bool a8);   // 0x008388D0
};

class CThingVillage : public CThingGameObject {
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& pos, long player);
};

bool CThingVillage::ConstructFromParams(long defIndex, const C3DVector& pos, long player)
{
    bool ok;
    {
        CCharString name("");
        ok = CThingGameObject::ConstructFromParams(defIndex, pos, player, 0, name, 0, 0, true);
    }
    return ok;
}