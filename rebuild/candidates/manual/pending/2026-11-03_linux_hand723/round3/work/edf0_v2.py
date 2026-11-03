LH="long rowBase = y * 17;\n        long x;\n        for (x = 0; x < 16; x += 2) {\n            long index = x + rowBase;"
VARIANTS = [
 ("e3", [(LH, "for (long x = 0; x < 16; x += 2) {\n            long index = x + y * 17;")], None, "e1 + x + y*17 (LICM)"),
 ("e4", [(LH, "for (long x = 0; x < 16; x += 2) {\n            long index = x + 17 * y;")], None, "e1 + x + 17*y"),
 ("e5", [(LH, "for (long x = 0; x < 16; x += 2) {\n            long index = 17 * y + x;")], None, "e1 + 17*y + x"),
 ("e6", [(LH, "for (long x = 0; x < 16; x += 2) {\n            long index = y * 17;\n            index += x;")], None, "e1 + index=y*17; index+=x"),
]
