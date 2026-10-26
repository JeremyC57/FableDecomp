// CTCBase::StatGetDriverType() -- the driver type a component class registers with: 0 (not a driver)
// Leaf: ?StatGetDriverType@CTCBase@@SIKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006869C0, 3 bytes: xor eax,eax ; ret.
// Identity (sweep3; was the descriptive _global::ReturnZero_ret0_006869c0, before that the propagated
//   "CEngineInternalPrimitiveBase::AddChildPrimitive"): Ego_r.pdb / FableWin.pdb ?StatGetDriverType@CTCBase@@SIKXZ (public
//   static __fastcall, returns unsigned long); Ego_r order HandleMessage 0x006B1DA0, StatGetDriverType 0x006B1DB0,
//   GetDefinitionManager 0x006B1DC0, EndSerialising 0x006B1DD0 = retail 0x006869B0 / 0x006869C0 / 0x006869D0 / 0x006869E0;
//   its 327 call sites are the component-registration statics (e.g. 0x004EE2A5): the call's result is pushed as the
//   unsigned long between the class's Alloc pointer and its name for CTCBase::CRegisteredTCInfo's ctor 0x004D2EF0
//   (Ego_r ??0CRegisteredTCInfo@CTCBase@@QAE@P6IPAV1@AAVCThing@@@ZKABVCCharString@@@Z), i.e. T::StatGetDriverType()
//   inherited from CTCBase by every non-driver component.
class CTCBase {
public:
    static unsigned long __fastcall StatGetDriverType();
};

unsigned long __fastcall CTCBase::StatGetDriverType()
{
    return 0;
}
