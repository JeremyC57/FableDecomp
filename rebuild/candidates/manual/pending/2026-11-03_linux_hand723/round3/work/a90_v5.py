BL="    float alpha = (1.0f - params.ThemeChannels[0].TextureSets[0].Blends[0]) * 255.0f;\n    float blend = Clamp(alpha / 255.0f, 0.0f, 1.0f);\n"
CB="""    C4DVector colour;
    colour.X = Settings.Colour.R;
    colour.Y = Settings.Colour.G;
    colour.Z = Settings.Colour.B;
    colour.W = params.ThemeChannels[0].Scalers[2] * Settings.ColourToReflectionFactor;
"""
VARIANTS = [
 ("c12", [(BL+CB, CB+BL)], None, "colour block before blend"),
 ("c13", [("#define FABLE_RETAIL_CENGINEWATERRENDERER\n","#define FABLE_RETAIL_CENGINEWATERRENDERER\n#define FABLE_RETAIL_CARRAY\n")], None, "FABLE_RETAIL_CARRAY"),
]
