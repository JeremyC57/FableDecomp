// CTCTargetingSpirit::OnInitialActivate() -- the base targeting activation, then take the maximum targeting distance from the owner's targeting sub-def (its per-type distance map entry for key 0x10, 0 when absent)
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F98E0, length 114 bytes (padded row 128); CTCTargetingSpirit vtable 0x012419A4 slot 6
// ("vfunc_6" on CTC classes is CTCBase::OnInitialActivate).
// Callees: CTCTargetingBase::OnInitialActivate @ 0x007F59C0 (fills PTargetingDef +0x30; manifest label "CTCTargetingAI::OnInitialActivate" is the
// shared body), CDefClassBase::GetSubDef<T>(CDefPointer<const T>*) @ 0x007EA960 (owner def +0x70; manifest instantiation
// "<CAICreatureWillPowerIndicatorDef>" is a byte twin), std::map<long,float>::find(const long&) @ 0x007F9830 (iterator returned through the
// hidden pointer; manifest "std::map<int,int>::find"), the def's virtual slot 1 (destroy). Constant 0.0f @ 0x0122DEDC.
// Layout: Ego_r.pdb CTCTargetingSpirit (0x58): CTCTargetingBase 0x38, MaxTargetingDistance +0x38. The sub-def's map sits at +0x74 (STLport
// _Rb_tree: header node pointer first; the value of a node at +0x14 after the 16-byte node links and the key).
// Shape: the def pointer local is zeroed and filled by GetSubDef, read once into a named pointer (ebx) and released by hand at the end;
// `it._M_node != map._M_header` compared directly; the float is a ternary stored once.
struct CThing;
struct CRbNode {
    long     _M_color;
    CRbNode* _M_parent;
    CRbNode* _M_left;
    CRbNode* _M_right;
    long     first;                             // +0x10
    float    second;                            // +0x14
};
struct CDistanceMap {                           // std::map<long, float>
    struct iterator {
        CRbNode* _M_node;
        iterator() { }
    };
    CRbNode* _M_header;                         // +0x00
    long     _M_node_count;                     // +0x04
    iterator find(const long& key) const;       // 0x007F9830
};
static __forceinline float FindDistance(const CDistanceMap& map, long key)
{
    CDistanceMap::iterator it = map.find(key);
    return it._M_node != map._M_header ? it._M_node->second : 0.0f;
}
struct CTargetingSpiritDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0x74 - 0x08];
    CDistanceMap Distances;                     // +0x74
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CTargetingSpiritDef>* out) const;   // 0x007EA960
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CTargetingSpiritDef>* out) const
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThingDefHolder {
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                        // +0x70
};
struct CTCTargetingBase {
    void* __vfptr;                              // +0x00 (CTCBase)
    CThingDefHolder* ParentThing;               // +0x04
    unsigned char _pad08[0x38 - 0x08];
    void OnInitialActivate();                   // 0x007F59C0
};
struct CTCTargetingSpirit : CTCTargetingBase {
    float MaxTargetingDistance;                 // +0x38
    void OnInitialActivate();
};

void CTCTargetingSpirit::OnInitialActivate()
{
    CTCTargetingBase::OnInitialActivate();
    CDefPointer<CTargetingSpiritDef> def;
    ParentThing->PDef->GetSubDef(&def);
    CTargetingSpiritDef* d = def.Object;
    MaxTargetingDistance = FindDistance(d->Distances, 0x10);
    if (--d->RefCount == 0)
        d->Destroy();
}