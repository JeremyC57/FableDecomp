// ReturnShort2_00510020() -- returns the 16-bit constant 2
// Leaf: ?ReturnShort2_00510020@@YAFXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00510020, 5 bytes: mov ax,2 ; ret (66 B8 02 00 C3).
// Identity: descriptive. The gap-scan row has no caller and no real data reference (the four .rdata "references" in
//   xref.py are UTF-16 menu strings at 0x01238636 / 0x012386B6 / 0x01258832 / 0x012588E4 whose bytes happen to spell the
//   address); its neighbours 0x0050FF40 / 0x0050FF90 / 0x005101B0 are CCountedPointer<QuadricOptimiserInternals::
//   COptimisedPrimitive> release helpers, so it sits in the quadric-optimiser TU. No PDB name set yields a 16-bit
//   constant getter there, so the name is descriptive.
// Shape: a 16-bit return type (`mov ax,imm16`: a short, not a long or bool); `this` is not used, so a free function.
short ReturnShort2_00510020()
{
    return 2;
}