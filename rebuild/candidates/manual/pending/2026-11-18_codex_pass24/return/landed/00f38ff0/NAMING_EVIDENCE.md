# CSurfaceCompositor::SetBaseBlendModes

Ego_r and FableWin agree on the protected, non-virtual, parameterless member. Ego_d places it beside SetBlendMode and StartComposition in lib_surface_compositor.obj. The landed StartComposition calls 0x00F38FF0. The leaf ends at its return, with an extent of 1,842 bytes.

Ego_d names the sole local renderer, a CRenderManager reference. The native call to GFGetSystemManager and the two loaded member offsets give the same receiver as the landed SetBlendMode. Each state comes from the original CDeviceState member at that offset. The private conversion tables have established base-address bindings. The object uses the correct interior offsets for texture mode, argument, shade and cull entries. MAIN_RELOCATIONS.json records every base and addend.

Unlike the incoming draft, the native colour-write update uses an immediate 15. The submitted source enables all four channels, as the landed StartComposition does. The four-boolean spelling is supported by the sibling while the immediate alone cannot distinguish equivalent overloads.

The test executes the copied member body against the real shared render-state setters. It checks values, ordering, dirty flags, previous-value captures, unchanged resubmission, capture masks and previously queued states in eight scenarios. The test exposes private renderer storage only in its own translation unit to inspect the side effects.
