O="tex_diff * Parameters.UnsplitTextureWeight + normal_diff * normal_diff * Parameters.UnsplitNormalWeight;"
F={"nn_paren":"tex_diff * Parameters.UnsplitTextureWeight + (normal_diff * normal_diff) * Parameters.UnsplitNormalWeight;",
   "wnn":"tex_diff * Parameters.UnsplitTextureWeight + Parameters.UnsplitNormalWeight * normal_diff * normal_diff;",
   "w_nn":"tex_diff * Parameters.UnsplitTextureWeight + Parameters.UnsplitNormalWeight * (normal_diff * normal_diff);",
   "wt":"Parameters.UnsplitTextureWeight * tex_diff + normal_diff * normal_diff * Parameters.UnsplitNormalWeight;",
   "wt_wnn":"Parameters.UnsplitTextureWeight * tex_diff + Parameters.UnsplitNormalWeight * normal_diff * normal_diff;",
   "wt_nnp":"Parameters.UnsplitTextureWeight * tex_diff + (normal_diff * normal_diff) * Parameters.UnsplitNormalWeight;",
   "rev":"normal_diff * normal_diff * Parameters.UnsplitNormalWeight + tex_diff * Parameters.UnsplitTextureWeight;",
   "paren_t":"(tex_diff * Parameters.UnsplitTextureWeight) + (normal_diff * normal_diff * Parameters.UnsplitNormalWeight);",
   "paren_t2":"(tex_diff * Parameters.UnsplitTextureWeight) + ((normal_diff * normal_diff) * Parameters.UnsplitNormalWeight);"}
VARIANTS=[(k,(lambda v:(lambda s: s.replace(O,v)))(v)) for k,v in F.items()]
