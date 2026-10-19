# Radial blur scratch textures

00b5bf10 (185 bytes): actual renderer and pixel-format headers, audited scratch-pool methods and native global pool offset3190. Replaces a nonnegative page, initializes D3D format21 and allocates both 256-square textures even if the first allocation fails. Exact-source host/native tests cover old-page absence/zero/positive and all allocation-success combinations. Observed new2 grades plus UNCERTAIN historical reservation4 (reserved6/10, not a proven historical total).
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
