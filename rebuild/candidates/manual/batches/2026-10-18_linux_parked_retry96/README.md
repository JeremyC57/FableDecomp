# Truncate packed narrow-string buffers

009a0080 (166 bytes): native CBasicString<char> packed13-byte view, PData0/StringLength4/DataLength31bits8/UseFastExtendc. The native array allocator and delete[] bindings are bfeb22/bfeb1c. Moving the intermediate null store inside the nonnull delete branch recovered the exact body without volatile stores or instruction encoding. Native capacity assignment preserves the reserved high bit.

Main independently passes hostO2 and canonical VC7.1 byte/behavior checks. Root restricted raw capacity-word seeding to the native fixture: host unsigned long and packed-bitfield layout differ, so the worker's eight-byte write could overrun the host subobject. All ordinary behavior checks run on both compilers. Expanded cases include equal/larger no-op lengths, zero prefix, alignment, copied prefix, terminators and cleared padding; native checks additionally cover size/offsets and reserved capacity bit. Ten HIGH plus one XHIGH grade. Production unchanged; test-only corrections and expanded coverage are verification.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
