// CTCActionUseBase::GetActionUseAnimationGroupName() const -- the text of the def's AnimationGroup from the global def-string table
// Leaf: ?GetActionUseAnimationGroupName@CTCActionUseBase@@QBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EABA0, 56 bytes, thiscall, `ret 4` (hidden CCharString result).
// Identity (proven): Ego_r.pdb ?GetActionUseAnimationGroupName@CTCActionUseBase@@QBE?AVCCharString@@XZ; CTCActionUseBase::PActionUseDef is the CDefPointer<const CActionUseDef> at +0xC and CActionUseDef::AnimationGroup the CDefString at +0x28 (both Ego_r.pdb); it sits among the CTCActionUseBase methods (0x007EAB90 OnActionUse, 0x007EAC10 OnCreate) and its caller is CCreatureAction_UseLamp::Initialise 0x00847DF0. The manifest's "CTCActionUseBase::GetActionUseAnimationGroupName" is propagated across
//   byte-identical bodies.
// Callees: CDefStringTable::GetString(long) const 0x009D49B0 on g_DefStringTable 0x013CA828 (Ego_r.pdb GDefStringTable; the
//   landed TUs' spelling), CCharString::CCharString(const CCharString&) 0x0099EC30, CCharString::~CCharString() 0x0099EAE0.
// Shape (family M186, representative 0x00670010): `CCharString name = g_DefStringTable.GetString(PActionUseDef->AnimationGroup.TablePos);
//   return name;` -- the table's result lands in the `push ecx` local, is copied into the hidden result and destroyed (no
//   NRVO in VC7.1). Speed TU.
class CCharString {
public:
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
    void* PStringData;                                    // +0x00
};
class CDefString {
public:
    long TablePos;                                        // +0x00
};
class CDefStringTable {
public:
    CCharString GetString(long index) const;              // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                  // 0x013CA828 (Ego_r.pdb GDefStringTable)
template <class T> class CDefPointer {
public:
    T* operator->() const { return Data; }
    T* Data;                                              // +0x00
};
class CActionUseDef {                                             // Ego_r.pdb
public:
    char Base[0x28];
    CDefString AnimationGroup;                                     // +0x28
};
class CTCActionUseBase {
public:
    CCharString GetActionUseAnimationGroupName() const;
    void* VTable;                                         // +0x00 (CTCBase)
    void* Base[2];                                        // +0x04
    CDefPointer<const CActionUseDef> PActionUseDef;              // +0xC
};
CCharString CTCActionUseBase::GetActionUseAnimationGroupName() const
{
    CCharString name = g_DefStringTable.GetString(PActionUseDef->AnimationGroup.TablePos);
    return name;
}