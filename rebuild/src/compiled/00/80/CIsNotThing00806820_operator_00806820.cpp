// CIsNotThing_00806820::operator()(const CIntelligentPointer<CThing>& p) const -- predicate: the pointer's item is not the remembered thing
// Leaf: ??RCIsNotThing_00806820@@QBE_NABV?$CIntelligentPointer@VCThing@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00806820, 25 bytes, thiscall `ret 4`.
// Identity (descriptive): an unreferenced COMDAT of the player targeting TU (beside std::find<CBaseIntelligentPointer*>
//   0x008066A0): `this` holds one pointer at +0, the argument is an intelligent pointer whose item comes from
//   CBaseIntelligentPointer::GetPItem 0x00A01B50; the result is `GetPItem() != Thing` as a bool. Shape: Thing is read into esi BEFORE the call, which needs a named local copy (written straight, VC7.1 keeps `this` and reloads the member after the call: 27 v 25)
//   (`xor ecx,ecx ; setne cl ; mov al,cl`). Ego_r.pdb (a Dinkumware build) has no matching functor, so the class name is ours.
class CBaseObject;
class CThing;
class CBaseIntelligentPointer {
public:
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};

class CIsNotThing_00806820 {
public:
    CBaseObject* Thing;                             // +0x00
    bool operator()(const CIntelligentPointer<CThing>& p) const;
};

bool CIsNotThing_00806820::operator()(const CIntelligentPointer<CThing>& p) const
{
    CBaseObject* thing = Thing;
    return p.GetPItem() != thing;
}