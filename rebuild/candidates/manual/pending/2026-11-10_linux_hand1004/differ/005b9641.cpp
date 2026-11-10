// CTCInventoryClothing::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- while the screen is active with a mannequin: advance the mannequin's animation for this frame, place it in the UI_MISC_THINGS_DEF clothing viewport (UI-scaled screen rectangle), orient it, and hand its morphed animating mesh primitives (one per body / per clothing item) to the renderer
// Grade: DIFFER(none v1455) -- not attempted (0 grades): no shared headers for CEnginePrimitivePrepareInfo, CUIMiscThingsDef (+0x1E4..+0x200 viewport rect / offsets), CRightHandedSet, C3DOrientationInterpolator, CEnginePrimitive2DMorphedAnimatingMesh, CEngineTextureMorphSet or CSectionSet; x87-heavy body
// Leaf: ?DrawPreparePrimitivesForRendering@CTCInventoryClothing@@UAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B9641, thiscall `add ebp,74h ; leave ; ret 4` at 0x005B9BED, biased frame
//   (`push ebp ; lea ebp,[esp-74h] ; sub esp,168h`): optimize("s").
// EXTENT: the catalog row is 1,867 bytes but the function ends at 0x005B9BF0 (1,455 bytes = 0x5AF); 0x005B9BF0..0x005B9D8C is a run of
//   small out-of-line STLport / CVectorMap helpers (iterator ctors `mov eax,ecx ; mov [eax],[esp+4] ; ret 4`, `==` via neg/sbb,
//   `[ecx]+0x10` node values, pair ctors, a 0x18-byte vector size()) -- recorded in SPLITS.tsv; their names are not proven here.
// Identity: CTCInventoryClothing vtable 0x0124356C slot 11 (+0x2C) = 0x005B9641; Ego_r.pdb / FableWin.pdb
//   ?DrawPreparePrimitivesForRendering@CTCInventoryClothing@@UAEXABVCEnginePrimitivePrepareInfo@@@Z (28 name-set hits); the Ego_r
//   donor order puts it right after ConstructScreenPC (donor 0x005650FF). The catalog's "vfunc_11" is the slot number.
// Decoded outline (for the next attempt; callees as bound in reloc_bindings / landed rows):
//   if (!Active || !PMannequin) return;                                       // +0x58, +0x158
//   CTCAnimationComplex* anim = 0; PMannequin->GetTC(TCI 0x5A, &anim);
//   <local 0x4C2DD0 ctor at [ebp+1Ch]>; anim->GetAnimation() 0x0070B460 -> CAnimateThing call 0x0070E650(&local, info.<+0x14 float>)
//   CDefPointer<const CUIMiscThingsDef> misc; GetDefinitionManager()->GetDef(GetDefGlobalIndex(CCharString("UI_MISC_THINGS_DEF"), true), &misc)
//     (0x0044C6B0 / 0x009AD390 / GetDef<CUIMiscThingsDef> 0x0042B312);
//   C2DVector pos = NUISystem::CUIScreenUnitConverter::ConvertVector(misc->+0x1F8) 0x0052E580; three CManager::GetUIScale() 0x0041CF47 reads
//     scale the +0x1E4..+0x1F0 rect and +0x200 offset into a screen rectangle;
//   the mannequin's PTCDriver-style member at this+0x68 (+0xD8 vector copied, counted pointer inc/dec) and misc +0x1F4;
//   CRightHandedSet(C3DVector(0,0,-1) , C3DVector(0,0,0)) 0x00487050 -> C3DOrientationInterpolator(set, set) 0x005508D0;
//   PMannequin TC 3 (0x004365B0 lookup) -> virtual +0x74; TC lookups 0x786870 / 0x5BA124 / 0xA3C350 / 0x5BA0CC / 0x5BA0F8; CSectionSet()
//     0x004AD460; CTCHeroAttachableAppearanceModifiers 0x706860 / 0x707B10; a std::set walk (0x4292C0) inserting into a morph map
//     (0x5BA765 / 0x5BA778 / 0x5BBAF9); two loops over 0x18-byte elements (`idiv 18h` sizes) each building a
//     CEnginePrimitive2DMorphedAnimatingMesh 0x005BC1B0, inserting its handle (MorphPrimitiveHandles 0x005BBFD6), calling the
//     renderer's virtual +0x5C and releasing (0x005BC216); then 0x005BC2D7, free, ~CMember 0x004AC8F0, ~vector<CEngineTextureMorph>
//     0x005BC0D3, ~CBase 0x0099A2E0, the misc def release, and the 0x004C2DF0 local destructor.
// Next idea: build engine/CUIMiscThingsDef.h, CRightHandedSet.h and C3DOrientationInterpolator.h from Ego_r (struct_layouts_egor.tsv)
//   first, then author the body in the ConstructScreenPC TU style (same-TU GetDef<T> body, optimize("s")).
#pragma optimize("s", on)
