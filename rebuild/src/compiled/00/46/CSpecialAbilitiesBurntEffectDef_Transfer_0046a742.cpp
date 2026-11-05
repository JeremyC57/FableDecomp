// Leaf: ?CSpecialAbilitiesBurntEffectDef_Transfer@@YIXPAVCCameraManagerSetDef@@PAXPAVCPersistContext@@@Z
#pragma optimize("s",on)
class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    template <class T> void Transfer(T& value);                 // 0x00431061 Transfer(float&)
    void Transfer_00466a47(int* value);             // 0x00466A47 per-type transfer instantiation, element type not attributed
};
class CCameraManagerSetDef { public:
  char pad0[0x3c];
  int f3c;
  int CameraManagerMain;
  int CameraManagerCombat;
  int CameraManagerPCMain;
};
void __fastcall CSpecialAbilitiesBurntEffectDef_Transfer(CCameraManagerSetDef* def, void* unused, CPersistContext* ctx) {
  ctx->Transfer(*(float*)(&def->f3c));
  ctx->Transfer(*(float*)(&def->CameraManagerMain));
  ctx->Transfer(*(float*)(&def->CameraManagerCombat));
  ctx->Transfer_00466a47(&def->CameraManagerPCMain);
}
// Stand-in (owner rule 2026-11-05): CSpecialAbilitiesBurntEffectDef_Transfer for CSpecialAbilitiesBurntEffectDef::Transfer; RTTI vtable slot ?Transfer@CSpecialAbilitiesBurntEffectDef@@UAEXAAVCPersistContext@@@Z + PDB name set (egor/fablewin).
