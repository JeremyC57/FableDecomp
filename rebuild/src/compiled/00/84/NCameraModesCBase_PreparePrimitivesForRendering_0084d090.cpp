// NCameraModes::CBase::PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) const -- the base camera mode
// prepares no debug primitives (empty virtual, slot 5)
// Leaf: ?PreparePrimitivesForRendering@CBase@NCameraModes@@UBEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084D090, 3 bytes: ret 4.
// Identity (sweep3; was the gap-scan sub_0084d090, catalog NCameraModes::CBase::vfunc_5): slot 5 of the NCameraModes::CBase
//   vtable 0x0124DD74 (and of 18 more camera-mode vtables that inherit it); of the CBase virtuals in Ego_r.pdb / FableWin.pdb
//   only ?PreparePrimitivesForRendering@CBase@NCameraModes@@UBEXABVCEnginePrimitivePrepareInfo@@@Z returns void and takes an
//   argument (the others take none or return bool/float), and Ego_r places it directly after GetCamTools (0x00844C80), as
//   retail 0x0084D090 follows GetCamTools 0x0084D080. The vtable is modelled with placeholder slots 0-4.
class CEnginePrimitivePrepareInfo;

namespace NCameraModes {

class CBase {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) const;   // slot 5
};

void CBase::PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& /*info*/) const
{
}

}
