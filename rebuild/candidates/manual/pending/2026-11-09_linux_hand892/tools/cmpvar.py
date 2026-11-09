# writes comparator variants for the 0x0080xxxx pair<C3DVector,long> TU rows
import sys
A = "/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent892"
OLD_DECL = "    float GetCompareValueForPoint(const C3DVector& point) const;   // 0x007F9660\n};"
OLD_OP = None
VOP = {
 "L1": "    bool operator()(const C3DVector& a, const C3DVector& b) const\n    {\n        return GetCompareValueForPoint(a) > GetCompareValueForPoint(b);\n    }\n",
 "L2": "    bool operator()(const C3DVector& a, const C3DVector& b) const\n    {\n        float aValue = GetCompareValueForPoint(a);\n        float bValue = GetCompareValueForPoint(b);\n        return aValue > bValue;\n    }\n",
 "L3": "    bool operator()(const C3DVector& a, const C3DVector& b) const\n    {\n        return (GetCompareValueForPoint(a) > GetCompareValueForPoint(b));\n    }\n",
 "L4": "    bool operator()(const C3DVector& a, const C3DVector& b) const\n    {\n        float aValue = GetCompareValueForPoint(a);\n        float bValue = GetCompareValueForPoint(b);\n        if (aValue > bValue)\n            return true;\n        return false;\n    }\n",
}
