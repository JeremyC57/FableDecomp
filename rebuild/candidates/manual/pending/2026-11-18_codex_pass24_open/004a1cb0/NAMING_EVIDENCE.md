# Naming evidence

- Ego_r 0x004AA1E0 and FableWin 0x01B3FCF0 give the exact virtual leaf. Retail vtable reference 0x0123911C independently establishes its slot.
- Ego_d world.obj supplies info, buildings, c1, pdef, rhset, bbox, transform, inv_transform, bounding_sphere_centre, bounding_sphere_radius, right, forward, up, pinfo, scale and weather_mask_primitive. Retail copies the building definition, mesh bounds, axis vectors and position through the corresponding scopes.
- Both PDBs and the native caller instructions identify the base particle-manager prepare call and the same-TU building search specialization. The established GetDef body appears in neighboring world code. Retail +0x04 is the named RenderSizeX field in the recorded physics mesh info.
