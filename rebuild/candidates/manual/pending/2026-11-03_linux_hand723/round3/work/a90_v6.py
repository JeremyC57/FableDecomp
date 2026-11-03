H="""static inline float Clamp(float v, float lo, float hi)
{
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}
"""
B="    float blend = Clamp(alpha / 255.0f, 0.0f, 1.0f);\n"
VARIANTS = [
 ("c14", [(H,""),(B,"    float blend = alpha / 255.0f;\n    if (blend < 0.0f)\n        blend = 0.0f;\n    else if (blend > 1.0f)\n        blend = 1.0f;\n")], None, "inline clamp, no helper"),
 ("c15", [(H,""),(B,"    float blend = alpha / 255.0f;\n    if (blend < 0.0f)\n        blend = 0.0f;\n    if (blend > 1.0f)\n        blend = 1.0f;\n")], None, "inline clamp 2 ifs"),
 ("c16", [(H,"inline float GFClamp01(float a)\n{\n    if (a < 0.0f)\n        a = 0.0f;\n    else if (a > 1.0f)\n        a = 1.0f;\n    return a;\n}\n"),(B,"    float blend = GFClamp01(alpha / 255.0f);\n")], None, "GFClamp01 (landed spelling)"),
]
