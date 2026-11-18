# 0x00A36A80 naming evidence

The selected RTM object emits `?GetDummyObjectPositionAnimated@C3DMeshObject@@QBEXABVCCharString@@JABVC3DAnimationBlendState@@1MAAVC3DAnimationManager@@AAVC3DVector@@33_N@Z`. Ego_r donor 0x00B8D860 and FableWin donor 0x031F8470 carries this exact decorated spelling.
The Ego_d module record identifies `C3DMeshObject::GetDummyObjectPositionAnimated` in `lib_3d_mesh_object.obj`. C3DMeshObject::GetDummyObjectPositionAnimated (debug length 1058).
Retail directly calls this member at 0x004BE8E7.
Retail directly calls this member at 0x004C0BD2.
The Ego_d entry is the adjacent overload. The retail return stack adjustment and the two older donor signatures distinguish the selected overload.
The imported draft used local models. The submitted port uses the engine headers. See the operand review for binding status.

The returned port calls CCRC::Calc and C3DMeshStats::FindDummyObjectsBegin. The pair return type is const std::pair<unsigned long,C3DDummyObject>*. It is present in Ego_r and FableWin and the landed GetNoDummyObjects sibling calls the same 0x00A2E960 target. The native 0x38 stride and +4 payload corroborate that type. Position, the first direction and the second direction are transformed independently, as exercised by the direct fixture.
