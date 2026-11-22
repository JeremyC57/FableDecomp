# Divine Wrath native probes

| Change from the preceding relevant probe | Native result |
|---|---|
| Shared owner and typed engine calls with all effect lookups opaque | 1,878 bytes, 390 matching instructions |
| Expose the shared effect lookup and CRC bodies | 1,986 bytes, 539 matching instructions |
| Expand the real intelligent-pointer assignment | 2,013 bytes, 395 matching instructions |
| Expose the STLport const lookup so the third effect lookup's search inlines | 2,031 bytes, 395 matching instructions |
| Expose the original out-of-line GetSubDef body, whose parameter escape information changes register allocation | 2,017 bytes, 574 matching instructions |
| Use the original typed CRC loop from the donor and shared CCRC header | 2,034 bytes, 603 matching instructions, one byte differs |
| Enable the existing named length and text locals inside the shared effect lookup | 2,034 bytes, all 604 instructions and every relocated operand verified |

The final CRC difference before the named locals was the base and index encoding in the hand-effect byte load at 0x0078F245. Equivalent subscript and pointer-arithmetic spellings of the CRC loop did not alter it. The visible GetSubDef COMDAT independently matches retail 0x00669580. The native check was repeated on the verified combined headers.

The default speed-optimized object emitted differing, unreferenced lookup and erase COMDATs. The shared declaration-only erase opt-in retained its external owner at 0x00792300. The unsigned-long lookup specialization uses the retail owner’s size setting, preserving the same lookup symbol and inline callers while its out-of-line byte extent matches 0x005D83B3. The final check verifies every emitted bound COMDAT.
