# CWaveLoader::LoadWaveFile identity and validation

Both original PDBs identify the exact public thiscall CWaveLoader::LoadWaveFile returning CCountedPointer<CWaveFile> by value, with CDataInputStream& input: Ego_r00C19A40, FableWin033FF0F0. Original module1189 lib_wave_loader.obj preserves wavefile, is, total_length,end_pos,file_header,datatype,wave_format,chunk_header,chunk_length,chunk_start,samples,size,bits,audio_data,c1 and their original types/scopes. Full donor walk and native caller00A4289F/RIFF-WAVE string walk independently bind the function. The nested format layout is the original16 bytes; CWaveFile is24 bytes with the original CArray<short> data at+12.

Nine new source strategies were tried after reading both prior drafts/tests and116 historical grades. The final lever was simultaneous visibility of the two REAL STLport4.6 vector<short> operations: sized constructor and assignment. Each independently matched its helper yet left both MOV/LEA residues in the target. With both visible the target matches all1575 bytes/527 instructions; no added inline device or compiler flags. Original PDB source locals, original Initialise member and message constructors are preserved. The unsigned8-bit bias expression avoids negative signed shifts; exact source spelling is inferred, low16-bit behavior is proven.

The selected TU emits six callable helpers with independently matching retail bodies: string equality88 and Compare40, vector sized constructor97 and assignment236, counted-pointer Reset103 and DeleteData34. Their current native address bindings are checked recursively; there is no unmatched called helper. Uncalled default-vector constructor15 is a separate overload and is NOT associated with the sized constructor address. The1575-byte assigned target includes the compiler-emitted final INT3 after the final throw; no fabricated extent correction.

Names on the two vector helpers were previously CArray wrappers or unsigned-short vectors. Original donor audio_data is CArray<short>; actual native helper body is its std::vector<short> base member. The shared CArray forwarding wrappers inline. Original STLport4.6 constructor/assignment source and native complete bodies prove the exact short instantiations. DeleteData's static fastcall name is in BOTH PDBs, matched to the original donor Reset chain and exact34 native bytes; the old Destroy name is wrong. No old spelling copied merely to satisfy bindings.

Exception metadata is checked recursively, not merely its relocated pointer: ThrowInfo16, catchable array12, two catchable types28 each, and type descriptors33/29 all match, with native type_info vtable. Native RTTI strings prove both class keys V and inheritance CWaveFileException:CExceptionBase. The actual original const-char message constructor is FableWin033FF5C0, forwarding to CExceptionBase. Release callsites expand empty state; strings do not survive production, consistent with every native throw. Header additions use a new PASS22 opt-in and leave old/default behavior intact.

The portable fixture copies the ENTIRE selected target body verbatim. It stubs only dependencies using host-portable types and reads exactly4 bytes for unsigned-long stream values even on LP64.2304 combinations plus an empty RIFF test pass on VC71/Od/W3/GR- without EHsc and g++64C++03; four actual throwing branches additionally pass under GCC. Coverage includes8/16bit PCM,1..4 channels,zero/nonzero samples,signed extremes,odd unknown chunks,fmt extensions,ignored BitsPerSample,overwrite by later data chunk,and complete final positions/seek counts. No NRVO reliance: counted fixture explicitly implements sharing/copy lifetime. Exceptions guarded as required on VC. This tests reconstructed source logic; it does not execute the retail binary.

## Exact symbol/address evidence

- `?LoadWaveFile@CWaveLoader@@QAE?AV?$CCountedPointer@VCWaveFile@@@@AAVCDataInputStream@@@Z` ->0x00AAD650: egor_pdb_names.tsv: 00c19a40	?LoadWaveFile@CWaveLoader@@QAE?AV?$CCountedPointer@VCWaveFile@@@@AAVCDataInputStream@@@Z; fablewin_pdb_names.tsv: 033ff0f0	?LoadWaveFile@CWaveLoader@@QAE?AV?$CCountedPointer@VCWaveFile@@@@AAVCDataInputStream@@@Z.

- `?ReadString@CDataInputStream@@QAE?AVCCharString@@K@Z` ->0x00996BE0: egor_pdb_names.tsv: 00afeb20	?ReadString@CDataInputStream@@QAE?AVCCharString@@K@Z; fablewin_pdb_names.tsv: 03081950	?ReadString@CDataInputStream@@QAE?AVCCharString@@K@Z.

- `??9CCharString@@QBE_NPBD@Z` ->0x0099E960: egor_pdb_names.tsv: 00afa270	??9CCharString@@QBE_NPBD@Z; fablewin_pdb_names.tsv: 02f61900	??9CCharString@@QBE_NPBD@Z.

- `__TI2?AVCWaveFileException@@` ->0x01369580: native recursive operand/content proof; existing canonical CRT/string binding or compiler-generated exception metadata.

- `__CTA2?AVCWaveFileException@@` ->0x01369574: native recursive operand/content proof; existing canonical CRT/string binding or compiler-generated exception metadata.

- `__CT??_R0?AVCWaveFileException@@@81` ->0x01369558: native recursive operand/content proof; existing canonical CRT/string binding or compiler-generated exception metadata.

- `??_R0?AVCWaveFileException@@@8` ->0x01398EE8: native recursive operand/content proof; existing canonical CRT/string binding or compiler-generated exception metadata.

- `??_7type_info@@6B@` ->0x012A8370: native recursive operand/content proof; existing canonical CRT/string binding or compiler-generated exception metadata.

- `__CT??_R0?AVCExceptionBase@@@81` ->0x013692B4: native recursive operand/content proof; existing canonical CRT/string binding or compiler-generated exception metadata.

- `??_R0?AVCExceptionBase@@@8` ->0x0137B6D8: native recursive operand/content proof; existing canonical CRT/string binding or compiler-generated exception metadata.

- `__CxxThrowException@8` ->0x00BFEB84: native recursive operand/content proof; existing canonical CRT/string binding or compiler-generated exception metadata.

- `?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z` ->0x00993CA0: egor_pdb_names.tsv: 00afbd30	?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z; fablewin_pdb_names.tsv: 03080f80	?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z.

- `??8?$CBasicString@D@@QBE_NPBD@Z` ->0x004115A0: egor_pdb_names.tsv: 0040c910	??8?$CBasicString@D@@QBE_NPBD@Z; fablewin_pdb_names.tsv: 01899d90	??8?$CBasicString@D@@QBE_NPBD@Z.

- `??0?$vector@FV?$allocator@F@std@@@std@@QAE@I@Z` ->0x00AADD80: fablewin_pdb_names.tsv: 033eed70	??0?$vector@FV?$allocator@F@std@@@std@@QAE@I@Z.

- `_malloc` ->0x00BFEA0E: native recursive operand/content proof; existing canonical CRT/string binding or compiler-generated exception metadata.

- `??2@YAPAXI@Z` ->0x00BFEA1A: native recursive operand/content proof; existing canonical CRT/string binding or compiler-generated exception metadata.

- `?Reset@?$CCountedPointer@VCWaveFile@@@@QAEXPAVCWaveFile@@@Z` ->0x00AADFF0: egor_pdb_names.tsv: 00c1a0e0	?Reset@?$CCountedPointer@VCWaveFile@@@@QAEXPAVCWaveFile@@@Z; fablewin_pdb_names.tsv: 033ffaf0	?Reset@?$CCountedPointer@VCWaveFile@@@@QAEXPAVCWaveFile@@@Z.

- `??3@YAXPAX@Z` ->0x00BFE9BC: native recursive operand/content proof; existing canonical CRT/string binding or compiler-generated exception metadata.

- `?DeleteData@?$CCountedPointer@VCWaveFile@@@@SIXPAX@Z` ->0x00AADFC0: egor_pdb_names.tsv: 00c1a090	?DeleteData@?$CCountedPointer@VCWaveFile@@@@SIXPAX@Z; fablewin_pdb_names.tsv: 033ffc30	?DeleteData@?$CCountedPointer@VCWaveFile@@@@SIXPAX@Z.

- `_free` ->0x00BFEA14: native recursive operand/content proof; existing canonical CRT/string binding or compiler-generated exception metadata.

- `??4?$vector@FV?$allocator@F@std@@@std@@QAEAAV01@ABV01@@Z` ->0x00AADE80: fablewin_pdb_names.tsv: 033ff910	??4?$vector@FV?$allocator@F@std@@@std@@QAEAAV01@ABV01@@Z.

- `_memmove` ->0x00BFEAE6: native recursive operand/content proof; existing canonical CRT/string binding or compiler-generated exception metadata.

- `??1CCharString@@QAE@XZ` ->0x0099EAE0: egor_pdb_names.tsv: 00afa3f0	??1CCharString@@QAE@XZ; fablewin_pdb_names.tsv: 02f60f80	??1CCharString@@QAE@XZ.

- `?Compare@?$CBasicString@D@@KIJPBD0@Z` ->0x00411570: egor_pdb_names.tsv: 0040c7a0	?Compare@?$CBasicString@D@@KIJPBD0@Z; fablewin_pdb_names.tsv: 01899dc0	?Compare@?$CBasicString@D@@KIJPBD0@Z.
