# Relocation audit

The exact leaf `?GetStringExtents@CStaticFontBank@@UBE?AVC2DExtentsF@@PB_W_N@Z` is 287 bytes. Its two COFF absolute relocations are local compiler literal symbols. At leaf offset +0x08, `__real@00000000` matches the float zero loaded at 0x00AB7B06 from 0x0122DEDC. At +0x2F, `__real@4f800000` matches the float 4294967296.0 loaded at 0x00AB7B2D from 0x0122DCB4 to correct the integer-to-float conversion.
