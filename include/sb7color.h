/*
 * Copyright � 2012-2015 Graham Sellers
 *
 * This code is part of the OpenGL SuperBible, 6th Edition.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __SB7COLOR_H__
#define __SB7COLOR_H__

// #include <vmath.h>
#include <glm/glm.hpp>

namespace sb7
{

struct color
{
    static const glm::vec4 AliceBlue;
    static const glm::vec4 AntiqueWhite;
    static const glm::vec4 Aqua;
    static const glm::vec4 Aquamarine;
    static const glm::vec4 Azure;
    static const glm::vec4 Beige;
    static const glm::vec4 Bisque;
    static const glm::vec4 Black;
    static const glm::vec4 BlanchedAlmond;
    static const glm::vec4 Blue;
    static const glm::vec4 BlueViolet;
    static const glm::vec4 Brown;
    static const glm::vec4 BurlyWood;
    static const glm::vec4 CadetBlue;
    static const glm::vec4 Chartreuse;
    static const glm::vec4 Chocolate;
    static const glm::vec4 Coral;
    static const glm::vec4 CornflowerBlue;
    static const glm::vec4 Cornsilk;
    static const glm::vec4 Crimson;
    static const glm::vec4 Cyan;
    static const glm::vec4 DarkBlue;
    static const glm::vec4 DarkCyan;
    static const glm::vec4 DarkGoldenRod;
    static const glm::vec4 DarkGray;
    static const glm::vec4 DarkGreen;
    static const glm::vec4 DarkKhaki;
    static const glm::vec4 DarkMagenta;
    static const glm::vec4 DarkOliveGreen;
    static const glm::vec4 DarkOrange;
    static const glm::vec4 DarkOrchid;
    static const glm::vec4 DarkRed;
    static const glm::vec4 DarkSalmon;
    static const glm::vec4 DarkSeaGreen;
    static const glm::vec4 DarkSlateBlue;
    static const glm::vec4 DarkSlateGray;
    static const glm::vec4 DarkTurquoise;
    static const glm::vec4 DarkViolet;
    static const glm::vec4 DeepPink;
    static const glm::vec4 DeepSkyBlue;
    static const glm::vec4 DimGray;
    static const glm::vec4 DodgerBlue;
    static const glm::vec4 FireBrick;
    static const glm::vec4 FloralWhite;
    static const glm::vec4 ForestGreen;
    static const glm::vec4 Fuchsia;
    static const glm::vec4 Gainsboro;
    static const glm::vec4 GhostWhite;
    static const glm::vec4 Gold;
    static const glm::vec4 GoldenRod;
    static const glm::vec4 Gray;
    static const glm::vec4 Green;
    static const glm::vec4 GreenYellow;
    static const glm::vec4 HoneyDew;
    static const glm::vec4 HotPink;
    static const glm::vec4 IndianRed;
    static const glm::vec4 Indigo;
    static const glm::vec4 Ivory;
    static const glm::vec4 Khaki;
    static const glm::vec4 Lavender;
    static const glm::vec4 LavenderBlush;
    static const glm::vec4 LawnGreen;
    static const glm::vec4 LemonChiffon;
    static const glm::vec4 LightBlue;
    static const glm::vec4 LightCoral;
    static const glm::vec4 LightCyan;
    static const glm::vec4 LightGoldenRodYellow;
    static const glm::vec4 LightGray;
    static const glm::vec4 LightGreen;
    static const glm::vec4 LightPink;
    static const glm::vec4 LightSalmon;
    static const glm::vec4 LightSeaGreen;
    static const glm::vec4 LightSkyBlue;
    static const glm::vec4 LightSlateGray;
    static const glm::vec4 LightSteelBlue;
    static const glm::vec4 LightYellow;
    static const glm::vec4 Lime;
    static const glm::vec4 LimeGreen;
    static const glm::vec4 Linen;
    static const glm::vec4 Magenta;
    static const glm::vec4 Maroon;
    static const glm::vec4 MediumAquaMarine;
    static const glm::vec4 MediumBlue;
    static const glm::vec4 MediumOrchid;
    static const glm::vec4 MediumPurple;
    static const glm::vec4 MediumSeaGreen;
    static const glm::vec4 MediumSlateBlue;
    static const glm::vec4 MediumSpringGreen;
    static const glm::vec4 MediumTurquoise;
    static const glm::vec4 MediumVioletRed;
    static const glm::vec4 MidnightBlue;
    static const glm::vec4 MintCream;
    static const glm::vec4 MistyRose;
    static const glm::vec4 Moccasin;
    static const glm::vec4 NavajoWhite;
    static const glm::vec4 Navy;
    static const glm::vec4 OldLace;
    static const glm::vec4 Olive;
    static const glm::vec4 OliveDrab;
    static const glm::vec4 Orange;
    static const glm::vec4 OrangeRed;
    static const glm::vec4 Orchid;
    static const glm::vec4 PaleGoldenRod;
    static const glm::vec4 PaleGreen;
    static const glm::vec4 PaleTurquoise;
    static const glm::vec4 PaleVioletRed;
    static const glm::vec4 PapayaWhip;
    static const glm::vec4 PeachPuff;
    static const glm::vec4 Peru;
    static const glm::vec4 Pink;
    static const glm::vec4 Plum;
    static const glm::vec4 PowderBlue;
    static const glm::vec4 Purple;
    static const glm::vec4 RebeccaPurple;
    static const glm::vec4 Red;
    static const glm::vec4 RosyBrown;
    static const glm::vec4 RoyalBlue;
    static const glm::vec4 SaddleBrown;
    static const glm::vec4 Salmon;
    static const glm::vec4 SandyBrown;
    static const glm::vec4 SeaGreen;
    static const glm::vec4 SeaShell;
    static const glm::vec4 Sienna;
    static const glm::vec4 Silver;
    static const glm::vec4 SkyBlue;
    static const glm::vec4 SlateBlue;
    static const glm::vec4 SlateGray;
    static const glm::vec4 Snow;
    static const glm::vec4 SpringGreen;
    static const glm::vec4 SteelBlue;
    static const glm::vec4 Tan;
    static const glm::vec4 Teal;
    static const glm::vec4 Thistle;
    static const glm::vec4 Tomato;
    static const glm::vec4 Turquoise;
    static const glm::vec4 Violet;
    static const glm::vec4 Wheat;
    static const glm::vec4 White;
    static const glm::vec4 WhiteSmoke;
    static const glm::vec4 Yellow;
    static const glm::vec4 YellowGreen;
};

}

#endif // __SB7COLOR_H__
