# 0x00877840 naming evidence

The selected RTM object emits `?IsLineOfSight@CTools@NCameraModes@@QBE_NABV?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@ABVC3DVector@@1AAV5@2AAMPAPBVCThing@@W4ELOSTest@12@_N@Z`. Ego_r donor 0x0085E280 and FableWin donor 0x029C1E60 carries this exact decorated spelling.
The Ego_d module record identifies `NCameraModes::CTools::IsLineOfSight` in `cm_helper_functions.obj`. NCameraModes::CTools::IsLineOfSight (debug length 1084).
Retail directly calls this member at 0x0069988C.
Retail directly calls this member at 0x006EB1C9.
The imported draft used local models. The submitted port uses the engine headers. See the operand review for binding status.

Camera membership and the vector-of-const-things overload agree with Ego_d, Ego_r and the sibling call from 00877C40. The three out-of-line queries target 008771A0, 008754A0 and 00875550. The thing query uses const CThing**, consistent with the native out-argument, PDB and shared declaration. The global epsilon is used twice.
