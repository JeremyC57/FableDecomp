// GFCalcThingUID(unsigned __int64 mapUID, unsigned __int64 thingID) -- builds a 64-bit thing UID: map UID in bits 40+, per-map thing id below
// Leaf: ?GFCalcThingUID
//
// Retail 0x0051D990, 33 bytes.  Name from FableWin.pdb ?GFCalcThingUID@@YI_K_K0@Z (__fastcall, but both
// 64-bit arguments still travel on the stack, so the callee pops 0x10).  Catalog label "Multiply64Bit".
// Callee: 0x00BFEB50 _allmul (VC does not strength-reduce an __int64 multiply by 2^40 into shifts).
// Only caller seen: CThingManager::RemapUIDForSerialisation 0x0051E210 (landed; it names this MakeUID_0051d990).
unsigned __int64 __fastcall GFCalcThingUID(unsigned __int64 mapUID, unsigned __int64 thingID)
{
    return mapUID * 0x10000000000ui64 + thingID;
}