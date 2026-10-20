// Grade: DIFFER(311v286) -- frame packing: retail homes the item modifier in the dead `defName` slot (the wear flag in its high byte) and shares the suit-part key word with the worn modifier (frame 0x10); ours gives each a slot (0x18) and caches the zero in ebx. First form with an early `return` duplicated the destructors (338v286).
// CTCInventoryClothing::SetAsWearing(const CCharString& defName) -- makes the hero wear the named clothing item: unless the piece already worn in that suit part cannot be removed, selects the item and dresses the owner in the selected clothes
// Retail 0x005B413D, 286 bytes, `ret 4`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: CTCBase::GetDefinitionManager 0x006869D0 (x3), CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&)
// 0x009AD410, GetDef(long, CDefPointer<CThingObjectDef>*) 0x0042B040, CDefClassBase::GetSubDef<CAppearanceModifierDef>
// 0x0057FF92 (x2), map<EClothingSuitPart, long>::find 0x005BA986 (returns the node; manifest "RBTree_FindNode"),
// CTCInventoryBase::SelectItemWithDefIndex(long) 0x005BDFB1, SetThingAsWearingSelectedClothes(CThing*) 0x005B406D,
// the defs' slot 1 on release.
// Shape: four CDefPointer locals with inline null-tested releases; the item's modifier is homed in the dead `defName`
// slot and the `wear` flag in its high byte; the worn piece's def and modifier are zeroed together and the modifier's
// release folds away on the GetDef failure path; the suit part key shares a frame word with the worn modifier;
// UseMannequin is cleared around SelectItemWithDefIndex.
// Layout (retail): CurrentClothingSelected +0x140, UseMannequin +0x15C; ParentThing +0x04; CAppearanceModifierDef
// IsRemoveable +0x44, SuitPart +0x4C (Ego_r.pdb, 8 below the PDB); the map node's value +0x14.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CThing;
struct CCharString {
    void* m_pStringData;
    CCharString(const CCharString& other);
    ~CCharString();
};
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other);
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CAppearanceModifierDef : CDefPointeeBase {
    unsigned char _pad_0x08[0x44 - 0x08];
    bool IsRemoveable;                               // +0x44
    unsigned char _pad_0x45[0x4c - 0x45];
    long SuitPart;                                   // +0x4c EClothingSuitPart
};
struct CThingObjectDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CAppearanceModifierDef>* out);   // 0x0057FF92
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
    bool GetDef(long globalIndex, CDefPointer<CThingObjectDef>* out);   // 0x0042B040
};
struct SSuitPartNode {                               // _Rb_tree_node<pair<const EClothingSuitPart, long>>
    void* _M_links[4];
    long key;                                        // +0x10
    long value;                                      // +0x14
};
struct CSuitPartMap {                                // map<EClothingSuitPart, long>
    SSuitPartNode* _M_header; unsigned long _M_node_count; long _M_key_compare;
    SSuitPartNode* end() { return _M_header; }
    SSuitPartNode* find(const long& part);           // 0x005BA986
};

struct CTCInventoryClothing {
    void* __vftable;
    CThing* ParentThing;                             // +0x04
    unsigned char _pad_0x008[0x140 - 0x08];
    CSuitPartMap CurrentClothingSelected;            // +0x140
    unsigned char _pad_0x14c[0x15c - 0x14c];
    bool UseMannequin;                               // +0x15c
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    void SelectItemWithDefIndex(long defIndex);      // 0x005BDFB1 (CTCInventoryBase)
    void SetThingAsWearingSelectedClothes(CThing* thing);   // 0x005B406D
    void SetAsWearing(const CCharString& defName);
};

void CTCInventoryClothing::SetAsWearing(const CCharString& defName)
{
    long defIndex = GetDefinitionManager()->GetDefGlobalIndexFromName(defName);
    CDefPointer<CThingObjectDef> def;
    if (GetDefinitionManager()->GetDef(defIndex, &def)) {
        CDefPointer<CAppearanceModifierDef> modifier;
        if (def->GetSubDef(&modifier)) {
            bool wear = true;
            long part = modifier->SuitPart;
            SSuitPartNode* worn = CurrentClothingSelected.find(part);
            if (worn != CurrentClothingSelected.end()) {
                CDefPointer<CThingObjectDef> wornDef;
                CDefPointer<CAppearanceModifierDef> wornModifier;
                if (GetDefinitionManager()->GetDef(worn->value, &wornDef)) {
                    if (wornDef->GetSubDef(&wornModifier))
                        wear = wornModifier->IsRemoveable;
                }
            }
            if (wear) {
                UseMannequin = false;
                SelectItemWithDefIndex(defIndex);
                SetThingAsWearingSelectedClothes(ParentThing);
                UseMannequin = true;
            }
        }
    }
}
