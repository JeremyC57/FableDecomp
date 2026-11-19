O="""    if (new_size <= 1)
        new_size = 1;
    unsigned long new_levels = GFGetTopBit(new_size) + 1;
"""
V={"ternary":"""    unsigned long new_levels = GFGetTopBit(new_size > 1 ? new_size : 1) + 1;
""",
"limit":"""    GFLimit(new_size, 1L, new_size);
    unsigned long new_levels = GFGetTopBit(new_size) + 1;
""",
"uns":"""    if (new_size <= 1)
        new_size = 1;
    unsigned long new_levels = GFGetTopBit((unsigned long)new_size) + 1;
""",
"sep":"""    if (new_size <= 1)
        new_size = 1;
    unsigned long new_levels = GFGetTopBit(new_size);
    new_levels++;
""",
"lt1_else":"""    if (new_size <= 1)
        new_size = 1;
    unsigned long new_levels;
    new_levels = GFGetTopBit(new_size) + 1;
""",
"pre":"""    unsigned long new_levels;
    if (new_size <= 1)
        new_size = 1;
    new_levels = GFGetTopBit(new_size) + 1;
""",
}
VARIANTS=[(k,(lambda v:(lambda s:s.replace(O,v)))(v)) for k,v in V.items()]
def two(s):
    s=s.replace("    long new_size = GFFloatToLongNear((float)sqrt(total_count / Scene->GetMaxAveragePrimitivesPerCell())) * 2 - 1;\n",
     "    long new_size = GFFloatToLongNear((float)sqrt(total_count / Scene->GetMaxAveragePrimitivesPerCell()));\n    new_size = new_size * 2 - 1;\n")
    return s
VARIANTS.append(("two",two))
def sqf(s):
    return s.replace("(float)sqrt(total_count / Scene->GetMaxAveragePrimitivesPerCell())","sqrtf(total_count / Scene->GetMaxAveragePrimitivesPerCell())")
VARIANTS.append(("sqrtf",sqf))
