// Leaf: ?Transfer@CCameraManagerSetDef@@UAEXAAVCPersistContext@@@Z
#pragma optimize("s",on)
class CPersistContext { public:
  template <class T> void Transfer(T& value);
};
class CCameraManagerSetDef { public:
  char pad0[0x38];
  int f3c;
  int CameraManagerMain;
  int CameraManagerCombat;
  int CameraManagerPCMain;
    virtual void Transfer(class CPersistContext&);
};
void CCameraManagerSetDef::Transfer(class CPersistContext& ctx)
{
    CCameraManagerSetDef* def = this;
  ctx.Transfer(*(long*)(&def->f3c));
  ctx.Transfer(*(long*)(&def->CameraManagerMain));
  ctx.Transfer(*(long*)(&def->CameraManagerCombat));
  ctx.Transfer(*(long*)(&def->CameraManagerPCMain));
}
// Flatten route (flatten_pilot v4, NF324): CCameraManagerSetDef_Transfer -> CCameraManagerSetDef::Transfer, the RTTI-placed PDB member; class model: the row's local class model.
