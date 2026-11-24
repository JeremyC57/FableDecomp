# Landing nf382 b4: 0x00E75EF0 is CV_ExposeMayorScript::PreventConflictWithTCE

The self carried `?WatchForEndOfScript@CQ_BanditCampScript@NScript@@QAEXXZ`, which belongs to 0x00D013A0 (not landed; the table binds it there with witness 0x00D00AF0). Sources: both PDB name sets, and CV_ExposeMayorScript::Main stores its address at 0x00E75EA0 as Ego_r's Main stores PreventConflictWithTCE. Ruling NF382-p2 (advisor decision, owner delegated; GOTCHAS landing nf382): 0x00D00AF0 loses its HEAD resolution, which was a wrong body; it resolves again when 0x00D013A0 lands.
