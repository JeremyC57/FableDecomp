// CThingFilter_SolidToThing::operator()(const CThing& thing) const -- true for every thing except the source thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00661640, 12 bytes: mov eax,[esp+4] ; cmp eax,[ecx] ; setne al ; ret 4.
// Naming: both callers pass the third filter of a CFilter_Bind3<CThingFilter_IsNot,
//   `anonymous namespace'::CThingFilter_ShotsToCheckForCollisionWith, CThingFilter_SolidToThing>: the landed
//   Bind3::operator()<CThing> 0x007C2800 loads [this+8] (the F3 reference) into ecx before the call at 0x007C2862, after
//   calling F1 = CThingFilter_IsNot::operator() at 0x006A76A0 (landed; a different 16-byte body, `xor eax,eax ; cmp ;
//   setne`) through [this+0]; GetAllThingsInRoughArea<..., that Bind3> 0x007C2D10 calls it at 0x007C2EDC. Ego_r.pdb
//   and FableWin.pdb define ??RCThingFilter_SolidToThing@@QBE_NABVCThing@@@Z (and the Bind3 type above). Was landed as
//   "CThingFilter_IsNot::operator()" (FINDINGS chunk 396).
// Layout: Ego_r.pdb CThingFilter_SolidToThing (8 bytes): +0x00 CThing* SourceThing, +0x04 CTCSolidToThing*
//   PTCSolidToThing (unused here).
// Shape: the `?:` form gives the retail `cmp ; setne al` directly; the reference compares by address.
// Leaf: ??RCThingFilter_SolidToThing@@QBE_NABVCThing@@@Z
class CThing;
class CTCSolidToThing;

class CThingFilter_SolidToThing {
public:
    CThing*          SourceThing;           // +0x00
    CTCSolidToThing* PTCSolidToThing;       // +0x04
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_SolidToThing::operator()(const CThing& thing) const
{
    return (&thing != SourceThing) ? true : false;
}
