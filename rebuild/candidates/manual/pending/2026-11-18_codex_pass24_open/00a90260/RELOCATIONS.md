# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0xD | `?GetSize@CIndexBuffer@@QBEJXZ` | 0x00A9026C | 0x00AC1F10 | reloc_bindings |
| +0x1E | `??0?$CArray@K@@QAE@I@Z` | 0x00A9027D | 0x00A96100 | FableWin CArray<unsigned long> constructor identity and retail reserve/zero fill with one argument. A prior catalog labeled the vector base. |
| +0x91 | `??2@YAPAXI@Z` | 0x00A902F0 | 0x00BFEA1A | reloc_bindings |
| +0x9F | `??0CIndexBuffer@@QAE@XZ` | 0x00A902FE | 0x00AC1EC0 | reloc_bindings |
| +0xD8 | `?Initialise@CIndexBuffer@@QAEXKKW4EIndexBufferFormat@@W4EIndexBufferPool@@W4EPrimitiveType@@@Z` | 0x00A90337 | 0x00AC1F70 | reloc_bindings |
| +0xE8 | `?Lock@CIndexBuffer@@QAEPAEKKK@Z` | 0x00A90347 | 0x00AC1FB0 | reloc_bindings |
| +0xF6 | `?Lock@CIndexBuffer@@QAEPAEKKK@Z` | 0x00A90355 | 0x00AC1FB0 | reloc_bindings |
| +0x181 | `?Unlock@CIndexBuffer@@QAEXXZ` | 0x00A903DC | 0x00AC1F00 | reloc_bindings |
| +0x189 | `?Unlock@CIndexBuffer@@QAEXXZ` | 0x00A903E4 | 0x00AC1F00 | reloc_bindings |
| +0x1E5 | `_free` | 0x00A90442 | 0x00BFEA14 | reloc_bindings |

10 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
