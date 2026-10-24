// CInsertNameInSet::operator()(const CThing* thing) const -- for_each functor: when the thing's physics component has a mesh and that mesh object carries no navigation line-list info, insert the thing's definition name into the target set
// Leaf: ??RCInsertNameInSet@@QBEXPBVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AD3D0, 129 bytes (144 with padding), `ret 4` (__fastcall).
// Identity: Ego_r.pdb `??RCInsertNameInSet@@QBEXPBVCThing@@@Z`; CInsertNameInSet { set<CCharString>* DefNames; } (4 bytes);
//   the caller is _For_each<const CThing**, CInsertNameInSet> 0x004AD890.
// Callees: CTCPhysicsBase slot 93 (+0x174, GetPhysicsMeshInfo), CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850 (by
//   value through a hidden pointer), CPhysicsMeshObject::HasNavigationLineListInfo 0x00A12C90, ~CPhysicsMeshObject
//   0x00A127D0, CThing::GetDefName 0x004C7CC0 (CDefString by hidden pointer, passed on as one dword),
//   CDefStringTable::GetString 0x009D49B0 on the global table 0x013CA828, _Rb_tree::insert_unique 0x004AC380 (the
//   pair<iterator,bool> result dropped), ~CCharString 0x0099EAE0.
// Layout: CThing::PhysicsTC at retail +0x60 (as the landed CTools::GetCollisionThing 0x00875E60); CPhysicsMeshObject is
//   0x40 bytes.
// Shape: the physics pointer is a named local for the null test and slot-93 call and re-read for GetPhysicsMeshObject; the
//   name goes through an inline `CDefString::GetString() const` (Ego_r has no out-of-line body; TablePos -> the table's
//   GetString(long) const, PDB `?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z`) and the inline STLport
//   set::insert -> _M_t.insert_unique: that is what re-forms the string temporary's address with `lea` (a direct
//   `insert(table.GetString(def))` pushes GetString's eax; a named string local lets the pair share the CDefString word
//   and shrinks the frame to 0x48). The string temporary lands in the dead argument slot and dies before the mesh.
// Label correction: catalogued as `Physics_CheckNavigability`; the real routine is CInsertNameInSet::operator().
struct CCharString {
    void* PStringData;
    ~CCharString();                                // 0x0099EAE0
};

class CDefStringTable {
public:
    CCharString GetString(long table_pos) const;   // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;           // 0x013CA828

class CDefString {
public:
    long TablePos;                                 // +0x00
    CDefString& operator=(const CDefString& other);   // user assignment: returned through the hidden pointer
    CCharString GetString() const { return g_DefStringTable.GetString(TablePos); }
};

struct CThingPhysicsMeshInfo;

struct CPhysicsMeshObject {
    virtual void v00();
    void*         Mesh;                            // +0x04 CIVCountedPointer<CPhysicsMesh const>
    float         PositionMatrix[12];              // +0x08 CMatrix3x4
    float         Scale;                           // +0x38
    float         OO_Scale;                        // +0x3c
    ~CPhysicsMeshObject();                         // 0x00A127D0
    bool HasNavigationLineListInfo() const;        // 0x00A12C90
};

struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092();
    virtual const CThingPhysicsMeshInfo* GetPhysicsMeshInfo();   // slot 93 (+0x174)
    CPhysicsMeshObject GetPhysicsMeshObject() const;             // 0x006B0850
};

class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                     // +0x60
    CDefString GetDefName() const;                 // 0x004C7CC0
};

struct CCharStringSetIterator { void* _M_node; };
struct CCharStringSetInsertResult {                // pair<iterator, bool>
    CCharStringSetIterator first;
    bool second;
};
struct CCharStringTree {                           // STLport _Rb_tree<CCharString, CCharString, _Identity, less>
    void* _M_header;
    unsigned int _M_node_count;
    char _M_key_compare;
    CCharStringSetInsertResult insert_unique(const CCharString& value);   // 0x004AC380
};
struct CCharStringSet {                            // STLport set<CCharString>
    CCharStringTree _M_t;
    CCharStringSetInsertResult insert(const CCharString& __x) { return _M_t.insert_unique(__x); }
};

class CInsertNameInSet {
public:
    CCharStringSet* DefNames;                      // +0x00
    void operator()(const CThing* thing) const;
};

void CInsertNameInSet::operator()(const CThing* thing) const
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (physics && physics->GetPhysicsMeshInfo()) {
        CPhysicsMeshObject mesh = thing->PhysicsTC->GetPhysicsMeshObject();
        if (!mesh.HasNavigationLineListInfo())
            DefNames->insert(thing->GetDefName().GetString());
    }
}