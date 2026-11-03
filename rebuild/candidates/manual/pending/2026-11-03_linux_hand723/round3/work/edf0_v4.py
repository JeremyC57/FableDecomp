LH="long rowBase = y * 17;\n        long x;\n        for (x = 0; x < 16; x += 2) {\n            long index = x + rowBase;"
VARIANTS = [
 ("e12", [(LH, "long x = 0;\n        long rowBase = y * 17;\n        for (; x < 16; x += 2) {\n            long index = rowBase + x;")], None, "x=0; rowBase; rowBase + x"),
 ("e13", [(LH, "for (long x = 0, rowBase = y * 17; x < 16; x += 2) {\n            long index = rowBase + x;")], None, "for (x=0, rowBase=y*17; rowBase+x"),
 ("e14", [(LH, "for (long x = 0; x < 16; x += 2) {\n            long index = y * 17;\n            index = x + index;")], None, "index = x + index"),
 ("e15", [(LH, "long rowBase = y * 17;\n        for (long x = 0; x < 16; x += 2) {\n            long index = rowBase;\n            index += x;")], None, "index = rowBase; index += x"),
]
