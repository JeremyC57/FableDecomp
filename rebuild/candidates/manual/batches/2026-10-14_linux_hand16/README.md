# Batch `2026-10-14_linux_hand16` — hand-authored CCachedFile .. CCham functions: 40 bodies (36 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the seventeenth chunk of
class-labelled open functions of 17–96 bytes: `CCachedVolumes` layer lookups, `CCamera` view /
FOV setters, `CCategory` item queries (`find_if` with `bind2nd(mem_fun_ref(...))`, the retail
layout four bytes below the PDB header after `ItemsInCategory`), `CTCInventoryBase` /
`CInventoryCategoryDef` accessors (also PDB-4 per vector), `CTCWeapon` def reads, counted-record
constructors, `CPrizeScoreDef` / `CTCThingOwner` / `CHitLocationDef` / `CCompositeSurfaceInfo`
constructors and `CScriptObject::Init` bodies. Propagated labels were corrected from the vtables
and layouts (listed in the pass notes); the `CCategory` rows use a local `CCategory_Retail` model
with the PDB names.

Lessons recorded by the pass: `return new T(*this)` with the copy constructor inlined needs a
{first,last} member object plus a separately initialised capacity word; `CDefPointer` scoped access
is an `operator->` holder temporary (AddRef null-checked, Release not); the Dinkumware `bind2nd`
front end is written inline, not as a POD; an empty tag with a user constructor passed by const
reference is homed in the high byte of a live argument slot; frame-pointer bodies of the def TUs
are `optimize("y",off)` + `("s",on)`; `pos += dir * d` on a 12-byte vector is an inline
`operator*` temporary plus `operator+=`. Four bodies differ by load order or register roles only
(`GetNavigationLayerAt`, `SetView`, `SetFOV`, `GetNumberOfItemsOfTypeInCategory`).

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
