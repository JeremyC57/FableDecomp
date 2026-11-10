# GOTCHAS-ready one-liners (chunk 1014)

- 0x00703210 CThingObject::Create: read `object->GetPhysicsTC()` inside each branch and make the out-of-line `CTCParamsPhysicsBase` ctor body visible (same-TU `__declspec(noinline)`); with a declaration only, VC7.1 assumes the temporary may capture the object and swaps ebx/edi.
- 0x00703210: the default `CRightHandedSet()` is inline in retail (Up 0,0,1 / Forward 0,1,0 stored in place); a header opt-in ctor beats a hand-built set.
- 0x00B84720 CEngineSubPrimitiveRenderer::AddSubPrimitive: put the scaled sort distance in a named float local, not back into the `distance` parameter: the parameter form moves the `__false_type` temporary slot and the prologue `fld`.
- 0x009FA620 CTexture::GetNextValidDimensions: GFGetTopBit / GFGetNextPowerOfTwo must be FABLE_FORCEINLINE (two expansions each exceed the plain-inline budget); `IsNeedingPower2Textures` as the `>>1 & 1` accessor gives retail's `shr ecx,1 ; test cl,1`.
- 0x007EA2F0 NTargeting::GetTargetingPositionOnThing: header bodies alone (const `GetBulletTimeManager`, inline `IsBulletTimeActive`, inline `GetPosition(float)`) fixed the return-block layout; the fallback must be `GetPosition(t) + C3DVector(0,0,0.25f)`, not `pos.Z += 0.25f` (392v411 -> match).
- 0x009E3F60 CPixelFormat::ConvertRGBToPFDData: the catalog extent (576) spans ~16 functions; the function is 189 bytes (`ret 8` at 0x009E401A). Running-sum channel positions (RPos/APos as sums of the lower channels' bit counts) fix the add order.
- 0x00725F10: the shared ROUGHAREABODY body is not the retail shape for this instance (733v590): retail walks the mapwho layer table by pointer against g_NoMapwhoLevels.
