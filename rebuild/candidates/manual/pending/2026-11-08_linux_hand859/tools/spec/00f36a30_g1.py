LOOP_OLD = """    long i;
    for (i = 1; i < octaves; ++i) {
        double increment = noise->PerlinNoise(p) * ExponentArray[i] * value;
        value += increment;
        p.X *= Lacunarity;
        p.Y *= Lacunarity;
    }"""
AXES = [
 ("inc", [("", "", []),
          ("i2", "value += product", [("        double increment = noise->PerlinNoise(p) * ExponentArray[i] * value;\n        value += increment;", "        value += noise->PerlinNoise(p) * ExponentArray[i] * value;")]),
          ("i3", "value = value + product", [("        double increment = noise->PerlinNoise(p) * ExponentArray[i] * value;\n        value += increment;", "        value = value + noise->PerlinNoise(p) * ExponentArray[i] * value;")]),
          ("i4", "signal local", [("        double increment = noise->PerlinNoise(p) * ExponentArray[i] * value;\n        value += increment;", "        double signal = noise->PerlinNoise(p) * ExponentArray[i];\n        value += signal * value;")])]),
 ("type", [("", "", []),
           ("it", "int counter", [("    long i;\n", "    int i;\n")])]),
 ("lac", [("", "", []),
          ("ll", "lacunarity local", [("    double octaves = Octaves;\n", "    double octaves = Octaves;\n    double lacunarity = Lacunarity;\n"), ("        p.X *= Lacunarity;\n        p.Y *= Lacunarity;\n    }", "        p.X *= lacunarity;\n        p.Y *= lacunarity;\n    }")])]),
 ("loop", [("", "", []),
           ("wl", "while loop", [("    for (i = 1; i < octaves; ++i) {", "    i = 1;\n    while (i < octaves) {"), ("    }\n    double remainder", "        ++i;\n    }\n    double remainder")])]),
 ("copy", [("", "", []),
           ("pm", "p copied member-wise", [("    C2DVector p = position;", "    C2DVector p;\n    p.X = position.X;\n    p.Y = position.Y;")])]),
 ("rem", [("", "", []),
          ("r2", "remainder via a long local", [("    double remainder = Octaves - (long)Octaves;", "    long whole = (long)Octaves;\n    double remainder = Octaves - whole;")]),
          ("r3", "remainder from the octaves local", [("    double remainder = Octaves - (long)Octaves;", "    double remainder = octaves - (long)octaves;")])]),
 ("noise", [("", "", []),
            ("nd", "Noise read directly", [("    CNoise* noise = Noise;\n", ""), (">noise->", "Noise->")])]),
]
