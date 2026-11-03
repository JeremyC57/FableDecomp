LH="long rowBase = y * 17;\n        long x;\n        for (x = 0; x < 16; x += 2) {\n            long index = x + rowBase;"
VARIANTS = [
 ("e7", [(LH, "long rowBase = y * 17;\n        for (long x = 0; x < 16; x += 2) {\n            long index = x + rowBase;")], None, "rowBase then for(long x=0"),
 ("e8", [(LH, "long x = 0;\n        long rowBase = y * 17;\n        for (; x < 16; x += 2) {\n            long index = x + rowBase;")], None, "x=0; rowBase; for(;"),
 ("e9", [(LH, "for (long x = 0, rowBase = y * 17; x < 16; x += 2) {\n            long index = x + rowBase;")], None, "for (x=0, rowBase=y*17;"),
 ("e10", [(LH, "const long rowBase = y * 17;\n        long x;\n        for (x = 0; x < 16; x += 2) {\n            long index = x + rowBase;")], None, "const rowBase"),
 ("e11", [(LH, "long x;\n        long rowBase;\n        for (x = 0, rowBase = y * 17; x < 16; x += 2) {\n            long index = x + rowBase;")], None, "for (x=0, rowBase=..."),
]
